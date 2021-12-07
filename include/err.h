#ifndef _H_ERR_
#define _H_ERR_
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

#define _MAKE_OUT(title, file, color)                                        \
    do {                                                                     \
        va_list argptr;                                                      \
        va_start(argptr, msg);                                               \
        char* buffer = (char*)malloc(MAX_MSG_LENGTH);                        \
        snprintf(buffer, MAX_MSG_LENGTH, color "[ %s ] %s" COLOR_CLEAR "\n", \
                 title, msg);                                                \
        vfprintf(file, buffer, argptr);                                      \
        free(buffer);                                                        \
        va_end(argptr);                                                      \
    } while (0);

INLINE void info(const char* msg, ...) {
#ifdef LOG_INFO
    _MAKE_OUT("Info", stdout, BLUE);
#endif
}

INLINE void error(const char* msg, ...) {
#ifdef LOG_ERROR
    _MAKE_OUT("Error", stdout, YELLOW);
#endif
}

INLINE void panic(const char* msg, ...) {
#ifdef LOG_PANIC
    _MAKE_OUT("Panic", stderr, RED);
#endif
    exit(0);
}

INLINE void debug(const char* msg, ...) {
#ifdef LOG_DEBUG
    _MAKE_OUT("Debug", stdout, ATTR(40) GREEN);
#endif
}

INLINE void message(const char* title, const char* msg, ...) {
    _MAKE_OUT(title, stdout, WHITE);
}

#endif