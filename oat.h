/* Ken Sheedlo
 * Generic adaptation of Bob Jenkins' One-at-a-Time hash for kmdata. */

#include<stdint.h>
#include<stdlib.h>
#include<string.h>

/* Hashes a generic key of size len bytes. */
int32_t oat_hash(const void *key, size_t len);

/* Hashes a string. Note that although str is passed in as void *, it must be
 * a valid C string. */
int32_t oat_string_hash(const void *str);

