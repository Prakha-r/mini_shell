#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "cd.h"

enum _path_type{
  ABS_P,
  REAL_P,
  HOME_P
};

int cd_cmd(int argc, char **args){
  if(args[1] == NULL){
    fprintf(stderr, "cd: no such file or directory\n");
    return 1;
  }
  if(args[1][0] == '.'){
    _change_dir(args[1], REAL_P);
  }
  else if(args[1][0] == '~'){
    _change_dir(args[1], HOME_P);
  }
  else{
    _change_dir(args[1], ABS_P);
  }
  return 0;
}

void _change_dir(const char * path, path_type p){
  if(path == NULL)
    return ;
  switch (p) {
    case ABS_P:
      if(chdir(path)!=0){
        fprintf(stderr, "cd: no such file or directory: %s\n", path);
      }
      break;
    case REAL_P:
      char resolved_path[FILENAME_MAX];
      char *abs_path = NULL;
      if((abs_path = realpath(path, resolved_path)) != NULL){
        if(chdir(resolved_path)!=0){
          fprintf(stderr, "cd: no such file or directory: %s\n", resolved_path);
        }
      }
      break;
    case HOME_P:
      char * home = getenv("HOME");
      if(chdir(home)!=0){
        fprintf(stderr, "cd: no such file or directory: %s\n", home);
      }
      break;
    default:
        fprintf(stderr, "cd: no such file or directory: %s\n", path);
  }
  return ;
}

