#include "test.h"
#include "lex.h"
#include "parser.h"
#include <string.h>
#include <stdlib.h>
#include "err.h"

TEST(test_test) {
    printf("Hello World\n");
}

TEST(test_lexical) {
    YY_BUFFER_STATE bp;
    bp = yy_scan_string("vim \"hello world\"\n");
    yyparse();
    yy_delete_buffer(bp);
}


TEST(test_error) {
    info("how a wonderful thing: %s!", "BBQ");
    error("a %s is expected!", "cmd");
    panic("you shoudn't do such a foolish thing!");
}