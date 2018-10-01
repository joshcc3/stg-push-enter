#include "hash_map.h"
#include "resizable_array.h"
#include "mmanager.h"
#include <assert.h>

bool hash_map_element_equals(void const *a, void const *b, void const *extra)
{
    struct equals_typeclass * given = (struct equals_typeclass *) extra;
    return given->equal(((struct hash_map_element*)a)->key, ((struct hash_map_element*)b)->key, given->extra);
}
bool hash_map_element_not_equals(void const *a, void const *b, void const *extra)
{
    struct equals_typeclass * given = (struct equals_typeclass *) extra;
    return given->not_equal(((struct hash_map_element*)a)->key, ((struct hash_map_element*)b)->key, given->extra);
}

hash_t hash_map_element_hash(void const *inp, const void *extra)
{
    struct obj_typeclass * given = (struct obj_typeclass *) extra;
    return given->hash(((struct hash_map_element*)inp)->key, given->extra);
}

void init_hash_map(struct hash_map ** const inp_ptr, size_t size, const struct equals_typeclass * e_t, const struct obj_typeclass * o_t)
{
  assert(inp_ptr && size >= 0 && e_t && o_t);

  *inp_ptr = (struct hash_map*)new(sizeof(struct hash_map));
  struct hash_map* inp = *inp_ptr;
  assert(inp);

  inp->wrapped = NULL;
  {
    struct hash_set *tmp;
    hset_init(&tmp, size);
    inp->wrapped = tmp;
  }
  assert(inp->wrapped);

  struct equals_typeclass *e_tt = (struct equals_typeclass*)new(sizeof(struct equals_typeclass));
  e_tt->equal = &hash_map_element_equals;
  e_tt->not_equal = &hash_map_element_not_equals;
  e_tt->extra = e_t;
  inp->e_t = e_tt;

  struct obj_typeclass *o_tt = (struct obj_typeclass*)new(sizeof(struct obj_typeclass));
  o_tt->hash = &hash_map_element_hash;
  o_tt->extra = o_t;
  inp->o_t = o_tt;

  assert(inp && inp->wrapped);
  assert(resizable_array_capacity(inp->wrapped->hash_table) == size);
  assert(hset_size(inp->wrapped) == 0);
}


static void std_pre(const struct hash_map * const inp)
{
    assert(inp);
    assert(inp->wrapped);
}

err_t deinit_hash_map(struct hash_map ** const inp_ptr)
{
    std_pre(*inp_ptr);
    struct hash_map *inp = *inp_ptr;

    err_t result = hset_deinit(&(inp->wrapped));

    if(result == 0)
    {
        assert(inp->wrapped == NULL);
        *inp_ptr = NULL;
    }

    return result;
}

int hash_map_size(const struct hash_map * inp)
{
    std_pre(inp);
    return hset_size(inp->wrapped);
}

bool hash_map_contains_key(const struct hash_map * inp,
                           const void * element)
{
    assert(inp && element);

    struct hash_map_element hash_map_element = { .key = element, .value = NULL };
    bool found = hset_contains(inp->wrapped, &hash_map_element, inp->e_t, inp->o_t);

    assert(!found || hash_map_size(inp) >= 1);
    assert(hash_map_size(inp) != 0 || !found);

    return found;
}

bool hash_map_get(const struct hash_map * inp, const void* key, const void ** output)
{
    assert(inp && key && output);
    *output = NULL;

    struct hash_map_element * elem = (struct hash_map_element*)new(sizeof(struct hash_map_element));
    elem->key = key;
    elem->value = NULL;

    const void *hash_map_bucket = NULL;
    err_t result = hset_get_element(inp->wrapped, elem, inp->e_t, inp->o_t, &hash_map_bucket);

    if(result == 0) *output = (const void*)(((struct hash_map_element *)hash_map_bucket)->value);

    delete((void**)&elem);

    assert(!hash_map_contains_key(inp, key) || (result == 0));

    return result == 0;
}

void hash_map_put(struct hash_map **inp_ptr, const void* key, const void * value)
{
    assert(inp_ptr && *inp_ptr && key);

    struct hash_map *inp = *inp_ptr;
    struct hash_map_element *hash_elem = new(sizeof(struct hash_map_element));
    hash_elem->key = key;
    hash_elem->value = value;
    struct hash_set * set = inp->wrapped;
    hset_insert_element(&set, hash_elem, inp->e_t, inp->o_t);
    inp->wrapped = set;

    assert(hash_map_contains_key(*inp_ptr, key));
    const void *out;
    hash_map_get(*inp_ptr, key, &out);
    assert(out == value);


}


void hash_map_delete(struct hash_map ** inp_ptr, const void * key)
{
    assert(inp_ptr && *inp_ptr && key);

    struct hash_map *inp = *inp_ptr;
    const struct hash_map_element hash_elem = { .key = key, .value = NULL };
    const void *del_elem;
    struct hash_set* hset = inp->wrapped;
    hset_delete_element(&hset, &hash_elem, &del_elem, inp->e_t, inp->o_t);
    inp->wrapped = hset;
    assert(!hash_map_contains_key(*inp_ptr, key));

}
