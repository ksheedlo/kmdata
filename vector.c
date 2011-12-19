/* Ken Sheedlo
 * kmdata Data Structures Library
 * Vector implementation */

#include "vector.h"

void vec_init(vector_t *vec, size_t size){
    size_t asize = VEC_MAX(size, VEC_MINIMUM_SIZE);
    void **new_data = malloc(asize * sizeof(void *));
    if(new_data == NULL){
        fprintf(stderr, "Memory allocation failure.\n");
        vec->data_length = 0;
        return;
    }

    vec->data = new_data;
    vec->data_length = asize;
    vec->size = 0;
}

void vec_clear(vector_t *vec, int32_t free_data){
    if(free_data){
        for(int i = 0; i < vec->size; i++){
            free(vec->data[i]);
        }
    }
    free(vec->data);
    vec->data = NULL;
    vec->data_length = 0;
    vec->size = 0;
}

int _vec_resize(vector_t *vec, size_t new_size){
    if(new_size <= vec->data_length){
        /* Should never need to shrink the vector */
        return 0;
    }

    //XXX
    void **new_block = realloc(vec->data, new_size * sizeof(void *));
    if(new_block == NULL){
        fprintf(stderr, "Memory allocation failure.\n");
        return 1;
    }

    vec->data = new_block;
    vec->data_length = new_size;
    return 0;
}

void vec_add(vector_t *vec, void *value){
    /* Adds the specified value to the end of the vector. */
    if(vec->size == vec->data_length){
        int st = _vec_resize(vec, vec->data_length * VEC_RESIZE_FACTOR);
        if(!st)
            return;
    }

    vec->data[vec->size++] = value;
}

void *vec_get(vector_t *vec, int32_t i){
    if(i >= vec->size){
        return NULL;
    }
    return vec->data[i];
}

void *vec_remove(vector_t *vec, int32_t i){
    if(i >= vec->size){
        return NULL;
    }

    void *ret = vec->data[i];
    memmove((vec->data + i), (vec->data + i + 1), (vec->size - (1+i))*sizeof(void *));
    vec->size--;
    return ret;
}

void *vec_set(vector_t *vec, int32_t i, void *value){
    if(i > vec->size){
        return NULL;
    }

    if(i == vec->size){
        if(vec->size == vec->data_length){
            int st = _vec_resize(vec, vec->data_length * VEC_RESIZE_FACTOR);
            if(!st)
                return NULL;
        }
        vec->size++;
    }

    void *ret = vec->data[i];
    vec->data[i] = value;
    return ret;
}

void *vec_addi(vector_t *vec, int32_t i, void *value){
    /* Adds the value to the vector at position i. 
     * If i is past the end of the vector, just add it at the end of the 
     * vector. */
    if(i > vec->data_length){
        i = vec->size;   
    }
    if(vec->size == vec->data_length){
        int st = _vec_resize(vec, vec->data_length * VEC_RESIZE_FACTOR);
        if(!st)
            return NULL;
    }

    memmove((vec->data + (i+1)), (vec->data + i), (vec->size - i)*sizeof(void *));
    vec->data[i] = value;
    return value;
}

void vec_print(FILE *output, vector_t *vec, void (*disp)(FILE *, const void *)){
    fprintf(output, "[ ");
    int i;
    for(i = 0; i+1 < vec->size; i++){
        disp(output, vec->data[i]);
        fprintf(output, ", ");
    }
    if(vec->size != 0)
        disp(output, vec->data[i]);

    fprintf(output, "] ");
}
