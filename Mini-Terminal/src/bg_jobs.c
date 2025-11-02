#include "bg_jobs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bg_process bg_list[MAX_BG_JOBS];
int job_count = 0;

void add_bg_job(pid_t pid, const char *cmd) {
    if(job_count < MAX_BG_JOBS) {
        bg_list[job_count].job_id = job_count + 1;
        bg_list[job_count].pid = pid;
        strncpy(bg_list[job_count].cmd, cmd, 255);
        bg_list[job_count].cmd[255] = '\0';
        printf("[%d] %d\n", bg_list[job_count].job_id, pid);
        job_count++;
    }
}

void remove_bg_job(pid_t pid) {
    for(int i = 0; i < job_count; i++) {
        if(bg_list[i].pid == pid) {
            for(int j = i; j < job_count - 1; j++)
                bg_list[j] = bg_list[j + 1];
            job_count--;
            break;
        }
    }
}

void print_done_job(pid_t pid) {
    for(int i = 0; i < job_count; i++) {
        if(bg_list[i].pid == pid) {
            printf("\n[%d]+  Done\t\t%s\n", bg_list[i].job_id, bg_list[i].cmd);
            fflush(stdout);
            remove_bg_job(pid);
            break;
        }
    }
}


