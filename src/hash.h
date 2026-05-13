#ifndef HASH_H
#define HASH_H

typedef struct _hash_node hash_node;
typedef struct _hash_map hash_map;

hash_map * initialization(void);
int hash_function(char * cmd_name);
void set_builtin(hash_map *map, char * cmd_name, int (*func)(int, char **));
hash_node * search_builtin(hash_map *map, char * cmd_name);
int execute_func(hash_node * node, int argn, char ** args);

#endif // !HASH_H
