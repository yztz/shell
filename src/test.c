#include "test.h"

static int test_num = 0;
static void (* test_functions[TEST_LIST_MAX])(void); 
static char *test_name_list[TEST_LIST_MAX];

void _test_add_func(void (*fn)(void), char *name) {
    test_name_list[test_num] = name;    
    test_functions[test_num] = fn;    
    test_num++;    
}

void start_test() {
    for(size_t i = 0; i < test_num; i++) {
        printf("[TEST %s]\n", test_name_list[i]);
        test_functions[i]();
    }
}