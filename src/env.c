#include "env.h"
#include <signal.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>


int terminal = STDIN_FILENO;  // 终端输入为标准输入
struct termios terminal_flag;
int pgid;

char current_dir[PATH_LENTH];
char default_dir[PATH_LENTH];

void shell_init() {
    // 这里的目的在于当ysh以后台形式运行时，则暂停ysh，直到切换到前台进程
    while (tcgetpgrp(terminal) != (pgid = getpgrp())) {
        kill(-pgid, SIGTTIN);
    }

    signal(SIGINT, SIG_IGN);    // 用户中断
    signal(SIGQUIT, SIG_IGN);   // core dump
    signal(SIGTSTP, SIG_IGN);   // 控制终端要求终止
    signal(SIGTTIN, SIG_IGN);   // 后台要求读取
    signal(SIGTTOU, SIG_IGN);   // 后台要求输出

    pgid = getpid();
    if (setpgid(pgid, pgid) < 0) {
        panic("Couldn't put the shell in its own process group");
    }

    tcsetpgrp(terminal, pgid);
    
    getcwd(current_dir, PATH_LENTH);
    getcwd(default_dir, PATH_LENTH);
    tcgetattr(terminal, &terminal_flag);
}


int set_terminal_flag(struct termios flag) {
    return tcsetattr(terminal, TCSADRAIN, &flag);
}
int restore_terminal_flag(struct termios *flag) {
    tcgetattr(terminal, flag);
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

#define PROMPT_FORMAT   \
    BOLD GREEN "Ysh: "BLUE"%s" COLOR_CLEAR WHITE"$ " COLOR_CLEAR
    
char * readln() {
    char prompt[PATH_LENTH * 2];
    snprintf(prompt, PATH_LENTH * 2, PROMPT_FORMAT, current_dir);
    char * line;
    int len;
    do {
        line = readline(prompt);
        len = strlen(line);
    } while (len == 0);
    char *ret = (char *) malloc(len + 1);
    strcpy(ret, line);
    *(ret + len) = '\n';
    *(ret + len + 1) = '\0';
    add_history(line);
    free(line);
    return ret;
}