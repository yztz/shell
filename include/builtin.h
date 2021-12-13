#ifndef _H_BUILTIN_
#define _H_BUILTIN_

#include "err.h"
#include "job.h"

/* 指令名的最大长度 */
#define BUILTIN_CMD_MAX_LENGTH 255
/* 内建指令表大小 */
#define MAX_BUILTIN_NUM 255

/* 定义内建指令函数类型 */
typedef int (*builtin_func)(proc_t process,
                            int in_file,
                            int out_file,
                            int err_file);

struct builtin {
    /* 内建指令名 */
    char name[BUILTIN_CMD_MAX_LENGTH];
    /* 内建指令函数指针 */
    builtin_func func;
};

typedef struct builtin builtin_t;

int add_builtin_cmd(const char* name, builtin_func func);
builtin_func find_cmd(const char* name);

/* 导出内建指令函数 */
#define EXPORT_BUILT_IN(name, func) \
    _constructor void _built_in_init_##name() { add_builtin_cmd(#name, func); }

/* 内建指令标准函数头（包含导出） */
#define BUILT_IN(name)                                                    \
    int _shell_built_in_##name(proc_t process, int in_file, int out_file, \
                               int err_file);                             \
    EXPORT_BUILT_IN(name, _shell_built_in_##name)                         \
    int _shell_built_in_##name(proc_t process, int in_file, int out_file, \
                               int err_file)

#endif