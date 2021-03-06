#ifndef __FUNCTIONS_UTIL__
#define __FUNCTIONS_UTIL__

#include "structs.h"
#include "list.h"


void insert_pair_into_block(Block* block, char* insert_name, off_t insert_offset, size_t fns);
int insert_pair(int fd, hole_map* holes, MDS* mds, char* insert_name, off_t insert_offset, size_t block_size, size_t fns);
int shift_pairs_to_the_left(char* name, uint remaining_pairs, size_t size_of_pair, size_t fns);
int remove_pair(Block* block, char* remove_name, size_t fns);
int directory_data_block_Is_Full(Block* block, size_t block_size, size_t fns);
int directory_data_block_Is_Empty(Block* block);
off_t get_offset(Block* block, char* target_name, size_t fns);

off_t find_hole(hole_map* holes, size_t my_size);
int shift_holes_to_the_left(hole_map* holes, uint hole_position);
int shift_holes_to_the_right(hole_map* holes, uint hole_position);

uint number_of_sub_entities_in_directory(MDS* current_directory, size_t fns);
off_t directory_get_offset(int fd, MDS* directory, size_t block_size, size_t fns, char* target_name);
int name_exists_in_directory(int fd, MDS* directory, size_t block_size, size_t fns, char* target_name);
off_t get_offset_from_path(int fd, superblock* my_superblock, Stack_List* list, char original_path[]);
int get_legit_name_from_path(int fd, superblock* my_superblock, Stack_List* list, char original_path[], char* legit_name);

int copy_from_linux_to_cfs(int fd, superblock* my_superblock, hole_map* holes, MDS* imported_file, int linux_file_fd, size_t linux_file_size);
int copy_from_cfs_to_linux(int fd, superblock* my_superblock, MDS* source, int linux_file_fd);

int remove_MDS_blocks(int fd, superblock* my_superblock, hole_map* holes, MDS* remove_entity);
int remove_pair_from_directory(int fd, superblock* my_superblock, hole_map* holes, MDS* parent_entity, off_t parent_offset, off_t remove_offset);

#endif
