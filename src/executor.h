#ifndef EXECUTOR_H
#define EXECUTOR_H

int find_executable(char *cmd_name, char *out_path);
void execute_path(char *path, char **args);

#endif // !EXECUTOR_H
