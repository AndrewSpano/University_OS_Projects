#ifndef __FUNCTIONS2__
#define __FUNCTIONS2__

#include "structs.h"
#include "macros.h"
#include "list.h"


int get_nth_pair(MDS* mds, char** name, off_t* offset, int fd, int n);
int get_type(int fd, off_t offset);
int print_characteristics(int fd, off_t offset);
int cfs_ls(int fd, off_t offset, int flag_a, int flag_r, int flag_l, int flag_u, int flag_d, int flag_h);
int insert_hole(hole_map* holes, off_t my_start, off_t my_end, int fd);




#endif
