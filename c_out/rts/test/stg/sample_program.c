#include "stg/lang/lang.h"


void generate_program() {
  
  struct expr default_expression = { .tag = 2, .expr = { .primop_expr = { 
	.name = { .char_arr = "ERROR", .length = 5 },
	.argn = 0,
	.args = NULL
      }
    }};
  
  struct alt def = { .tag = 1, .alternative = { 
      .default_alternative = { 
	.varname = { .char_arr = "z", .length = 1 }, 
	.case_expr = &default_expression } 
    } 
  };

  struct var a_var = { .id = { .char_arr = "a", .length = 1 } };
  struct var b_var = { .id = { .char_arr = "b", .length = 1 } };
  struct var x_var = { .id = { .char_arr = "x", .length = 1 } };
  struct var y_var = { .id = { .char_arr = "y", .length = 1 } };

  struct var nested_case_varlist[1];
  nested_case_varlist[0] = b_var;

  struct atom plus_args[2];
  struct atom tmp = { .tag = 3, .atom = { .var_lit = a_var } };
  struct atom tmp2 = { .tag = 3, .atom = { .var_lit = b_var } };
  plus_args[0] = tmp;
  plus_args[1] = tmp2;
  

  struct expr nested_case_expr = { .tag = 2, .expr = { .primop_expr = { 
	.name = { .char_arr = "+", .length = 1 },
	.argn = 2,
	.args = plus_args
      }
    }};

  struct alt nested_case_alt = { .tag = 0, .alternative = { .constructor_alternative = {
	.constructor = { .char_arr = "I#", .length = 2 },
	.var_list = nested_case_varlist,
	.case_expr = &nested_case_expr
      }
    }};

  
  struct expr y_expr = { .tag = 0, .expr = { .atom_expr = {
	.tag = 3,
	.atom = { .var_lit = y_var },
      }}};

  struct expr x_expr = { .tag = 0, .expr = { .atom_expr = {
	.tag = 3,
	.atom = { .var_lit = x_var },
      }}};

  struct alt nested_case_alts[2];
  nested_case_alts[0] = nested_case_alt;
  nested_case_alts[1] = def;

  struct expr nested_case  = { .tag = 4, .expr = { .case_expr = {
	.case_e = &y_expr,
	.alts = nested_case_alts
      }
    }};

  
  struct var main_case_varlist[1];
  main_case_varlist[0] = a_var;



  struct alt main_case_alt = { .tag = 0, .alternative = { .constructor_alternative = {
	.constructor = { .char_arr = "I#", .length = 2 },
	.var_list = main_case_varlist,
	.case_expr = &nested_case
      }
      
    }};


  struct alt main_alts[2];
  main_alts[0] = main_case_alt;
  main_alts[1] = def;

  struct expr main_case_expr = { .tag = 4, .expr = { .case_expr = {
	.case_e = &x_expr,
	.alts = main_alts
      }} };

  struct var plusInt_arg_list[2];
  plusInt_arg_list[0] = x_var;
  plusInt_arg_list[1] = y_var;


  struct fun function_object = { 
    .arg_list = plusInt_arg_list,
    .argn = 2, 
    .fun_body = &main_case_expr
  };
  
  struct func_decl declaration = {
    .func_name = { .char_arr = "plusInt", .length = 7 },
    .func_object = { .tag = 0, .object = { .function_object = function_object } }
  };


}

int main()
{
  generate_program();
}
