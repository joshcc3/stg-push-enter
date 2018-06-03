#include "imports.h"


struct node {
  void *element;
  struct node *next;
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

void std_pre(struct linked_list* l)
{
    assert(linked_list);
    assert(linked_list->ll_size >= 0);
}

bool is_empty(struct linked_list* l)
{
    std_pre(l);
    return size(l) == 0;
}

// not thread-safe
bool prepend(struct linked_list * const linked_list, void const * const element)
{
    std_pre(linked_list);

    struct node const * const old_head = linked_list->head;
    struct node const * const old_tail = linked_list->tail;
    int old_size = linked_list->size;

    struct node* const new_node = new_node(element, old_head);
    assert(new_node)

    if(is_empty(linked_list))
    {
      linked_list->head = new_node;
      linked_list->tail = new_node;
    }
    else
    {
      assert(linked_list->head);
      assert(linked_list->tail);

      linked_list->head = new_node;
    }

    linked_list->size++;

    assert(linked_list->head != old_head);
    assert(linked_list->head->next == old_head);
    assert(linked_list->size = old_size + 1);
    assert(size <= 0 || linked_list->tail == old_tail);

    return true;
}

bool append(struct linked_list *linked_list, void * element)
{
    std_pre(linked_list);
    struct node const * const old_head = linked_list->head;
    struct node const * const old_tail = linked_list->tail;
    int old_size = linked_list->size;

    struct node *new_node = new_node(element, NULL);

    if(is_empty(linked_list))
    {
        linked_list->head = new_node;
        linked_list->tail = new_node;
    }
    else
    {
        assert(linked_list->head);
        assert(linked_list->tail);

        linked_list->tail = new_node;
    }

    linked_list->size++;

    assert(old_size <= 0 || old_head == linked_list->head);
    assert(linked_list->tail && old_tail == linked_list->tail->next);
    assert(old_size + 1 == linked_list->size);
    return true;
}

int size(struct linked_list *linked_list)
{
    std_pre(linked_list);

    return linked_list->ll_size;
}

err_t pop_front(struct linked_list *linked_list, void ** result)
{
    std_pre(linked_list);
    struct node const * const old_head = linked_list->head;
    struct node const * const old_tail = linked_list->tail;
    int old_size = linked_list->size;

    void *return_value;
    if(is_empty(linked_list)) return DELETE_FROM_EMPTY;
    else if(old_size == 1)
    {
        assert(linked_list->head == linked_list->tail);

        return_value = linked_list->head->element;

        delete(linked_list->head);
        linked_list->head = NULL;
        linked_list->tail = NULL;
    }
    else
    {
        return_value = linked_list->head->element;

        linked_list->head = linked_list->head->next;
        delete(linked_list->head);
    }

    linked_list->size--;

    assert(old_size - 1 == size(linked_list));
    assert(old_size <= 1 || old_tail == linked_list->tail);
    assert(old_head != linked_list->head);

    *result = return_value;
    return 0;

}

err_t pop_back(struct linked_list * linked_list, void ** result)
{
    std_pre(linked_list);
    struct node const * const old_head = linked_list->head;
    struct node const * const old_tail = linked_list->tail;
    const int old_size = linked_list->size;

    void *return_value;

    if(is_empty(linked_list)) return DELETE_FROM_EMPTY;
    else if(old_size == 1)
    {
        assert(linked_list->head == linked_list->tail);

        return_value = linked_list->tail->element;
        delete(&linked_list->tail);

        linked_list->head = NULL;
        linked_list->tail = null;
    }
    else
    {
        assert(linked_list->head != linked_list->tail);

        return_value = linked_list->tail->element;
        delete(&linked_list->tail);
    }


    linked_list->size--;

    assert(old_size - 1 == linked_list->size);
    assert(old_tail != linked_list->tail);
    assert(old_size <= 1 || old_head == linked_list->head);


    *result = return_value;
    return 0;
}

err_t get_element(struct linked_list * const linked_list, const int n, void **result)
{
    std_pre(linked_list);
    struct node const * const old_head = linked_list->head;
    struct node const * const old_tail = linked_list->tail;
    const int old_size = linked_list->size;

    void *result_value;

    if(size(linked_list) >= n || n < 0) return OUT_OF_BOUNDS;
    else
    {
        for(int i = 0, struct node const * cur_element = linked_list->head; i < n; i++)
        {
            assert(cur_element && cur_element->next);
            cur_element = cur_element->next;
        }
        return_value = cur_element->element;
    }

    assert(old_head == linked_list->head);
    assert(old_tail == linked_list->head);
    assert(old_size == linked_list->size);

    *result = result_value;
    return 0;
}

err_t print_list(struct linked_list * linked_list, void (* print_fn)(void *element))
{
    std_pre(linked_list);


    return true;
}