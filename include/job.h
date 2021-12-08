#ifndef _H_JOB_
#define _H_JOB_

#include <sys/types.h>
#include <termios.h>

/* 最大参数数目 */
#define MAX_ARG_NUM 20
/* 最大进程数目 */
#define MAX_PROC_NUM 10

/*
    这样做的好处在于后续改变状态可以少一步判断
    比如说在continue_job中，对于completed的进程
    我们需要先用if来判断进程是否completed，再选择是否设置为running...
    相当于是使用空间换时间
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
    /* 参数字符串数组 */
    char *args[MAX_ARG_NUM];
    /* 参数数量 */
    int argc;
    /* 进程pid */
    pid_t pid;
    /* 进程状态 一共有四种状态， 由二进制前四位来表示 */
    int status;
};

typedef struct process *proc_t;

struct job {
    /* ID */
    int id;
    /* 链表节点 */
    struct job *next;
    /* 命令原文 */
    char *command;
    /* 进程指针数组 */
    proc_t processes[MAX_PROC_NUM];
    /* 进程数目 */
    size_t process_num;

    /* 标准输入文件描述符 */
    int stdin;
    /* 标准输出文件描述符 */
    int stdout;
    /* 标准错误输出文件描述符 */
    int stderr;
    /* 输入文件 */
    char *in_file;
    /* 输出文件 */
    char *out_file;
    /* 错误输出文件 */
    char *err_file;
    /* 是否在前台执行 */
    int fg;

    /* 任务进程组ID */
    pid_t pgid;
    /* 任务输入模式 */
    struct termios terminal_flag;
};

typedef struct job *job_t;

//------------------------------------
//
// 创建以及释放
//
//------------------------------------
/* 创建一个新进程 */
proc_t  create_process();
/* 创建一个新任务 */
job_t   create_job();
/* 释放任务内存 */
void    free_job(job_t job);
/* 释放进程 */
void    free_process(proc_t proc);
/* 给进程添加参数 */
int     process_add_arg(proc_t proc, char *arg);
/* 给任务添加进程 */
int     job_add_process(job_t job, proc_t process);
/* 收集并释放已完成的任务 */
void    collect_completed_jobs();

//------------------------------------
//
// 进程&任务 控制
//
//------------------------------------
/* 执行任务 */
int     execute_job(job_t job);
/* 执行进程 */
int     execute_process(proc_t process, int pgid, int in_file, int out_file, int err_file, int fg);
/* 阻塞，直到任务结束 */
void    wait_job(job_t job);
/* 给定waitpid返回status值，来设置对应pid进程的状态 */
int     change_proc_status(pid_t pid, int status);
/* 更新进程状态 */
void    update_status();
/* 添加任务到任务链表 */
void    add_to_jobs(job_t job);
/* 通过任务ID寻找任务 */
job_t   find_job_by_id(int id);
/* 检测任务是否完成 */
int     job_is_completed(job_t job);
/* 检测任务是否停止 */
int     job_is_stopped(job_t job);
/* 将任务放置到前台 */
void    put_job_fg(job_t job, int cont);
/* 继续任务 */
void    continue_job(job_t job, int foreground);


//------------------------------------
//
// 其他
//
//------------------------------------
extern struct job job_list_head;

#define for_each_job(p, phead) \
    for (p = (phead)->next;p;p=p->next)

#define for_each_proc(p, job)   \
    size_t __i = 0;             \
    for (p = (job)->processes[0]; __i < (job)->process_num; __i++, p = (job)->processes[__i])

#endif 