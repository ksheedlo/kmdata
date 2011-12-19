/* Ken Sheedlo
 * Unit tests for kmdata data structures. */

#include "unittest.h"

int32_t assert_intptr_lstcontents(list_t *lst, intptr_t *expect, int32_t len){
    assert(lst->length == len);

    node_t *node = lst->head->next;
    for(int i = 0; i < len; i++){
        assert(((intptr_t)node->data) == expect[i]);
        node = node->next;
    }
    return 0;
}

void generic_intptr_disp(FILE *output, const void *value){
    fprintf(output, "%d", (intptr_t)value);
}

int32_t test_list_addfirst(){
    list_t foo;
    list_init(&foo);

    list_addfirst(&foo, (void *)5);
    list_addfirst(&foo, (void *)1);
    list_addfirst(&foo, (void *)42);

    intptr_t expected[] = {42, 1, 5};

    int32_t ret = assert_intptr_lstcontents(&foo, expected, 3);

    list_clear(&foo, 0);
    return !ret;
}

int32_t test_list_addlast(){
    list_t list;
    list_init(&list);

    list_addlast(&list, (void *)0xF00);
    list_addlast(&list, (void *)0);
    list_addlast(&list, (void *)42);

    intptr_t expected[] = {0xF00, 0, 42};

    int32_t ret = assert_intptr_lstcontents(&list, expected, 3);

    list_clear(&list, 0);
    return !ret;
}

int32_t test_list_insertbefore(){
    list_t list;
    list_init(&list);

    list_addlast(&list, (void *)0xF00);
    list_addlast(&list, (void *)31337);

    node_t *node = list.head->next->next; //31337
    list_insertbefore(node, (void *)42);
    list_insertbefore(list.head, (void *)39);
    list_insertbefore(list.head->next, (void *)2039);

    intptr_t expected[] = {2039, 0xF00, 42, 31337, 39};

    int32_t ret = assert_intptr_lstcontents(&list, expected, 5);

    list_clear(&list, 0);
    return !ret;
}

int32_t test_list_insertafter(){
    list_t list;
    list_init(&list);

    list_addlast(&list, (void *)42);
    list_addlast(&list, (void *)1337);
    
    node_t *node = list.head->next; //42
    list_insertafter(node, (void *)0xdeadbeef);
    list_insertafter(list.head, (void *)0xbadbeef);
    list_insertafter(list.head->prev, (void *)0xF00);

    intptr_t expected[] = {0xbadbeef, 42, 0xdeadbeef, 1337, 0xF00};

    int32_t ret = assert_intptr_lstcontents(&list, expected, 5);

    list_clear(&list, 0);
    return !ret;
}

int32_t test_list_remove(){
    list_t list;
    list_init(&list);

    list_addlast(&list, (void *)1);
    list_addlast(&list, (void *)42);
    list_addlast(&list, (void *)0x20);
    list_addlast(&list, (void *)0xF00);
    list_addlast(&list, (void *)2);

    intptr_t x[] = {1, 42, 0x20, 0xF00, 2};

    int32_t ret = assert_intptr_lstcontents(&list, x, 5);

    list_remove(list.head->next->next);
    list_remove(list.head->prev);

    intptr_t x2[] = {1, 0x20, 0xF00};

    ret = ret | assert_intptr_lstcontents(&list, x2, 3);
    
    list_clear(&list, 0);
    return !ret;
}

int32_t generic_intptr_eq(const void *foo, const void *bar){
    return ((intptr_t)foo) == ((intptr_t)bar);
}

int32_t test_list_match(){
    /*Pass in a match and a non-match to make sure match works properly*/
    list_t lst1, lst2;
    list_init(&lst1);
    list_init(&lst2);

    list_addlast(&lst1, (void *)0);
    list_addlast(&lst1, (void *)42);
    list_addlast(&lst1, (void *)20);
    list_addlast(&lst1, (void *)0xF00);
    list_addlast(&lst1, (void *)2);
    
    list_addlast(&lst2, (void *)42);
    list_addlast(&lst2, (void *)20);
    list_addlast(&lst2, (void *)0xF00);

    int32_t st = list_match(&lst1, lst1.head->next->next, &lst2, lst2.head->next, 
                            generic_intptr_eq, 3);

    if(!st){
        list_clear(&lst1, 0);
        list_clear(&lst2, 0);
        return 0;
    }

    st = list_match(&lst1, lst1.head->next, &lst2, lst2.head->next, generic_intptr_eq, 3);
    list_clear(&lst1, 0);
    list_clear(&lst2, 0);

    return !st;
}

