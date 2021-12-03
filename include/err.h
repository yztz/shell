#ifndef _H_ERR_
#define _H_ERR_
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

#define MAX_MSG_LENGTH 255

#define _MAKE_OUT(title, file, color)                                        \
    va_list argptr;                                                          \
    va_start(argptr, msg);                                                   \
    char* buffer = (char*)malloc(MAX_MSG_LENGTH);                            \
    snprintf(buffer, MAX_MSG_LENGTH, color "[ " title " ] %s\n" COLOR_CLEAR, \
             msg);                                                           \
    vfprintf(file, buffer, argptr);                                          \
    free(buffer);                                                            \
    va_end(argptr);

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
    _MAKE_OUT("Debug", stdout, COLOR(40, 37));
#endif
}          

#endif