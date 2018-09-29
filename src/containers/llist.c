#include "containers/llist.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "containers/mmanager.h"
#include "stg/math.h"
#include "stg/util.h"



static void std_pre(const struct linked_list* const l)
{
    assert(l);
    assert(l->size >= 0);
}

err_t new_node(const void * element, struct node* next, struct node** result)
{
    struct node* node = (struct node*) new(sizeof (struct node));
    node->element = element;
    node->next = next;

    *result = node;
    return 0;
}

static void free_node(struct node** node)
{
    delete((void**) node);
}

struct linked_list * new_llist()
{
    struct linked_list * new_list = (struct linked_list *) new(sizeof(struct linked_list));
    assert(new_list);

    new_list->size = 0;
    new_list->head = NULL;
    new_list->tail = NULL;

    return new_list;
}

err_t deinit_llist(struct linked_list** inp_ptr)
{
    struct linked_list* inp = *inp_ptr;
    std_pre(inp);

    int deallocs = 0;
    struct node** current_node = &inp->head;
    while(*current_node)
    {
      struct node* next_ptr = (*current_node)->next;

      free_node(current_node);
      assert(!*current_node);

      *current_node = next_ptr;
      deallocs++;
    }
    assert(!inp->head);
    assert(deallocs == inp->size);

    inp->tail = NULL;
    delete((void**) inp_ptr);

    assert(!*inp_ptr);

    return 0;
}

int ll_size(struct linked_list const *linked_list)
{
    std_pre(linked_list);

    return linked_list->size;
}

bool ll_is_empty(struct linked_list* l)
{
    std_pre(l);
    return ll_size(l) == 0;
}


static err_t get_node(struct linked_list const * const linked_list, const int actual_index, struct node **result)
{
    std_pre(linked_list);
    struct node const * const old_head = linked_list->head;
    struct node const * const old_tail = linked_list->tail;
    const int old_size = linked_list->size;
    assert(actual_index < old_size && actual_index >= 0);

    void *result_value;

    struct node * cur_element = linked_list->head;
    for(int i = 0; i < actual_index; i++)
    {
        assert(cur_element && cur_element->next);
        cur_element = cur_element->next;
    }
    result_value = cur_element;

    assert(old_head == linked_list->head);
    assert(old_tail == linked_list->tail);
    assert(old_size == linked_list->size);

    *result = result_value;
    return 0;
}

/* I don't think we can clone the element here because:
   (1) How do you even clone a void*? Maybe you require the user to clone -- error prone :/
   (2) We have to keep track of which elements have been requested before we destroy this -- not too hard, just mark it
       in the node struct.
*/
err_t ll_insert_element(struct linked_list * const linked_list, const int n, const void* element)
{
    std_pre(linked_list);
    struct node const * const old_head = linked_list->head;
    struct node const * const old_tail = linked_list->tail;
    const int old_size = ll_size(linked_list);

    err_t return_code = 0;

    const int actual_index = n == old_size ? n : zero_based_index(n, old_size);
    assert(actual_index >= 0 && actual_index <= old_size); // actual_index == old_size => append


    if(actual_index == 0)
    {
       struct node* cur_head = linked_list->head;
       struct node* node;
       new_node(element, cur_head, &node);
       assert(node);

       linked_list->head = node;
       if(old_size == 0) linked_list->tail = node;
    }
    else
    {
        assert(actual_index > 0);

        struct node* prev_node;
        return_code = get_node(linked_list, actual_index - 1, &prev_node);

        struct node* node;

        new_node(element, prev_node->next, &node);
        assert(node);

        prev_node->next = node;
        if(actual_index == old_size) linked_list->tail = node;
    }

    linked_list->size++;

    assert(old_size + 1 == ll_size(linked_list));
    assert(linked_list->head);
    assert(linked_list->tail);
    assert(actual_index == 0 || linked_list->head == old_head);
    assert(actual_index == old_size || linked_list->tail == old_tail);

    return return_code;
}


