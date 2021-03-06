#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "util.h"



/* Extracts the n-th string from a buffer with n or more strings.
   Returns 1 if it succeds, else returns 0. */
int get_nth_string(char* str, const char buf[], int n)
{
  /* check for valid parameters */
  if (n <= 0)
  {
    return 0;
  }


  int i = 0;
  int j = 1;
  /* use the loop to skip the first n - 1 strings, while also checking
     for errors */
  for (; j <= n - 1; j++)
  {

    while (i < MAX_BUFFER_SIZE && buf[i] != ' ' && buf[i] != '\t' && buf[i] != '\n' && buf[i] != '\0')
    {
      i++;
    }

    if (i == MAX_BUFFER_SIZE || (buf[i] != ' ' && buf[i] != '\t'))
    {
      return 0;
    }

    while (i < MAX_BUFFER_SIZE && (buf[i] == ' ' || buf[i] == '\t'))
    {
      i++;
    }

  }

  /* check if we have already iterated through the buffer */
  if (i == MAX_BUFFER_SIZE || (buf[i] == '\n' || buf[i] == '\0'))
  {
    return 0;
  }


  int k = 0;
  /* extract the n-th string */
  while (buf[i] != ' ' && buf[i] != '\t' && buf[i] != '\n' && buf[i] != '\0')
  {
    str[k] = buf[i];
    i++;
    k++;

    if (i == MAX_BUFFER_SIZE)
    {
      return 0;
    }
  }
  str[k] = '\0';

  return 1;

}


/* If the buffer has a valid option, it returns the number of that option (1-14)
   Else, if there is an error, then -1 is returned */
int get_option(const char buffer[])
{
  /* string that contains the option */
  char option[MAX_BUFFER_SIZE];
  /* get the first string of the buffer to distinguish which option we have */
  int retval = get_nth_string(option, buffer, 1);

  if (retval == 0)
  {
    /* some error occured */
    return -1;
  }

  if (!strcmp(option, "cfs_workwith"))
  {
    return 1;
  }
  else if (!strcmp(option, "cfs_mkdir"))
  {
    return 2;
  }
  else if (!strcmp(option, "cfs_touch"))
  {
    return 3;
  }
  else if (!strcmp(option, "cfs_pwd"))
  {
    return 4;
  }
  else if (!strcmp(option, "cfs_cd"))
  {
    return 5;
  }
  else if (!strcmp(option, "cfs_ls"))
  {
    return 6;
  }
  else if (!strcmp(option, "cfs_cp"))
  {
    return 7;
  }
  else if (!strcmp(option, "cfs_cat"))
  {
    return 8;
  }
  else if (!strcmp(option, "cfs_ln"))
  {
    return 9;
  }
  else if (!strcmp(option, "cfs_mv"))
  {
    return 10;
  }
  else if (!strcmp(option, "cfs_rm"))
  {
    return 11;
  }
  else if (!strcmp(option, "cfs_import"))
  {
    return 12;
  }
  else if (!strcmp(option, "cfs_export"))
  {
    return 13;
  }
  else if (!strcmp(option, "cfs_create"))
  {
    return 14;
  }
  else if (!strcmp(option, "cfs_exit"))
  {
    return 15;
  }
  else
  {
    /* else no valid option has been found */
    return -1;
  }


}


/* returns 1 if a character exists in a substring; else returns 0 */
int char_exists_in_string(const char* str, char x)
{
  uint len = strlen(str);
  int i = 0;
  for (; i < len; i++)
  {
    if (str[i] == x)
    {
      return 1;
    }
  }

  return 0;
}


/* returns 1 if the string given is a parameter (option) string; else returns 0
   i.e. for cfs_ls, -l is a parameter (option) string */
int is_parameter(const char* str)
{
  if (str == NULL)
  {
    return 0;
  }

  return (str[0] == '-' && str[1] != '\0');
}


/* returns 1 if the given string is a positive integer; else returns 0 */
int is_positive_integer(const char* str)
{
  int len = strlen(str);
  int i = 0;
  for (; i < len; i++)
  {
    if (str[i] < '0' || str[i] > '9')
    {
      return 0;
    }
  }

  return 1;
}


