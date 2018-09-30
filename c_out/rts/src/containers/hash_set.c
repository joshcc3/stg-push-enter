#include "hash_set.h"

#include <assert.h>

#include "stg/math.h"
#include "containers/mmanager.h"
#include "typeclasses.h"

// ---------------- Internal Functions -----------------------

static const int GROWTH_FACTOR = 2;
static const double GROW_LOAD_FACTOR_THRESHOLD = 1.5;
static const double SHRINK_LOAD_FACTOR_THRESHOLD = 0.5;
static const int HSET_MIN_SIZE = 16;


static void std_pre(struct hash_set const * inp)
{
    assert(inp->size >= 0 &&
           inp->hash_table);
}

int hset_size(struct hash_set const *inp) { return inp->size; }

int hset_capacity(struct hash_set *inp) { return resizable_array_capacity(inp->hash_table); }

bool hset_is_empty(struct hash_set * inp) { return hset_size(inp) > 0; }

err_t hset_to_arr_list(struct hash_set const * inp, struct arr_list ** output)
{
    std_pre(inp);
    int old_size = hset_size(inp);

    struct arr_list * result;
    int return_code = new_arrlist(max(hset_size(inp), HSET_MIN_SIZE), &result);
    assert(!return_code);

    int index = 0;
    for (int i = 0; i < resizable_array_capacity(inp->hash_table); i++)
    {
        struct linked_list * bucket;
        return_code = resizable_array_get_element(inp->hash_table, i, (const void **)&bucket);
        assert(return_code == 0);

        for (int j = 0; bucket && j < ll_size(bucket); j++)
        {
            const void *elem;
            ll_get_element(bucket, j, &elem);
            assert(elem);
            arrl_append(result, elem);
            index++;
        }
    }
    assert(index == hset_size(inp));

    *output = result;

    assert(old_size == hset_size(inp));
    assert(output && *output && arrl_size(*output) == old_size);

    return 0;
}


static double load_factor(int size, int buckets) { return (double)size/buckets; }

static err_t must_grow  (int size, int buckets) { return load_factor(size, buckets) >= GROW_LOAD_FACTOR_THRESHOLD; }
static err_t must_shrink(int size, int buckets) { return buckets >= HSET_MIN_SIZE * GROWTH_FACTOR && load_factor(size, buckets) <= SHRINK_LOAD_FACTOR_THRESHOLD; }
static err_t must_resize(struct hash_set const * inp, int n)
{
    int buckets = resizable_array_capacity(inp->hash_table);
    return must_grow(n, buckets) || must_shrink(n, buckets);
}

static err_t resize(struct hash_set ** inp_ptr, int n, struct equals_typeclass const * e_t, struct obj_typeclass const * o_t)
{
    struct hash_set * inp = *inp_ptr;
    int old_size = inp->size;
    int old_capacity = resizable_array_capacity(inp->hash_table);

    if (!must_resize(inp, n)) return 0;

    struct resizable_array * old_wrapped = inp->hash_table;

    int new_capacity;

    if (must_grow(n, old_capacity)) new_capacity = GROWTH_FACTOR * old_capacity;
    else if (must_shrink(n, old_capacity)) new_capacity = max(HSET_MIN_SIZE, old_capacity/GROWTH_FACTOR);
    else assert(false); // Attempted to resize when there was no need to.
    assert(new_capacity * GROW_LOAD_FACTOR_THRESHOLD >= n);
    assert(new_capacity * GROW_LOAD_FACTOR_THRESHOLD > old_size);

    struct hash_set * new_hash_set;
    hset_init(&new_hash_set, new_capacity);

    struct arr_list * arrlist_rep;
    hset_to_arr_list(inp, &arrlist_rep);
    assert(arrlist_rep);
    assert(arrl_size(arrlist_rep) == old_size);

    while(arrl_size(arrlist_rep) > 0)
    {
        const void *elem;
        arrl_pop_back(arrlist_rep, &elem);
        int return_code = hset_insert_element(&new_hash_set, elem, e_t, o_t);
        assert(return_code == 0);
    }

    deinit_arrlist(&arrlist_rep);
    hset_deinit(&inp);

    *inp_ptr = new_hash_set;
    inp = *inp_ptr;


    assert(old_size == inp->size);
    assert(old_wrapped != inp->hash_table);

    return 0;
}

// --------- Exposed Functions -------------

// Change resize to be called every time.
err_t hset_init(struct hash_set ** result, size_t capacity)
{
    struct resizable_array* hash_table;
    new_resizable_array(capacity, &hash_table);
    assert(hash_table);

    *result = malloc(sizeof (struct hash_set));
    (*result)->hash_table = hash_table;
    (*result)->size = 0;

    assert(*result && hset_size(*result) == 0 && (*result)->hash_table);

    return 0;
}

err_t hset_deinit(struct hash_set ** inp)
{
    assert(inp);

    deinit_resizable_array(&((*inp)->hash_table));
    assert(!(*inp)->hash_table);

    delete((void**) inp);

    assert(!*inp);
    return 0;
}

