#include "containers/resizable_array.h"

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "containers/mmanager.h"
#include "stg/math.h"
#include "stg/util.h"

static void std_pre(const struct resizable_array * inp)
{
    assert(inp);
    assert(inp->size >= 0);
    assert(inp->capacity > inp->size && inp->capacity >= MIN_SIZE);
    assert(inp->size == 0 || inp->array);
}

err_t new_resizable_array(size_t initial_capacity, struct resizable_array** result)
{
    assert(initial_capacity >= MIN_SIZE);

    struct resizable_array* res = (struct resizable_array*) new(sizeof (struct resizable_array));
    res->capacity = initial_capacity;
    res->size = 0;

    const void** arr = (const void**)new_calloc(initial_capacity, sizeof (void*));
    res->array = arr;

    *result = res;

    assert(*result);
    return 0;
}

err_t deinit_resizable_array(struct resizable_array** inp_ptr)
{
    struct resizable_array* inp = *inp_ptr;
    std_pre(inp);

    assert(inp->array);
    delete((void**) &inp->array);

    delete((void**) inp_ptr);

    assert(!*inp_ptr);

    return 0;
}

int resizable_array_size(const struct resizable_array * inp)
{
    std_pre(inp);
    return inp->size;
}

int resizable_array_capacity(const struct resizable_array* inp)
{
    std_pre(inp);
    return inp->capacity;
}

int resizable_array_is_empty(const struct resizable_array* inp)
{
    return resizable_array_size(inp) == 0;
}

err_t resizable_array_get_element(struct resizable_array const* inp, int n, const void **result)
{
    std_pre(inp);
    int old_capacity = inp->capacity;
    int old_size = inp->size;
    const void** old_array = inp->array;

    int actual_index = zero_based_index(n, old_size);

    *result = inp->array[actual_index];

    assert(old_capacity == inp->capacity);
    assert(old_size == inp->size);
    assert(old_array == inp->array);

    return 0;

}

err_t resizable_array_set_element(struct resizable_array const * inp, int index, void const * element)
{
    std_pre(inp);
    int old_capacity = inp->capacity;
    int old_size = inp->size;
    const void** old_array = inp->array;

    int actual_index = zero_based_index(index, old_size);
    inp->array[actual_index] = element;

    const void *elem;
    resizable_array_get_element(inp, index, &elem);

    assert(elem == element);
    assert(inp->size == old_size);
    assert(inp->array == old_array);
    assert(inp->capacity == old_capacity);

    return 0;
}

err_t resizable_array_append(struct resizable_array * inp, const void *element)
{
    std_pre(inp);
    assert(inp->size < inp->capacity);
    int old_size = inp->size;

    inp->array[old_size] = element;
    inp->size++;

    assert(old_size + 1 == inp->size);

    return 0;
}

err_t resizable_array_pop_back(struct resizable_array * inp, const void ** element_ptr)
{
    std_pre(inp);
    assert(inp->size >= 0);
    int old_size = inp->size;

    *element_ptr = inp->array[old_size - 1];
    inp->size--;

    assert(old_size - 1 == inp->size);

    return 0;

}

