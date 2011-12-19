/* Ken Sheedlo
 * kmdata Data Structures Library
 * Vector implementation. */

#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>

#define VEC_MINIMUM_SIZE    100
#define VEC_RESIZE_FACTOR   2

#define VEC_MAX(a,b) ((a)>(b) ? (a) : (b))

typedef struct {
    void **data;
    size_t data_length;
    size_t size;
} vector_t;

/* Private functions. */

int _vec_resize(vector_t *vec, size_t new_size);

/* Public API */

void vec_init(vector_t *vec, size_t size);

void vec_clear(vector_t *vec, int32_t free_data);

void vec_add(vector_t *vec, void *value);

void *vec_get(vector_t *vec, int32_t i);

void *vec_remove(vector_t *vec, int32_t i);

void *vec_set(vector_t *vec, int32_t i, void *value);

void *vec_addi(vector_t *vec, int32_t i, void *value);

void vec_print(FILE *output, vector_t *vec, void (*disp)(FILE *, const void *));


