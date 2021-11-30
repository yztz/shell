#include "job.h"
#include <stdlib.h>
#include "err.h"


static struct job job_list_head;

void _init_proc(proc_t process) {
    //todo:
}

void _init_job(job_t job) {
    //todo:

}

job_t create_job() {
    job_t p = &job_list_head;
    job_t new_job = (job_t) malloc(sizeof(struct job));
    if(!new_job) {
        error("job malloc failure");
        return NULL;
    }
    while(p->next) p = p->next;

    _init_job(new_job);

    p->next = new_job;
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

int process_add_arg(proc_t process, const char *arg) {
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

    new_proc = (proc_t) malloc(sizeof(struct process));
    if(!new_proc) {
        error("process malloc failure");
        return NULL;
    }

    _init_proc(new_proc);

    return new_proc;
}