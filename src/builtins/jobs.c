#include "builtin.h"
#include "job.h"
#include "env.h"
#include <string.h>

extern struct job job_list_head;

// void format_command(job_t job) {
//     int offset = 0;
//     proc_t proc;
//     for_each_proc(proc, job) {
//         for (size_t i = 0; i < proc->argc; i++) {
//             char *p = proc->args[i];
//             while (*p != '\0') {
//                 buffer[offset++] = *p;
//             }
//         }
//     }
// }

BUILT_IN(jobs) {
    job_t job;
    update_status();
    for_each_job(job, &job_list_head) {
        if (job_is_completed(job)) continue;
        if (job_is_stopped(job)) {
            umsg("(%d) Stopped %s", job->id, job->command);
        } else {
            umsg("(%d) Running %s", job->id, job->command);
        }
    }
    return 0;
}