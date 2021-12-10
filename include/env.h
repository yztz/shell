#ifndef _H_ENV_
#define _H_ENV_

#include <termios.h>
#include <sys/types.h>
#include "err.h"
/* 路径的最大长度 */
#define PATH_LENTH 1024
/* shell名称 */
#define SHELL_NAME "Ysh"
// 面向用户的消息
#define umsg(msg, ...) message(SHELL_NAME, msg, ##__VA_ARGS__)

/* 初始化shell */
void        shell_init();
/* 获取当前的工作目录 */
const char* get_dir();
/* 设置工作目录 */
int         set_dir(const char *dir);
/* 恢复默认工作目录 */
void        restore_dir();
/* 设置输入模式 */
int         set_terminal_flag(struct termios flag);
/* 恢复输入模式 */
int         restore_terminal_flag(struct termios *flag);
/* readline相关封装 */
char *      readln();

extern int ysh_terminal;
extern pid_t ysh_pgid;
extern struct termios ysh_terminal_flag;

#endif