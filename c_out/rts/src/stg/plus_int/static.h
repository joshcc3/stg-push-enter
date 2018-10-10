#ifndef STATIC_H
#define STATIC_H

// TODO: Need to fix the import structure - there should be a types module. There should be one for the stack.
// TODO: Then there should be the actual function that initializes everything and should import from the constructors and stack.

// heap object - info pointer, payload

#include "stg/defs.h"
#include "stack.h"

#ifdef DEBUG_OLD 
#include "stg/list/list.h"
#include "stg/plus_int/code.h"
#endif

void init_bindings(hash_map**);
bool arg_satisfaction_check(int);


#ifdef DEBUG_OLD 
// TODO: where should these go? The defns of the constructor are in code.h
struct info_table int_constructor_info_table;
struct info_table plus_info_table;
#endif

#endif
