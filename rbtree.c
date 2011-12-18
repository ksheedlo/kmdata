/* Ken Sheedlo and Nick Vanderweit
 * A red-black tree implementation. */

#include "rbtree.h"

rbnode_t *rbt_getnode(rbtree_t *tree, void *key){
    /* Looks up key in the tree and returns the node containing it, or NULL if
     * no such node is an elt of the tree */
    rbnode_t *node = tree->root;
    while(node != NULL){
        int32_t diff = tree->cmp(key, node->key);
        if(diff == 0){
            return node;
        }
        node = node->child[(diff > 0)]; /* RIGHT if greater, LEFT otherwise */
    }
    return NULL;
}

uint8_t rbt_color(rbnode_t *node){
    if(node == NULL){
        return BLACK;
    }
    return node->color;
}

rbnode_t *_rbt_insert_r(rbnode_t *root, rbtree_t *tree, void *key, 
                            void *data, rbnode_t *result){
    if(root == NULL){
        root = malloc(sizeof(rbnode_t));
        if(root == NULL){
            fprintf(stderr, "%s\n", "Memory allocation failure.");
            return NULL;
        }

        root->key = key;
        root->data = data;
        root->tree = tree;
        tree->size = tree->size + 1;
        root->color = RED;
        root->child[LEFT] = root->child[RIGHT] = NULL;

        result->key = key;
        result->data = data;
    }else{
        /* Keys match. Overwrite and store the old K,V pair */
        int32_t diff = tree->cmp(root->key, key);
        int32_t dir;
        if(diff == 0){
            result->key = root->key;
            result->data = root->data;
            root->key = key;
            root->data = data;
            return root;
        }else{
            dir = diff < 0;
        }

        root->child[dir] = _rbt_insert_r(root->child[dir], tree, key, data, result);

        if(rbt_color(root->child[dir]) == RED){
            if(rbt_color(root->child[!dir]) == RED){
                root->color = RED;
                root->child[LEFT]->color = BLACK;
                root->child[RIGHT]->color = BLACK;
            }else{
                if(rbt_color(root->child[dir]->child[dir]) == RED){
                    root = _rbt_rotates(root, !dir);
                }else if(rbt_color(root->child[dir]->child[!dir]) == RED){
                    root = _rbt_rotated(root, !dir);
                }
            }
        }
    }
    return root;
}

void *rbt_insert(rbtree_t *tree, void *key, void *data, rbnode_t *result){
    result->key = NULL;
    result->data = NULL;
    tree->root = _rbt_insert_r(tree->root, tree, key, data, result);
    tree->root->color = BLACK;
    return result->data;
}

void *rbt_get(rbtree_t *tree, void *key){
    rbnode_t *result = rbt_getnode(tree, key);
    if(result == NULL){
        return NULL;
    }

    return result->data;
}

void *rbt_remove(rbtree_t *tree, void *key, rbnode_t *result){
    result->data = NULL;
    result->key = NULL;
    result->color = WORKING;

    tree->root = _rbt_remove_r(tree->root, key, result);
    if(result->data != NULL){
        tree->size = tree->size - 1;
    }
    if(tree->root != NULL){
        tree->root->color = BLACK;
    }

    return result->data;
}

int32_t _rbnode_assert(rbnode_t *node){
    int32_t lh, rh;
    rbtree_t *tree = node->tree;

    if(node == NULL){
        return 1;
    }
    rbnode_t *lnode = node->child[LEFT], *rnode = node->child[RIGHT];

    /* Consecutive red links test */
    if(rbt_color(node) == RED){
        if(rbt_color(lnode) == RED || rbt_color(rnode) == RED){
            fprintf(stderr, "Red violation\n");
            return 0;
        }
    }

    lh = _rbnode_assert(lnode);
    rh = _rbnode_assert(rnode);

    /* Check to make sure this is a valid BST */
    if((lnode != NULL && tree->cmp(lnode->key, node->key) > 0)
        || (rnode != NULL && tree->cmp(rnode->key, node->key) < 0)){
        fprintf(stderr, "Binary tree violation\n");
        return 0;
    }
    
    /* Check for a black height mismatch */
    if(lh != 0 && rh != 0 && lh != rh){
        fprintf(stderr, "Black violation\n");
        return 0;
    }

    if(lh != 0 && rh != 0){
        return rbt_color(node) == RED ? lh : lh + 1;
    }
    return 0;
}

int32_t rbt_assert(rbtree_t *tree){
    /* Perform node assertions on the root of the tree recursively */
    return _rbnode_assert(tree->root);
}

rbnode_t *_rbt_rotates(rbnode_t *root, int32_t dir){
    /* Single rotation around the root, in the requested direction. */
    rbnode_t *save = root->child[!dir];
    
    root->child[!dir] = save->child[dir];
    save->child[dir] = root;

    root->color = RED;
    save->color = BLACK;

    return save;
}

rbnode_t *_rbt_rotated(rbnode_t *root, int32_t dir){
    /* Same as _rbt_rotates, but now a double rotation. */
    root->child[!dir] = _rbt_rotates(root->child[!dir], !dir);
    return _rbt_rotates(root, dir);
}

rbnode_t *_rbt_remove_r(rbnode_t *root, void *key, rbnode_t *result){
    if(root == NULL){
        result->color = DONE;
    }else{
        int32_t dir;
        int32_t diff = root->tree->cmp(root->key, key);

        if(diff == 0){
            result->data = root->data;
            result->key = root->key;
            if(root->child[LEFT] == NULL || root->child[RIGHT] == NULL){
                rbnode_t *save = root->child[(root->child[0] == NULL)];
                
                if(rbt_color(root) == RED){
                    result->color = DONE;
                }else if(rbt_color(save) == RED){
                    save->color = BLACK;
                    result->color = DONE;
                }

                free(root);
                return save;
            }else{
                rbnode_t *heir = root->child[LEFT];

                while(heir->child[RIGHT] != NULL){
                    heir = heir->child[RIGHT];
                }
                
                root->data = heir->data;
                root->key = heir->key;
                key = heir->key;
            }
        }

        dir = root->tree->cmp(root->key, key) < 0;
        root->child[dir] = _rbt_remove_r(root->child[dir], key, result);

        if(!rbt_color(result) == DONE){
            root = _rbt_remove_balance(root, dir, result);
        }
    }
    return root;
}

rbnode_t *_rbt_remove_balance(rbnode_t *root, int32_t dir, rbnode_t *result){
    rbnode_t *p = root;
    rbnode_t *s = root->child[!dir];

    if(rbt_color(s) == RED){
        root = _rbt_rotates(root, dir);
        s = p->child[!dir];
    }

    if(s != NULL){
        if(rbt_color(s->child[LEFT]) == BLACK && rbt_color(s->child[RIGHT]) == BLACK){
            if(rbt_color(p) == RED){
                result->color = DONE;
            }
            p->color = BLACK;
            s->color = RED;
        }else{
            uint8_t save = p->color;
            int32_t new_root = (root == p);

            if(rbt_color(s->child[!dir]) == RED){
                p = _rbt_rotates(p, dir);
            }else{
                p = _rbt_rotated(p, dir);
            }

            p->color = save;
            p->child[LEFT]->color = BLACK;
            p->child[RIGHT]->color = BLACK;

            if(new_root){
                root = p;
            }else{
                root->child[dir] = p;
            }

            result->color = DONE;
        }
    }

    return root;
}