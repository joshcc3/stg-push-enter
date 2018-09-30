#ifndef HEAP_H
#define HEAP_H

#include "data/string_.h"
#include "bindings.h"
#include "plus_int/static.h"
#include "plus_int/stack.h"

#define THUNK_GET_BINDINGS(thunk) (hash_map*)(((void**)get_ref(thunk))[1]);


void unroll_pap(void **);

// thunk object, case continuation, current bindings, update key for the case, update key for the update cont
ref thunk_continuation(ref, ref (*)(struct hash_map*), struct hash_map*, int, ref);

// bindings, thunk continuation
ref create_thunk(struct hash_map*, ref (*)(ref));

// number of args, constructor id, name, pointer to allocated constructor obj with args filled in
// no need to generate an info table at runtime
// ref create_con(void *const *);

// Blackholes are only created from a thunk when it's being evaluated
// ref create_blackhole();

// bindings and the slow entry point
//ref create_fun(struct hash_map*, ref (*)(ref));

// function object, pointer to allocatd pap with args filled in
// same as for constructors
// ref create_pap(struct fun *ptr, void *const *);


#endif