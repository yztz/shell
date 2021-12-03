#include <stdio.h>
#include "test.h"

#define DEBUG

int main() {
    #ifdef DEBUG
    start_test();
    #endif

    return 0;
}