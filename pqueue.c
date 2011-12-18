/* Ken Sheedlo
 * kmdata Data Structures Library
 * Priority Queue implementation */

#include "pqueue.h"

void pqueue_init(pqueue_t *queue, int32_t (*cmp)(const void *, const void *)){
    vec_init(&(queue->vec), 0);
    queue->cmp = cmp;
}

void pqueue_add(pqueue_t *queue, void *value){
    int32_t index = queue->vec.size;
    vec_add(&(queue->vec), value);
    if(queue->vec.size == index){
        return; /* Vector failed to add */
    }

    /* Do the bubble up and swap, shoop doop a do wop */
    while(index > 0 && queue->cmp(value, queue->vec.data[index >> 1]) < 0){
        void *oldval = vec_set(&(queue->vec), index >> 1, value);
        vec_set(&(queue->vec), index, oldval);
        index >>= 1;
    }
}

void *pqueue_min(pqueue_t *queue){
    if(queue->vec.size == 0){
        return NULL;
    }

    void *result = vec_get(&(queue->vec), 0);
    void *foo = vec_remove(&(queue->vec), queue->vec.size - 1);
    
    if(queue->vec.size != 0){
        /* Set the head to foo and bubble down */
        vec_set(&(queue->vec), 0, foo);
        index = 0;
        while(index << 1 < queue->vec.size){
            int32_t cmp0 = queue->cmp(foo, queue->vec.data[index << 1]);
            int32_t cmp1;
            if((index << 1) + 1 < queue->vec.size){
                cmp1 = queue->cmp(foo, queue->vec.data[(index << 1) + 1]);
            }else{
                cmp1 = -1;
            }
            if(cmp0 < 0 && cmp1 < 0)
                break;

            int32_t new_index = queue->cmp(queue->vec.data[index << 1], 
                queue->vec.data[(index << 1) + 1]) < 0 ? index << 1 : (index << 1) + 1;
            void *oldval = vec_set(&(queue->vec), new_index, foo);
            vec_set(&(queue->vec), index, oldval);
            index = new_index;
        }
    }
    return result;
}


