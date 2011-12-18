/* Ken Sheedlo
 * Generic adaptation of Bob Jenkins' One-at-a-Time hash for kmdata. 
 *
 * OAT was chosen over the other notorious Jenkins hash for it's simplicity. 
 * Users can go implement that and pass it in if they want. For the rest of us,
 * here it is. For fixed size data types, just wrap oat_hash with the size of
 * your type and you're good to go. I've included a wrapper for strings.
 */

#include "oat.h"

int32_t oat_hash(const void *key, size_t len){
    const uint8_t *p = key;
    int32_t hash = 0;
    
    for(int i = 0; i < len; i++){
        hash += p[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

int32_t oat_string_hash(const void *str){
    /* Pass in a valid C string. If it's not a valid C string, god help you */
    size_t len = strlen((const char *)str);
    return oat_hash(str, len);
}
