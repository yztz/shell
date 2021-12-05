#ifndef _H_ENV_
#define _H_ENV_

#include <termios.h>
#include "err.h"
#define PATH_LENTH 1024
#define SHELL_NAME "Ysh"
// 面向用户的消息
#define umsg(msg, ...) message(SHELL_NAME, msg, __VA_ARGS__)

void        shell_init();
const char* get_dir();
int         set_dir(const char *dir);
void        restore_dir();
int         set_terminal_flag(struct termios flag);
int         restore_terminal_flag();
char *      readln();

#endif