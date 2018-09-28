#ifndef BINDINGS_H
#define BINDINGS_H

#include "containers/hash_map.h"
#include "containers/arraylist.h"

struct arr_list *pointer_table;

struct ref {
  void **ptr;
};

void init_pointer_table(size_t);

void* get_ref(struct ref);

void* upd_ref(struct ref, void *);

// Insert a new heap object into the pointer table and return the pointer to the 
void new_ref(size_t, struct ref*);

// Get the pointer table entry associated with int or NULL if it doesn't exist
void get_binding(struct hash_map *, int, const struct ref*);

// Associate a key with an existing entry in the pointer table
void put_binding_ref(struct hash_map *, int, const struct ref);



#endif
