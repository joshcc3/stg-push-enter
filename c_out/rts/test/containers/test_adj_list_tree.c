#include <assert.h>
#include <stdio.h>

#include "containers/adj_list_tree.h"

hash_t hash_int(void const* element, void const * extra) { return abs(*((int const*)element)); }

bool int_pointer_equals(void const *p1, void const *p2, void const * extra) { return *(int*)p1 == *(int*)p2; }
bool int_pointer_not_equals(void const *p1, void const *p2, void const * extra) { return *(int*)p1 != *(int*)p2; }

const struct equals_typeclass int_equals_typeclass_witness = { .equal = int_pointer_equals, .not_equal = int_pointer_not_equals, NULL };
const struct obj_typeclass int_obj_typeclass = { .hash = hash_int, NULL };


void test_add_two_edges_for_node() {
  struct adj_list_tree *tree = NULL;
  init_adj_list_tree(&tree, 5);
  adj_list_tree_add_edge(tree, 0, 1);
  adj_list_tree_add_edge(tree, 0, 2);
  // They should be side effect free
  assert(adj_list_tree_edge_exists(tree, 0, 1));
  assert(adj_list_tree_edge_exists(tree, 0, 2));
  assert(adj_list_tree_edge_exists(tree, 0, 1));
  assert(adj_list_tree_edge_exists(tree, 0, 2));
  assert(!adj_list_tree_edge_exists(tree, 0, 3));
  assert(!adj_list_tree_edge_exists(tree, 1, 2));
  assert(!adj_list_tree_edge_exists(tree, 2, 0));
  assert(adj_list_tree_edge_exists(tree, 0, 1));
  assert(adj_list_tree_edge_exists(tree, 0, 2));
  
  deinit_adj_list_tree(&tree);

}

void test_complete_graph() {
  struct adj_list_tree *tree = NULL;
  init_adj_list_tree(&tree, 5);

  for(int i = 0; i < 5; i++) 
  {
    for(int j = 0; j < 5; j++)
    {
      if(i != j) adj_list_tree_add_edge(tree, i, j);
    }

  }
  for(int i = 0; i < 5; i++) 
  {
    for(int j = 0; j < 5; j++)
    {
      if(i != j) assert(adj_list_tree_edge_exists(tree, i, j));
      else assert(!adj_list_tree_edge_exists(tree, i, j));
    }

  }
  
  deinit_adj_list_tree(&tree);
}


void test_add_an_edge() {

    struct adj_list_tree *tree = NULL;
    init_adj_list_tree(&tree, 5);
    

    assert(adj_list_tree_edge_exists(tree, 0, 1) == false);
    assert(adj_list_tree_add_edge(tree, 0, 1) == false);
    assert(adj_list_tree_edge_exists(tree, 0, 1) == true);

    deinit_adj_list_tree(&tree);
}


int main() {
  test_add_an_edge();
  printf("Tested adding an edge\n");
  test_add_two_edges_for_node();
  printf("Tested adding two edges\n");
  test_complete_graph();
  printf("Tested complete graph\n");
  
}
