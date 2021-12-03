#ifndef _H_BUILTIN_
#define _H_BUILTIN_

#include "common.h"
#include "job.h"

#define BUILTIN_CMD_MAX_LENGTH 255
#define MAX_BUILTIN_NUM 255

typedef int (*builtin_func)(proc_t process,
                            int in_file,
                            int out_file,
                            int err_file);

struct builtin {
    char name[BUILTIN_CMD_MAX_LENGTH];
    builtin_func func;
};

typedef struct builtin builtin_t;


int             add_builtin_cmd(const char* name, builtin_func func);
builtin_func    find_cmd(const char *name);

#define EXPORT_BUILT_IN(name, func)                        \
    __attribute__((constructor)) void _built_in_init_##name() { \
        add_builtin_cmd(#name, func);                      \
    }

#define BUILT_IN(name) \
    int _shell_built_in_##name(proc_t process, int in_file, int out_file, int err_file);\
    EXPORT_BUILT_IN(name, _shell_built_in_##name)  \
    int _shell_built_in_##name(proc_t process, int in_file, int out_file, int err_file)




#endif