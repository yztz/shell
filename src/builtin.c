#include <string.h>
#include "builtin.h"
#include "env.h"
#include "err.h"

static builtin_t builtins[MAX_BUILTIN_NUM];

static int num = 0;

int add_builtin_cmd(const char *name, builtin_func func) {
    if (num == MAX_BUILTIN_NUM) {
        error("too mant builtin cmd!");
        return -1;
    }

    strncpy(builtins[num].name, name, BUILTIN_CMD_MAX_LENGTH);
    builtins[num].func = func;
    num++;
    return 0;
}

BUILT_IN(cd) {
    int ret = 0;
    if(process->argc == 1) { // 只有cd，没有参数
        restore_dir();
    } else {
        char *dir = process->args[1];
        ret = set_dir(dir);
        if (ret == -1) error("cd: %s: No such file or directory", dir);
    }
    return ret;
}

BUILT_IN(exit) {
    exit(0);
} 

builtin_func find_cmd(const char *name) {
    for (size_t i = 0; i < num; i++) {
        if (strcmp(name, builtins[i].name) == 0) {
            return builtins[i].func;
        }
    }

    return NULL;
}
