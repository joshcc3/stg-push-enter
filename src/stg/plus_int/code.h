#ifndef CODE_H
#define CODE_H

#include "static.h"

// data Int = I# #Int
struct i_hash {
  struct info_table *info_ptr;
  int val;
};

void* (*plus_compiled)(void *);

#endif
