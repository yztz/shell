#include "job.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "builtin.h"
#include "common.h"
#include "env.h"
#include "err.h"

extern int tty_interactive;
extern int terminal;
extern pid_t pgid;
extern struct termios terminal_flag;

static struct job job_list_head;

INLINE void _init_proc(proc_t process) {
    process->status = WAITING;
    process->argc = 0;
    process->pid = -1;
}

INLINE void _init_job(job_t job) {
    job->next = NULL;
    job->pgid = 0;
    job->in_file = NULL;
    job->out_file = NULL;
    job->err_file = NULL;
    job->stdin = STDIN_FILENO;
    job->stdout = STDOUT_FILENO;
    job->stderr = STDERR_FILENO;
    job->fg = 1;
    job->process_num = 0;
    job->terminal_flag = terminal_flag;
}

job_t create_job() {
    job_t new_job = (job_t)malloc(sizeof(struct job));
    if (!new_job) {
        error("job malloc failure");
        return NULL;
    }
    _init_job(new_job);
    return new_job;
}

int job_add_process(job_t job, proc_t process) {
    if (!job) {
        error("the job has not been inited!");
        return -1;
    }

    if (job->process_num == MAX_PROC_NUM) {
        error("too many processes in a job");
        return -1;
    }

    job->processes[job->process_num++] = process;

    return 0;
}

int process_add_arg(proc_t process, char* arg) {
    if (!process) {
        error("the process has not been inited!");
        return -1;
    }
    if (process->argc == MAX_ARG_NUM) {
        error("too many arguments");
        return -1;
    }

    process->args[process->argc++] = arg;

    return 0;
}

proc_t create_process() {
    proc_t new_proc;

    new_proc = (proc_t)malloc(sizeof(struct process));
    if (!new_proc) {
        error("process malloc failure");
        return NULL;
    }

    _init_proc(new_proc);
    return new_proc;
}

#define OUT_FLAG O_CREAT | O_TRUNC | O_WRONLY, 0666

int execute_job(job_t job) {
    int in_file;
    int out_file;
    int err_file;
    pid_t pid;
    int fd[2] = {0, 0};

    if (!job) {
        error("the job has not been inited");
        return -1;
    }

    if (job->in_file) {  // 输入重定向
        if ((job->stdin = open(job->in_file, O_RDONLY)) == -1) {
            error("file %s doesn't exist", job->in_file);
            goto err;
        }
    }

    if (job->out_file) {  // 输出重定向
        if ((job->stdout = open(job->out_file, OUT_FLAG)) == -1) {
            error("fail to open file %s", job->out_file);
            goto err;
        }
    }

    if (job->err_file) {  // 错误重定向
        if ((job->stderr = open(job->err_file, OUT_FLAG)) == -1) {
            error("fail to open file %s", job->err_file);
            goto err;
        }
    }

    in_file = job->stdin;
    err_file = job->stderr;
    // debug("total num of proc is: %d", job->process_num);
    for (size_t i = 0; i < job->process_num; i++) {
        if (i + 1 < job->process_num) {
            if (pipe(fd) == -1) {  // 存在后续管道进程
                error("fail to make pipe");
                goto err;
            }
            out_file = fd[1];
        } else {
            out_file = job->stdout;
        }

        proc_t process = job->processes[i];
        builtin_func bf = NULL;
        if ((bf = find_cmd(process->args[0]))) {
            // todo:
            int ret = bf(process, in_file, out_file, err_file);
            process->status = COMPLETED;

            if (ret == -1)
                return -1;
        } else {
            if ((pid = fork()) == -1) {
                error("fork fail");
                goto err;
            }
            if (pid == 0) {  // child
                // todo:
                execute_process(process, job->pgid, in_file, out_file, err_file,
                                job->fg);
            } else {  // parent
                process->pid = pid;
                process->status = RUNNING;
                if (!job->pgid) {
                    job->pgid = pid;
                }
            }
            /*
            为什么这里需要关闭？
            因为先前执行了一遍fork，相关数据结构已经是duplicated了
            */
            if (in_file > 2 && close(in_file) == -1) {
                error("fail to close in_file");
                goto err;
            }
            if (out_file > 2 && close(out_file) == -1) {
                error("fail to close out_file");
                goto err;
            }
            if (err_file > 2 && close(err_file) == -1) {
                error("fail to close err_file");
                goto err;
            }
        }
        in_file = fd[0];
    }

    add_to_jobs(job);

    if (job->fg) {
        put_job_fg(job);
    } else {
        // todo:
    }

    return 0;

err:
    error("something wrong happened...");
    if (fd[0] > 2)
        close(fd[0]);
    if (fd[1] > 2)
        close(fd[1]);
    if (job->stdin > 2)
        close(job->stdin);
    if (job->stdout > 2)
        close(job->stdout);
    if (job->stderr > 2)
        close(job->stderr);
    return -1;
}

