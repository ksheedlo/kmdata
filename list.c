/* Ken Sheedlo
 * Simple circular linked list implementation */

#include "list.h"

void list_init(list_t *list){
    //Initialize the list to empty (one sentinel node).
    node_t *new_node = malloc(sizeof(node_t));
    if(new_node == NULL){
        CriticalError("Failed to allocate memory");
        return;
    }
    new_node->data = NULL;
    new_node->next = new_node;
    new_node->prev = new_node;
    new_node->list = list;

    list->head = new_node;
    list->length = 0;
}

void list_addfirst(list_t *list, void *data){
    //Add data to a new node at the head of the list.
    node_t *new_node = malloc(sizeof(node_t));
    if(new_node == NULL){
        CriticalError("Failed to allocate memory");
        return;
    }

    new_node->data = data;
    new_node->next = list->head->next;
    new_node->prev = list->head;
    new_node->list = list;

    new_node->next->prev = new_node;
    list->head->next = new_node;

    list->length = list->length + 1;
}

void list_addlast(list_t *list, void *data){
    //Add data to a new node at the tail of the list.
    node_t *new_node = malloc(sizeof(node_t));
    if(new_node == NULL){
        CriticalError("Failed to allocate memory");
        return;
    }

    new_node->data = data;
    new_node->list = list;
    new_node->next = list->head;
    new_node->prev = list->head->prev;

    new_node->prev->next = new_node;
    list->head->prev = new_node;

    list->length = list->length + 1;
}

void list_insertbefore(node_t *node, void *data){
    //Insert data into a new node before the specified node.
    node_t *new_node = malloc(sizeof(node_t));
    if(new_node == NULL){
        CriticalError("Failed to allocate memory");
        return;
    }

    new_node->data = data;
    new_node->list = node->list;
    new_node->next = node;
    new_node->prev = node->prev;
    node->prev = new_node;
    new_node->prev->next = new_node;

    node->list->length = node->list->length + 1;
}

void list_insertafter(node_t *node, void *data){
    node_t *new_node = malloc(sizeof(node_t));
    if(new_node == NULL){
        CriticalError("Failed to allocate memory");
        return;
    }

    new_node->data = data;
    new_node->list = node->list;
    new_node->prev = node;
    new_node->next = node->next;
    node->next = new_node;
    new_node->next->prev = new_node;

    node->list->length = node->list->length + 1;
}

void *list_remove(node_t *node){
    /*Remove the specified node from it's list and return a ptr to its data
     * entry. */
    void *ret = node->data;

    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->list->length = node->list->length - 1;

    free(node);
    return ret;
}

void list_clear(list_t *list, int32_t free_data){
    //Clear the list, freeing data if specified.
    while(list->head->next != list->head){
        void *data = list_remove(list->head->next);
        if(free_data){
            free(data);
        }
    }
    free(list->head);
}

int32_t list_match(list_t *lst0, node_t *s0, list_t *lst1, node_t *s1, 
    int32_t (*eq)(const void *, const void*), int32_t count){
    
    node_t *lhs = s0, *rhs = s1;
    for(int i = 0; i<count; i++){
        if(lhs == lst0->head || rhs == lst1->head){
            return 0;  /*We hit the end of the list */
        }
        if(!eq(lhs->data, rhs->data)){
            return 0; /*Values not equal w.r.t. user-defined function */
        }
        lhs = lhs->next;
        rhs = rhs->next;
    }
    return 1;
}

void list_print(FILE *output, list_t *lst, void (*disp)(FILE *, const void *)){
    fprintf(output, "[");
    node_t *foo = lst->head->next; 
    while(foo != lst->head){
        disp(output, foo->data);
        if(foo->next != lst->head){
            fprintf(output, ", ");
        }
        foo = foo->next;
    }

    fprintf(output, "]\n");
}

void list_map(list_t *rop, list_t *op, void *(*map)(const void *)){
    /* Performs a map operation on op, storing results in rop.
     *
     */
    node_t *node = op->head->next;
    while(node != op->head){
        void *map_data = map(node->data);
        list_addlast(rop, map_data);

        node = node->next;
    }
}

void list_filter(list_t *rop, list_t *op, int32_t (*filt)(const void *)){
    /* Performs a filter operation on op, storing results in rop.
     *
     * Note that if a given data object passes the filter, only the pointer will
     * be copied into the new list. This means that callers SHOULD NOT FREE the
     * filtered list's data if they wish to continue using the unfiltered source
     * and vice versa. */
    node_t *node = op->head->next;
    while(node != op->head){
        if(filt(node->data)){
            list_addlast(rop, node->data);
        }

        node = node->next;
    }
}

void list_zip(list_t *rop, list_t *op1, list_t *op2){
    /* Zips op1 and op2 up into a list of tuples. Stores results in rop.
     *
     * This function allocates memory for new tuples but does not copy
     * data from op1, op2. Only a shallow pointer copy is performed.
     * Users should free the tuples but not their fst, snd pointers when finished.
     */
    node_t *foo = op1->head->next;
    node_t *bar = op2->head->next;
    while(foo != op1->head && bar != op2->head){
        tuple_t *tuple = tuple_new(foo->data, bar->data);
        if(tuple == NULL){
            fprintf(stderr, "%s\n", "Failed to allocate memory for a tuple");
            return;
        }
        list_addlast(rop, tuple);

        foo = foo->next;
        bar = bar->next;
    }
}

void list_zipwith(list_t *rop, list_t *op1, list_t *op2, 
    void *(*zip)(const void *, const void *)){
    /* Zips op1 and op2 with the specified function, and stores the result in rop.
     *
     * The caller will be responsible for any memory allocated by the zip fct.
     */
    node_t *foo = op1->head->next;
    node_t *bar = op2->head->next;
    while(foo != op1->head && bar != op2->head){
        void *result = zip(foo->data, bar->data);
        list_addlast(rop, result);

        foo = foo->next;
        bar = bar->next;
    }
}
