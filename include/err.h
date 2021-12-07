#ifndef _H_ERR_
#define _H_ERR_
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

#define MAX_MSG_LENGTH 1024

#define _MAKE_OUT(title, file, color)                                     \
    do {                                                                  \
        va_list argptr;                                                   \
        va_start(argptr, msg);                                            \
        char* buffer = (char*)malloc(MAX_MSG_LENGTH);                     \
        snprintf(buffer, MAX_MSG_LENGTH,                                  \
                 "[ " color "%s" COLOR_CLEAR " ] " color "%s" COLOR_CLEAR \
                 "\n",                                                    \
                 title, msg);                                             \
        vfprintf(file, buffer, argptr);                                   \
        free(buffer);                                                     \
        va_end(argptr);                                                   \
    } while (0);

INLINE void info(const char* msg, ...) {
    _MAKE_OUT("Info", stdout, BLUE);
}

INLINE void error(const char* msg, ...) {
    _MAKE_OUT("Error", stdout, YELLOW);
}

INLINE void panic(const char* msg, ...) {
    _MAKE_OUT("Panic", stderr, RED);
    exit(0);
}

INLINE void debug(const char* msg, ...) {
#ifdef DEBUG
    _MAKE_OUT("Debug", stdout, ATTR(40) GREEN);
#endif
}

INLINE void message(const char* title, const char* msg, ...) {
    _MAKE_OUT(title, stdout, WHITE);
}

#endif