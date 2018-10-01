#include <assert.h>
#include <stdio.h>

#include "containers/hash_map.h"

/*
struct hash_map_element {
    const void * const key;
    const void * const value;
};

// A hash map consists of a hash set of a pair of (key, value) where equality between the elements is based on the key.
struct hash_map {
    struct hash_set * wrapped;
};


void init_hash_map(struct hash_map ** const, size_t, struct equals_typeclass const*, struct obj_typeclass const*);

err_t deinit_hash_map(struct hash_map ** const);

int hash_map_size(const struct hash_map *);

bool hash_map_contains_key(const struct hash_map *, const void *);

bool hash_map_get(const struct hash_map *, const void*, const void **);

bool hash_map_put(const struct hash_map *, const void*, const void *);

bool hash_map_delete(const struct hash_map **, const void *);

*/

hash_t hash_int(void const* element, void const * extra) { return abs(*((int const*)element)); }

bool int_pointer_equals(void const *p1, void const *p2, void const * extra) { return *(int*)p1 == *(int*)p2; }
bool int_pointer_not_equals(void const *p1, void const *p2, void const * extra) { return *(int*)p1 != *(int*)p2; }

const struct equals_typeclass ptr_equals_typeclass_witness = { .equal = int_pointer_equals, .not_equal = int_pointer_not_equals, NULL };
const struct obj_typeclass int_obj_typeclass = { .hash = hash_int, NULL };

int main() {

    struct hash_map *map = NULL;
    init_hash_map(&map, 16, &ptr_equals_typeclass_witness, &int_obj_typeclass);

    const int k_value = 1;
    const int value1 = 100;

    {
        int *val1;
        int result = hash_map_get(map, (const void*)&k_value, (const void **)&val1);
        assert(result == false);
    }
    

    printf("About to put hashmap\n");
    hash_map_put(&map, &k_value, &value1);

    {
      const int* val = NULL;
      bool result = hash_map_get(map, (const void *)&k_value, (const void **)&val);
      assert(result == true);
      assert(val == &value1);
    }

    hash_map_delete(&map, (const void *)&k_value);
    {
      const int* val = NULL;
      bool result = hash_map_get(map, (const void *)&k_value, (const void **)&val);
      assert(result == false);
      assert(val == NULL);
    }
    {
      const int* val = NULL;
      int k_value2 = 3;
      bool result = hash_map_get(map, (const void *)&k_value2, (const void **)&val);
      assert(result == false);
      assert(val == NULL);
    }
    


    assert(hash_map_size(map) == 0);
    hash_map_put(&map, &k_value, &value1);
    assert(hash_map_size(map) == 1);

    int value2 = 1;
    hash_map_put(&map, &k_value, &value2);
    assert(hash_map_size(map) == 1);

    int k_value2 = 10;
    int value3 = 12;
    hash_map_put(&map, &k_value2, &value3);
    assert(hash_map_size(map) == 2);

    int value4 = 110;
    hash_map_put(&map, &k_value2, &value4);
    assert(hash_map_size(map) == 2);

    {
      const int* val = NULL;
      int k_value2 = 10;
      bool result = hash_map_get(map, (const void *)&k_value2, (const void **)&val);
      assert(result == true);
      assert(val == &value4);
    }

    deinit_hash_map(&map);
    printf("Testing complete\n");
}