/* returns positive value if str has a character that does not exists in flags;
   else returns 0 */
int contains_unknown_flag(const char* str, const char* flags)
{
  int len = strlen(str);
  int number_of_flags = strlen(flags);

  int i = 1;
  for (; i < len; i++)
  {
    int j = 0;
    int counter = 0;
    for (; j < number_of_flags; j++)
    {
      if (str[i] == flags[j])
      {
        counter++;
      }
    }

    if (counter < 1)
    {
      return i;
    }
  }

  return 0;
}


/* returns non-negative value if the path given is absolute; else returns 0 */
int path_is_absolute(const char path[])
{
  return path[0] == '/';
}


/* removes and places the last name of a path to the destination string */
int extract_last_entity_from_path(char path[], char* destination_string)
{
  int len = strlen(path);
  int i = len - 1;

  /* clear the useless "/" */
  while (i >= 0 && path[i] == '/')
  {
    path[i] = 0;
    i--;
  }

  /* if we reach the beggining of the path, then something like "/" or "///"
     was given, which means we should go to the root directory */
  if (i < 0)
  {
    strcpy(destination_string, "/");
    return 1;
  }

  char reversed_string[MAX_BUFFER_SIZE] = {0};
  int k = 0;

  /* get the string inside an array, reversed */
  while (i >= 0 && path[i] != '/')
  {
    reversed_string[k++] = path[i--];
    path[i + 1] = 0;
  }

  i = 0;
  k--;
  while (k >= 0)
  {
    destination_string[i++] = reversed_string[k--];
  }


  return 1;
}


/* ask user if they want to perfrom an operation. Returns 1 if user wants, else
   returns 0. */
int get_approval(char* source, char* destination, const char* operation)
{
  char ask_option[MAX_BUFFER_SIZE] = {0};
  printf("Do you want to %s the entity \"%s\" to the destination \"%s\"? Enter Y for yes, or N for no.\n", operation, source, destination);
  fgets(ask_option, MAX_BUFFER_SIZE, stdin);

  char answer[MAX_BUFFER_SIZE] = {0};
  get_nth_string(answer, ask_option, 1);

  while (strcmp(answer, "Y") && strcmp(answer, "y") && strcmp(answer, "Yes") && strcmp(answer, "YES") && strcmp(answer, "yes") && strcmp(answer, "N") && strcmp(answer, "n") && strcmp(answer, "No") && strcmp(answer, "NO") && strcmp(answer, "no\n"))
  {
    printf("Unknown option entered. Please re-enter your option.\n");
    memset(ask_option, 0, MAX_BUFFER_SIZE);
    fgets(ask_option, MAX_BUFFER_SIZE, stdin);

    memset(answer, 0, MAX_BUFFER_SIZE);
    get_nth_string(answer, ask_option, 1);
  }

  if (!strcmp(answer, "N") || !strcmp(answer, "n") || !strcmp(answer, "No") || !strcmp(answer, "NO") || !strcmp(answer, "no"))
  {
    return 0;
  }

  return 1;
}


/* same as above, just one string instead of 2 */
int get_approval_2(char* string, const char* operation)
{
  char ask_option[MAX_BUFFER_SIZE] = {0};
  printf("Do you want to %s the entity \"%s\"? Enter Y for yes, or N for no.\n", operation, string);
  fgets(ask_option, MAX_BUFFER_SIZE, stdin);

  char answer[MAX_BUFFER_SIZE] = {0};
  get_nth_string(answer, ask_option, 1);

  while (strcmp(answer, "Y") && strcmp(answer, "Yes") && strcmp(answer, "YES") && strcmp(answer, "yes") && strcmp(answer, "y") && strcmp(answer, "N") && strcmp(answer, "No") && strcmp(answer, "No") && strcmp(answer, "no") && strcmp(answer, "n"))
  {
    printf("Unknown option entered. Please re-enter your option.\n");
    memset(ask_option, 0, MAX_BUFFER_SIZE);
    fgets(ask_option, MAX_BUFFER_SIZE, stdin);

    memset(answer, 0, MAX_BUFFER_SIZE);
    get_nth_string(answer, ask_option, 1);
  }

  if (!strcmp(answer, "N") || !strcmp(answer, "n") || !strcmp(answer, "No") || !strcmp(answer, "No") || !strcmp(answer, "no"))
  {
    return 0;
  }

  return 1;
}


