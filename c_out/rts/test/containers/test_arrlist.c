#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "containers/arraylist.h"

err_t map_fn(void **element_ptr)
{
    int* element = (int*)(*element_ptr);
    printf("%d, ", *element);

    return 0;
}

void test_insert_and_delete()
{
    /*
    err_t new_arrlist(size_t, struct arr_list**);

    int arrl_size(const struct arr_list *const);

    int arrl_capacity(const struct arr_list *const);

    int arrl_is_empty(const struct arr_list *const);

    err_t arrl_get_element(struct arr_list const * const, const int, void **);

    err_t arrl_append(struct arr_list * const, void *);

    err_t arrl_pop_back(struct arr_list *const, void * *const);
    */

    struct arr_list* alist;
    new_arrlist(20, &alist);

    int element1 = 1, element2 = 2, element3 = 3;
    int *store = 0;

    arrl_append(alist, &element1);
    arrl_get_element(alist, 0, (const void**)&store);
    arrl_pop_back(alist, (const void**)&store);
    assert(*store == element1);


    arrl_append(alist, &element1);
    arrl_append(alist, &element2);
    arrl_pop_back(alist, (const void**)&store);
    assert(*store == element2);
    arrl_pop_back(alist, (const void**)&store);
    assert(*store == element1);

    arrl_append(alist, &element1);
    arrl_append(alist, &element2);
    arrl_append(alist, &element3);
    arrl_pop_back(alist, (const void**)&store);
    assert(*store == element3);
    arrl_pop_back(alist, (const void**)&store);
    assert(*store == element2);
    arrl_pop_back(alist, (const void**)&store);
    assert(*store == element1);

    int elements[24];
    for(int i = 0; i < 24; i++)
    {
        elements[i] = i;
        arrl_append(alist, elements + i);

        arrl_get_element(alist, i, (const void**)&store);
        assert(*store == elements[i]);
    }

    assert(arrl_size(alist) == 24);
    assert(arrl_capacity(alist) == 80);
    arrl_pop_back(alist, (const void**)&store);
    arrl_pop_back(alist, (const void**)&store);
    arrl_pop_back(alist, (const void**)&store);
    arrl_pop_back(alist, (const void**)&store);
    arrl_pop_back(alist, (const void**)&store);

    assert(arrl_size(alist) == 19);
    assert(arrl_capacity(alist) == 40);

    deinit_arrlist(&alist);

}


void test_insert_and_get()
{
    /*
    err_t new_arrlist(size_t, struct arr_list**);

    int arrl_size(const struct arr_list *const);

    int arrl_capacity(const struct arr_list *const);

    int arrl_is_empty(const struct arr_list *const);

    err_t arrl_get_element(struct arr_list const * const, const int, void **);

    err_t arrl_append(struct arr_list * const, void *);

    err_t arrl_pop_back(struct arr_list *const, void * *const);
    */

    struct arr_list* alist;
    new_arrlist(20, &alist);

    int element1 = 1, element2 = 2, element3 = 3;
    int *store = 0;

    arrl_append(alist, &element1);
    arrl_get_element(alist, 0, (const void**)&store);
    assert(*store == element1);


    arrl_append(alist, &element2);
    arrl_get_element(alist, 0, (const void**)&store);
    assert(*store == element1);
    arrl_get_element(alist, 1, (const void**)&store);
    assert(*store == element2);

    arrl_append(alist, &element3);
    arrl_get_element(alist, 0, (const void**)&store);
    assert(*store == element1);
    arrl_get_element(alist, 1, (const void**)&store);
    assert(*store == element2);
    arrl_get_element(alist, 2, (const void**)&store);
    assert(*store == element3);


    int elements[21];
    for(int i = 3; i < 24; i++)
    {
        elements[i - 3] = i;
        arrl_append(alist, elements + i - 3);

        arrl_get_element(alist, i, (const void**)&store);
        assert(*store == elements[i - 3]);
    }

    deinit_arrlist(&alist);

}



int main()
{
    test_insert_and_delete();
    return 0;
}

