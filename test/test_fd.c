#include <fcntl.h>
#include "unistd.h"
#include "err.h"
#include "test.h"

TEST(open) {
    char buffer[30];
    size_t len;
    int fd = open("test.txt", O_RDONLY);
    int fd2 = open("test.txt", O_RDONLY);

    len = read(fd, buffer, 30);
    buffer[len] = '\0';
    info("fd = %d len = %d content = %s", fd, len, buffer);
    len = read(fd2, buffer, 30);
    buffer[len] = '\0';
    info("fd = %d len = %d content = %s", fd, len, buffer);
}

TEST(open_fork) {
    char buffer[30];
    size_t len;
    int fd = open("test.txt", O_RDONLY);
    pid_t pid = fork();

    if (pid == 0) { // child
        len = read(fd, buffer, 30);
        buffer[len] = '\0';
        info("child: fd = %d len = %d content = %s", fd, len, buffer);
    } else {
        len = read(fd, buffer, 30);
        buffer[len] = '\0';
        info("parent: fd = %d len = %d content = %s", fd, len, buffer);
    }
}