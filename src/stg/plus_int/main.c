#include "code.h"
#include "containers/mmanager.h"
#include <assert.h>
#include <stdio.h>

void* main_function(void* no_arg)
{
  /*
    main = 
     let a = I# 1 in 
     let b = I# 2 in
     case plus_int a b of
       I# z -> print_int z
   */


  struct i_hash *a_ptr = (struct i_hash *)new(sizeof(struct i_hash));
  a_ptr->info_ptr = &int_constructor_info_table;
  a_ptr->val = 1;

  struct i_hash *b_ptr = (struct i_hash *)new(sizeof(struct i_hash));
  b_ptr->info_ptr = &int_constructor_info_table;
  b_ptr->val = 1;

  // neither a or b are free in the body of the case so there are no relevant bindings

  void **sp_ = (void**)stack_pointer;
  sp_--;
  *sp_ = a_ptr;
  sp_--;
  *sp_ = b_ptr;

  stack_pointer = (char*)sp_;

  void *res = (plus_info_table.extra.function.fast_entry_point)(NULL);

  struct info_table *con_tbl = *(struct info_table **)res;

  void *con_result;
  if(con_tbl->type == 5)
  {
    con_result = (con_tbl->extra.thunk_info.return_address)(res);
  }
  else if(con_tbl->type == 1) con_result = res;

  struct info_table *con_info_tbl = *(struct info_table**)con_result;
  assert(con_info_tbl->type == 1);
  
  int value = *(int*)(con_result + sizeof(void*));
  printf("%d\n", value);

  return NULL;
}

