#ifndef _H_ERR_
#define _H_ERR_
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "common.h"

/*
一共有四种日志等级
+-------+-------+
| panic |  严重  |
+-------+-------+
| error |  错误  |
+-------+-------+
|  info |  消息  |
+-------+-------+
| debug |  调试  |
+-------+-------+
*/

/* 默认消息级别日志 */
#if (!defined LOG_DEBUG) && (!defined LOG_INFO) && (!defined LOG_ERROR) && \
    (!defined LOG_PANIC)
#define LOG_INFO
#endif

#ifdef LOG_DEBUG
#define LOG_INFO
#endif
#ifdef LOG_INFO
#define LOG_ERROR
#endif
#ifdef LOG_ERROR
#define LOG_PANIC
#endif

#define MAX_MSG_LENGTH 1024
/*
直接将buffer声明为全局静态变量以节省malloc与free的时间开销
但是造成一定程度上的空间浪费（许多文件都会使用日志）但是也有隔离文件日志的好处
本着简单易用的原则，不另外增加err.c文件
可以选择在main.c中定义这个变量，然后在该文件中使用extern来引入，但是这样在一定程度上也增加了耦合
但是幸运的是，这几种模式的切换是非常简单的，只需要改变下面这条声明语句的方式即可
*/
static char __err_msg_buffer__[MAX_MSG_LENGTH];
static int __err_msg_std_out__ = STDOUT_FILENO;
static int __err_msg_err_out__ = STDERR_FILENO;

#define LOG_STD_OUT __err_msg_std_out__
#define LOG_ERR_OUT __err_msg_err_out__

INLINE void set_log_out(int out_fd, int err_fd) {
    if (out_fd > 0)
        __err_msg_std_out__ = out_fd;
    if (err_fd > 0)
        __err_msg_err_out__ = err_fd;
}

/*
注意：下面的实现中，无论是宏实现，还是内联实现，对应的helper都是先使用snprintf先格式化了一遍
这样做的必要性在于两点：
1.我们首先要对字符串做一些格式化处理。
2.尤其是对于宏实现来说，传入的内容msg未知，可能是字符串字面量，可能是字符指针，这样做可以确保安全性。
*/

/*
对比
性能：55开
程序大小：
macro
   text    data     bss     dec     hex filename
  32808    1168   87424  121400   1da38 ysh
inline
   text    data     bss     dec     hex filename
  34451    1168   87424  123043   1e0a3 ./ysh
inline > macro
*/

//----------------------------------
// inline implementation
//----------------------------------
#define _INLINE_OUT_HELPER(title, fd, color)                      \
    do {                                                          \
        va_list argptr;                                           \
        va_start(argptr, msg);                                    \
        snprintf(__err_msg_buffer__, MAX_MSG_LENGTH,              \
                 color "[ %s ] %s" COLOR_CLEAR "\n", title, msg); \
        vdprintf(fd, __err_msg_buffer__, argptr);                 \
        va_end(argptr);                                           \
    } while (0);

INLINE void _inline_info(const char* msg, ...) {
#ifdef LOG_INFO
    _INLINE_OUT_HELPER("Info", LOG_STD_OUT, BLUE);
#endif
}

INLINE void _inline_error(const char* msg, ...) {
#ifdef LOG_ERROR
    _INLINE_OUT_HELPER("Error", LOG_ERR_OUT, YELLOW);
#endif
}

INLINE void _inline_panic(const char* msg, ...) {
#ifdef LOG_PANIC
    _INLINE_OUT_HELPER("Panic", LOG_ERR_OUT, RED);
#endif
    exit(0);
}

INLINE void _inline_debug(const char* msg, ...) {
#ifdef LOG_DEBUG
    _INLINE_OUT_HELPER("Debug", LOG_STD_OUT, ATTR(40) GREEN);
#endif
}

INLINE void _inline_message(const char* title, const char* msg, ...) {
    _INLINE_OUT_HELPER(title, LOG_STD_OUT, WHITE);
}

//----------------------------------
// inline implementation end
//----------------------------------

//----------------------------------
// macro implementation
//----------------------------------
#define _MACRO_OUT_HELPER(title, msg, fd, color, ...)             \
    do {                                                          \
        snprintf(__err_msg_buffer__, MAX_MSG_LENGTH,              \
                 color "[ %s ] %s" COLOR_CLEAR "\n", title, msg); \
        dprintf(fd, __err_msg_buffer__, ##__VA_ARGS__);           \
    } while (0);

#ifdef LOG_DEBUG
#define _macro_debug(msg, ...) \
    _MACRO_OUT_HELPER("Debug", msg, LOG_STD_OUT, ATTR(40) GREEN, ##__VA_ARGS__)
#else
#define _macro_debug(msg, ...)
#endif

#ifdef LOG_INFO
#define _macro_info(msg, ...) \
    _MACRO_OUT_HELPER("Info", msg, LOG_STD_OUT, BLUE, ##__VA_ARGS__)
#else
#define _macro_info(msg, ...)
#endif

#ifdef LOG_ERROR
#define _macro_error(msg, ...) \
    _MACRO_OUT_HELPER("Error", msg, LOG_ERR_OUT, YELLOW, ##__VA_ARGS__)
#else
#define _macro_error(msg, ...)
#endif

#ifdef LOG_PANIC
#define _macro_panic(msg, ...)                                           \
    do {                                                                 \
        _MACRO_OUT_HELPER("Panic", msg, LOG_ERR_OUT, RED, ##__VA_ARGS__) \
        exit(0);                                                         \
    } while (0);
#else
#define _macro_panic(msg, ...) exit(0)
#endif

#define _macro_message(title, msg, ...) \
    _MACRO_OUT_HELPER(title, msg, LOG_STD_OUT, WHITE, ##__VA_ARGS__)

//----------------------------------
// macro implementation end
//----------------------------------

/* uncomment to use inline implementation */
#define USING_MACRO

/* using inline implementation */
#ifndef USING_MACRO
#define info _inline_info
#define error _inline_error
#define panic _inline_panic
#define debug _inline_debug
#define message _inline_message
/* using macro implementation */
#else
#define info _macro_info
#define error _macro_error
#define panic _macro_panic
#define debug _macro_debug
#define message _macro_message
#endif

#endif