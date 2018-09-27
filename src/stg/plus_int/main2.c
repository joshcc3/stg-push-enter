#include "code.h"
#include "containers/mmanager.h"
#include <assert.h>
#include <stdio.h>
#include "main.h"
#include "typeclasses.h"

void* main_function()
{
  // TODO Complete the implementation of the remaining stuff.
  // Build PAPs to see how they work
  // Next implement the prelude and try and create lists and sum lazy lists.
  /*
    -- Test to see that the thunk is only computed once
    let a = I# 1 in
    let b = I# 2 in
    let c = THUNK (plus_int a b) in
    case plus_int c c of
      I# d -> print d
    
   */

  
  struct i_hash* a = (struct i_hash*)new(sizeof(struct i_hash));
  a->info_ptr = &int_ptr_info_table;
  a->val = 1;

  struct i_hash* b = (struct i_hash*)new(sizeof(struct i_hash));
  b->info_ptr = &int_ptr_info_table;
  b->val = 2;

  struct hash_map *bindings;
  hash_map_init(&bindings, 16, &int_ptr_equals_typeclass, &int_ptr_obj_typeclass);
  int a_key = 0;
  int b_key = 1;
  
  hash_map_put(bindings, (const void*)&a_key, (const void*)&a);
  hash_map_put(bindings, (const void*)&b_key, (const void*)&b);

  struct info_table *info = (struct info_table *)new(sizeof(struct info_table));
  info->type = 5;
  info->extra.thunk_info.return_address = c_cont;
  
  void **c = new(sizeof(void*)*2);
  c[0] = (void*)info;
  c[1] = bindings;

  push_case_frame(alternatives_evaluator, 2, bindings);

  push(b);
  push(a);
  return case_continuation(plus_int(NULL));
  
}
