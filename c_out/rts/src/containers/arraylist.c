#include "containers/arraylist.h"

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

#include "containers/mmanager.h"
#include "stg/math.h"
#include "stg/util.h"

static const int GROWTH_FACTOR = 2;

static err_t must_grow  (int capacity, int n) { return capacity <= n * GROWTH_FACTOR; }
static err_t must_shrink(int capacity, int n)
{
    return capacity >= MIN_SIZE * GROWTH_FACTOR && capacity >= n * GROWTH_FACTOR * GROWTH_FACTOR;
}
static err_t must_resize(struct resizable_array const * inp, int n)
{
    int capacity = inp->capacity;
    return must_grow(capacity, n) || must_shrink(capacity, n);
}

static err_t resize(struct resizable_array* inp, int n)
{

    assert(must_resize(inp, n));

    assert(n * GROWTH_FACTOR >= MIN_SIZE &&
           n < inp->capacity &&
           n * GROWTH_FACTOR >= inp->size &&
           n <= inp->size * GROWTH_FACTOR);

    int old_size = inp->size;
    int old_capacity = inp->capacity;
    const void ** old_array = inp->array;

    int new_capacity;

    if (must_grow(old_capacity, n)) new_capacity = GROWTH_FACTOR * old_capacity;
    else if (must_shrink(old_capacity, n)) new_capacity = max(MIN_SIZE, old_capacity/GROWTH_FACTOR);
    else assert(false); // Attempted to resize when there was no need to.
    assert(new_capacity >= n * GROWTH_FACTOR);
    assert(new_capacity > old_size);

    const void** new_arr = (const void**) new(sizeof (void*) * new_capacity);
    void* ptr = memcpy(new_arr, inp->array, sizeof (void*)* inp->size);
    assert(ptr == new_arr);

    delete((void**) &inp->array);

    inp->array = new_arr;
    inp->capacity = new_capacity;


    assert(old_size == inp->size);
    assert(old_array != inp->array);
    assert(old_capacity != inp->capacity);

    return 0;
}

err_t new_arrlist(size_t initial_capacity, struct arr_list** result)
{
    assert(initial_capacity >= MIN_SIZE);

    struct arr_list* res = (struct arr_list*) new(sizeof (struct arr_list));
    new_resizable_array(initial_capacity, &res->wrapped);
    assert(res->wrapped);
    assert(res->wrapped->array);
    assert(res->wrapped->capacity == initial_capacity);
    assert(res->wrapped->size == 0);

    *result = res;

    return 0;
}


err_t deinit_arrlist(struct arr_list** inp_ptr)
{
    deinit_resizable_array(&(*inp_ptr)->wrapped);
    assert(!(*inp_ptr)->wrapped);

    delete((void**) inp_ptr);
    assert(!*inp_ptr);

    return 0;
}

int arrl_size(const struct arr_list * inp) { return resizable_array_size(inp->wrapped); }

int arrl_capacity(const struct arr_list* inp) { return resizable_array_capacity(inp->wrapped); }

int arrl_is_empty(const struct arr_list* inp) { return resizable_array_is_empty(inp->wrapped); }

err_t arrl_get_element(struct arr_list const* inp, int n, const void **res)
{
    return resizable_array_get_element(inp->wrapped, n, res);
}

err_t arrl_append(struct arr_list * inp, const void *element)
{
    const void* old_array = inp->wrapped->array;
    int old_capacity = arrl_capacity(inp);
    int old_size = arrl_size(inp);

    int return_code = 0;
    if(must_grow(inp->wrapped->capacity, old_size + 1)) return_code = resize(inp->wrapped, old_size + 1);
    return_code |= resizable_array_append(inp->wrapped, element);
    assert(inp->wrapped->size * GROWTH_FACTOR >= old_capacity || old_array == inp->wrapped->array);
    assert(inp->wrapped->size * GROWTH_FACTOR <  old_capacity|| GROWTH_FACTOR*old_capacity == inp->wrapped->capacity);

    return return_code;
}

err_t arrl_pop_back(struct arr_list * inp, const void ** ptr)
{
    const void* old_array = inp->wrapped->array;
    int old_capacity = arrl_capacity(inp);
    int old_size = arrl_size(inp);

    int return_code = 0;
    if(must_shrink(inp->wrapped->capacity, old_size - 1)) return_code = resize(inp->wrapped, old_size - 1);

    return_code |= resizable_array_pop_back(inp->wrapped, ptr);
    assert(!must_grow(old_capacity, inp->wrapped->size) ||
           (old_array != inp->wrapped->array && old_capacity * GROWTH_FACTOR <= inp->wrapped->capacity)); // must

    return return_code;
}
