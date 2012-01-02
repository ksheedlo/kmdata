/* Ken Sheedlo
 * kmdata Data Structures Library
 * Parallel Mapping List */

#include "plist.h"

void plist_init(plist_t *list, int32_t nthreads, int32_t options){
    if(nthreads <= 0){
        nthreads = 1;
    }

    list->nthreads = nthreads;
    pnode_t *heads = calloc(nthreads, sizeof(pnode_t));
    if(heads == NULL){
        list->head = NULL;
        list->length = -1;
        list->options |= PL_ERROR;
        return;
    }

    for(int i = 0; i < nthreads; i++){
        /* Store the size of each sub-list in the head's data ptr */
        heads[i].data = (void *)0;

        /* The head's own head ptr should point to the actual list */
        heads[i].head = (pnode_t *)list;
        heads[i].next = (heads + i);
        heads[i].prev = heads[i].next;
    }

    list->length = 0;
    list->options = options;
    list->head = heads;
}

void plist_lstinit(plist_t *rop, list_t *op, int32_t nthreads, int32_t options){
    /* TODO: implement this */
}

void plist_insert(plist_t *list, void *data){
    /* Inserts data into an arbitrary position in the list. */
    intptr_t min = INTPTR_MAX;
    int32_t min_i = -1;
    int32_t threads = list->nthreads;
    pnode_t *heads = list->head;
    for(intptr_t i = 0; i < threads; i++){
        if(((intptr_t)heads[i].data) < min){
            min = (intptr_t)heads[i].data;
            min_i = i;
        }
    }

    pnode_t *node = malloc(sizeof(*node));
    if(node == NULL){
        list->options |= PL_ERROR;
        return;
    }

    node->data = data;
    node->head = (heads + min_i);
    node->next = heads[min_i].next;
    node->prev = node->head;
    node->next->prev = node;
    heads[min_i].next = node;
    heads[min_i].data = (void *)(min + 1);

    list->length = list->length + 1;
}

pnode_t *plist_firstnode(plist_t *list){
    pnode_t *heads = list->head;
    int32_t threads = list->nthreads;
    for(int i = 0; i < threads; i++){
        if(heads[i].next != (heads+i)){
            return heads[i].next;
        }
    }
    return NULL;
}

pnode_t *plist_nextnode(pnode_t *node){
    
}
