#include "env.h"
#include <signal.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include "err.h"
#include "common.h"


int terminal = STDIN_FILENO;  // 终端输入为标准输入
struct termios terminal_flag;
int pgid;

char current_dir[255];
char default_dir[255];

void shell_init() {
    while (tcgetpgrp(terminal) != (pgid = getpgrp())) {
        kill(-pgid, SIGTTIN);
    }

    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);

    pgid = getpid();
    if (setpgid(pgid, pgid) < 0) {
        error("Couldn't put the shell in its own process group");
        exit(1);
    }

    tcsetpgrp(terminal, pgid);
    
    getcwd(current_dir, PATH_LENTH);
    getcwd(default_dir, PATH_LENTH);
    tcgetattr(terminal, &terminal_flag);
}


int set_terminal_flag(struct termios flag) {
    return tcsetattr(terminal, TCSADRAIN, &flag);
}
int restore_terminal_flag() {
    return tcsetattr(terminal, TCSADRAIN, &terminal_flag);
}


const char* get_dir() {
    return current_dir;
}
int set_dir(const char* dir) {
    int res = chdir(dir);
    if (res == -1) return -1;

    getcwd(current_dir, PATH_LENTH);
    return 0;
}

void restore_dir() {
    set_dir(default_dir);
}