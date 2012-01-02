/* Ken Sheedlo
 * kmdata Data Structures Library
 * Parallel Mapping List */

#include "tuple.h"

#define PL_ALLOW_IMBALANCE      1
#define PL_STRICT_ORDERING      2
#define PL_FREE_DATA            4
#define PL_REDUCE_COMMUTATIVE   8
#define PL_ERROR                (1<<31)

typedef struct _pl_node {
    void *data;
    struct _pl_node *head;
    struct _pl_node *next;
    struct _pl_node *prev;
} pnode_t;

typedef struct {
    pnode_t *head;
    size_t length;
    int32_t nthreads;
    int32_t options;
} plist_t;

/* Initializes a new empty plist. */
void plist_init(plist_t *list, int32_t nthreads, int32_t options);

/* Initializes a parallel mapping list from a standard list. */
void plist_lstinit(plist_t *rop, list_t *op, int32_t nthreads, int32_t options);

/* Inserts a new piece of data into the list in an arbitrary position. */
void plist_insert(plist_t *list, void *data);

void plist_addfirst(plist_t *list, void *data);

void plist_addlast(plist_t *list, void *data);

void plist_insertbefore(pnode_t *node, void *data);

void plist_insertafter(pnode_t *node, void *data);

void *plist_remove(pnode_t *node);

pnode_t *plist_firstnode(plist_t *list);

pnode_t *plist_nextnode(pnode_t *node);

pnode_t *plist_prevnode(pnode_t *node);

void plist_clear(plist_t *list, int32_t options);

void plist_print(FILE *output, plist_t *lst, void (*disp)(FILE *, const void *));

void plist_map(list_t *rop, plist_t *op, void *(*map)(const void *));

void plist_filter(list_t *rop, plist_t *op, int32_t (*filt)(const void *));

void *plist_reduce(plist_t *op, void *(*rfunc)(const void *, const void *),
    void *start);

void plist_to_list(list_t *rop, plist_t *op);

