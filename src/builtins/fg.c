#include "builtin.h"
#include "env.h"
#include "job.h"
#include "stdlib.h"

BUILT_IN(fg) {
    if (process->argc > 1) {
        char *id_str = process->args[1];
        int id = atoi(id_str);
        job_t job = find_job_by_id(id);
        if (job) {
            continue_job(job, 1);
            return 0;
        } else {
            umsg("job ID = %d doesn't exist");
            return -1;
        }
    } else {
        umsg("a job ID is expected");
        return -1;
    }
}