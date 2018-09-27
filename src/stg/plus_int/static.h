#ifndef STATIC_H
#define STATIC_H


// heap object - info pointer, payload

#include "data/string_.h"


// functions as well as continuations take a pointer to the info table of the structure, the stack pointer should be globally available

#define INITIAL_STACK_SIZE 32*1024

struct fun {
  void* (*fast_entry_point)(void *); // pointer to the function object, first word is a pointer to the info table. the rest is the payload
  void* (*slow_entry_point)(void *);
  int arity;
};

struct update_info {
  void* (*return_address)(void*);
};

struct case_info {
  void* (*return_address)(void*);
};


struct con {
  int arity;
  int con_num;
  struct string_ con_name;
};

struct pap {
  struct fun * ptr;
};

struct thunk {
  void* (*return_address)(void*);
};

struct blackhole {
  int a;
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
  struct case_info case_info;
  struct update_info update_info;
  struct pap pap_info;
  struct thunk thunk_info;
  struct blackhole blackhole_info;
};

struct info_table {
  int type;
  union info_table_u extra;
  struct layout layout;

};

// All of these are basically function closures
struct update_frame {
  struct info_table *tbl;
  void *update_ref;
  // Section 5: I'm not sure why we dont have to actually consider case frames for the argument satisfaction check
  struct update_frame *next_update_frame;
};


struct case_frame {
  struct info_table *tbl;
  // TODO: these free variables will need to be collected when the case frame is popped
  struct hash_map *free_vars;
  int update_key;
  void* (*alternatives_evaluator)(struct hash_map*);
};



// for easier pointer arithmetic necessary for pushing and popping stack frames
char *stack_pointer;
char *su_register; // used for tracking the current update frames


// TODO: where should these go? The defns of the constructor are in code.h
struct info_table int_constructor_info_table;
struct info_table plus_info_table;


#endif
