SRC_ROOT := src
GCC_FLAGS := -Wall -ggdb
COMPILER := ${comp}
GCC_CMD := ${COMPILER} $(GCC_FLAGS) -I. -I $(SRC_ROOT)

all: test/containers/target/linked_list_test test/containers/target/arrlist_test test/containers/target/hashset_test test/containers/target/hashmap_test test/containers/target/adj_list_tree_test

test/containers/target/containers/%.o: src/containers/%.c
	mkdir -p test/containers/target/containers
	$(GCC_CMD) $^ -c -o $@

test/containers/target/stg/%.o: src/stg/%.c
	mkdir -p test/containers/target/stg
	$(GCC_CMD) $^ -c -o $@

test/containers/target/linked_list_test: test/containers/test_ll.c test/containers/target/containers/mmanager.o test/containers/target/containers/llist.o\
					 test/containers/target/stg/math.o test/containers/target/stg/util.o
	$(GCC_CMD) -I . $^ -o $@

test/containers/target/arrlist_test: test/containers/test_arrlist.c  test/containers/target/containers/mmanager.o test/containers/target/containers/arraylist.o\
				     test/containers/target/containers/resizable_array.o test/containers/target/stg/math.o test/containers/target/stg/util.o
	$(GCC_CMD) -I . $^ -o $@

test/containers/target/hashset_test: test/containers/test_hash_set.c test/containers/target/containers/mmanager.o test/containers/target/containers/arraylist.o\
				     test/containers/target/stg/math.o test/containers/target/stg/util.o test/containers/target/containers/llist.o\
				   test/containers/target/containers/hash_set.o test/containers/target/containers/resizable_array.o
	$(GCC_CMD) -I . $^ -o $@

test/containers/target/hashmap_test: test/containers/test_hash_map.c test/containers/target/containers/mmanager.o test/containers/target/containers/arraylist.o\
				     test/containers/target/stg/math.o test/containers/target/stg/util.o test/containers/target/containers/llist.o\
				   test/containers/target/containers/hash_set.o test/containers/target/containers/resizable_array.o test/containers/target/containers/hash_map.o
	$(GCC_CMD) -I . $^ -o $@

test/containers/target/adj_list_tree_test: test/containers/test_adj_list_tree.c test/containers/target/containers/mmanager.o test/containers/target/containers/arraylist.o\
				     test/containers/target/stg/math.o test/containers/target/stg/util.o test/containers/target/containers/llist.o\
				   test/containers/target/containers/hash_set.o test/containers/target/containers/resizable_array.o test/containers/target/containers/hash_map.o test/containers/target/containers/adj_list_tree.o
	$(GCC_CMD) -I . $^ -o $@


.PHONY clean:
clean:
	rm -rf test/containers/target/