void *generic_intptr_negate(const void *ptr){
    intptr_t foo = (intptr_t)ptr;
    return (void *)(-foo);
}

int32_t test_list_map(){
    list_t list, mapped;
    list_init(&list);
    list_init(&mapped);

    list_addlast(&list, (void *)2);
    list_addlast(&list, (void *)3);
    list_addlast(&list, (void *)5);
    list_addlast(&list, (void *)(-7));
    list_addlast(&list, (void *)(-11));

    list_map(&mapped, &list, generic_intptr_negate);

    intptr_t expected[] = {-2, -3, -5, 7, 11};

    int32_t ret = assert_intptr_lstcontents(&mapped, expected, 5);
    
    list_clear(&list, 0);
    list_clear(&mapped, 0);
    return !ret;
}

int32_t generic_intptr_even(const void *ptr){
    intptr_t foo = (intptr_t)ptr;
    return (foo % 2) == 0;
}

int32_t test_list_filter(){
    list_t list, filtered;
    list_init(&list);
    list_init(&filtered);

    list_addlast(&list, (void *)1);
    list_addlast(&list, (void *)-2);
    list_addlast(&list, (void *)31337);
    list_addlast(&list, (void *)65536);

    list_filter(&filtered, &list, generic_intptr_even);

    intptr_t expected[] = {-2, 65536};

    int32_t ret = assert_intptr_lstcontents(&filtered, expected, 2);

    list_clear(&list, 0);
    list_clear(&filtered, 0);
    return !ret;
}

void generic_tuple_intptr_disp(FILE *output, const void *value){
    tuple_t *tuple = (tuple_t *)value;
    fprintf(output, "(%d, %d)", (intptr_t)tuple->fst, (intptr_t)tuple->snd);
}

int32_t test_list_zip(){
    list_t foo, bar, zipped;
    list_init(&foo);
    list_init(&bar);
    list_init(&zipped);

    intptr_t lvals[] = { 2, 3, 5, 7, 11, 13, 17};
    intptr_t rvals[] = { 1, 1, 2, 3, 5, 8, 13};

    for(int i = 0; i < 7; i++){
        list_addlast(&foo, (void *)lvals[i]);
        list_addlast(&bar, (void *)rvals[i]);
    }

    list_zip(&zipped, &foo, &bar);

    node_t *node = zipped.head->next;
    for(int i = 0; i < 7; i++){
        tuple_t *zap = (tuple_t *)node->data;
        if(((intptr_t)zap->fst) != lvals[i] || ((intptr_t)zap->snd) != rvals[i]){
            list_clear(&foo, 0);
            list_clear(&bar, 0);
            list_clear(&zipped, 0);
            return 0;
        }
        node = node->next;
    }
    list_clear(&foo, 0);
    list_clear(&bar, 0);
    list_clear(&zipped, 0);
    return 1;
}

void *generic_intptr_mul(const void *foo, const void *bar){
    intptr_t baz = (intptr_t)foo, zap = (intptr_t)bar;
    return (void *)(baz * zap);
}

int32_t test_list_zipwith(){
    list_t foo, bar, zipped;
    list_init(&foo);
    list_init(&bar);
    list_init(&zipped);

    intptr_t lvals[] = {2, 5, 11, 17};
    intptr_t rvals[] = {3, 7, 13, 19};
    for(int i = 0; i < 4; i++){
        list_addlast(&foo, (void *)lvals[i]);
        list_addlast(&bar, (void *)rvals[i]);
    }

    intptr_t expected[] = {6, 35, 143, 323};

    list_zipwith(&zipped, &foo, &bar, generic_intptr_mul);

    int32_t ret = assert_intptr_lstcontents(&zipped, expected, 4);
    list_clear(&foo, 0);
    list_clear(&bar, 0);
    list_clear(&zipped, 0);
    return !ret;
}

