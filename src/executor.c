#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "executor.h"

#ifdef _WIN32
#define PATH_DELIMITER ';'
#else
#define PATH_DELIMITER ':'
#endif

int find_executable(char *cmd_name, char *out_path){
  char *path_env = getenv("PATH");
  if(!path_env){
    fprintf(stderr, "getenv error \n");
    return -1;
  }
  char *path_env_cpy = strdup(path_env);
  char * path = strtok(path_env_cpy,(char[]){PATH_DELIMITER, '\0'}); //this seperates the string based on the delimeter 
  while (path)
  {
    char full_path[FILENAME_MAX] = {0};
    snprintf(full_path, sizeof(full_path), "%s/%s", path, cmd_name);
    if (access(full_path, X_OK) == 0) {
      strcpy(out_path, full_path);
      free(path_env_cpy);
      return 0;
    }

    path = strtok(NULL,(char[]){PATH_DELIMITER, '\0'});
  }
  free(path_env_cpy);
  return -1;
}

void execute_path(char *path, char **args){
  pid_t pid = fork();  // Create a new process
  if (pid == 0) {
    execv(path, args);
    perror("exec failure");
  } else if(pid>0){
    waitpid(pid, NULL, 0);
  } else{
    perror("fork failed");
    exit(EXIT_FAILURE);
  }
}

