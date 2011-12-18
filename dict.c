/* Ken Sheedlo
 * kmdata Data Structures Library
 * dict implementation based on hash tables. */

#include "dict.h"


void dict_init(dict_t *dict, size_t size, int32_t (*hash)(const void *), 
    int32_t (*eq)(const void *, const void *), int32_t options){
    size_t length = MAX(size, DICTHT_MINIMUM_SIZE);
    dict->table = calloc(length, sizeof(bucket_t *));
    if(dict->table == NULL){
        fprintf(stderr, "%s\n", "Memory allocation failure.");
        return;
    }

    dict->hash = hash;
    dict->eq = eq;
    dict->length = length;
    dict->load = 0;
    dict->flags = options;
}

void *dict_get(dict_t *dict, void *key){

    int32_t hash = dict->hash(key);
    int32_t index = MOD(hash, dict->length);

    bucket_t *bucket = *(dict->table + index);

    if(bucket == NULL)
        return NULL;

    while(!(dict->eq(key, bucket->key))){
        if(bucket->next == NULL)
            return NULL;
        bucket = bucket->next;
    }
    return bucket->data;
}

int32_t dict_add(dict_t *dict, void *key, void *data, bucket_t *result){
    /* Check the size of the dict. Do we need to grow? */
    if(((double)(dict->load + 1)) / dict->length > DICTHT_MAX_LOAD
        && !(dict->flags & DICTHT_NO_GROW)){
        
        _dict_resize(dict, (size_t)(dict->length * DICTHT_GROW_FACTOR));
    }

    int32_t hash = dict->hash(key);
    int32_t index = MOD(hash, dict->length);

    bucket_t *new_bucket = malloc(sizeof(bucket_t));
    if(!new_bucket){
        fprintf(stderr, "%s\n", "Memory allocation failure.");
        result->key = NULL;
        return -1;
    }

    new_bucket->data = data;
    new_bucket->key = key;
    new_bucket->next = NULL;

    bucket_t **target = dict->table + index;
    if(*target == NULL){
        *target = new_bucket;
    }else{
        bucket_t *bucket = *target;
        bucket_t *last_bucket = NULL;
        do{
            last_bucket = bucket;
            if(dict->eq(key, bucket->key)){
                /* Scan down the buckets and replace the old one if the keys are
                 * actually equal. */
                result->key = bucket->key;
                result->data = bucket->data;
                bucket->key = key;
                bucket->data = data;
                free(new_bucket);
                return hash;
            }
            bucket = bucket->next;
        }while(bucket != NULL);
        last_bucket->next = new_bucket;
    }
    dict->load = dict->load + 1;
    result->key = key;
    result->data = data;
    return hash;
}

void *dict_remove(dict_t *dict, void *key, bucket_t *result){
    /* Check the size. Should we be using a smaller table? */
    if((((double)(dict->load - 1)) / dict->length) < DICTHT_MIN_LOAD
        && !(dict->flags & DICTHT_NO_SHRINK) && (dict->length > DICTHT_MINIMUM_SIZE)){
        
        size_t new_size = MAX((size_t)(dict->length / DICTHT_GROW_FACTOR), 
                            DICTHT_MINIMUM_SIZE);

        _dict_resize(dict, new_size);
    }
    /* Assume no match is found initially */
    result->key = NULL;
    result->data = NULL;

    int32_t hash = dict->hash(key);
    int32_t index = MOD(hash, dict->length);

    bucket_t *bucket = *(dict->table + index);
    if(bucket == NULL){
        return NULL; /* no such entry in the table */
    }

    bucket_t *last_bucket = NULL;
    do{
        if(dict->eq(key, bucket->key)){
            dict->load = dict->load - 1;
            void *ret = bucket->data;
            result->key = bucket->key;
            result->data = ret;
            
            if(last_bucket == NULL){
                *(dict->table + index) = bucket->next;
            }else{
                last_bucket->next = bucket->next;
            }
            free(bucket);
            return ret;
        }
        last_bucket = bucket;
    }while((bucket = bucket->next) != NULL);

    return NULL; /*no match found*/
}

void dict_clear(dict_t *dict, int32_t options){
    int32_t free_keys = options & DICTHT_FREE_KEYS;
    int32_t free_values = options & DICTHT_FREE_VALUES;

    for(int i = 0; i<dict->length; i++){
        if(dict->table[i] != NULL){
            bucket_t *bucket = dict->table[i];
            bucket_t *next_bucket;
            do{
                next_bucket = bucket->next;
                if(free_keys){
                    free(bucket->key);
                }
                if(free_values){
                    free(bucket->data);
                }
                free(bucket);
            }while((bucket = next_bucket) != NULL);
        }
    }
    free(dict->table);
    dict->load = 0;
    dict->length = 0;
    dict->hash = NULL;
    dict->eq = NULL;
}

void dict_print(FILE *output, dict_t *dict, void (*disp_key)(FILE *, const void *),
    void (*disp_value)(FILE *, const void *)){
    
    fprintf(output, "{ ");
    int32_t count = 0;
    for(int i = 0; i<dict->length; i++){
        if(dict->table[i] != NULL){
            bucket_t *bucket = dict->table[i];
            do{
                disp_key(output, bucket->key);
                fprintf(output, ": ");
                disp_value(output, bucket->data);
                if(++count < dict->load){
                    fprintf(output, ", ");
                }
            }while((bucket = bucket->next) != NULL);
        }
    }
    fprintf(output, "} ");
}

void _dict_resize(dict_t *dict, size_t new_size){
    bucket_t **new_table = calloc(new_size, sizeof(bucket_t *));      
    if(!new_table){
        fprintf(stderr, "%s\n", "Memory allocation failure.");
        return;
    }

    /* Go through the old table and rehash the entries into the new one */
    for(int i = 0; i < dict->length; i++){
        if(dict->table[i] != NULL){
            bucket_t *bucket = dict->table[i];
            bucket_t *next_bucket;
            do{
                /* This is fast and screws up the old table to no end, so do it
                 * right */
                int32_t hash = dict->hash(bucket->key);
                int32_t new_index = MOD(hash, new_size);
                next_bucket = bucket->next;
                bucket->next = new_table[new_index];
                new_table[new_index] = bucket;
            }while((bucket = next_bucket) != NULL);
        }
    }

    free(dict->table);
    dict->table = new_table;
    dict->length = new_size;
}
