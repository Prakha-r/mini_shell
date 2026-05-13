#include <stdio.h>
#include <stdlib.h>

int exit_cmd(int argn, char **args){
  if(args[1] == NULL){
    exit(0);
  }
  else{
    int status = atoi(args[1]);
    exit(status);
  }
  return 0;
}