void wait_job(job_t job) {
    int pgid = job->pgid;
    int status;
    pid_t pid;
    do {
        /*
            WNOHANG：如果没有任何已经结束的子进程则马上返回, 不予以等待.
            WUNTRACED：如果子进程进入暂停执行情况则马上返回,
           但结束状态不予以理会
        */
        pid = waitpid(-pgid, &status, WUNTRACED);
        change_proc_status(pid, status);
    } while (!job_is_stopped(job) && !job_is_completed(job));
}

void add_to_jobs(job_t job) {
    job_t h = &job_list_head;
    while (h->next)
        h = h->next;
    h->next = job;
    job->next = NULL;
}

void free_job(job_t job) {
    if (!job)
        return;

    sfree(&job->in_file);
    sfree(&job->out_file);
    sfree(&job->err_file);
    // 释放进程
    for (size_t i = 0; i < job->process_num; i++) {
        free_process(job->processes[i]);
        sfree(&job->processes[i]);
    }
}

void free_process(proc_t proc) {
    if (!proc)
        return;

    for (size_t i; i < proc->argc; i++) {
        sfree(&proc->args[i]);
    }
}

int job_is_completed(job_t job) {
    for (size_t i = 0; i < job->process_num; i++) {
        if (job->processes[i]->status != COMPLETED)
            return 0;
    }
    return 1;
}
int job_is_stopped(job_t job) {
    for (size_t i = 0; i < job->process_num; i++) {
        if (job->processes[i]->status == STOPPED)
            return 1;
    }
    return 0;
}

int change_proc_status(pid_t pid, int status) {
    if (pid == -1)
        return -1;

    job_t job = &job_list_head;
    proc_t proc = NULL;
    size_t i;
    while (job->next) {
        job = job->next;
        for (i = 0; i < job->process_num; i++) {
            proc = job->processes[i];
            if (proc->pid == pid) {
                if (WIFSTOPPED(status)) {
                    debug("%d: STOPPED", pid);
                    proc->status = STOPPED;
                } else {
                    proc->status = COMPLETED;
                    if (WIFSIGNALED(status)) {
                        // todo:
                        error("%d is terminated by signal %d", pid,
                              WTERMSIG(status));
                    }
                }

                return 0;
            }
        }
    }

    return -1;
}

void put_job_fg(job_t job) {
    job->fg = 1;
    tcsetpgrp(terminal, job->pgid);
    set_terminal_flag(job->terminal_flag);

    wait_job(job);

    tcsetpgrp(terminal, pgid);
    restore_terminal_flag();
}
void put_job_bg(job_t job) {

}

int execute_process(proc_t process,
                    int pgid,
                    int in_file,
                    int out_file,
                    int err_file,
                    int fg) {
    pid_t pid = getpid();
    pgid = pgid ? pgid : pid;
    setpgid(pid, pgid);
    debug("ready to execute %s", process->args[0]);
    // debug("in_file is: %d", in_file);
    // debug("out_file is: %d", out_file);

    // 恢复默认信号处理
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    if (fg) {  // todo:
        // 这个函数是设置前台进程组，作为后台进程，此举将会导致进程被暂停
        tcsetpgrp(STDIN_FILENO, pgid);
    }
    if (in_file > 2) {
        dup2(in_file, STDIN_FILENO);
        close(in_file);
    }
    if (out_file > 2) {
        dup2(out_file, STDOUT_FILENO);
        close(out_file);
    }
    if (err_file > 2) {
        dup2(err_file, STDERR_FILENO);
        close(err_file);
    }
    execvp(process->args[0], process->args);
    panic("fail to execute %s", process->args[0]);

    return -1;
}