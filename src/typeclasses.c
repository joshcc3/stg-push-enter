#include "typeclasses.h"
#include "stdlib.h"

bool pointer_equals(void const *p1, void const *p2, const void* extra) { return p1 == p2; }
bool pointer_not_equals(void const *p1, void const *p2, const void* extra) { return p1 != p2; }

bool int_ptr_equals(void const *p1, void const *p2, const void* extra) { return *(int*)p1 == *(int*)p2; }
bool int_ptr_not_equals(void const *p1, void const *p2, const void* extra) { return *(int*)p1 != *(int*)p2; }

hash_t int_hash(void const *p1, void const *extra) { return *(int*)p1; }

struct equals_typeclass const ptr_equals_typeclass_witness = { .equal = pointer_equals, .not_equal = pointer_not_equals, NULL };

const struct equals_typeclass int_ptr_equals_typeclass = { .equal = int_ptr_equals, .not_equal = int_ptr_not_equals, NULL };
const struct obj_typeclass int_ptr_obj_typeclass = { .hash = int_hash, NULL };