err_t hset_get_element(struct hash_set *inp,
                       const void *element,
                       struct equals_typeclass const * e_t,
                       struct obj_typeclass const * o_t,
                       const void **result)
{
    std_pre(inp);

    bool found;
    const void *candidate = NULL;
    int return_code;

    hash_t hash = o_t->hash(element, o_t->extra);
    assert(hash >= 0);

    int actual_index = hash % hset_capacity(inp);

    struct linked_list* bucket;
    resizable_array_get_element(inp->hash_table, actual_index, (const void**)&bucket);

    if (!bucket) found = false;
    else
    {
        ll_search(bucket, element, e_t, &candidate);
        if (candidate) found = true;
        else found = false;
    }

    if (found)
    {
        return_code = 0;
        *result = candidate;
    }
    else
    {
        return_code = ELEMENT_NOT_FOUND;
        *result = NULL;
    }

    assert(return_code != 0 || (*result && e_t->equal(element, *result, e_t->extra)));
    assert(return_code == 0 || (return_code == ELEMENT_NOT_FOUND && !*result));

    return return_code;
}

int hset_contains(struct hash_set *inp, const void *element, struct equals_typeclass const* e_t, struct obj_typeclass  const* o_t)
 {
    std_pre(inp);

    const void *result;
    int return_code = hset_get_element(inp, element, e_t, o_t, &result);

    if (return_code == 0) return true;
    else if (return_code == ELEMENT_NOT_FOUND) return false;
    else if (return_code != ELEMENT_NOT_FOUND && return_code < 0) return return_code;

    // should never reach here, return code > 0
    assert(false);
    return -1;
}

err_t hset_insert_element(struct hash_set ** inp_ptr,
                          const void * element,
                          struct equals_typeclass const* e_t,
                          struct obj_typeclass  const* o_t)
{
    struct hash_set *inp = *inp_ptr;
    std_pre(inp);
    int old_size = hset_size(inp);
    int result = 0;

    const int contains_result = hset_contains(inp, element, e_t, o_t);
    if(contains_result == true)
    {
        const void* del_elem;
        hset_delete_element(inp_ptr, element, &del_elem, e_t, o_t);
        hset_insert_element(inp_ptr, element, e_t, o_t);
        result = 1;
    }
    else if (contains_result == false)
    {
        int buckets = resizable_array_capacity(inp->hash_table);
        if (must_grow(old_size + 1, buckets)) resize(&inp, old_size + 1, e_t, o_t);

        hash_t hash = o_t->hash(element, o_t->extra);
        assert(hash >= 0);

        int actual_index = hash % hset_capacity(inp);

        struct linked_list* bucket;
        resizable_array_get_element(inp->hash_table, actual_index, (const void**)&bucket);

        if (!bucket)
        {
            bucket = new_llist();
            assert(bucket);

            result |= resizable_array_set_element(inp->hash_table, actual_index, (void**)bucket);
            assert(result == 0);
        }
        result |= ll_insert_element(bucket, ll_size(bucket), element);
        assert(result == 0);

        inp->size++;
    }
    else
    {
        assert(false);
        // No other errors should arise
    }

    *inp_ptr = inp;

    assert(contains_result || old_size + 1 == hset_size(inp));
    return result;

}

err_t hset_delete_element(struct hash_set ** inp_ptr,
                          const void * elem,
                          const void ** del_elem,
                          struct equals_typeclass const* e_t,
                          struct obj_typeclass const* o_t)
{
    struct hash_set *inp = *inp_ptr;

    std_pre(inp);
    int old_size = hset_size(inp);
    struct resizable_array* old_hash_table = inp->hash_table;

    int result = 0;
    int found = hset_contains(inp, elem, e_t, o_t);
    if(found == false)
    {
        *del_elem = NULL;
        result = ELEMENT_NOT_FOUND;
    }
    else if(found == true)
    {
        int buckets = resizable_array_capacity(inp->hash_table);
        if (must_shrink(old_size - 1, buckets)) resize(&inp, old_size - 1, e_t, o_t);
        hash_t hash = o_t->hash(elem, o_t->extra);
        assert(hash >= 0);

        int actual_index = hash % hset_capacity(inp);

        struct linked_list* bucket;
        resizable_array_get_element(inp->hash_table, actual_index, (const void**)&bucket);
        assert(bucket);

        int index;
        result |= ll_index_of(bucket, elem, e_t, &index);
        result |= ll_delete_element(bucket, index, del_elem);
        assert(result == 0 && index >= 0 && *del_elem);

        inp->size--;

    }
    else assert(false);

    *inp_ptr = inp;

    assert(!found || (hset_size(inp) + 1 == old_size && del_elem));
    assert(found || (hset_size(inp) == old_size && *del_elem == NULL && inp->hash_table == old_hash_table));


    return result;
}