/* returns non-zero value if the offset is the offset of the root directory;
   else returns 0 */
int is_root_offset(superblock* my_superblock, off_t offset)
{
  return my_superblock->root_directory == offset;
}



/* --------------------------------       GET FUNCTION PARAMETERS           ----------------------------------- */



/* pretty much self_explanatory */
int get_cfs_touch_parameters(const char buffer[], int* flag_a, int* flag_m)
{
  char str[MAX_BUFFER_SIZE] = {0};
  char flags[3] = {0};
  strcpy(flags, "am");

  int index = 2;
  while (get_nth_string(str, buffer, index) && is_parameter(str))
  {
    int exists = contains_unknown_flag(str, flags);
    if (exists)
    {
      printf("The parameter '%c' given in the option %s does not exist. Available options are -a and -m.\n", str[exists], str);
      return 0;
    }

    if (char_exists_in_string(str, 'a'))
    {
      *flag_a = 1;
    }
    if (char_exists_in_string(str, 'm'))
    {
      *flag_m = 1;
    }

    index++;
  }

  return 1;
}


/* pretty much self_explanatory */
int get_cfs_ls_parameters(const char buffer[], int* flag_a, int* flag_r, int* flag_l, int* flag_u, int* flag_d, int* flag_h)
{
  char str[MAX_BUFFER_SIZE] = {0};
  char flags[7] = {0};
  strcpy(flags, "arludh");

  int index = 2;
  while (get_nth_string(str, buffer, index) && is_parameter(str))
  {
    int exists = contains_unknown_flag(str, flags);
    if (exists)
    {
      printf("The parameter '%c' given in the option %s does not exist. Available options are -a, -r, -l, -u, -d and -h\n", str[exists], str);
      return 0;
    }
    if (char_exists_in_string(str, 'a'))
    {
      *flag_a = 1;
    }
    if (char_exists_in_string(str, 'r'))
    {
      *flag_r = 1;
    }
    if (char_exists_in_string(str, 'l'))
    {
      *flag_l = 1;
    }
    if (char_exists_in_string(str, 'u'))
    {
      *flag_u = 1;
    }
    if (char_exists_in_string(str, 'd'))
    {
      *flag_d = 1;
    }
    if (char_exists_in_string(str, 'h'))
    {
      *flag_h = 1;
    }

    index++;
  }

  return 1;
}


/* pretty much self_explanatory */
int get_cfs_cp_parameters(const char buffer[], int* flag_R, int* flag_i, int* flag_r)
{
  char str[MAX_BUFFER_SIZE] = {0};
  char flags[4] = {0};
  strcpy(flags, "Rir");

  int index = 2;
  while (get_nth_string(str, buffer, index) && is_parameter(str))
  {
    int exists = contains_unknown_flag(str, flags);
    if (exists)
    {
      printf("The parameter '%c' given in the option %s does not exist. Available options are -R, -i and -r.\n", str[exists], str);
      return 0;
    }

    if (char_exists_in_string(str, 'R'))
    {
      *flag_R = 1;
    }
    if (char_exists_in_string(str, 'i'))
    {
      *flag_i = 1;
    }
    if (char_exists_in_string(str, 'r'))
    {
      *flag_r = 1;
    }

    index++;
  }

  return 1;
}


/* pretty much self_explanatory */
int get_cfs_mv_parameters(const char buffer[], int* flag_i)
{
  char str[MAX_BUFFER_SIZE] = {0};
  char flags[2] = {0};
  strcpy(flags, "i");

  int index = 2;
  while (get_nth_string(str, buffer, index) && is_parameter(str))
  {
    int exists = contains_unknown_flag(str, flags);
    if (exists)
    {
      printf("The parameter '%c' given in the option %s does not exist. Only availabe option is -i.\n", str[exists], str);
      return 0;
    }

    if (char_exists_in_string(str, 'i'))
    {
      *flag_i = 1;
    }

    index++;
  }

  return 1;
}


