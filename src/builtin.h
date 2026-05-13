#ifndef BUILTIN_H
#define BUILTIN_H


void init_builtin();
int is_builtin(char *cmd_name);
int run_builtin(int argc, char ** args);

#endif // !BUILTIN_H
