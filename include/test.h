#ifndef _H_TEST_
#define _H_TEST_

#include <stdio.h>
#include "common.h"

#define TEST_LIST_MAX 255

void _test_add_func(void (*fn)(void), char *name);

void start_test();


#define TEST(NAME) \
    static void __TEST_##NAME(); \
    _constructor static void __INIT_TEST_##NAME() {  \
        _test_add_func(__TEST_##NAME, #NAME);   \
    }   \
    static void __TEST_##NAME()

#endif