/* pretty much self_explanatory */
int get_cfs_rm_parameters(const char buffer[], int* flag_i, int* flag_r)
{
  char str[MAX_BUFFER_SIZE] = {0};
  char flags[3] = {0};
  strcpy(flags, "ir");

  int index = 2;
  while (get_nth_string(str, buffer, index) && is_parameter(str))
  {
    int exists = contains_unknown_flag(str, flags);
    if (exists)
    {
      printf("The parameter '%c' given in the option %s does not exist. Available options are -i and -R.\n", str[exists], str);
      return 0;
    }
    if (char_exists_in_string(str, 'i'))
    {
      *flag_i = 1;
    }
    if (char_exists_in_string(str, 'r'))
    {
      *flag_r = 1;
    }

    index++;
  }

  return 1;
}


/* pretty much self_explanatory */
int get_cfs_create_parameters(const char buffer[], size_t* bs, size_t* fns, size_t* cfs, uint* mdfn, char* cfs_filename)
{
  char str[MAX_BUFFER_SIZE] = {0};

  /* flag to make sure no parameter is given twice or more */
  int flag_bs = 0;
  int flag_fns = 0;
  int flag_cfs = 0;
  int flag_mdfn = 0;

  /* initialize with default values */
  *bs = DEFAULT_BLOCK_SIZE;
  *fns = DEFAULT_FNS;
  *cfs = DEFAULT_CFS;
  *mdfn = DEFAULT_MDFN;

  /* get the parameters */
  int index = 2;
  int exists = 0;
  while ((exists = get_nth_string(str, buffer, index)) && is_parameter(str))
  {
    char number_as_string[MAX_DIGITS] = {0};
    get_nth_string(number_as_string, buffer, index + 1);
    if (!is_positive_integer(number_as_string))
    {
      printf("Wrong value for the %s parameter. Give a positive integer. Value given is: %s.\n", str, number_as_string);
      return 0;
    }

    if (!strcmp(str, "-bs"))
    {
      if (flag_bs == 1)
      {
        printf("Error: same string %s, given twice.\n", str);
        return 0;
      }

      flag_bs = 1;
      size_t block_size = atoi(number_as_string);
      *bs = block_size;
    }
    else if (!strcmp(str, "-fns"))
    {
      if (flag_fns == 1)
      {
        printf("Error: same string %s, given twice.\n", str);
        return 0;
      }

      flag_fns = 1;
      size_t filename_size = atoi(number_as_string);
      *fns = filename_size;
    }
    else if (!strcmp(str, "-cfs"))
    {
      if (flag_cfs == 1)
      {
        printf("Error: same string %s, given twice.\n", str);
        return 0;
      }

      flag_cfs = 1;
      size_t max_file_size = atoi(number_as_string);
      *cfs = max_file_size;
    }
    else if (!strcmp(str, "-mdfn"))
    {
      if (flag_mdfn == 1)
      {
        printf("Error: same string %s, given twice.\n", str);
        return 0;
      }

      flag_mdfn = 1;
      uint max_dir_file_number = atoi(number_as_string);
      *mdfn = max_dir_file_number;
    }
    else
    {
      printf("Wrong parameter given. Available parameters are: -bs, -fns, -cfs, mfdn,\n");
      return 0;
    }

    index += 2;
  }

  if (!exists)
  {
    printf("No name was given for the cfs file.\n");
    return 0;
  }

  if (get_nth_string(str, buffer, index + 1))
  {
    printf("Wrong input: Only 1 cfs file can be created at a time.\n");
    return 0;
  }


  /* get the filename */
  get_nth_string(cfs_filename, buffer, index);

  return 1;
}






/* ---------------------------------   NAVIGATORS   ----------------------------------------- */


/* function used to navigate through directory data blocks */
off_t* pointer_to_offset(char* pointer, size_t fns)
{
  char* name = pointer;
  name += fns;

  off_t* return_address = (off_t *) name;

  return return_address;
}


