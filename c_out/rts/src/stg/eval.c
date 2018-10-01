#include "stg/lang.h"


struct expr e;

if(state.expr.tag == let) {
  struct let l = e.expr.let_expr;
  state.expr = substitute(l.var, freshX, l.let_body);
  state.heap[freshX] = l.let_body;
  
}


if(state.expr.tag == case_) {
  struct expr * e = state.expr.case_st.case_e.expr;
  struct alt * a = state.expr.case_st.alts;

  if(e.tag != atom) {
    state.expr = e;
    state.stack.push(case_cont(a));
  }
  else if(e.tag == atom)
  {
    if(e.expr.atom_expr.tag != var) {
      struct case_alt_default d = a.get(-1).alternative.default_alternative;
      if(e.expr.atom_expr.tag == intl) {
	state.expr = substitute(d.varname, e.expr.atom_expr.int_lit, d.case_expr)
      }
      else if(e.expr.atom_expr.tag == dbl) {
	state.expr = substitute(d.varname, e.expr.atom_expr.double_lit, d.case_expr)
      }
      else if(e.expr.atom_expr.tag == strl) {
	state.expr = substitute(d.varname, e.expr.atom_expr.string_lit, d.case_expr)
      }
      else { CRASH }
    }
    else {
      struct var v = e.expr.atom_expr.atom.var_lit;
      struct con con_n = state.heap.get(v.id);
      for(struct alt _alt in a) {
	if(alt.tag == constructor) {
	  struct case_alt_con c = alt.alternative.constructor_alternative;
	  if (c.constructor == con_n.constructor) {
	    state.expr = _alt.case_expr;
	    for(int i = 0; i < con_n.argn; i++) {
	      state.expr = substitute(c.var_list[i], con_n.atom_list[i]. state.expr);
	    }
	  }

      }
    }
    
  }

  }
 }
if(state.expr.tag == atom) {
  if(state.expr.atom_expr.atom.tag == var) {
    if(state.heap)
  }
 }
  
  
}
