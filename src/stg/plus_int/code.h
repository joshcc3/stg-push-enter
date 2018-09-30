#ifndef CODE_H
#define CODE_H

#include "static.h"

// data Int = I# #Int
struct i_hash {
  struct info_table *info_ptr;
  int val;
};

struct ref plus_int_slow(ref);
struct ref plus_int_fast(ref, ref);

void c_int(int, ref*);

void init_int();

#endif
