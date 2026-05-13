#ifndef CD_H
#define CD_H

typedef enum _path_type path_type;

int cd_cmd(int argc, char **args);
void _change_dir(const char * path, path_type p);

#endif // !CD_H
