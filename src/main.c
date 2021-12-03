#include <stdio.h>
#include "test.h"
#include "parser.h"
#include "lex.h"
#include "env.h"


void wait_input();

int main() {
    // start_test();
    wait_input();
    return 0;
}

void wait_input() {
    shell_init();
    char buffer[255];
    while(1) {
        printf("%s $ ", get_dir());
        scanf("%[^\n]", buffer);
        getchar();
        int len = strlen(buffer);
        buffer[len] = '\n';
        buffer[len + 1] = '\0';
        YY_BUFFER_STATE bp;
        bp = yy_scan_string(buffer);
        yyparse();
        yy_delete_buffer(bp);
    }
}