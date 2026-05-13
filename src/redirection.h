#ifndef REDIRECTION_H
#define REDIRECTION_H
#include "parser.h"

void restore_redirect();
void redirect_output(CMD * cmd);

#endif // !REDIRECTION_H
