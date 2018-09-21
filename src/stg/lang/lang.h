#ifndef LANG_H
#define LANG_H

#include "data/string_.h"
#include "containers/resizable_array.h"

struct var {
  struct string_ id;
};

union atom_u {
  int int_lit;
  double double_lit;
  struct string_ string_lit;
  struct var var_lit;
};

struct atom {
  int tag;
  union atom_u atom;
};

struct function_call {
  struct string_ name;
  int argn;
  struct atom* args;
  
};

struct primop {
  struct string_ name;
  int argn;
  struct atom* args;
};


struct let {
  struct string_ var;
  struct object * var_body;
  struct expr * let_body;

};

struct case_st {
  struct expr * case_e;
  struct alt * alts;
};

union expr_u {
  struct atom atom_expr;
  struct function_call func_call_expr;
  struct primop primop_expr;
  struct let let_expr;
  struct case_st case_expr;
};

struct expr {
  int tag;
  union expr_u expr;
};


struct case_alt_con {
  struct string_ constructor;
  struct var * var_list;
  struct expr * case_expr;
};

struct case_alt_default {
  struct string_ varname;
  struct expr * case_expr;
};


union alt_u {
  struct case_alt_con constructor_alternative;
  struct case_alt_deault default_alternative;
};

struct alt {
  int tag;
  union alt_u alternative;
};

struct fun {
  struct var *arg_list;
  struct expr *fun_body;
};

struct pap {
  struct string_ fun_name;
  struct atom* application_list;
};

struct con {
  struct string_ con_name;
  struct atom *atom_list;
};

struct object_u {
  struct fun function_object;
  struct pap partial_ap_object;
  struct con constructor_object;
  struct expr thunk_object;
  bool blackhole;
};

struct func_decl {
  struct string_ func_name;
  struct object  func_object;
};


struct program {
  struct func_decl * function_declarations;
  struct func_decl * main;
}

#endif
