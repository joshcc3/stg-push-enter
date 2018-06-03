#ifndef
#define

struct linked_list * new_llist();

void prepend(struct linked_list *, void * element)

void append(struct linked_list *, void * element)

int size(struct linked_list *)

void * pop_front(struct linked_list *)

void * pop_back(struct linked_list *)

void * get_front(struct linked_list *)

void * get_back(struct linked_list *)


#endif