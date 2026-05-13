#ifndef PARSER_H
#define PARSER_H
#include <stdbool.h>

typedef enum redirect_to_{
  STD_OUT,
  STD_ERR,
  APPEND
}redirect_to;

typedef struct redirect_{
  redirect_to type;
  bool append;
  char * filename;
}redirect;

typedef struct command_ {
  char *args[128];
  char buffer[1024];
  int argn;
  redirect *redirector;
  int redirect_no; 
}CMD;

void reset_cmd(CMD ** cmd);
CMD * parser(char *command_t, size_t command_size);
int tokenize_cmd(char *command, char * buffer, char *args[]);

#endif // !PARSER_H
