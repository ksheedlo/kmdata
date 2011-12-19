/* kmdata Data Structures Library
 * Unit test program. */

#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

#include "dict.h"
#include "oat.h"
#include "rbtree.h"
#include "vector.h"

int32_t assert_intptr_lstcontents(list_t *lst, intptr_t *expect, int32_t len);

void generic_intptr_disp(FILE *output, const void *value);

int32_t test_list_addfirst();

int32_t test_list_addlast();

int32_t test_list_insertbefore();

int32_t test_list_insertafter();

int32_t test_list_remove();

int32_t generic_intptr_eq(const void *foo, const void *bar);

int32_t test_list_match();

void *generic_intptr_negate(const void *ptr);

int32_t test_list_map();

int32_t generic_intptr_even(const void *ptr);

int32_t test_list_filter();

int32_t test_list_zip();

void *generic_intptr_mul(const void *foo, const void *bar);

int32_t test_list_zipwith();

int32_t test_dict_add();

int32_t test_dict_remove();

void alpha26(char *buf, intptr_t n);

int32_t test_dict_resize();

int32_t generic_strcmp(const void *lhs, const void *rhs);

int32_t test_rbt_add();

int32_t test_rbt_remove();

int32_t test_rbt_remove_empty();

int32_t test_rbt_maxn();

int32_t test_rbt_minn();

int32_t assert_intptr_veccontents(vector_t *vec, intptr_t *expected, size_t n);

int32_t test_vec_add();

int32_t test_vec_remove();

int32_t test_vec_set();

int32_t test_vec_addi();

int32_t test_vec_resize();

