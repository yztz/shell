#include "test.h"
#include "common.h"
#include <time.h>

static int test_num = 0;
static void (* test_functions[TEST_LIST_MAX])(void); 
static char *test_name_list[TEST_LIST_MAX];

void _test_add_func(void (*fn)(void), char *name) {
    test_name_list[test_num] = name;    
    test_functions[test_num] = fn;    
    test_num++;    
}

void start_test() {
    clock_t start, end;
    for(size_t i = 0; i < test_num; i++) {
        printf(WHITE"[TEST %s]"COLOR_CLEAR"\n", test_name_list[i]);
        start = clock();
        test_functions[i]();
        end = clock();
        printf(GREEN"[%s COMPLETED %.3fms]"COLOR_CLEAR"\n", test_name_list[i], (double) (end - start) / CLOCKS_PER_SEC * 1000);
    }
}