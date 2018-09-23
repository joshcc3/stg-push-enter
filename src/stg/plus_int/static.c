
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
	    I# x -> print_int x

 */
#include "data/string.h"
#include "static.h"
struct con con_info = { .arity = 1, .con_num = 0, .con_name = { .char_arr = "I#", .length = 2 } };
struct arg_entry con_entries[1];
con_entries[0] = { .size = sizeof(int), .pointer = false };
struct layout con_layout = { .num = 1, .entries = con_entries };
struct info_tabl int_c = {
  .type = 1,
  .extra { .constructor = con_info },
  .layout = con_layout
}

