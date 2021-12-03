#include "job.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "err.h"
#include <signal.h>
#include "common.h"

extern int tty_interactive;
extern int terminal;

static struct job job_list_head;

INLINE void _init_proc(proc_t process) {
    process->status = WAITING;
    process->argc = 0;
    process->pid = -1;
}

INLINE void _init_job(job_t job) {
    job->next = NULL;
    job->pgid = -1;
    job->in_file = NULL;
    job->out_file = NULL;
    job->err_file = NULL;
    job->stdin = STDIN_FILENO;
    job->stdout = STDOUT_FILENO;
    job->stderr = STDERR_FILENO;
    job->fg = 1;
    job->process_num = 0;
}

job_t create_job() {
    job_t p = &job_list_head;
    job_t new_job = (job_t)malloc(sizeof(struct job));
    if (!new_job) {
        error("job malloc failure");
        return NULL;
    }
    while (p->next)
        p = p->next;

    _init_job(new_job);

    p->next = new_job;

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

int execute_job(job_t job) {
    if (!job) {
        error("the job has not been inited");
        return -1;
    }

    if(job->in_file) { // 输入重定向
        job->stdin = 
    }

    if (job->stdout != STDOUT_FILENO) { // 输出重定向

    }

    if (job->stderr != STDERR_FILENO) {

    }

    for (size_t i = 0; i < job->process_num; i++) {
        
    }

    
}



int execute_process(proc_t process, int in_file, int out_file, int err_file, int fg) {
    // todo:
}