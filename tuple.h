/* Ken Sheedlo
 * Simple tuple implementation in C. */

#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>

typedef struct {
    void *fst;
    void *snd;
} tuple_t;

void tuple_init(tuple_t *tuple, void *first, void *second);

tuple_t *tuple_new(void *first, void *second);

void tuple_print(FILE *output, 
                tuple_t *tuple, 
                void (*ldisp)(FILE *, const void *),
                void (*rdisp)(FILE *, const void *));

int32_t tuple_equal(tuple_t *lhs, 
                tuple_t *rhs, 
                int32_t (*leq)(const void *, const void *),
                int32_t (*req)(const void *, const void *));

