#include "data/string_.h"
#include "static.h"
#include "stack.h"

// TODO: Need to perform stack overflow and underflow checks whenever you push/pop from the stack


/*
  
  data Int = I# Int#
 
  print_int :: Int# -> IO ()



  plus :: Int -> Int -> Int
  plus a b = case a of
                I# x -> case b of
		           I# y -> I# (x + y)

  main = let z = I# 1 in
         let y = I# 2 in
	 case (plus z y) of
	    I# x -> case x of
	              v -> print_int v

 */
/*
void* case_cont(struct case_frame* frame, void *value)
{
  int k = 3;
  hash_map_put(frame->free_vars, &k, value);
  // we dont need the case frame now..
  stack_pointer += (struct case_frame);

  return continuation1(frame->free_vars);

}

void* continuation1(struct hash_map *bindings) {
  
  // let res = THUNK (x +# y) in 
  // let res2 = I# res in res2
  struct info_table *thunk1_info_table = (struct info_table *)new(sizeof(void *));
  thunk1_info_table->type = 5;
  thunk1_info_table->layout = NULL; // TODO: Need to intialize the layout
  thunk1_info_table->extra.thunk_info.return_address = thunk1_cont;
  void *thunk1 = new(sizeof(void*)*2);
  thunk1[0] = thunk1_info_table;
  thunk1[1] = bindings;

  int *res_key = (int*)new(sizeof(int));
  *res_key = 4;
  hash_map_put(&bindings, (const void*)res_key, (const void*)(thunk1));

  struct i_hash *constructor1 = (struct i_hash)new(sizeof(struct i_hash));
  constructor1->info_ptr = &int_constructor_info_table;
  void *tmp;
  hash_map_get(bindings, res_key, &tmp);
  constructor1->val = tmp;
  int *res2_key = (int*)new(sizeof(int));
  *res2_key = 5;
  hash_map_put(bindings, (const void*)res2_key, (const void*)constructor1);
  // Is it safe to deinit 'bindings' here?
      
  return constructor1;
}


int thunk1_cont(void *thunk_object) 
{
      
  struct hash_map *bindings = (struct hash_map *)(thunk_object + 1);
  int *vx;
  int *vy;
  int x = 2;
  int y = 3;
  hash_map_get(bindings, &x, &vx);
  hash_map_get(bindings, &y, &vy);
  return x + y;
  
}

void push_update_frame(void *update_ref) {
      stack_pointer -= sizeof(struct update_frame);
      struct update_frame *upd_frame = (struct update_frame*)(stack_pointer);
      upd_frame->update_ref = update_ref;
      upd_frame->next_update_frame = su_register;
      su_register = stack_pointer;
      upd_frame->tbl = { .type = 3, .extra = { .case_info = { .return_address = update_continuation } } };
}

void push_case_frame(void* (*continuation)(struct case_frame*, void*), struct hash_map *bindings) {

      stack_pointer -= sizeof(struct case_frame);
      struct case_frame *case_frame = (struct case_frame*)(stack_pointer);
      case_frame->free_vars = bindings;
      struct info_table *case_info_ptr = (struct info_table*)new(sizeof(struct info_table));
      case_info_ptr->type = 2;
      case_info_ptr->extra.case_info.return_address = continuation;
      case_frame->tbl = case_info_ptr;
}


// we expect all of our arguments to be passed on the stack
void* plus_compiled() 
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
/*
  struct hash_map *bindings = NULL;
  init_hash_map(&bindings, 16, &int_equals_typeclass, &int_obj_typeclass);
  // the garbage collector will need to refcount this bindings map and free it when it is done - not sure how this would be implemented.

  void *a = (void *)stack_pointer;
  stack_pointer += sizeof(void*);
  void *b = (void *)stack_pointer;
  stack_pointer += (sizeof(void *));

  int *a_key = (int*)new(sizeof(int));
  *a_key = 0;
  int *b_key = (int*)new(sizeof(int));
  *b_key = 1;
  hash_map_put(&bindings, (const void*)a_key, (const void *)a);
  hash_map_put(&bindings, (const void*)b_key, (const void *)b);

  struct info_table *a_info = (struct info_table *)(*a);
  struct info_table *b_info = (struct info_table *)(*b);

  // We know that the arguments are of type Int which means they can only be the Constructor or Thunks that evaluate to the Constructor

  if(a_info->type == 1)
  {
    int *x_key = (int*)new(sizeof(int));
    *x_key = 2;
    // The payload of a Constructor contains its arguments
    hash_map_put(&bindings, (const void*)x_key, (const void *)a + 1;
    /*
                I# x -> case b of
		           I# y -> let res = THUNK (x +# y) in 
			           let res2 = I# res in res2
    */
/*
    if(b_info->type == 1)
    {
      int y_key = 3;
      hash_map_put(&bindings, (const void*)&y_key, (const void *)b + 1);
      return continuation1(bindings);
    }
    else
    {
      assert(b_info->type == 5);

      // push the case continuation
      push_case_frame(case_cont, bindings);

      // push the update continuation
      push_update_frame(b);

      // set b to a blackhole in the heap
      b_info->type = 6;

      // enter the function for the thunk providing the necessary arguments - the address of the thunk
      void *b_computed = (b_info->extra.thunk_info.return_address)(b);
      struct update_frame *top_most = (struct update_frame *)(stack_pointer + sizeof(struct update_frame));
      
    }
    
  }
  else
  {
    assert(false);
  }

  
}

*/
