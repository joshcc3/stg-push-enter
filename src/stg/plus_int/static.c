
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
#include "data/string_.h"
#include "static.h"

// we expect all of our arguments to be passed on the stack
void plus_compiled() 
{
  /*

  plus :: Int -> Int -> Int
  plus a b = case a of
                I# x -> case b of
		           I# y -> let res = THUNK (x +# y) in 
			           let res2 = I# res in res2

  */
  
  // fast entry point for known calls

  arg a = pop();
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



struct info_table int_constructor_info_table;
struct info_table plus_info_table;

int main()
{
  struct con con_info = { .arity = 1, .con_num = 0, .con_name = { .char_arr = "I#", .length = 2 } };
  struct arg_entry con_entries[1];
  struct layout con_layout = { .num = 1, .entries = con_entries };
  union info_table_u con_info_ = { .constructor = con_info };
  int_constructor_info_table.type = 1;
  int_constructor_info_table.extra = con_info_;
  int_constructor_info_table.layout = con_layout;

  struct arg_entry e1 = { .size = sizeof(int), .pointer = false };
  con_entries[0] = e1;
  
  struct arg_entry plus_entries[2];
  struct arg_entry plus_entry1 = { .size = sizeof(int*), .pointer = true };
  struct arg_entry plus_entry2 = { .size = sizeof(int*), .pointer = true };
  plus_entries[0] = plus_entry1;
  plus_entries[1] = plus_entry2;
  struct layout plus_layout = { .num = 2, .entries = plus_entries };

  struct fun plus_extra = {
    .code = plus_compiled,
    .arity = 2
    
  };
  union info_table_u plus_extra_ = { .function = plus_extra };
  plus_info_table.type = 0;
  plus_info_table.extra = plus_extra_;
  plus_info_table.layout = plus_layout;

}
