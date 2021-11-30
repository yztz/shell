#ifndef _H_ERR_
#define _H_ERR_

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MSG_LENGTH 255

/*
bg     fg
40: 黑 30: 黑
41: 红 31: 红
42: 绿 32: 绿
43: 黄 33: 黄
44: 蓝 34: 蓝
45: 紫 35: 紫
46: 深绿 36: 深绿
47: 白色 37: 白色
*/
#define COLOR(bg, fg) "\033[" #bg ";" #fg "m"
#define RED COLOR(, 31)
#define YELLOW COLOR(, 33)
#define BLUE COLOR(, 34)

#define _MAKE_OUT(title, file, color)                                         \
    va_list argptr;                                                           \
    va_start(argptr, msg);                                                    \
    char* buffer = (char*)malloc(MAX_MSG_LENGTH);                             \
    snprintf(buffer, MAX_MSG_LENGTH, color "[ " title " ] %s\033[0m\n", msg); \
    vfprintf(file, buffer, argptr);                                           \
    free(buffer);                                                             \
    va_end(argptr);

static inline void info(const char* msg, ...) {
    _MAKE_OUT("Info", stdout, BLUE);
}

static inline void error(const char* msg, ...) {
    _MAKE_OUT("Error", stdout, YELLOW);
}

static inline void panic(const char* msg, ...) {
    _MAKE_OUT("Panic", stderr, RED);
    exit(1);
}

static inline void debug(const char* msg, ...) {
#ifdef DEBUG
    _MAKE_OUT("Debug", stdout, COLOR(47, 32));
#endif
}

#endif