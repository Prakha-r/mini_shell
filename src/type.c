#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "executor.h"
#include "builtin.h"

int type_cmd(int argc, char **args){
  if(is_builtin(args[1]) != -1){
    printf("%s is a shell builtin\n", args[1]);
    return 0;
  } 
  else{
    char out_path[FILENAME_MAX] = {0};
    if(find_executable(args[1], out_path) < 0){
      fprintf(stderr, "%s: not found\n", args[1]);
      return -1;
    }
    else{
      printf("%s is %s\n", args[1], out_path);
      memset(out_path, 0, sizeof(out_path)); 
      return 0;
    }
  }
}
