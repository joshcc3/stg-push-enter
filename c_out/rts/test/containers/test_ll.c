#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "containers/llist.h"

err_t map_fn(void **element_ptr)
{
    int* element = (int*)(*element_ptr);
    printf("%d, ", *element);

    return 0;
}


void test_insert_delete()
{
    struct linked_list * l = new_llist();

    int element1 = 1, element2 = 2, element3 = 3;
    const int *store = NULL;

    ll_insert_element(l, 0, &element1);
    ll_delete_element(l, 0, (const void**)&store);
    assert(*store == element1);

    ll_insert_element(l, 0, &element1);
    ll_delete_element(l, 0, (const void**)&store);
    assert(*store == element1);

    ll_insert_element(l, 0, &element1);
    ll_delete_element(l, 0, (const void**)&store);
    assert(*store == element1);

    ll_insert_element(l, 0, &element1);
    ll_insert_element(l, 0, &element2);
    ll_delete_element(l, 0, (const void**)&store);
    assert(*store == element2);
    ll_delete_element(l, 0, (const void**)&store);
    assert(*store == element1);

    ll_insert_element(l, 0, &element1);
    ll_insert_element(l, 0, &element2);
    ll_insert_element(l, 2, &element3);
    ll_delete_element(l, 2, (const void**)&store);
    assert(*store == element3);
    ll_delete_element(l, 1, (const void**)&store);
    assert(*store == element1);
    ll_delete_element(l, 0, (const void**)&store);
    assert(*store == element2);

    deinit_llist(&l);

}


void test_insert_get()
{
    /*
    err_t ll_insert_element(struct linked_list * const linked_list, const int n, void const * const element);

    err_t ll_delete_element(struct linked_list * const linked_list, const int n, void ** result);

    err_t get_element(struct linked_list const * const linked_list, const int n, void** result);

    err_t map_list(struct linked_list const * const linked_list, err_t map_fn(void**));

    */
    struct linked_list * l = new_llist();

    int element1 = 1, element2 = 2, element3 = 3, element4 = 4;
    const int *store = NULL;

    printf("\ninsert(l, 0, element1)\n");
    ll_insert_element(l, 0, &element1);
    ll_map(l, map_fn);
    ll_get_element(l, 0,(const void**)&store);
    assert(*store == element1);

    printf("\nll_insert_element(l, 0, element2)\n");
    ll_insert_element(l, 0, &element2);
    ll_map(l, map_fn);
    ll_get_element(l, 0, (const void**)&store);
    assert(*store == element2);


    printf("\nll_insert_element(l, 2, element3)\n");
    ll_insert_element(l, 2, &element3);
    ll_map(l, map_fn);
    ll_get_element(l, 2, (const void**)&store);
    assert(*store == element3);


    printf("\nll_insert_element(l, 1, element4)\n");
    ll_insert_element(l, 1, &element4);
    ll_map(l, map_fn);
    ll_get_element(l, 1, (const void**)&store);
    assert(*store == element4);

    deinit_llist(&l);
    printf("\nDone!\n");


}



int main()
{
    test_insert_delete();
    test_insert_get();
    return 0;
}

