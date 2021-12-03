#include "test.h"
#include "lex.h"
#include "parser.h"
#include <string.h>
#include <stdlib.h>
#include "err.h"
#include "env.h"

// TEST(test_test) {
//     printf("Hello World\n");
// }

// TEST(test_error) {
//     debug("notice : this is only a simple debug");
//     info("how a wonderful thing: %s!", "BBQ");
//     error("a %s is expected!", "cmd");
//     // panic("you shoudn't do such a foolish thing!");
// }

TEST(test_parser) {
    shell_init();
    char buffer[255];
    scanf("%[^\n]", buffer);
    int len = strlen(buffer);
    buffer[len] = '\n';
    buffer[len + 1] = '\0';
    YY_BUFFER_STATE bp;
    bp = yy_scan_string(buffer);
    yyparse();
    yy_delete_buffer(bp);
}