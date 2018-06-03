#include "imports.h"


struct node {
  void *element;
};

struct linked_list {
  int ll_size;
  struct node *head;
  struct node *tail;
};


struct linked_list * new_llist() {

    struct linked_list * new_list = (struct linked_list *) malloc(sizeof(struct linked_list));
    assert(new_list);

    new_list->ll_size = 0;
    new_list->head = NULL;
    new_list->tail = NULL;

    return new_list;

}

void prepend(struct linked_list *, void * element);

void append(struct linked_list *, void * element);

int size(struct linked_list *);

void * pop_front(struct linked_list *);

void * pop_back(struct linked_list *);

void * get_front(struct linked_list *);

void * get_back(struct linked_list *);

void print_list(struct linked_list *, void (*)(void *element));