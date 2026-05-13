#include <stdio.h>
#include <stdlib.h>
#include "echo.h"

int echo_cmd(int argn, char ** args){
  //needs some inprovement '-e' '-n' support
  // fprintf(stderr, "test err\n");
  for(int i = 1; i<argn; i++){
    printf("%s ", args[i]);
  }
  printf("\n");
  return 0;
}
