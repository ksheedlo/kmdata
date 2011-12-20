/* Ken Sheedlo
 * kmdata Data Structures Library
 * Network implementation based on adjacency list. */

#include "network.h"

void network_init(network_t *network, size_t size, int32_t (*hash)(const void *),
    int32_t (*eq)(const void *, const void *)){
    /* Simple wrapper for dict_init with default options */
    dict_init(network, size, hash, eq, DICTHT_DEFAULTS);
}

void *network_add_vx(network_t *network, void *key, void *data, bucket_t *result){
    
    gvx_t *new_vx = malloc(sizeof(*new_vx));
    new_vx->data = data;
    new_vx->edges = NULL;
    new_vx->vx_info = 0;

    dict_add(network, key, new_vx, result);
    if(result->data != new_vx){
        gvx_t *old_vx = (gvx_t *)result->data;
        result->data = old_vx->data;
        new_vx->edges = old_vx->edges;

        free(old_vx);
    }

    return result->data;
}

void *network_add_edge(network_t *network, void *akey, void *bkey, 
    void *edge_data, bucket_t *result){
    
    /* If the edge is already there, and we overwrite it's data, the old data
     * ptr is returned in result->data. */
    bucket_t _result;
    gvx_t *vxa;

    /* Check that there are valid edges under akey and bkey, If not, create them */
    if((vxa = (gvx_t *)dict_get(network, akey)) == NULL){
        
        gvx_t *new_vx = malloc(sizeof(*new_vx));
        if(!new_vx){
            fprintf(stderr, "Memory allocation failure.\n");
            result->data = NULL;
            result->key = NULL;
            return NULL;
        }
        new_vx->data = NULL;
        new_vx->edges = NULL;
        new_vx->vx_info = 0;

        dict_add(network, akey, new_vx, &_result);
        vxa = new_vx;
    }

    if(dict_get(network, bkey) == NULL){

        gvx_t *new_vx = malloc(sizeof(*new_vx));
        if(!new_vx){
            fprintf(stderr, "Memory allocation failure.\n");
            result->data = NULL;
            result->key = NULL;
            return NULL;
        }
        new_vx->data = NULL;
        new_vx->edges = NULL;
        new_vx->vx_info = 0;

        dict_add(network, bkey, new_vx, &_result);
    }

    /* Scan down vxa's edge set to make sure bkey is not already there */
    gedge_t *edge = vxa->edges;
    while(edge != NULL){
        if(network->eq(edge->key, bkey)){
            /* Put the old data into result and overwite it */
            result->data = edge->data;
            result->key = edge->key;
            edge->data = edge_data;
            edge->key = bkey;
            return result->data;
        }
        edge = edge->next;
    }

    /* Add the new vx at the start of the edge set */
    gedge_t *new_edge = malloc(sizeof(*new_edge));
    if(!new_edge){
        fprintf(stderr, "Memory allocation failure.\n");
        result->data = NULL;
        result->key = NULL;
        return NULL;
    }
        
    new_edge->key = bkey;
    new_edge->data = edge_data;
    new_edge->next = vxa->edges;
    vxa->edges = new_edge;

    result->key = bkey;
    result->data = edge_data;
    return edge_data;
}

void *network_get_vx(network_t *network, void *key){
    gvx_t *vx = dict_get(network, key);
    if(vx == NULL){
        return NULL;
    }

    return vx->data;
}

void *network_get_edge(network_t *network, void *akey, void *bkey){
    gvx_t *vx = dict_get(network, akey);
    if(vx == NULL){
        return NULL;
    }

    gedge_t *edge = vx->edges;
    while(edge != NULL){
        if(network->eq(edge->key, bkey)){
            return edge->data;
        }
        edge = edge->next;
    }
    return NULL;
}

void *network_remove_vx(network_t *network, void *key, bucket_t *result, int32_t options){
    gvx_t *vx = (gvx_t *)dict_remove(network, key, result);
    int32_t free_edge_values = options & NW_FREE_EDGE_VALUES;
    void *ret = vx->data;

    /* Free all the edges that vx points to */
    gedge_t *edge = vx->edges;
    while(edge != NULL){
        if(free_edge_values){
            free(edge->data);
        }
        gedge_t *next_edge = edge->next;
        free(edge);
        edge = next_edge;
    }

    /* TODO: Figure out what the policy is going to be for edges that point TO vx */
    list_t vx_set, dummy;
    list_init(vx_set);
    list_init(dummy);

    int32_t clear_edges(const void *ptr){
        /* Nested function that can be applied with a filter to quickly get
         * rid of outer incoming edges to vx. */
        gvx_t *ivx = (gvx_t *)ptr;
        gedge_t *outer_edge = ivx->edges;
        gedge_t *last_edge = NULL;
        while(outer_edge != NULL){
            if(network->eq(edge->key, key)){
                /* Need to remove the current edge */
                edge_t *old_edge = outer_edge;
                if(last_edge == NULL){
                    ivx->edges = old_edge->next;
                }else{
                    last_edge->next = old_edge->next;
                }
                if(free_edge_values){
                    free(old_edge->data);
                }
                outer_edge = old_edge->next;
                free(old_edge);
                continue;
            }
            last_edge = outer_edge;
            outer_edge = outer_edge->next;
        }
        return 0;
    }

    dict_value_set(&vx_set, network);
    list_filter(&dummy, &vx_set, clear_edges);

    list_clear(&dummy, 0);
    list_clear(&vx_set, 0);

    result->data = ret;
    /* Finally, free the vx itself */
    free(vx);
    return ret;
}

void *network_remove_edge(network_t *network, void *akey, void *bkey){
    gvx_t *vx = (gvx_t *)dict_get(network, akey);
    if(vx == NULL){
        return NULL;
    }

    gedge_t *edge = vx->edges;
    gedge_t *last_edge = NULL;
    while(edge != NULL){
        if(network->eq(edge->key, bkey)){
            void *ret = edge->data;
            if(last_edge == NULL){
                vx->edges = edge->next;
            }else{
                last_edge->next = edge->next;
            }
            free(edge);
            return ret;
        }
        last_edge = edge;
        edge = edge->next;
    }
    return NULL;
}