err_t ll_delete_element(struct linked_list * const linked_list, const int n, const void ** result)
{
    std_pre(linked_list);
    struct node * old_head = linked_list->head;
    struct node const * old_tail = linked_list->tail;
    const int old_size = ll_size(linked_list);

    err_t return_code;

    int actual_index = zero_based_index(n, old_size);
    assert(actual_index >= 0 && actual_index <= old_size); // actual_index == old_size => append

    struct node * node_to_free;
    if(old_size == 1)
    {
        *result = old_head->element;
        node_to_free = old_head;
        linked_list->head = NULL;
        linked_list->tail = NULL;
        return_code = 0;
    }
    else if(actual_index == 0)
    {
        *result = old_head->element;
        linked_list->head = old_head->next;
        node_to_free = old_head;
        return_code = 0;
    }
    else
    {
        struct node* prev_node;
        return_code = get_node(linked_list, actual_index - 1, &prev_node);
        assert(prev_node && prev_node->next);

        *result = prev_node->next->element;
        node_to_free = prev_node->next;
        prev_node->next = prev_node->next->next;

        if(actual_index == old_size - 1) linked_list->tail = prev_node;
    }
    assert(node_to_free);

    linked_list->size--;

    free_node(&node_to_free);
    assert(!node_to_free);

    assert(old_size - 1 == ll_size(linked_list));
    assert(actual_index == 0 || linked_list->head == old_head);
    assert(actual_index == old_size - 1 || linked_list->tail == old_tail);

    return return_code;
}

err_t ll_get_element(struct linked_list const * const linked_list, const int n, const void **result)
{
    struct node *interm_result;

    int status_code = get_node(linked_list, n, &interm_result);
    *result = interm_result->element;

    return status_code;
}

err_t ll_map(struct linked_list const * const linked_list, err_t map_fn(void**))
{
    std_pre(linked_list);
    struct node const * const old_head = linked_list->head;
    struct node const * const old_tail = linked_list->tail;
    const int old_size = linked_list->size;

    err_t return_code = -1;

    struct node * pointer = linked_list->head;
    for(int ix = 0; ix < old_size; ix++)
    {
        assert(pointer);
	    const void * reference = ((*pointer).element);
        return_code = return_code & map_fn(&reference);
        pointer = pointer->next;
    }

    assert(old_tail->next == pointer);
    assert(old_head == linked_list->head);
    assert(old_tail == linked_list->tail);
    assert(old_size == linked_list->size);

    return return_code;
}

err_t ll_index_of(struct linked_list const * inp,
                void const * element,
                struct equals_typeclass const * e_t,
                int *index)
{
    std_pre(inp);

    int return_code = 0;
    bool found = false;
    struct node* candidate = inp->head;

    int size = ll_size(inp);

    int i = 0;
    for(; i < size && candidate; i++)
    {
        if(e_t->equal(element, candidate->element, e_t->extra))
        {
            found = true;
            break;
        }
        candidate = candidate->next;
    }

    if (found)
    {
        *index = i;
        return_code = 0;
    }
    else
    {
        *index = -1;
        return_code = ELEMENT_NOT_FOUND;
    }

    assert((!found && !candidate && *index == -1) || (found && candidate && *index >= 0));
    assert(return_code == 0 || return_code == ELEMENT_NOT_FOUND);

    return return_code;

}


err_t ll_search(struct linked_list const * inp,
                void const * element,
                struct equals_typeclass const * e_t,
                void const **result)
{
    std_pre(inp);

    int return_code;
    bool found = false;
    struct node* candidate = inp->head;

    int size = ll_size(inp);

    for(int i = 0; i < size && candidate; i++)
    {
        if(e_t->equal(element, candidate->element, e_t->extra))
        {
            found = true;
            break;
        }
        candidate = candidate->next;
    }

    if (found)
    {
        *result = candidate->element;
        return_code = 0;
    }
    else
    {
        *result = NULL;
        return_code = ELEMENT_NOT_FOUND;
    }

    assert((!found && !candidate) || (found && candidate));
    assert(return_code != 0 || ((*result && e_t->equal(element, *result, e_t->extra)) && e_t->equal(*result, element, e_t->extra)));
    assert(return_code == 0 || (return_code == ELEMENT_NOT_FOUND && !*result));

    return return_code;
}
