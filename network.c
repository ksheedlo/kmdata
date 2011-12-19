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

void *network_add_edge(network_t *network, void *akey, void *bkey, void *edge_data, bucket_t *result){
    
    //TODO: implement this and the rest of the things
}
