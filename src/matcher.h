#ifndef MATCHER_H
#define MATCHER_H

typedef struct matcher_output{
  char * output_result;
  char *ptr;
  int output_found;
}matcher_output;

void free_and_null(matcher_output ** matcher);

matcher_output *get_till_char(char *source, int till, size_t source_size);

matcher_output * find(char *source, char * to_find, size_t source_size);


#endif  //MATCHER_H
