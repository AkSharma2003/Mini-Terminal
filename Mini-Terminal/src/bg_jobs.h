#ifndef BG_JOBS_H
#define BG_JOBS_H

#include <sys/types.h>

#define MAX_BG_JOBS 64

typedef struct {
    int job_id;
    pid_t pid;
    char cmd[256];
} bg_process;

extern bg_process bg_list[MAX_BG_JOBS];
extern int job_count;

void add_bg_job(pid_t pid, const char *cmd);
void remove_bg_job(pid_t pid);
void print_done_job(pid_t pid);

#endif
