#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "parser.h"
#include "builtin.h"
#include "executor.h"
#include "redirection.h"

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);
  init_builtin();
  CMD *cmd = NULL;
  while(true)
  {
    printf("$ ");
    char command[256] = {0};
    if(fgets(command, sizeof(command), stdin) != NULL){  //Retains newline character. Also adds a null terminator at the end
      command[strlen(command) - 1 ] = '\0';
      cmd = parser(command,sizeof(command));
      if(cmd->redirect_no > 0){
        redirect_output(cmd);
      } 
      if(run_builtin(cmd->argn, cmd->args) == -1){
        char out_path[FILENAME_MAX] = {0};
        if(find_executable(cmd->args[0], out_path)!=-1){
          execute_path(out_path, cmd->args);
        }
        else{
          fprintf(stderr, "%s: command not found\n", cmd->args[0]);
        }
      }
    }
    if(cmd->redirect_no > 0){
      restore_redirect();
    } 
    reset_cmd(&cmd);
  }
  return 0;
}
