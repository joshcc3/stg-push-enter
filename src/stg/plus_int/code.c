#include <assert.h>
#include "data/string_.h"
#include "static.h"
#include "typeclasses.h"
#include "stack.h"
#include "containers/mmanager.h"
#include "code.h"
#include "stg/bindings.h"

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

struct ref thunk1_cont(struct ref thunk_object_ref) 
{
  void *thunk_object = get_ref(thunk_object_ref);
  struct hash_map *bindings = (struct hash_map *)(((const void **)thunk_object)[1]);
  struct ref vx;
  struct ref vy;
  get_binding(bindings, 2, (const struct ref*)&vx);
  get_binding(bindings, 3, (const struct ref*)&vy);
  struct ref result_ref;
  new_ref(sizeof(int), &result_ref);
  void *result = get_ref(result_ref);
  *result = *get_ref(vx) + *get_ref(vy);
  return result_ref;
}


struct ref continuation1(struct hash_map *bindings) {
  
  // let res = THUNK (x +# y) in 
  // let res2 = I# res in res2
  struct info_table *thunk1_info_table = (struct info_table *)new(sizeof(void *));
  thunk1_info_table->type = 5;
  struct layout layout_tmp = { .num = 1 };
  thunk1_info_table->layout = layout_tmp; // TODO: Need to intialize the layout
  thunk1_info_table->extra.thunk_info.return_address = thunk1_cont;

  struct ref *thunk1_ref;
  new_ref(sizeof(void**)*2, &thunk1_ref);
  void**thunk1 = (void**)get_ref(thunk1_ref);
  thunk1[0] = (void*)thunk1_info_table;
  thunk1[1] = (void*)bindings;	
  
  int res_key = 4;

  put_binding(bindings, res_key, thunk1_ref);

  struct ref constructor1_ref;
  new_ref(sizeof(struct i_hash), &constructor1_ref);
  struct i_hash *constructor1 = (struct i_hash *)get_ref(constructor1_ref);

  constructor1->info_ptr = &int_constructor_info_table;
  struct ref tmp;
  get_binding(bindings, res_key, &tmp);
  struct info_table *tbl = *(struct info_table **)get_ref(tmp);
  struct ref thunk_result = (tbl->extra.thunk_info.return_address)(tmp);
  constructor1->val = *(int*)(get_ref(thunk_result));
  
  int res2_key = 5;
  put_binding(bindings, res2_key, constructor1_ref);
  // Is it safe to deinit 'bindings' here?
      
  return constructor1_ref;

}

struct ref alternatives_evaluator1(struct hash_map *bindings)
{

  struct ref b_;
  int b_key = 1;
  get_binding(bindings, b_key, &b_);

  void *b = get_ref(b_);
  struct info_table *b_info = *(struct info_table **)b;
  
  /*
    I# x -> case b of
		           I# y -> let res = THUNK (x +# y) in 
			   let res2 = I# res in res2
  */
  if(b_info->type == 1)
  {
    int y_key = 3;

    struct ref y_value_ref;
    new_ref(sizeof(int*), &y_value_ref);
    int* y_value = (int*)(get_ref(y_value_ref));
    *y_value = *(int*)(b + sizeof(void*));
    
    put_binding(bindings, y_key, y_value_ref);
    return continuation1(bindings);
  }
  else
  {
    assert(b_info->type == 5);
    
    // push the case continuation
    push_case_frame(continuation1, 3, bindings);
    
    // push the update continuation
    push_update_frame(b_);
    
    // set b to a blackhole in the heap
    b_info->type = 6;
    
    // enter the function for the thunk providing the necessary arguments - the address of the thunk
    struct ref b_computed = (b_info->extra.thunk_info.return_address)(b_);
    return case_continuation(update_continuation(b_computed));
  }
}


// we expect all of our arguments to be passed on the stack
struct ref plus_int (struct ref no_arg)
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

  struct ref a_ref;
  pop_ref(&a_ref);
  void *a = get_ref(a_ref);

  struct ref b_ref;
  pop_ref(&b_ref);
  void *b = get_ref(b_ref);

  int a_key = 0;
  int b_key = 1;
  put_binding(bindings, a_key, a_ref);
  put_binding(bindings, b_key, b_ref);


  struct info_table *a_info = *(struct info_table **)a;

  // We know that the arguments are of type Int which means they can only be the Constructor or Thunks that evaluate to the Constructor

  if(a_info->type == 1)
  {
    // The bindings dont escape this function (for unboxed values they will be copied so no need to worry about this stack getting cleaned up
    int x_key = 2;
    // The payload of a Constructor contains its arguments

    struct ref x_ref;
    new_ref(sizeof(void*), &x_ref);
    int *x = (int*)get_ref(x_ref);
    *x = *(int*)(a + sizeof(void*));

    put_binding(bindings, x_key, x_ref);
    return alternatives_evaluator1(bindings);
  }
  else
  {
    assert(a_info->type == 5);
    push_case_frame(alternatives_evaluator1, 2, bindings);
    push_update_frame(a);
    a_info->type = 6;
    struct ref a_computed = (a_info->extra.thunk_info.return_address(a));
    return case_continuation(update_continuation(a_computed));
  }

}
