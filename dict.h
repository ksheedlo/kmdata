/* Ken Sheedlo
 * kmdata Data Structures Library
 * dict implementation based on hash tables. */

#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>

#include "error_handling.h"

#define MAX(a,b) ((a)>(b) ? a : b)

#define MOD(x,m) (((x)%(m) + (m))%(m))

/* Constants for internal use. */
#define DICTHT_MINIMUM_SIZE 2039
#define DICTHT_GROW_FACTOR  2
#define DICTHT_MAX_LOAD     0.75
#define DICTHT_MIN_LOAD     0.25

/* Options for manipulating hash tables. */
#define DICTHT_FREE_KEYS    1
#define DICTHT_FREE_VALUES  2

/*Options for hash table state. */
#define DICTHT_DEFAULTS     0
#define DICTHT_NO_GROW      4
#define DICTHT_NO_SHRINK    8

typedef struct _ht_bucket {
    void *data;
    void *key;
    struct _ht_bucket *next;
} bucket_t;

typedef struct {
    bucket_t **table;
    int32_t (*hash)(const void *);
    int32_t (*eq)(const void *, const void *);
    size_t length;
    int32_t load;
    int32_t flags;
} dict_t;

/* PRIVATE FUNCTIONS */ 
void _dict_resize(dict_t *dict, size_t new_size);


/* PUBLIC API */

/* Adds the value with the specified key to the dict. If the key is already
 * in the dictionary, return it's old K-V pair in result. If an error occurs,
 * set result->key to NULL.
 */
int32_t dict_add(dict_t *dict, void *key, void *data, bucket_t *result);

/* Retrieves the data value associated with the given key from the dictionary.*/
void *dict_get(dict_t *dict, void *key);

/* Clear memory belonging to this dictionary. */
void dict_clear(dict_t *dict, int32_t options);

void *dict_remove(dict_t *dict, void *key, bucket_t *result);

/* Initialize a new dictionary given a valid pointer and some other important
 * stuff. */
void dict_init(dict_t *dict, size_t size, int32_t (*hash)(const void *), 
    int32_t (*eq)(const void *, const void *), int32_t options);

void dict_print(FILE *output, dict_t *dict, void (*disp_key)(FILE *, const void *),
    void (*disp_value)(FILE *, const void *));

