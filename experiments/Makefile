

all: test_linked_list

target/mmanager.o: mmanager.c
	mkdir -p target
	gcc -Wall mmanager.c -c -o target/mmanager.o

target/llist.o: llist.c
	mkdir -p target
	gcc -Wall llist.c -c -o target/llist.o

test_linked_list: main.c mmanager.o target/llist.o
	gcc -Wall main.c target/llist.o -o target/test_linked_list


.PHONY clean:
clean:
	rm -rf target/
