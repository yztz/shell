#include "test.h"
#include "err.h"

TEST(test_log, 1) {
    debug("notice : this is only a simple debug");
    info("what a wonderful thing: %s!", "BBQ");
    error("a %s is expected!", "cmd");
    // panic("you shoudn't do such a foolish thing!");
}