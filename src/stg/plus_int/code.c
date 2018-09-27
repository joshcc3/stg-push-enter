#include <assert.h>

#include "data/string_.h"
#include "static.h"
#include "typeclasses.h"
#include "stack.h"
#include "containers/hash_map.h"
#include "containers/mmanager.h"
#include "code.h"

// TODO: I dont think you can free stuff that is on the stack so the hash map which is full of references to the stack will need
// to be taken care of

// TODO: Need to perform stack overflow and underflow checks whenever you push/pop from the stack


// TODO: There's no need to use a hashmap to save bindings, you add them to the payload of the case continuation and free the block when the case continuation is popped

/*
  
  data Int = I# Int#
 
  print_int :: Int# -> IO ()



  plus :: Int -> Int -> Int
  plus a b = case a of
                I# x -> case b of
		           I# y -> let res2 = x +# y 
			     in I# res2

  main = let z = I# 1 in
         let y = I# 2 in
	 case (plus z y) of
	    I# x -> case x of
	              v -> print_int v

 */


void* thunk1_cont(void *thunk_object) 
{
      
  struct hash_map *bindings = (struct hash_map *)(((const void **)thunk_object)[1]);
  int *vx;
  int *vy;
  int x = 2;
  int y = 3;
  hash_map_get(bindings, (const void*)&x, (const void**)&vx);
  hash_map_get(bindings, (const void*)&y, (const void**)&vy);
  int *result = (int*)new(sizeof(int));
  *result = *vx + *vy;
  return (void*)result;
  
}


void* continuation1(struct hash_map *bindings) {
  
  // let res = THUNK (x +# y) in 
  // let res2 = I# res in res2
  struct info_table *thunk1_info_table = (struct info_table *)new(sizeof(void *));
  thunk1_info_table->type = 5;
  struct layout layout_tmp = { .num = 1 };
  thunk1_info_table->layout = layout_tmp; // TODO: Need to intialize the layout
  thunk1_info_table->extra.thunk_info.return_address = thunk1_cont;

  void**thunk1 = new(sizeof(void**)*2);
  thunk1[0] = (void*)thunk1_info_table;
  thunk1[1] = (void*)bindings;	

  int *res_key = (int*)new(sizeof(int));
  *res_key = 4;
  hash_map_put(&bindings, (const void*)res_key, (const void*)(thunk1));

  struct i_hash *constructor1 = (struct i_hash *)new(sizeof(struct i_hash));
  constructor1->info_ptr = &int_constructor_info_table;
  void *tmp;
  hash_map_get(bindings, res_key, (const void**)&tmp);
  struct info_table *tbl = *(struct info_table **)tmp;
  void *thunk_result = (tbl->extra.thunk_info.return_address)(tmp);
  constructor1->val = *(int*)(thunk_result);
  
  int *res2_key = (int*)new(sizeof(int));
  *res2_key = 5;
  hash_map_put(&bindings, (const void*)res2_key, (const void**)constructor1);
  // Is it safe to deinit 'bindings' here?
      
  return constructor1;

}

void* alternatives_evaluator1(struct hash_map *bindings)
{

  void *b;
  int b_key = 1;
  hash_map_get(bindings, (const void*)&b_key, (const void**)&b);

  struct info_table *b_info = *(struct info_table **)b;
  
  /*
    I# x -> case b of
		           I# y -> let res = THUNK (x +# y) in 
			   let res2 = I# res in res2
  */
  if(b_info->type == 1)
  {
    int y_key = 3;
    int y_value = *(int*)(b + sizeof(void*));
    hash_map_put(&bindings, (const void*)&y_key, (const void*)&y_value);
    return continuation1(bindings);
  }
  else
  {
    assert(b_info->type == 5);
    
    // push the case continuation
    push_case_frame(continuation1, 3, bindings);
    
    // push the update continuation
    push_update_frame(b);
    
    // set b to a blackhole in the heap
    b_info->type = 6;
    
    // enter the function for the thunk providing the necessary arguments - the address of the thunk
    void *b_computed = (b_info->extra.thunk_info.return_address)(b);
    void *top_most = (void*)(stack_pointer);
    return update_continuation(top_most, b_computed);
  }
}


// we expect all of our arguments to be passed on the stack
void* plus_int (void *no_arg)
{
  // the fast entry point (no argument satisfaction check) [We assume arguments are always passed on the stack]
  /*

plus_int a b = case a of
    I# x -> case b of
             I# y -> let res = THUNK (x +# y) in 
	             let res2 = I# res in res2
    
case (plus_unboxed 1 2) of
  case I# z -> case z of 
                   a -> print_int a

  */  
  // fast entry point for known calls

  struct hash_map *bindings = NULL;
  init_hash_map(&bindings, 16, &int_ptr_equals_typeclass, &int_ptr_obj_typeclass);
  // the garbage collector will need to refcount this bindings map and free it when it is done - not sure how this would be implemented.

  void *a = *(void **)stack_pointer;
  stack_pointer += (sizeof(void *));
  void *b = *(void **)stack_pointer;
  stack_pointer += sizeof(void*);

  int *a_key = (int*)new(sizeof(int));
  *a_key = 0;
  int *b_key = (int*)new(sizeof(int));
  *b_key = 1;
  hash_map_put(&bindings, (const void*)a_key, (const void *)a);
  hash_map_put(&bindings, (const void*)b_key, (const void *)b);


  struct info_table *a_info = *(struct info_table **)a;

  // We know that the arguments are of type Int which means they can only be the Constructor or Thunks that evaluate to the Constructor

  if(a_info->type == 1)
  {
    // The bindings dont escape this function (for unboxed values they will be copied so no need to worry about this stack getting cleaned up
    int x_key = 2;
    // The payload of a Constructor contains its arguments
    int x_value = *(int*)(a + sizeof(void*));
    hash_map_put(&bindings, (const void*)&x_key, (const void*)&x_value);
    return alternatives_evaluator1(bindings);
  }
  else
  {
    assert(a_info->type == 5);
    push_case_frame(alternatives_evaluator1, 2, bindings);
    push_update_frame(a);
    a_info->type = 6;
    void *a_computed = (a_info->extra.thunk_info.return_address(b));
    void *top_most = (void*)(stack_pointer);
    return update_continuation(top_most, a_computed);
  }

}
