

all: test_linked_list

target/llist.o: llist.c
	mkdir -p target
	gcc -Wall llist.c -c -o target/llist.o

test_linked_list: main.c target/llist.o
	gcc -Wall main.c target/llist.o -o target/test_linked_list


.PHONY clean:
clean:
	rm -rf target/
