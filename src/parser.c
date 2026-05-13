#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "parser.h"

// change this into a tokenizer and create a struct that stores the informations

void reset_cmd(CMD **cmd){
  if(*cmd != NULL) {
    free((*cmd)->redirector);
    free(*cmd);
    *cmd = NULL;  // now actually nulls the caller's pointer
  }
}

CMD * parser(char *command_t, size_t command_size){
  char *tokens_list[128];
  int redirect_no_ = 0;
  CMD * cmd = (CMD*)calloc(1, sizeof(CMD));
  cmd->redirector =(redirect *)calloc(128, sizeof(redirect));
  cmd->argn = 0;
  int token = tokenize_cmd(command_t, cmd->buffer, tokens_list);
  if(token == -1){
    printf("> ");
    int len = strlen(command_t);
    command_t[len] = '\n';
    if(fgets(command_t + len + 1 , command_size - len - 1, stdin) != NULL){  //Retains newline character.
      command_t[strlen(command_t) - 1 ] = '\0';
    }
    memset(cmd->buffer, 0, sizeof(cmd->buffer));
    token = tokenize_cmd(command_t, cmd->buffer, tokens_list);
  }
  tokens_list[token] = NULL;
  for(int i=0; i<token; i++){
    if(strcmp(tokens_list[i], ">")==0 || strcmp(tokens_list[i], "1>")==0 || strcmp(tokens_list[i], ">>") == 0){
      //redirect standard output
      if(strcmp(tokens_list[i], ">>") == 0){
        cmd->redirector[redirect_no_].append = true;
      }
      cmd->redirector[redirect_no_].type = STD_OUT;
      cmd->redirector[redirect_no_].filename = tokens_list[++i];
      redirect_no_++;
    }
    else if(strcmp(tokens_list[i], "2>")==0 || strcmp(tokens_list[i], "2>>") == 0){
      if(strcmp(tokens_list[i], "2>>") == 0){
        cmd->redirector[redirect_no_].append = true;
      }
      cmd->redirector[redirect_no_].type = STD_ERR;
      cmd->redirector[redirect_no_].filename = tokens_list[++i];
      redirect_no_++;
    }
    else{
      cmd->args[cmd->argn++] = tokens_list[i];
    }
  }
  cmd->redirect_no = redirect_no_;
  return cmd;
}

int tokenize_cmd(char *command, char * buffer, char *tokens_list[]) {
  bool single_qoute = false;
  bool double_qoute = false;
  bool in_token = false;
  char c;
  int j = 0, k = 0, token = 0;
  for(int i = 0; i <= strlen(command); i++)
  {
    c = command[i];
    if(c == '\'' && !double_qoute){
      single_qoute = !single_qoute;
      continue;
    }
    else if (c == '\"' && !single_qoute) {
      double_qoute = !double_qoute;
      continue;
    }
    else{
      if((c == ' ' || c == '\0') && !double_qoute && !single_qoute){
        if(in_token){
          buffer[j++] = '\0';
          tokens_list[token++] =  buffer + k;  
          k = j;
          in_token = false;
        }
      }
      else{
        if(c == '\\' && !single_qoute){
          char next = command[i+1];
          if(double_qoute){
            // inside double quotes: only escape special chars
            if(next == '"' || next == '\\' || next == '$' || next == '`' || next == '\n'){
              buffer[j++] = command[++i];
            }else if(next == 'n'){
              buffer[j++] = '\n';
              i++;
            } else {
              // backslash is literal, keep it
              buffer[j++] = c;
            }
          } else {
            // outside quotes: backslash escapes anything
            if(next != '\0'){
              buffer[j++] = command[++i];
            }
          }
          in_token = true;
        }
        else{
          buffer[j++] = c;
          in_token = true;  // The in_token keeps the track of the spaces if its true that means insert and when the first space is met it creates the token there makes the args[i] to point the token and after that every space is ignored as the in_token is false and it eill only become zero when another character is met and is added to the buffer
        }
      }
    }
  }
  if(single_qoute || double_qoute)
     token = -1;
  return token;
}


