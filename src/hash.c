#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

struct _hash_node{
  char *key; // cmd_name  
  int (*func)(int, char **);
  hash_node *next;
};

struct _hash_map{
  int no_elements;
  int capacity;
  hash_node ** arr;  // array of hash node 
};

hash_map * initialization(void){
  hash_map * map = (hash_map *)calloc(1,sizeof(hash_map));
  map->capacity = 100;
  map->no_elements = 0;
  map->arr = (hash_node **)calloc(map->capacity, sizeof(hash_node*));
  return map;
}


int hash_function(char * cmd_name){
  int index = 0;
  for(int i = 0; i<strlen(cmd_name); i++){
    index = (index * 31 + cmd_name[i]) % 100;
  }
  return index;
}

void set_builtin(hash_map * map, char *cmd_name, int (*func)(int, char**)){
  if(map->no_elements >= map->capacity){
    fprintf(stderr, "no capacity in the hash table\n");
    return;
  }
  int index = hash_function(cmd_name);
  hash_node * new_node =  (hash_node *)malloc(sizeof(hash_node));
  new_node->key = malloc(strlen(cmd_name)+1);
  strcpy(new_node->key, cmd_name);
  new_node->func = func;
  new_node->next = NULL;
  if(map->arr[index] == NULL){
    map->arr[index] = new_node;
  }
  else{
    new_node->next = map->arr[index];
    map->arr[index] = new_node;
  }
  map->no_elements++;

}

hash_node * search_builtin(hash_map * map, char * cmd_name){
  int index = hash_function(cmd_name);
  hash_node *node = map->arr[index];
  while(node != NULL){
    if(strcmp(node->key, cmd_name) == 0){
        return node;
    }
    node = node->next;
  }
  return NULL;
}

int execute_func(hash_node * node, int argn, char ** args){
  return node->func(argn, args);
}
