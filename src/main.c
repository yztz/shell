#include <stdio.h>
#include "test.h"
#include "parser.h"
#include "lex.h"
#include "env.h"


void wait_input();

int main() {
    #ifdef TEST_SAMPLE
    start_test();
    #endif
    wait_input();
    return 0;
}

void wait_input() {
    shell_init();
    while(1) {
        char *line = readln();
        YY_BUFFER_STATE bp;
        bp = yy_scan_string(line);
        yyparse();
        yy_delete_buffer(bp);
        free(line);
    }
}