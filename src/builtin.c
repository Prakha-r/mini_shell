#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "cd.h"
#include "pwd.h"
#include "echo.h"
#include "type.h"
#include "exit.h"
#include "builtin.h"

static hash_map *builtin_map;

void init_builtin(){
  builtin_map = initialization();
  set_builtin( builtin_map, "cd", cd_cmd );
  set_builtin( builtin_map, "pwd", pwd_cmd );
  set_builtin( builtin_map, "echo", echo_cmd );
  set_builtin( builtin_map, "type", type_cmd );
  set_builtin( builtin_map, "exit", exit_cmd );
}

int is_builtin(char *cmd_name){
  if(search_builtin(builtin_map, cmd_name) == NULL){
    return -1; // not found
  }
  return 0;
}

int run_builtin(int argc, char ** args){
  hash_node * node = search_builtin(builtin_map, args[0]);
  if(node == NULL){
    return -1;
  }
  return execute_func(node, argc, args);  
}


