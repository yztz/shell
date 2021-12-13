#include "test.h"
#include "err.h"
#include <fcntl.h>

TEST(test_log, 0) {
    debug("notice : this is only a simple debug");
    info("what a wonderful thing: %s!", "BBQ");
    error("a %s is expected!", "cmd");
    panic("you shoudn't do such a foolish thing!"); 
}

TEST(test_performance, 1) {
    int fd_null = open("/dev/null", O_WRONLY);
    if (fd_null == -1) panic("open failure");
    set_log_out(fd_null, -1);
    for(int i = 0; i < 1000000; i++) debug("current i is %d", i);
    close(fd_null);
}

TEST(test_bad_format, 0) {
    // int a = 2;
    // debug("a bad formt a = %s", a); // segmentation fault
    // printf("a bad formt a = %s", a);
}