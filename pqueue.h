/* Ken Sheedlo
 * kmdata Data Structures Library
 * Priority Queue implementation */

#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>

#include "vector.h"

typedef struct {
    vector_t vec;
    int32_t (*cmp)(const void *, const void *);
} pqueue_t;

void pqueue_init(pqueue_t *queue, int32_t (*cmp)(const void *, const void *));

void pqueue_add(pqueue_t *queue, void *value);

void *pqueue_min(pqueue_t *queue);