int32_t generic_string_eq(const void *lhs, const void *rhs){
    return !strcmp((const char *)lhs, (const char *)rhs);
}

void generic_string_disp(FILE *output, const void *str){
    fprintf(output, "%s", (const char *)str);
}

int32_t test_dict_add(){
    dict_t dict;
    dict_init(&dict, 0, oat_string_hash, generic_string_eq, DICTHT_DEFAULTS);

    char *keys[] = {"foo", "bar", "baz", "quuuuuux"};
    intptr_t values[] = {42, 0xF00, 1337, 31337};
    bucket_t result;

    for(int i = 0; i < 4; i++){
        dict_add(&dict, keys[i], (void *)values[i], &result);
        if((char *)result.key != keys[i] || (intptr_t)result.data != values[i]){
            dict_clear(&dict, 0);
            return 0;
        }
    }

    dict_add(&dict, "bar", (void *)0xdeadbeef, &result);
    if(strcmp((char *)result.key, "bar") || (intptr_t)result.data != 0xF00){
        dict_clear(&dict, 0);
        return 0;
    }

    intptr_t expected[] = {42, 0xdeadbeef, 1337, 31337};

    for(int i = 0; i<4; i++){
        intptr_t val = (intptr_t)dict_get(&dict, keys[i]);
        if(val != expected[i]){
            dict_clear(&dict, 0);
            return 0;
        }
    }

    dict_clear(&dict, 0);
    return 1;
}

int32_t test_dict_remove(){
    dict_t dict;
    dict_init(&dict, 0, oat_string_hash, generic_string_eq, DICTHT_DEFAULTS);

    char *keys[] = {"foo", "bar", "baz", "quuuuuux"};
    intptr_t values[] = {42, 0xF00, 1337, 31337};
    bucket_t result;

    for(int i = 0; i < 4; i++){
        dict_add(&dict, keys[i], (void *)values[i], &result);
    }

    intptr_t r0 = (intptr_t)dict_remove(&dict, "foo", &result);
    if(r0 != 42){
        dict_clear(&dict, 0);
        return 0;
    }

    intptr_t r1 = (intptr_t)dict_remove(&dict, "baz", &result);
    if(r1 != 1337){
        dict_clear(&dict, 0);
        return 0;
    }

    assert(dict.load == 2);
    char *xkeys[] = {"bar", "quuuuuux"};
    intptr_t xvals[] = {0xF00, 31337};

    for(int i = 0; i < 2; i++){
        intptr_t rt = (intptr_t)dict_get(&dict, xkeys[i]);
        if(rt != xvals[i]){
            dict_clear(&dict, 0);
            return 0;
        }
    }

    dict_clear(&dict, 0);
    return 1;
}

void alpha26(char *buf, intptr_t n){
    /* Converts an integer into a base-26 alphabetical representation of itself.
     * This is for internal testing only. No bounds checking is performed. 
     * Callers are responsible for passing in a big enough buffer. */

    int i = 0;
    while(n > 0){
        buf[i++] = (char)(n % 26) + 'a';
        n /= 26;
    }
    buf[i] = '\0';
}

