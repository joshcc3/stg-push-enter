SRC_ROOT := src
GCC_FLAGS := -Wall -ggdb
COMPILER := ${comp}
GCC_CMD := ${COMPILER} $(GCC_FLAGS) -I. -I $(SRC_ROOT)

all: test/stg/target/main1 test/stg/target/main2 test/stg/target/main3

test/stg/target/stg/plus_int/%.o: src/stg/plus_int/%.c
	mkdir -p test/stg/target/stg/plus_int
	$(GCC_CMD) $^ -c -o $@

test/stg/target/stg/list/%.o: src/stg/list/%.c
	mkdir -p test/stg/target/stg/list
	$(GCC_CMD) $^ -c -o $@

test/stg/target/stg/%.o: src/stg/%.c
	mkdir -p test/stg/target/stg/
	$(GCC_CMD) $^ -c -o $@

test/data/target/data/%.o: src/data/%.c
	mkdir -p test/data/target/data
	$(GCC_CMD) $^ -c -o $@

test/containers/target/containers/%.o: src/containers/%.c
	mkdir -p test/containers/target/containers
	$(GCC_CMD) $^ -c -o $@

test/target/%.o: src/%.c
	mkdir -p test/target
	$(GCC_CMD) $^ -c -o $@

test/stg/target/test_bindings: test/stg/test_bindings.c test/stg/target/stg/plus_int/code.o test/stg/target/stg/plus_int/stack.o test/stg/target/stg/plus_int/static.o test/stg/target/stg/plus_int/main.o test/stg/target/stg/list/list.o \
        test/stg/target/stg/heap.o \
		test/data/target/data/string_.o test/containers/target/containers/mmanager.o test/containers/target/containers/arraylist.o\
		     test/stg/target/stg/math.o test/target/typeclasses.o  test/stg/target/stg/bindings.o test/stg/target/stg/util.o test/containers/target/containers/llist.o\
		   test/containers/target/containers/hash_set.o test/containers/target/containers/resizable_array.o test/containers/target/containers/hash_map.o 

	$(GCC_CMD) -I . $^ -o $@

test/stg/target/main1: test/stg/target/stg/plus_int/code.o test/stg/target/stg/plus_int/stack.o test/stg/target/stg/plus_int/static.o test/stg/target/stg/plus_int/main.o test/stg/target/stg/list/list.o \
        test/stg/target/stg/heap.o \
		test/data/target/data/string_.o test/containers/target/containers/mmanager.o test/containers/target/containers/arraylist.o\
		     test/stg/target/stg/math.o test/target/typeclasses.o  test/stg/target/stg/bindings.o test/stg/target/stg/util.o test/containers/target/containers/llist.o\
		   test/containers/target/containers/hash_set.o test/containers/target/containers/resizable_array.o test/containers/target/containers/hash_map.o

	$(GCC_CMD) -I . $^ -o $@

test/stg/target/main2: test/stg/target/stg/plus_int/code.o test/stg/target/stg/plus_int/stack.o test/stg/target/stg/plus_int/static.o test/stg/target/stg/plus_int/main2.o test/stg/target/stg/list/list.o \
        test/stg/target/stg/heap.o \
		test/data/target/data/string_.o test/containers/target/containers/mmanager.o test/containers/target/containers/arraylist.o\
		     test/stg/target/stg/math.o test/stg/target/stg/bindings.o test/target/typeclasses.o test/stg/target/stg/util.o test/containers/target/containers/llist.o\
		   test/containers/target/containers/hash_set.o test/containers/target/containers/resizable_array.o test/containers/target/containers/hash_map.o 

	$(GCC_CMD) -I . $^ -o $@

test/stg/target/main3: test/stg/target/stg/plus_int/code.o test/stg/target/stg/plus_int/stack.o test/stg/target/stg/plus_int/static.o test/stg/target/stg/list/list.o \
        test/stg/target/stg/heap.o \
		test/data/target/data/string_.o test/containers/target/containers/mmanager.o test/containers/target/containers/arraylist.o\
		     test/stg/target/stg/math.o test/stg/target/stg/bindings.o test/target/typeclasses.o test/stg/target/stg/util.o test/containers/target/containers/llist.o\
		   test/containers/target/containers/hash_set.o test/containers/target/containers/resizable_array.o test/containers/target/containers/hash_map.o \
           test/stg/target/stg/main3.o
	$(GCC_CMD) -I . $^ -o $@


.PHONY clean:
clean:
	rm -rf test/data/target/
	rm -rf test/stg/target/
	rm -rf test/containers/target/
	rm -rf test/target/*.o
	rm -rf test/*.o

