
#include <assert.h>
#include <stdio.h>

#include "containers/hash_set.h"
// #include "typeclasses.h"

int hash_int(void const* element, void const * extra) { return abs(*((int const*)element)); }

bool pointer_equals(void const *p1, void const *p2, void const *extra) { return p1 == p2; }
bool pointer_not_equals(void const *p1, void const *p2, void const *extra) { return p1 != p2; }

struct equals_typeclass const ptr_equals_typeclass_witness = { .equal = pointer_equals, .not_equal = pointer_not_equals, NULL };


int test_mixed()
{

    struct obj_typeclass int_obj_typeclass = { .hash = hash_int, NULL };
    struct hash_set * set;
    hset_init(&set, 20);

    int element = 123;

    hset_insert_element(&set, &element, &ptr_equals_typeclass_witness, &int_obj_typeclass);
    assert(hset_contains(set, &element, &ptr_equals_typeclass_witness, &int_obj_typeclass) == true);
    assert(hset_size(set) == 1);

    hset_insert_element(&set, &element, &ptr_equals_typeclass_witness, &int_obj_typeclass);
    hset_insert_element(&set, &element, &ptr_equals_typeclass_witness, &int_obj_typeclass);
    assert(hset_contains(set, &element, &ptr_equals_typeclass_witness, &int_obj_typeclass) == true);
    assert(hset_size(set) == 1);

    int *value_ptr = malloc(sizeof(int*));
    hset_delete_element(&set, &element, (const void **)&value_ptr, &ptr_equals_typeclass_witness, &int_obj_typeclass);
    assert(hset_size(set) == 0);
    assert(*value_ptr == 123);
    assert(hset_contains(set, &element, &ptr_equals_typeclass_witness, &int_obj_typeclass) == false);


    hset_insert_element(&set, &element, &ptr_equals_typeclass_witness, &int_obj_typeclass);
    assert(hset_size(set) == 1);
    hset_insert_element(&set, &element, &ptr_equals_typeclass_witness, &int_obj_typeclass);
    hset_insert_element(&set, &element, &ptr_equals_typeclass_witness, &int_obj_typeclass);
    hset_insert_element(&set, &element, &ptr_equals_typeclass_witness, &int_obj_typeclass);
    assert(hset_contains(set, &element, &ptr_equals_typeclass_witness, &int_obj_typeclass) == true);
    assert(hset_size(set) == 1);

    hset_deinit(&set);
    printf("Testing mixed complete.\n");

    return 0;
}



void test_insert()
{
    struct obj_typeclass int_obj_typeclass = { .hash = hash_int, NULL };
    struct hash_set * set;
    hset_init(&set, 20);

    int element[100];
    for (int i = 0; i < 100; i++)
    {
        assert(hset_size(set) == i);
        element[i] = i*i;
        int *element_ptr = element + i;
        hset_insert_element(&set, element_ptr, &ptr_equals_typeclass_witness, &int_obj_typeclass);
        assert(hset_contains(set, element_ptr, &ptr_equals_typeclass_witness, &int_obj_typeclass) == true);
        assert(hset_size(set) == i + 1);
    }


    hset_deinit(&set);
    assert(!set);

    printf("Testing insert complete.\n");
}

int main()
{

    test_insert();
    test_mixed();

    return 0;
}