#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>    
#include <unistd.h>  
#include <string.h>
#include <stdbool.h>
#include "redirection.h"
#include "parser.h"

/* The > operator redirects the standard output of a command to a file. When you use >, the output that would normally appear on the terminal is instead written to the specified file.

If the file doesn't exist, it is created. If the file already exists, it is overwritten (replacing its old contents). */
int saved_stdout;
int saved_stderr;
void restore_redirect(){
  dup2(saved_stdout, STDOUT_FILENO);
  dup2(saved_stderr, STDERR_FILENO);
  close(saved_stdout);
  close(saved_stderr);
}

void redirect_output(CMD * cmd){
  //find the redirect symbol
  saved_stdout = dup(STDOUT_FILENO);
  saved_stderr = dup(STDERR_FILENO);
  int fp;
  for(int i = 0; i<cmd->redirect_no; i++){
    if(cmd->redirector[i].append == true){
      if((fp = open(cmd->redirector[i].filename, O_WRONLY|O_CREAT|O_APPEND, 0644))==-1){
        fprintf(stderr, "error creating the file \n");
        return;
      }
    }
    else{
      if((fp = open(cmd->redirector[i].filename, O_WRONLY|O_CREAT|O_TRUNC, 0644))==-1){
        fprintf(stderr, "error creating the file \n");
        return;
      }
    }
    if(cmd->redirector[i].type == STD_OUT){
      if(dup2(fp, STDOUT_FILENO) < 0){
        fprintf(stderr, "error redirecting the file descriptor\n");
        return;
      }
    }
    else if(cmd->redirector[i].type == STD_ERR){
      if(dup2(fp, STDERR_FILENO) < 0){
        fprintf(stderr, "error redirecting the file descriptor\n");
        return;
      }
    }
    close(fp);
  }
}

