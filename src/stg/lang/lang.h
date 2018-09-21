#ifndef LANG_H
#define LANG_H

#include "data/string_.h"
#include "containers/resizable_array.h"

err_t var(string_, union atom*);
err_t lit_int(int, union atom*);
err_t lit_double(double, union atom *);
err_t expr
err_t func_call(string_, int, union atom, union expr *);
err_t primop(string_, int, union atom, union expr *);
err_t atom(union atom, union expr *);
err_t let(string_, union object, union expr *, union expr *); 
err_t case_st(union expr *, union alt *, union expr *);

// Only allows variables for the second argument
err_t case_alt_con(string_, union atom *, union expr *, union alt *);
err_t case_alt_default(string_, union expr*, union alt *);

err_t fun(union atom *, union expr *, union object *);
err_t pap(string_, union atom *, union object *);
err_t con(string_, union atom *, union object *);
err_t thunk(union expr *, union object *);

#endif
