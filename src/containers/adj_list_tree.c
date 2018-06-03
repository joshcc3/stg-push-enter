#include "adj_list_tree.h"
#include <assert.h>
#include "mmanager.h"
#include <stg/math.h>



void std_pre(const struct adj_list_tree *inp, int from, int to)
{
  assert(inp->nodes > from && from >= 0 && inp->nodes > to && to >= 0);
}

/*
struct adj_list_tree {
  // hash map from int to hash set of int
  struct hash_map *wrapped;
};
*/

bool equals(void const *a, void const *b, void const* extra) 
{ 
  int int_a = *(int*)a;
  int int_b = *(int*)b;

  assert(int_a >= 0 && int_b >= 0);

  return int_a == int_b;

}

bool not_equals(void const *a, void const *b, void const* extra) {  return equals(a, b, extra) == false; }

hash_t hash(void const * a, void const *extra)
{
  int int_a = *(int*)a;

  assert(int_a >= 0);

  return int_a;

}


const struct equals_typeclass eq_t = { .equal = equals, .not_equal = not_equals };
const struct obj_typeclass o_t = { .hash = hash };

err_t init_adj_list_tree(struct adj_list_tree ** result, int nodes)
{
  assert(result);
  struct adj_list_tree *tmp_tree = (struct adj_list_tree *)new(sizeof(const struct adj_list_tree));
  tmp_tree->nodes = nodes;
  
  struct hash_map * mp = NULL;
  init_hash_map(&mp, max(nodes*MAGIC_MULTIPLIER, 16), &eq_t, &o_t);
  tmp_tree->wrapped = mp;

  *result = tmp_tree;

  assert(result && *result && *result == tmp_tree && (*result)->nodes == nodes && hash_map_size((*result)->wrapped) == 0);

  return 0;

}

err_t deinit_adj_list_tree(struct adj_list_tree ** input)
{
  assert(input && *input && (*input)->nodes >= 0);

  struct hash_map* tmp = (*input)->wrapped;
  deinit_hash_map(&tmp);
  assert(tmp == NULL);
  delete((void**)input);

  assert(*input == NULL);

  return 0;

}

bool adj_list_tree_add_edge(struct adj_list_tree *inp, int from, int to)
{
  std_pre(inp, from, to);
  assert(from != to);

  struct hash_set *set;
  bool present = hash_map_get(inp->wrapped, &from, (const void**)(&set));
  int *to_ptr = new(sizeof(int));
  *to_ptr = to;

  if(!present) { hset_init(&set, 16); }

  bool to_present = hset_contains(set, to_ptr, &eq_t, &o_t);
  hset_insert_element(&set, to_ptr, &eq_t, &o_t);

  struct hash_map * tmp_map = inp->wrapped;
  hash_map_put(&tmp_map, &from, set);
  inp->wrapped = tmp_map;
  
  assert(adj_list_tree_edge_exists(inp, from, to));

  return to_present;
}


bool adj_list_tree_edge_exists(const struct adj_list_tree *inp, int from, int to)
{
  std_pre(inp, from, to);

  const int *key = &from;

  const void* set_tmp;
  bool from_present = hash_map_get(inp->wrapped, key, &set_tmp);
  bool contains = false;
  if(from_present) {
    const struct hash_set * set = (struct hash_set *)set_tmp;
    contains = hset_contains(set, &to, &eq_t, &o_t);
  }

  assert(!contains || (hash_map_size(inp->wrapped) >= 1 && hash_map_contains_key(inp->wrapped, key)));
  return contains;

}

/*
bool adj_list_tree_delete_edge(int, int);
err_t adj_list_tree_get_edges(int, const struct linked_list **);
*/
