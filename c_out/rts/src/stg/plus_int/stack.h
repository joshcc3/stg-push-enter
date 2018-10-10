#ifndef STACK_H
#define STACK_H

#include "stg/defs.h"

void init_stack();
 

// for easier pointer arithmetic necessary for pushing and popping stack frames
char *stack_pointer;
char *su_register; // used for tracking the current update frames

// functions as well as continuations take a pointer to the info table of the structure, the stack pointer should be globally available

#define INITIAL_STACK_SIZE 32*1024*1024



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

struct ref update_continuation(struct ref);

struct ref case_continuation(struct ref);

void push_update_frame(struct ref);

// case cont, update key, bindings
void push_case_frame(struct ref (*)(struct hash_map*), int, struct hash_map *);

void push_ptr(struct ref);
void push_int(int);
void push_char(char);
void push_long(long);

void pop_ptr(struct ref*);

#endif
