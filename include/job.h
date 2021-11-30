#ifndef _H_JOB_
#define _H_JOB_

#include <sys/types.h>

#define MAX_ARG_NUM 20
#define MAX_PROC_NUM 10



struct process {
    char *args[MAX_ARG_NUM];
    int argc;

    pid_t pid;
    
};

typedef struct process *proc_t;

struct job {
    struct job *next;

    proc_t processes[MAX_PROC_NUM];

    int process_num;
};

typedef struct job *job_t;


proc_t  create_process();
job_t   create_job();
int     process_add_arg(proc_t proc, const char *arg);
int     job_add_process(job_t job, proc_t process);

#endif 