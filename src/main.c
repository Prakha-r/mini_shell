#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "matcher.h"

#ifdef _WIN32
    #define PATH_DELIMITER ';'
#else
    #define PATH_DELIMITER ':'
#endif

int type_path(char *m_ptr,char *out_path){
  int found_exe = 0;
  char *path_env = getenv("PATH");
  if(!path_env){
    fprintf(stderr, "getenv error \n");
    return found_exe;
  }
  char *path_env_cpy = strdup(path_env);
  char * path = strtok(path_env_cpy,(char[]){PATH_DELIMITER, '\0'}); //this seperates the string based on the delimeter 
  while (path)
  {
    DIR *d;
    struct dirent *dir;
    struct stat info;
    char file_path[FILENAME_MAX];
    char *exe_name = NULL;
    d = opendir(path);
    if(d!= NULL){
      while((dir = readdir(d))!= NULL){
        if(strcmp(dir->d_name,".")==0||strcmp(dir->d_name,"..")==0){
          continue;
        }
        char *dot = memchr(dir->d_name, '.', strlen(dir->d_name));
        if(dot!= NULL){
          matcher_output *match = get_till_char(dir->d_name, '.', strlen(dir->d_name));
          if(match){
            if(!match->output_result){
              fprintf(stderr, "not found\n");
              free(path_env_cpy);
              closedir(d);
              return found_exe;
            }
          }
          else{
            fprintf(stderr, "matcher error\n");
            free(path_env_cpy);
            closedir(d);
            return found_exe;
          }
          exe_name = malloc(strlen(match->output_result)+1);
          if(!exe_name){
            fprintf(stderr, "memory allocation error\n");
            free(path_env_cpy);
            return found_exe;
          }
          if(!strcpy(exe_name,match->output_result)){
            fprintf(stderr, "error copying\n");
            free(path_env_cpy);
            closedir(d);
            return found_exe;
          }
          free_and_null(&match);
        }
        else{
          exe_name = malloc(strlen(dir->d_name)+1);
          if(!strcpy(exe_name,dir->d_name)){
            fprintf(stderr, "error copying\n");
            free(path_env_cpy);
            closedir(d);
            return found_exe;
          }
        }
        if(strcmp(m_ptr +strcspn(m_ptr, " ")+1, exe_name) == 0 && strlen(m_ptr +strcspn(m_ptr, " ")+1)==strlen(exe_name)){
          sprintf(file_path, "%s/%s",path,dir->d_name);
          if(stat(file_path,&info)!=1){
            if (info.st_mode & S_IXUSR) {
              strcpy(out_path,file_path);
              found_exe =1;
              break;
            }
          }
        }
        if (exe_name) {
          free(exe_name);
          exe_name = NULL;
        }
      }
    }
    closedir(d);
    if(found_exe){
      break;
    }
    path = strtok(NULL,(char[]){PATH_DELIMITER, '\0'}); //here we used null as when we use strtok the null means that start from the previously left place
  }
  free(path_env_cpy);
  return found_exe;
}

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);
  const char * builtins[100] ={"exit", "echo", "type"};
  do
  {
    printf("$ ");
    char command[256];
    if(fgets(command, sizeof(command), stdin) != NULL){  //Retains newline character.
      command[strcspn(command, "\n")] = '\0';
      if(strncmp(command, "exit", 4)==0){
        if((int)command[strcspn(command, " ")+1]-48 == 0){ // as the ascii equivalent of character 0 is 48 
          exit(0);
        }
          exit(1);
      }
      else if(strncmp(command, "echo", 4)== 0){
        printf("%s\n", command +strcspn(command, " ")+1);
      }
      else if(strncmp(command, "type", 4)== 0){
        int length = sizeof(builtins) / sizeof(builtins[0]);
        int found = 0;
        char * m_ptr = command;
        for(int i =0;i<length&&builtins[i]!=NULL;i++){
          if(strcmp(m_ptr +strcspn(m_ptr, " ")+1, builtins[i]) == 0  ){
            found = 1;
            break;
          }
        }
        if(found == 0){
          char path[FILENAME_MAX]={0};
          int found_exe = type_path(m_ptr,path);
          if(found_exe &&found == 0){
            printf("%s is %s\n", m_ptr+strcspn(m_ptr, " ")+1, path);
          }
          else{
            fprintf(stderr, "%s: not found\n", m_ptr+strcspn(m_ptr, " ")+1);
          }
        }
        else{
          printf("%s is a shell builtin\n", m_ptr+strcspn(m_ptr, " ")+1);
        }
      }
      else{
        fprintf(stderr, "%s: command not found\n", command);
      }
    }
  }while(true);
  return 0;
}
