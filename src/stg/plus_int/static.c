#include "data/string_.h"
#include "static.h"
#include "code.h"
#include "containers/mmanager.h"
#include "main.h"
#include "stg/bindings.h"
#include <assert.h>

bool arg_satisfaction_check(int size)
{
    assert((su_register - stack_pointer) > 0);
    return (su_register - stack_pointer) >= size;
}

char* allocate_stack(int stack_size) { return (char*)new(stack_size); }

int main()
{
  stack_pointer = allocate_stack(INITIAL_STACK_SIZE);
  stack_pointer += INITIAL_STACK_SIZE;
  
  init_int();
  init_list();

  init_pointer_table(16);
  
  struct ref inp;
  main_function(inp);
}
