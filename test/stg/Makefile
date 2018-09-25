SRC_ROOT := src
GCC_FLAGS := -Wall -ggdb
COMPILER := ${comp}
GCC_CMD := ${COMPILER} $(GCC_FLAGS) -I. -I $(SRC_ROOT)

all: test/stg/target/test

test/stg/target/stg/plus_int/%.o: src/stg/plus_int/%.c
	mkdir -p test/stg/target/stg/plus_int
	$(GCC_CMD) $^ -c -o $@

test/data/target/data/%.o: src/data/%.c
	mkdir -p test/data/target/data
	$(GCC_CMD) $^ -c -o $@

test/data/target/containers/%.o: src/containers/%.c
	mkdir -p test/data/target/containers
	$(GCC_CMD) $^ -c -o $@

test/stg/target/test: test/stg/target/stg/plus_int/code.o test/stg/target/stg/plus_int/stack.o test/stg/target/stg/plus_int/static.o \
		test/data/target/data/string_.o test/containers/target/containers/mmanager.o test/containers/target/containers/arraylist.o\
		     test/containers/target/stg/math.o test/containers/target/stg/util.o test/containers/target/containers/llist.o\
		   test/containers/target/containers/hash_set.o test/containers/target/containers/resizable_array.o test/containers/target/containers/hash_map.o 

	$(GCC_CMD) -I . $^ -o $@


.PHONY clean:
clean:
	rm -rf test/data/target/
	rm -rf test/stg/target/

