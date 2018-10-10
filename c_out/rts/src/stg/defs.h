#ifndef DEFS_H
#define DEFS_H

#include "data/string_.h"
#include "stg/bindings.h"


typedef struct fun {
  /*
    takes the function object as the argument
    the fast entry point assumes that all the arguments are on the stack and pops them off
    this is used for the KNOWN FUNCTION CALL rule
   */
   // This takes a variable number of arguments..
  // struct ref (*fast_entry_point)(struct ref); // pointer to the function object, first word is a pointer to the info table. the rest is the payload

  // the slow entry point is used when the function is statically unknown (arguments to functions/variables declared in let bindings)
  struct ref (*slow_entry_point)(struct ref);
  int arity;
} fun;

struct update_info {
  // Closure, the ref points to the object that needs to be updated
  struct ref (*return_address)(struct ref);
};

typedef struct case_info {
  // Closure, the ref points to the bindings of the free variables
  ref (*return_address)(ref);
} case_info;


typedef struct con {
  int arity;
  int con_num;
  struct string_ con_name;
} con;

typedef struct pap {
  struct info_table * info_ptr;
  int size;
} pap;

typedef struct thunk {
  // takes the thunk object as the argument
  struct ref (*return_address)(struct ref);
} thunk;

typedef struct blackhole {
  int a;
} blackhole;


typedef struct arg_entry {
  int size; // bytes
  bool pointer;
  size_t offset;
} arg_entry;


struct layout {
  int num;
  struct arg_entry *entries;
};


// pap's are not contained in the info table but in the payload itself
union info_table_u {
  struct fun function;
  struct con constructor;
  struct case_info case_info;
  struct update_info update_info;
  struct pap pap_info;
  struct thunk thunk_info;
  struct blackhole blackhole_info;
};

typedef struct info_table {
  int type;
  union info_table_u extra;
  struct layout layout;

} info_table;

#endif
