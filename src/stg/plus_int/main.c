#include "code.h"
#include "containers/mmanager.h"
#include <assert.h>
#include <stdio.h>
#include "stg/bindings.h"
#include "stack.h"

void* main_function(void* no_arg)
{
  /*
    main = 
     let a = I# 1 in 
     let b = I# 2 in
     case plus_int a b of
       I# z -> print_int z
   */


  struct ref a_ref;
  new_ref(sizeof(struct i_hash), &a_ref);
  struct i_hash *a_ptr = get_ref(a_ref);
  a_ptr->info_ptr = &int_constructor_info_table;
  a_ptr->val = 1;

  struct ref b_ref;
  new_ref(sizeof(struct i_hash), &b_ref);
  struct i_hash *b_ptr = get_ref(b_ref);
  b_ptr->info_ptr = &int_constructor_info_table;
  b_ptr->val = 1;

  // neither a or b are free in the body of the case so there are no relevant bindings

  push_ptr(b_ref);
  push_ptr(a_ref);
  
  struct ref rand;
  struct ref res_ref = (plus_info_table.extra.function.fast_entry_point)(rand);
  void *res = get_ref(res_ref);

  struct info_table *con_tbl = *(struct info_table **)res;

  struct ref con_result;
  if(con_tbl->type == 5)
  {
    con_result = (con_tbl->extra.thunk_info.return_address)(res_ref);
  }
  else if(con_tbl->type == 1) con_result = res_ref;

  struct info_table *con_info_tbl = *(struct info_table**)get_ref(con_result);
  assert(con_info_tbl->type == 1);
  
  int value = *(int*)(get_ref(con_result) + sizeof(void*));
  printf("%d\n", value);

  return NULL;
}

