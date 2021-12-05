#include <string.h>
#include "builtin.h"

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


builtin_func find_cmd(const char *name) {
    for (size_t i = 0; i < num; i++) {
        if (strcmp(name, builtins[i].name) == 0) {
            return builtins[i].func;
        }
    }

    return NULL;
}