/* function used to navigate through directory data blocks */
char* pointer_to_next_name(char* pointer, size_t fns)
{
  off_t* offset = pointer_to_offset(pointer, fns);
  offset++;

  char* return_address = (char *) offset;

  return return_address;
}






/* ----------------------------  INITIALIZATION FUNCTIONS  --------------------------------- */


void initialize_superblock(superblock* my_superblock, char* cfs_filename, int fd, off_t root_directory_offset, size_t current_size, size_t bs, size_t fns, size_t cfs, uint mdfn)
{
  my_superblock->total_entities = 1;
  my_superblock->fd = fd;
  my_superblock->root_directory = root_directory_offset;
  my_superblock->current_size = current_size;
  my_superblock->block_size = bs;
  my_superblock->filename_size = fns;
  my_superblock->max_file_size = cfs;
  my_superblock->max_dir_file_number = mdfn;
  strcpy(my_superblock->cfs_filename, cfs_filename);
}



void initialize_holes(hole_map* holes, uint n, uint current_holes, off_t hole_start)
{
  holes->current_hole_number = current_holes;
  holes->holes_table[0].start = hole_start;
  holes->holes_table[0].end = 0;

  int i = 1;
  for (; i < n; i++)
  {
    holes->holes_table[i].start = 0;
    holes->holes_table[i].end = 0;
  }
}



void initialize_MDS(MDS* mds, uint id, uint type, uint number_of_hard_links, uint blocks_using, size_t size, off_t parent_offset, off_t first_block)
{
  mds->id = id;
  mds->type = type;
  mds->number_of_hard_links = number_of_hard_links;
  mds->blocks_using = blocks_using;

  mds->size = size;
  mds->parent_offset = parent_offset;
  mds->first_block = first_block;

  time_t my_time = time(NULL);
  mds->creation_time = my_time;
  mds->access_time = my_time;
  mds->modification_time = my_time;
}



void initialize_data_Block(Block* block, size_t block_size)
{
  // block->next_block = 0;
  // block->bytes_used = 0;
  // size_t size_for_data = block_size - sizeof(Block);
  // memset(block->data, 0, size_for_data);
  memset(block, 0, block_size);
}



void initialize_Directory_Data_Block(Block* block, size_t block_size, size_t fns, off_t self_offset, off_t parent_offset)
{
  initialize_data_Block(block, block_size);

  char* name = (char *) block->data;
  off_t* offset = pointer_to_offset(name, fns);

  strcpy(name, ".");
  *offset = self_offset;

  name = pointer_to_next_name(name, fns);
  offset = pointer_to_offset(name, fns);

  strcpy(name, "..");
  *offset = parent_offset;

  /* we have currently stored 2 directories (./ and ../, and their offsets) */
  block->bytes_used = 2 * fns + 2 * sizeof(off_t);
}






/* --------------------------------  FAST ACCESS  --------------------------------------- */


superblock* get_superblock(int fd)
{
  superblock* my_superblock = NULL;

  MALLOC_OR_DIE_2(my_superblock, sizeof(superblock));

  LSEEK_OR_DIE_2(fd, 0, SEEK_SET, my_superblock);

  READ_OR_DIE(fd, my_superblock, sizeof(superblock));

  return my_superblock;
}


hole_map* get_hole_map(int fd)
{
  hole_map* holes = NULL;

  MALLOC_OR_DIE_2(holes, sizeof(hole_map));

  LSEEK_OR_DIE_2(fd, sizeof(superblock), SEEK_SET, holes);

  READ_OR_DIE(fd, holes, sizeof(hole_map));

  return holes;
}


MDS* get_MDS(int fd, off_t offset)
{
  if (offset == 0)
  {
    printf("Wrong offset give in get_MDS().\n");
    return NULL;
  }

  MDS* mds = NULL;

  MALLOC_OR_DIE_2(mds, sizeof(MDS));

  LSEEK_OR_DIE_2(fd, offset, SEEK_SET, mds);

  READ_OR_DIE(fd, mds, sizeof(MDS));

  return mds;
}


