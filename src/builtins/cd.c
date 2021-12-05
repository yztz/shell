#include "builtin.h"
#include "env.h"

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