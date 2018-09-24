#include "data/string.h"
#include "static.h"

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
// we expect all of our arguments to be passed on the stack
void plus_compiled() 
{
  /*


case (plus_unboxed 1 2) of
  case I# z -> case z of 
                   a -> print_int a


plus_unboxed 1 2, case () of ...

x -> 1, y -> 2, res -> xnew, res2 -> xnew2
// how do we represent the (x +# y) - I guess it's a heap object which has the thunk and the payload of the heap object will contain the args
xnew -> THUNK(x +# y), let res2 = I# res in res2
xnew2 -> I# xnew, res2
case xnew of (a -> print_int a)
xnew, case () of..
x +# y, Upd xnew ()
3, Upd xnew (), case ()
xnew -> 3, 

z -> xnew
case (I# xnew), s, H


----
Qs:

Does update just enter the code of the next stack frame then? Yes, it just examines the stack frame and passes that as the argument to the next frame with the newly computed answer as the input to the next continuation.

How do we represent the free variables better?
All of the free variables across the nested calls will need to have been saved according to this.. not sure how that works..

  plus :: Int -> Int -> Int
  plus a b = case a of
                I# x -> case b of
		           I# y -> let res = THUNK (x +# y) in 
			           let res2 = I# res in res2


  */
  
  // fast entry point for known calls

  struct hash_map *bindings = NULL;
  init_hash_map(&bindings, 16, &int_equals_typeclass, &int_obj_typeclass);

  void *a = pop(sizeof(void *));
  void *b = pop(sizeof(void *));
  int *a_key = (int*)new(sizeof(int));
  *a_key = 0;
  int *b_key = (int*)new(sizeof(int));
  *b_key = 1;
  hash_map_put(bindings, (const void*)a_key, (const void *)a);
  hash_map_put(bindings, (const void*)b_key, (const void *)b);

  struct info_table *a_info = (struct info_table *)(*a);
  struct info_table *b_info = (struct info_table *)(*b);


  if(a_info->type == 1)
  {
    int *x_key = (int*)new(sizeof(int));
    *x_key = 2;
    hash_map_put(bindings, (const void*)x_key, (const void *)(a + sizeof(void*)));
    /*
                I# x -> case b of
		           I# y -> let res = THUNK (x +# y) in 
			           let res2 = I# res in res2
    */
    if(b_info->type == 1)
    {
      int *y_key = (int*)new(sizeof(int));
      *y_key = 3;
      hash_map_put(bindings, (const void*)y_key, (const void *)(b + sizeof(void*)));

      // let res = THUNK (x +# y) in 
      // let res2 = I# res in res2
      struct info_table *thunk1_info_table = (struct info_table *)new(sizeof(void *));
      thunk1_info_table->type = 5;
      thunk1_info_table->layout = NULL; // TODO: Need to intialize the layout
      thunk1_info_table->extra.thunk_info = thunk1_cont;
      void *thunk1 = new(sizeof(void*)*2);
      thunk1[0] = plus_int_thunk1_info_table;
      thunk1[1] = bindings;

      int *res_key = (int*)new(sizeof(int));
      *res_key = 4;
      hash_map_put(bindings, (const void*)res_key, (const void*)(thunk1));

      // Continue from here
      
    }

    
  }
  else if(type == 5)
  {
    
  }
  else assert(false);

  if(casecon(a)) { plus_compiler_cont1(a.payload[0]); substitute(x, value, ref_map); }
  else {
    expr = a; 
    push(plus_compiler_cont1);
  }
  
}
// need a way of passing the refmaps between continuations
void plus()
{
  arg x = pop();
  xnew = malloc(thunk(x +# y)); // how do we store a thunk and what do we pass to it?
  ref_map[res] = xnew;
  xnew2 = malloc(con(I# res));
  ref_map[res2] = xnew2;
  return xnew2;
}

void plus_compiler_cont1(struct con x) 
{
  arg b = pop();
  push(x);
  if(casecon(b)) { plus_compiler_cont2(b.payload[0]); }
  else {
    expr = b;
    push(plus_compiler_cont2);
  }
}

