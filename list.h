/* Ken Sheedlo
 * Simple circularly linked list implementation */

#include "tuple.h"
#include "error_handling.h"

struct _ll_t;   /*Forward declaration*/

typedef struct _ll_node {
    void *data;
    struct _ll_t *list;
    struct _ll_node *next;
    struct _ll_node *prev;
} node_t;

typedef struct _ll_t {
    node_t *head;
    size_t length;
} list_t;

void list_init(list_t *list);

void list_addfirst(list_t *list, void *data);

void list_addlast(list_t *list, void *data);

void list_insertbefore(node_t *node, void *data);

void list_insertafter(node_t *node, void *data);

void *list_remove(node_t *node);

void list_clear(list_t *list, int32_t free_data);

int32_t list_match(list_t *lst0, node_t *s0, list_t *lst1, node_t *s1, 
    int32_t (*eq)(const void *, const void *), int32_t count);

void list_print(FILE *output, list_t *lst, void (*disp)(FILE *, const void *));

void list_map(list_t *rop, list_t *op, void *(*map)(const void *));

void list_filter(list_t *rop, list_t *op, int32_t (*filt)(const void *));

void *list_reduce(list_t *op, void *(*rfunc)(const void *, const void *), 
    void *start);

void list_zip(list_t *rop, list_t *op1, list_t *op2);

void list_zipwith(list_t *rop, list_t *op1, list_t *op2,
    void *(*zip)(const void *, const void *));

