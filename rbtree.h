/* Ken Sheedlo
 * A red-black tree implementation.
 * Just includes and defs here, of course. 
 * 
 * Big thanks go out to Julienne Walker. This would not have been possible 
 * without his awesome tutorials on red-black trees.
 */

#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>

#define BLACK 0
#define RED 1

/* Colors -- users shouldn't concern themselves with these */
#define LEFT 0
#define RIGHT 1
#define DONE 2 /* internal constant for one of the remove routines */
#define WORKING 3

/* Flags for tree manipulation */
#define RBTREE_FREE_KEYS 1
#define RBTREE_FREE_VALUES 2

struct _rbt; /*Forward declaration */

typedef struct _rbt_node {
    struct _rbt_node *child[2];
    void *data;
    void *key;
    struct _rbt *tree;
    uint8_t color;
} rbnode_t;

typedef struct _rbt {
    rbnode_t *root;
    int32_t (*cmp)(const void *, const void *);
    size_t size;
} rbtree_t;

/*Node helper functions. */
rbnode_t *rbt_getnode(rbtree_t *tree, void *key);

uint8_t rbt_color(rbnode_t *node);

rbnode_t *_rbt_insert_r(rbnode_t *root, rbtree_t *tree, void *key, 
                            void *data, rbnode_t *result);

/* Implementation for a dictionary abstract data type. */
void rbt_init(rbtree_t *tree, int32_t (*cmp)(const void *, const void *));

void rbt_clear(rbtree_t *tree, int32_t options);

void *rbt_insert(rbtree_t *tree, void *key, void *data, rbnode_t *result);

void *rbt_get(rbtree_t *tree, void *key);

void rbt_print(FILE *output, rbtree_t *tree, void (*disp_key)(FILE *, const void *),
                void (*disp_value)(FILE *, const void *));

/* Returns: key's data entry if it was found and removed, NULL otherwise */
void *rbt_remove(rbtree_t *tree, void *key, rbnode_t *result);

int32_t _rbnode_assert(rbnode_t *node);

int32_t rbt_assert(rbtree_t *tree);

rbnode_t *_rbt_rotates(rbnode_t *root, int32_t dir);

rbnode_t *_rbt_rotated(rbnode_t *root, int32_t dir);

rbnode_t *_rbt_remove_r(rbnode_t *root, void *key, rbnode_t *result);

rbnode_t *_rbt_remove_balance(rbnode_t *root, int32_t dir, rbnode_t *result);

void _rbt_rclear(rbnode_t *node, int32_t options);

void _rbt_print_r(FILE *output, rbnode_t *node, void (*disp_key)(FILE *, const void *),
                    void (*disp_value)(FILE *, const void *));
