#include <stdio.h>
#include <stdlib.h>
#include "matcher.h"
#include <string.h>

void free_and_null(matcher_output ** matcher){
  if(matcher!=NULL && (*matcher)){
    if((*matcher)->output_result!=NULL){
      free((*matcher)->output_result);
      (*matcher)->output_result = NULL;
    }
    (*matcher)->ptr = NULL;
    (*matcher)->output_found = 0;
    free(*matcher);
    *matcher= NULL;  
  }

  else{
    printf("NOTHING TO FREE\n");
  }
  return;
}

matcher_output *get_till_char(char *source, int till, size_t source_size){
  if(source==NULL) return NULL;
  matcher_output * result = (matcher_output*)malloc(sizeof(matcher_output));
  if(result == NULL) {
    fprintf(stderr, "memory allocation error \n");
    return NULL; 
  }
  result->output_result = NULL;
  result->ptr = NULL;
  result->output_found = 0;
  result->ptr = memchr(source, till, source_size);
  if(!result->ptr) {
    fprintf(stderr, "cannot find the given character\n");
    free_and_null(&result);
    return result; 
  }
  size_t d_len = result->ptr- source;
  result->output_result = malloc(d_len+1);
  if(!result->output_result){
    fprintf(stderr, "memory allocation error \n");
    free_and_null(&result);
    return result;
  } 
  if(!strncpy(result->output_result, source, d_len)){
    fprintf(stderr, "memcpy error\n");
    free_and_null(&result);
    return result;
  }
  result->output_result[d_len] = '\0';
  result->output_found = 1;
  return result;
}

matcher_output * find(char *source, char * to_find, size_t source_size){
  if(source==NULL || to_find ==NULL) return NULL;
  matcher_output * result = (matcher_output*)malloc(sizeof(matcher_output));
  if(result == NULL) {
    fprintf(stderr, "memory allocation error \n");
    return NULL; 
  }
  result->output_result = NULL;
  result->ptr = NULL;
  result->output_found = 0;
  char ** alt_results = NULL;
  alt_results = (char **)malloc(10*sizeof(char *));
  size_t alt_r_num = 0;
  size_t tf_len = strlen(to_find);
  size_t m_len =0;
  while(!result->output_found && source_size>=tf_len){
    //here its doing infinite loop as when it come back it always check from the start of the source
    int f_len = 0;
    char * c = memchr(source, to_find[0], source_size);
    if(!c){
      printf("%s not found\n", to_find);
      break;
    }
    m_len = c-source;
    char data[100] ={0};
    data[0] = c[0];
    f_len++;
    for(int i=1; i<tf_len; i++){
      if(*(c-1) != ' '&& *(c-1) != '\n'&& *(c-1)!= '\0'&& *(c-1)!= '\t'){
        break;
      }
      if(to_find[i] == c[i]){
        f_len ++;
        data[i] = c[i];
      }
      if(f_len==tf_len){
        if(c[i+1] == ' ' ||c[i+1] == '\n'||c[i+1] == '\0'||c[i+1] == '\t'){
          result->output_result = malloc(strlen(data)+1);
          if(!result->output_result){
            fprintf(stderr, "memory allocation error \n");
            return result;
          }
          if(!strcpy(result->output_result, data)){
            fprintf(stderr, "memcpy error\n");
            free_and_null(&result);
            return result;
          }
          result->ptr = &c[i+1];
          result->output_found = 1;
          return result;
        }
        else {
          int j=i+1;
          while(c[j] != ' '&&c[j] != '\0'&&c[j] != '\t'&&c[j] != '\n'){
            data[j] = c[j];
            j++;
          }
          alt_results[alt_r_num] = calloc(100, sizeof(char));
          if(!alt_results || !alt_results[alt_r_num]){
            fprintf(stderr, "memory allocation error \n");
            return result;
          }
          if(!strcpy(alt_results[alt_r_num], data)){
            fprintf(stderr, "memcpy error\n");
            free_and_null(&result);
            return result;
          }
          alt_r_num++;
          result->ptr = &c[j];
        }
      }
    }
    source = source + m_len+1;
    source_size -= (m_len + 1);
  }
  if(result->output_found ==0 && alt_r_num>0){
    printf("%zu\n", alt_r_num);
    printf("cannot found specific string [%s], but found : ", to_find);
    printf("[");
    for(size_t i =0; i<alt_r_num; i++){
      printf("%s ",alt_results[i]);
    }
    printf("]");
    printf("\n");
    result->output_result = malloc(128);
    if(!result->output_result){
      fprintf(stderr, "memory allocation error \n");
      return result;
    }
    if(!strcpy(result->output_result, alt_results[alt_r_num-1])){
      fprintf(stderr, "memcpy error\n");
      free_and_null(&result);
      return result;
    }
  }
  return result;
}

