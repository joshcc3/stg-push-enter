SRC_ROOT := src
GCC_FLAGS := -Wall -ggdb
COMPILER := ${comp}
GCC_CMD := ${COMPILER} $(GCC_FLAGS) -I. -I $(SRC_ROOT)

all: test/data/target/test_string

test/data/target/data/%.o: src/data/%.c
	mkdir -p test/data/target/data
	$(GCC_CMD) $^ -c -o $@

test/data/target/stg/%.o: src/stg/%.c
	mkdir -p test/data/target/stg
	$(GCC_CMD) $^ -c -o $@

test/data/target/test_string: test/data/test_string.c test/containers/target/containers/mmanager.o test/data/target/data/string_.o
	$(GCC_CMD) -I . $^ -o $@


.PHONY clean:
clean:
	rm -rf test/data/target/