int32_t test_dict_resize(){
    char buf[12];
    dict_t dict;
    dict_init(&dict, 0, oat_string_hash, generic_string_eq, DICTHT_DEFAULTS);
    bucket_t result;

    for(intptr_t i = 11; i < 4093; i++){
        alpha26(buf, i);
        dict_add(&dict, strdup(buf), (void *)i, &result);
        if(strcmp(result.key, buf) || (intptr_t)result.data != i){
            dict_clear(&dict, DICTHT_FREE_KEYS);
            fprintf(stderr, "Dict add failure. Expected: (%s:%d), got: (%s:%d)", 
                buf, i, (char *)result.key, (intptr_t)result.data);
            return 0;
        }
    }

    assert(dict.length != 2039);
    size_t grow_size = dict.length;

    for(intptr_t i = 11; i < 4093; i++){
        alpha26(buf, i);
        intptr_t r0 = (intptr_t)dict_get(&dict, buf);
        if(r0 != i){
            fprintf(stderr, "Corrupted dictionary: expected %d, got %d", i, r0);
        }
    }

    for(intptr_t i = 500; i < 4093; i++){
        alpha26(buf, i);
        intptr_t r0 = (intptr_t)dict_remove(&dict, buf, &result);
        if(r0 != i || strcmp(result.key, buf)){
            fprintf(stderr, "Dict retrieve failure. Expected: (%s:%d), got: (%s:%d)",
                buf, i, (char *)result.key, r0);
            dict_clear(&dict, DICTHT_FREE_KEYS);
            return 0;
        }
        free(result.key); /* This was allocated with strdup */
    }

    assert(dict.length != grow_size);

    dict_clear(&dict, DICTHT_FREE_KEYS);
    return 1;
}

int32_t generic_strcmp(const void *lhs, const void *rhs){
    return strcmp((const char *)lhs, (const char *)rhs);
}

int32_t test_rbt_add(){
    rbtree_t tree;
    rbt_init(&tree, generic_strcmp);
    rbnode_t result;

    char *keys[] = {"foo", "manchu", "xyzzy", "pikachu", "asdfghj"};
    intptr_t values[] = { 2, 3, 5, 7, 11};

    for(int i = 0; i < 5; i++){
        rbt_insert(&tree, keys[i], (void *)values[i], &result);
    }

    int32_t as = rbt_assert(&tree);
    if(as == 0){
        rbt_clear(&tree, 0);
        return 0;
    }

    for(int i = 0; i < 5; i++){
        intptr_t r0 = (intptr_t)rbt_get(&tree, keys[i]);
        if(r0 != values[i]){
            rbt_clear(&tree, 0);
            return 0;
        }
    }

    rbt_clear(&tree, 0);
    return 1;
}

int32_t test_rbt_remove(){
    rbtree_t tree;
    rbt_init(&tree, generic_strcmp);
    rbnode_t result;

    char *keys[] = {"foo", "jew", "boo", "hoo", "bazzy", "snazzy", "jazzy", "lazy"};
    intptr_t values[] = {2, 3, 5, 7, 11, 13, 17, 19};

    for(int i = 0; i < 8; i++){
        rbt_insert(&tree, keys[i], (void *)values[i], &result);
        if(rbt_assert(&tree) == 0){
            rbt_clear(&tree, 0);
            return 0;
        }
    }

    char *rkeys[] = {"jew", "bazzy", "snazzy"};
    intptr_t rvalues[] = {3, 11, 13};
    int32_t as;

    for(int i = 0; i < 3; i++){
        intptr_t r0 = (intptr_t)rbt_remove(&tree, rkeys[i], &result);
        if(r0 != rvalues[i] || ((as = rbt_assert(&tree)) == 0)){
            fprintf(stderr, "\nExpected: [%s:%d], actual: [%s:%d]\n",
                rkeys[i], rvalues[i], (char *)result.key, (intptr_t)result.data);
            fprintf(stderr, "Tree assertion result: %d\n", as);

            rbt_clear(&tree, 0);
            return 0;
        }
    }

    char *xkeys[] = {"foo", "boo", "hoo", "jazzy", "lazy"};
    intptr_t xvalues[] = {2, 5, 7, 17, 19};

    for(int i = 0; i < 5; i++){
        intptr_t r0 = (intptr_t)rbt_get(&tree, xkeys[i]);
        if(r0 != xvalues[i] || rbt_assert(&tree) == 0){
            rbt_clear(&tree, 0);
            return 0;
        }
    }

    rbt_clear(&tree, 0);
    return 1;
}