Block* get_Block(int fd, size_t block_size, off_t offset)
{
  if (offset == 0)
  {
    printf("Wrong offset given in get_Block().\n");
    return NULL;
  }

  Block* block = NULL;

  MALLOC_OR_DIE_2(block, block_size);

  LSEEK_OR_DIE_2(fd, offset, SEEK_SET, block);

  READ_OR_DIE(fd, block, block_size);

  return block;
}





/* ----------------------------------  FAST SET  --------------------------------------- */


int set_superblock(superblock* superblock, int fd)
{
  DIE_IF_NULL(superblock);

  LSEEK_OR_DIE(fd, 0, SEEK_SET);

  WRITE_OR_DIE_2(fd, superblock, sizeof(superblock));

  return 1;
}


int set_hole_map(hole_map* holes, int fd)
{
  DIE_IF_NULL(holes);

  LSEEK_OR_DIE(fd, sizeof(superblock), SEEK_SET);

  /* update only the non-zero holes */
  size_t size_taken = sizeof(long unsigned int) + holes->current_hole_number * sizeof(hole);
  WRITE_OR_DIE_2(fd, holes, size_taken);

  return 1;
}


int set_MDS(MDS* mds, int fd, off_t offset)
{
  DIE_IF_NULL(mds);

  LSEEK_OR_DIE(fd, offset, SEEK_SET);

  WRITE_OR_DIE_2(fd, mds, sizeof(MDS));

  return 1;
}


int set_Block(Block* block, int fd, size_t block_size, off_t offset)
{
  DIE_IF_NULL(block);

  LSEEK_OR_DIE(fd, offset, SEEK_SET);

  WRITE_OR_DIE_2(fd, block, block_size);

  return 1;
}





/* --------------------------------  PRINT FUNCTIONS --------------------------------------- */


void print_superblock(superblock* my_superblock)
{
  printf("\n\nSUPERBLOCK\n\n");
  printf("fd = %d\n", my_superblock->fd);
  printf("cfs_filename = %s\n", my_superblock->cfs_filename);
  printf("total_entities = %u\n", my_superblock->total_entities);
  printf("root_directory = %lu\n", my_superblock->root_directory);
  printf("current_size = %lu\n", my_superblock->current_size);
  printf("bs = %ld\n", my_superblock->block_size);
  printf("fns = %lu\n", my_superblock->filename_size);
  printf("cfs = %lu\n", my_superblock->max_file_size);
  printf("mdfn = %u\n", my_superblock->max_dir_file_number);
}



void print_hole_table(hole_map* holes)
{
  printf("\n\nHOLE TABLE\n\n");
  printf("current_hole_number = %lu\n\n", holes->current_hole_number);

  int i = 0;
  for (; i < MAX_HOLES; i++)
  {
    printf("start = %lu, end = %lu\n", holes->holes_table[i].start, holes->holes_table[i].end);

    if (holes->holes_table[i].end == 0)
    {
      return;
    }
  }
}



void print_MDS(MDS* mds)
{
  printf("\n\nMDS\n\n");
  printf("id = %u\n", mds->id);
  printf("size = %lu\n", mds->size);
  printf("type = %u\n", mds->type);
  printf("number_of_hard_links = %u\n", mds->number_of_hard_links);
  printf("parent_offset = %lu\n", mds->parent_offset);
  printf("creation_time = %lu\n", mds->creation_time);
  printf("access_time = %lu\n", mds->access_time);
  printf("modification_time = %lu\n", mds->modification_time);
  printf("blocks_using = %u\n", mds->blocks_using);
  printf("first_block = %lu\n", mds->first_block);
}



void print_Directory_Data_Block(Block* block, size_t fns)
{
  printf("\n\nBLOCK\n\n");

  size_t size_of_pair = fns + sizeof(off_t);
  uint pairs = block->bytes_used / size_of_pair;


  char* name = (char *) block->data;
  off_t* offset = pointer_to_offset(name, fns);

  int i = 0;
  for (; i < pairs; i++)
  {
    printf("Directory name: %s, offset: %lu\n", name, *offset);

    name = pointer_to_next_name(name, fns);
    offset = pointer_to_offset(name, fns);
  }
}
