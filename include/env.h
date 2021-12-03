#ifndef _H_ENV_
#define _H_ENV_

#include <termios.h>
#define PATH_LENTH 1024

void        shell_init();
const char* get_dir();
int         set_dir(const char *dir);
void        restore_dir();
int         set_terminal_flag(struct termios flag);
int         restore_terminal_flag();

#endif