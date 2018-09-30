#ifndef STATIC_H
#define STATIC_H

// TODO: Need to fix the import structure - there should be a types module. There should be one for the stack.
// TODO: Then there should be the actual function that initializes everything and should import from the constructors and stack.

// heap object - info pointer, payload

#include "data/string_.h"
#include "stg/bindings.h"

#ifdef DEBUG_OLD 
#include "stg/list/list.h"
#include "stg/plus_int/code.h"
#endif

void init_bindings(hash_map**);
bool arg_satisfaction_check(int);

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



// All of these are basically function closures
struct update_frame {
  struct info_table *tbl;
  struct ref update_ref;
  // Section 5: I'm not sure why we dont have to actually consider case frames for the argument satisfaction check
  struct update_frame *next_update_frame;
};


typedef struct case_frame {
  struct info_table *tbl;
  // TODO: these free variables will need to be collected when the case frame is popped
  struct hash_map *free_vars;
  int update_key;
  struct ref (*alternatives_evaluator)(struct hash_map*);
} case_frame;



// for easier pointer arithmetic necessary for pushing and popping stack frames
char *stack_pointer;
char *su_register; // used for tracking the current update frames

// functions as well as continuations take a pointer to the info table of the structure, the stack pointer should be globally available

#define INITIAL_STACK_SIZE 32*1024

#ifdef DEBUG_OLD 
// TODO: where should these go? The defns of the constructor are in code.h
struct info_table int_constructor_info_table;
struct info_table plus_info_table;
#endif

#endif
