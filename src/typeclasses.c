#include "typeclasses.h"

bool pointer_equals(void const *p1, void const *p2, const void* extra) { return p1 == p2; }
bool pointer_not_equals(void const *p1, void const *p2, const void* extra) { return p1 != p2; }

struct equals_typeclass const ptr_equals_typeclass_witness = { .equal = pointer_equals, .not_equal = pointer_not_equals, NULL };
