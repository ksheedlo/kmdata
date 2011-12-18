/* Ken Sheedlo
 * Simple tuple implementation */

#include "tuple.h"

void tuple_init(tuple_t *tuple, void *first, void *second){
    tuple->fst = first;
    tuple->snd = second;
}

tuple_t *tuple_new(void *first, void *second){
    tuple_t *rop = malloc(sizeof(tuple_t));
    if(rop == NULL){
        fprintf(stderr, "%s\n", "Memory allocation failure.");
        return NULL;
    }

    rop->fst = first;
    rop->snd = second;

    return rop;
}

void tuple_print(FILE *output, tuple_t *tuple, void (*ldisp)(FILE *, const void *),
                    void (*rdisp)(FILE *, const void *)){
    /* Prints out the tuple in a generic fashion. */
    fprintf(output, "( ");
    ldisp(output, tuple->fst);
    fprintf(output, ", ");
    rdisp(output, tuple->snd);
    fprintf(output, ")");
}

int32_t tuple_equal(tuple_t *lhs, 
                tuple_t *rhs, 
                int32_t (*leq)(const void *, const void *),
                int32_t (*req)(const void *, const void *)){
    /* Tests two tuples for equality. */
    return leq(lhs->fst, rhs->fst) && req(lhs->snd, rhs->snd);
}
