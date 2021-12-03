#ifndef _H_JOB_
#define _H_JOB_

#include <sys/types.h>

#define MAX_ARG_NUM 20
#define MAX_PROC_NUM 10

enum status {
    COMPLETED,
    STOPPED,
    WAITING,
};

struct process {
    char *args[MAX_ARG_NUM];
    int argc;

    pid_t pid;
    int status;
};

typedef struct process *proc_t;

struct job {
    struct job *next;

    proc_t processes[MAX_PROC_NUM];

    size_t process_num;

    int stdin;
    int stdout;
    int stderr;

    char *in_file;
    char *out_file;
    char *err_file;

    int fg;

    pid_t pgid;
};

typedef struct job *job_t;


proc_t  create_process();
job_t   create_job();
int     process_add_arg(proc_t proc, char *arg);
int     job_add_process(job_t job, proc_t process);
int     execute_job(job_t job);
int     execute_process(proc_t process, int in_file, int out_file, int err_file, int fg);

#endif 