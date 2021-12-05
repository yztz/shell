#ifndef _H_JOB_
#define _H_JOB_

#include <sys/types.h>
#include <termios.h>

#define MAX_ARG_NUM 20
#define MAX_PROC_NUM 10

/*
    这样做的好处在于后续改变状态可以少一步判断
*/
#define COMPLETED   (1 << 0)
#define STOPPED     (1 << 1)
#define WAITING     (1 << 2)
#define RUNNING     (1 << 2)

#define set_status(s, t)    ((s) |= t)
#define clear_status(s, t)  ((s) &= ~t)
#define assert_status(s, t) ((s) & t)

#define is_completed(s)  assert_status(s, COMPLETED)
#define is_stopped(s)    assert_status(s, STOPPED)
#define is_waiting(s)    assert_status(s, WAITING)
#define is_running(s)    assert_status(s, RUNNING)

struct process {
    char *args[MAX_ARG_NUM];
    int argc;

    pid_t pid;
    int status;
};

typedef struct process *proc_t;

struct job {
    int id;
    struct job *next;
    char *command;
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

    struct termios terminal_flag;
};

typedef struct job *job_t;


proc_t  create_process();
job_t   create_job();
int     process_add_arg(proc_t proc, char *arg);
int     job_add_process(job_t job, proc_t process);
int     execute_job(job_t job);
int     execute_process(proc_t process, int pgid, int in_file, int out_file, int err_file, int fg);
void    wait_job(job_t job);
int     change_proc_status(pid_t pid, int status);
void    update_status();
void    add_to_jobs(job_t job);

int     job_is_completed(job_t job);
int     job_is_stopped(job_t job);

void    free_job(job_t job);
void    free_process(proc_t proc);

void    put_job_fg(job_t job);
// void    put_job_bg(job_t job);
void    continue_job(job_t job, int foreground);
void    collect_completed_jobs();

#define for_each_job(p, phead) \
    for (p = (phead)->next;p;p=p->next)

#define for_each_proc(p, job)   \
    size_t __i = 0;             \
    for (p = (job)->processes[0]; __i < (job)->process_num; __i++, p = (job)->processes[__i])

#endif 