#include "data/string_.h"
#include "static.h"
#include "containers/mmanager.h"

#ifdef DEBUG_OLD
#include "code.h"
#include "main.h"
#endif

#include "stg/bindings.h"
#include "containers/hash_map.h"
#include "typeclasses.h"
#include "stack.h"

#include <assert.h>


void init_bindings(hash_map** bindings) { init_hash_map(bindings, 16, &int_ptr_equals_typeclass, &int_ptr_obj_typeclass); }

bool arg_satisfaction_check(int size)
{
    assert((su_register - stack_pointer) > 0);
    return (su_register - stack_pointer) >= size;
}

int main()
{
  init_stack();

  init_pointer_table(64);
  init_pointer_table(16);

  struct ref null;
  main_function();
}