int32_t test_rbt_get_miss(){
    rbtree_t tree;
    rbt_init(&tree, generic_strcmp);
    rbnode_t result;

    char *keys[] = {"foo", "bar", "car"};
    intptr_t values[] = {2, 3, 5};

    for(int i = 0; i < 3; i++){
        rbt_insert(&tree, keys[i], (void *)values[i], &result);
        if(rbt_assert(&tree) == 0){
            rbt_clear(&tree, 0);
            return 0;
        }
    }

    char *try[] = {"construct", "additional", "pylons"};

    for(int i = 0; i < 3; i++){
        if(rbt_get(&tree, try[i]) != NULL || rbt_assert(&tree) == 0){
            rbt_clear(&tree, 0);
            return 0;
        }
    }

    rbt_clear(&tree, 0);
    return 1;
}

int32_t test_rbt_insert_overwrite(){
    rbtree_t tree;
    rbt_init(&tree, generic_strcmp);
    rbnode_t result;

    char *keys[] = {"foo", "bar", "car"};
    intptr_t values[] = {2, 3, 5};

    for(int i = 0; i < 3; i++){
        rbt_insert(&tree, keys[i], (void *)values[i], &result);
    }

    intptr_t ovrw[] = {7, 11, 13};

    for(int i = 0; i < 3; i++){
        rbt_insert(&tree, keys[i], (void *)ovrw[i], &result);
        if(((intptr_t)result.data) != values[i] || rbt_assert(&tree) == 0){
            rbt_clear(&tree, 0);
            return 0;
        }
    }

    for(int i = 0; i < 3; i++){
        intptr_t r0 = (intptr_t)rbt_get(&tree, keys[i]);
        if(r0 != ovrw[i]){
            rbt_clear(&tree, 0);
            return 0;
        }
    }

    rbt_clear(&tree, 0);
    return 1;
}

int32_t test_rbt_remove_empty(){
    /* Tests various possible remove misses on an rbtree. */
    rbtree_t tree;
    rbt_init(&tree, generic_strcmp);
    rbnode_t result;

    void *r0 = rbt_remove(&tree, "beer", &result); /* There is no beer in this tree */
    if(r0 != NULL || result.data != NULL || result.key != NULL){
        rbt_clear(&tree, 0);
        return 0;
    }

    char *keys[] = {"foo", "bar", "car"}; /* At least there's a bar */
    intptr_t values[] = {2, 3, 5};

    for(int i = 0; i < 3; i++){
        rbt_insert(&tree, keys[i], (void *)values[i], &result);
    }

    char *try[] = {"zap", "qux", "xyzzy"}; /* These are not in the tree either */
    for(int i = 0; i < 3; i++){
        r0 = rbt_remove(&tree, try[i], &result);
        if(r0 != NULL || result.data != NULL || result.key != NULL){
            rbt_clear(&tree, 0);
            return 0;
        }
    }

    rbt_remove(&tree, "bar", &result);  /* This economy's a bitch. Can't even keep a bar */
    r0 = rbt_remove(&tree, "bar", &result);
    if(r0 != NULL || result.data != NULL || result.key != NULL){
        rbt_clear(&tree, 0);
        return 0;
    }

    rbt_clear(&tree, 0);
    return 1;
}

int main(int argc, char **argv){
    int32_t (*TESTS[])() = {
        test_list_addfirst,
        test_list_addlast,
        test_list_insertbefore,
        test_list_insertafter,
        test_list_remove,
        test_list_match,
        test_list_map,
        test_list_filter,
        test_list_zip,
        test_list_zipwith,
        test_dict_add,
        test_dict_remove, 
        test_dict_resize,
        test_rbt_add, 
        test_rbt_remove,
        test_rbt_get_miss,
        test_rbt_insert_overwrite,
        test_rbt_remove_empty
    };
    const int32_t TEST_LENGTH = sizeof(TESTS) / sizeof(TESTS[0]);

    fprintf(stderr, "Running %d tests", TEST_LENGTH);
    for(int i = 0; i < TEST_LENGTH; i++){
        int32_t (*test)() = TESTS[i];
        if(!test()){
            fprintf(stderr, "\nTest %d FAILED\n", i+1);
            return 1;
        }
        fprintf(stderr, ".");
    }
    fprintf(stderr, "\nAll tests passed\n");
    return 0;
}
