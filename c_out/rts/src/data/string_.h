#ifndef STRING_H
#define STRING_H

#include "err.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct string_ {
  char *char_arr;
  int length;
} string_;


err_t init_string_(const char *, int, struct string_ **);
err_t deinit_string_(struct string_ **);

int string_length(const struct string_ *);
char charAt(const struct string_ *, int);
err_t substring_(const struct string_ *, int, int, struct string_ **);
bool equals(const struct string_ *, const struct string_ *);

#endif
