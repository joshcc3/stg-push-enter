SRC_ROOT := src
GCC_FLAGS := -Wall -ggdb
COMPILER := ${comp}
GCC_CMD := ${COMPILER} $(GCC_FLAGS) -I. -I $(SRC_ROOT)

all: test/stg/target/sample_program

test/stg/target/stg/%.o: src/stg/%.c
	mkdir -p test/stg/target/stg
	$(GCC_CMD) $^ -c -o $@

test/data/target/data/%.o: src/data/%.c
	mkdir -p test/data/target/data
	$(GCC_CMD) $^ -c -o $@

test/stg/target/sample_program: test/stg/sample_program.c test/data/target/data/string_.o

	$(GCC_CMD) -I . $^ -o $@


.PHONY clean:
clean:
	rm -rf test/data/target/
	rm -rf test/stg/target/
