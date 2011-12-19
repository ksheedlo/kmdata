/* Ken Sheedlo
 * kmdata Data Structures Library
 * Network implementation based on adjacency list. */

#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>

#include "dict.h"

typedef struct _gedge {
    void *key;
    void *data;
    struct _gedge *next;
} gedge_t;

typedef struct _gvx {
    void *data;
    gedge_t *edges;
    int32_t vx_info;
} gvx_t;

/* The network structure is built entirely on top of the dict structure. No 
 * additional structure needs to be defined, but we will be making certain 
 * assumptions about the structure of a network that don't hold for a general
 * dict. So no, the network functions DO NOT want to see your dict. :) Networks
 * only, please. */
typedef dict_t network_t;

void network_init(network_t *network, size_t size, int32_t (*hash)(const void *),
    int32_t (*eq)(const void *, const void *));

/* In case of accidental overwrites, a pointer to a key-data pair is necessary. 
 * Pass a bucket_t in to network_add_vx. */
void *network_add_vx(network_t *network, void *key, void *data, bucket_t *result);

void *network_add_edge(network_t *network, void *akey, void *bkey, void *edge_data, bucket_t *result);

void *network_get_vx(network_t *network, void *key);

void *network_get_edge(network_t *network, void *akey, void *bkey);

void *network_remove_vx(network_t *network, void *key);

void *network_remove_edge(network_t *network, void *akey, void *bkey);

void network_dfs(network_t *network, void *skey, 
    void (*func)(void *data, int32_t ct));

void network_sp_bfs(network_t *network, list_t *rop, void *start_key, void *end_key);

void network_sp_dijkstra(network_t *network, list_t *rop, void *start_key, 
    void *end_key, double (*weight)(const void *edge_value));

void network_print(FILE *output, network_t *network, 
    void (*disp_vx)(FILE *output, const void *data), 
    void (*disp_edge)(FILE *output, const void *data));
