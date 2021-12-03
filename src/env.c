#include "env.h"
#include <unistd.h>

int tty_interactive = 0;
int terminal = STDIN_FILENO;    // 终端输入为标准输入

void shell_init() {
    /*
        因为终端的标准输入可能会被重定向为文件输入，此时的便是不可交互的。
        这样做有利于对于脚本文件的支持，当然这不在当前的计划之内...
    */
    tty_interactive = isatty(terminal);
}
