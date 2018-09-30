#ifndef BINDINGS_H
#define BINDINGS_H

#include "containers/hash_map.h"
#include "containers/arraylist.h"

struct arr_list *pointer_table;

#define NEW_REF(ref_name, val_type, val_size, val_name) ref ref_name; \
     new_ref(val_size, &ref_name); \
     val_type val_name = (void**)get_ref(ref_name);


#define GET_BINDING(ref_name, val_type, val_name, binding_key, bindings) ref ref_name;\
      get_binding(bindings, binding_key, &ref_name); \
      val_type val_name = (val_type)get_ref(ref_name);


// A ref stores an offset into the pointer table to avoid containing internal pointers
typedef struct ref {
  int* offset;
} ref;

void init_pointer_table(size_t);

void* get_ref(struct ref);

void upd_ref(struct ref, struct ref);

void upd_ref_value(struct ref, struct ref);

// Insert a new heap object into the pointer table and return the pointer to the 
void new_ref(size_t, struct ref*);

// Get the pointer table entry associated with int or NULL if it doesn't exist
void get_binding(struct hash_map *, int, struct ref*);

// Associate a key with an existing entry in the pointer table
void put_binding(struct hash_map *, int, struct ref);



#endif
