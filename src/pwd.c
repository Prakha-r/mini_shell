//this code is highly inspired by the pwd implementation of linux coreutils a lot of similarities are found

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include "pwd.h"

struct _file_path{
  char * buffer;
  size_t buffer_size;
  char * start;
};

int pwd_cmd(int argc, char ** args){
  file_path *p = file_path_init();
  getcwd_s(p);
  puts(get_path(p));
  free_and_null_path(p);
  return 0;
}

file_path *file_path_init(void){
  file_path * path =(file_path*)malloc(sizeof(file_path));
  if(path==NULL){
    return (file_path*)NULL;
  }
  path->buffer_size = 1026; // create a big size buffer to store the path 
  path->buffer = malloc(path->buffer_size); // allocate the memory 
  path->start = path->buffer + path->buffer_size - 1; // now the start is pointing at the end of the buffer, this so that we can add the file path from the back 
  path->start[0] = '\0';
  return path;
}

void free_and_null_path(file_path * path){
  free(path->buffer);
  free(path);
  path = NULL;
}

void add_file_name (file_path *path, char * parent_name, size_t parent_size){
  if(path==NULL){
    return;
  }
  path->start -= parent_size;
  path->start = memcpy(path->start  , parent_name, parent_size ); // now the start is pointing to the start of the name of the file
  path->start -= 1;
  path->start[0] = '/';
}

struct dirent * readdir_and_ignore(DIR * dir_p){
  struct dirent *dp;
  while((dp = readdir(dir_p)) != NULL){
    if(dp->d_name[0] == '.'){
      if(dp->d_name[1] == '\0' )
        continue;
      if(dp->d_name[1] == '.' && dp->d_name[2] == '\0')
        continue;
    }
    return dp;
  }
  return dp;
}

void go_up_add (struct stat * child, file_path * path){
  DIR * dir_p = opendir("..");
  if(dir_p == NULL){
    fprintf(stderr, "error opening directory\n");
    return;
  }
  int fd = dirfd(dir_p); //dirfd returns the file descriptor of the currently open file and on error returns -1
  //this makes it more reliable to use functions like fchdir as it depend on the file descriptor not the name
  if (fd >= 0 ? fchdir(fd):chdir("..")<0){  // here we are using a fallback mechanism to insure if dirfd doesn't work we still change the directory
    fprintf(stderr, "error changing directory\n");
    return;
  }
  
  struct stat parent; 
  if(fd >= 0 ? fstat(fd, &parent):stat("..", &parent)<0){
    fprintf(stderr, "error getting the stat\n");
    return;
  }
  ino_t ino; 
  bool found = false;
  while(true){
    struct dirent *dp;
    if((dp = readdir_and_ignore(dir_p)) == NULL){
      closedir(dir_p);
      dir_p = NULL;
      break;
    }
   // bool dev_no = (parent.st_dev == child->st_dev)  //if the parent and the child are on the same filesystem then there would be no clas on the inode no, but if the filesytem are different for both then there could be clash 
   // if(dev_no){
   //   ino =dp->d_ino;
   // }
    struct stat entity;
    if(lstat(dp->d_name, &entity)< 0){
      continue;
    }
    ino = entity.st_ino;

    if(ino != child->st_ino)
      continue;
    
    if(entity.st_dev == child->st_dev)
    {
      found = true;
      add_file_name( path , dp->d_name, strlen(dp->d_name));
      break;
    }

  }
  if(!found){
    fprintf(stderr, "cannot found\n");
    return;
  }
  *child = parent; 
}

void getcwd_s (file_path * path){
  DIR * dirp = opendir(".");
  int fd = dirfd(dirp);
  struct stat child, parent;
  if(stat(".", &child) <0 ){
    fprintf(stderr, "error getting stat\n");
    return;
  }
  while(true){
    if(stat("..", &parent) <0 ){
      fprintf(stderr, "error getting stat\n");
      return;
    }
    if(child.st_ino == parent.st_ino && child.st_dev == parent.st_dev)
      break;
    go_up_add(&child, path); 
  }
  if (fchdir(fd) != 0) {
    perror("fchdir");
  }
  close(fd);
}

char * get_path(file_path * path){
  return path->start;
}

