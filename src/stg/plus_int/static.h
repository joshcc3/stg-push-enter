// heap object - info pointer, payload

#include "data/string_.h"

struct fun {
  void (*code)();
  int arity;
};

struct con {
  int arity;
  int con_num;
  struct string_ con_name;
};

struct pap {
  struct fun * ptr;
};


struct arg_entry {
  int size; // bytes
  bool pointer;
};


struct layout {
  int num;
  struct arg_entry *entries;
};

// pap's are not contained in the info table but in the payload itself
union info_table_u {
  struct fun function;
  struct con constructor;
};

struct info_table {
  int type;
  union info_table_u extra;
  struct layout layout;

};
