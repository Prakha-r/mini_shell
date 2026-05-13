#ifndef PWD_H
#define PWD_H

#include <sys/stat.h>
#include <dirent.h>

typedef struct _file_path file_path;

int pwd_cmd(int argn, char ** args);
file_path *file_path_init(void);
void free_and_null_path(file_path *path);
void add_file_name (file_path *path, char * parent_name, size_t parent_size);
struct dirent * readdir_and_ignore(DIR * dir_p);
void go_up_add (struct stat * child, file_path * path);
void getcwd_s (file_path * path);
char * get_path(file_path * path);

#endif // !PWD_H
