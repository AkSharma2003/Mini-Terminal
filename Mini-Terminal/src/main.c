#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <time.h>
#include "bg_jobs.h"
#include "help.h"

#define MAX_INPUT 1024
#define MAX_ARG 64

// Get terminal width
int get_terminal_width() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}

// Print centered red banner with real-time date & time
void print_banner() {
    int width = get_terminal_width();

    // Get current date & time
    time_t t = time(NULL);
    struct tm tm_info = *localtime(&t);
    char time_str[64];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", &tm_info);

    // Banner lines
    char *lines[] = {
        "+-----------------------------------------------+",
        "|           Welcome to Ankits Terminal         |",
        "|                 %s                 |",  // Placeholder for time
        "+-----------------------------------------------+"
    };

    printf("\033[31m"); // Red color

    // Print first line
    int len = strlen(lines[0]);
    int padding = (width - len) / 2;
    if(padding < 0) padding = 0;
    printf("%*s%s\n", padding, "", lines[0]);

    // Print second line
    len = strlen(lines[1]);
    padding = (width - len) / 2;
    if(padding < 0) padding = 0;
    printf("%*s%s\n", padding, "", lines[1]);

    // Print third line with current time
    char line_with_time[256];
    snprintf(line_with_time, sizeof(line_with_time), "|       %s       |", time_str);
    len = strlen(line_with_time);
    padding = (width - len) / 2;
    if(padding < 0) padding = 0;
    printf("%*s%s\n", padding, "", line_with_time);

    // Print fourth line
    len = strlen(lines[3]);
    padding = (width - len) / 2;
    if(padding < 0) padding = 0;
    printf("%*s%s\n", padding, "", lines[3]);

    printf("\033[0m\n"); // Reset color
}

// SIGCHLD handler to show done jobs
void sigchld_handler(int sig) {
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("\n");
        print_done_job(pid);
        // Reprint prompt after job done
        char cwd[MAX_INPUT];
        time_t t = time(NULL);
        struct tm tm_info = *localtime(&t);
        char time_str[64];
        strftime(time_str, sizeof(time_str), "%H:%M:%S", &tm_info);

        if(getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("\033[36m%s\033[33m@%s DARK_T> \033[0m", cwd, time_str);
        } else {
            printf("\033[33mDARK_T> \033[0m");
        }
        fflush(stdout);
    }
}

int main() {
    signal(SIGCHLD, sigchld_handler);

    // Initial banner
    print_banner();

    char input[MAX_INPUT];
    char *args[MAX_ARG];

    while (1) {
        // Print prompt with current time
        char cwd[MAX_INPUT];
        time_t t = time(NULL);
        struct tm tm_info = *localtime(&t);
        char time_str[64];
        strftime(time_str, sizeof(time_str), "%H:%M:%S", &tm_info);

        if(getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("\033[36m%s\033[33m@%s DARK_T> \033[0m", cwd, time_str);
        } else {
            printf("\033[33mDARK_T> \033[0m");
        }
        fflush(stdout);

        if(fgets(input, sizeof(input), stdin) == NULL) {
            if(feof(stdin)) {
                printf("\nExiting shell (EOF).\n");
                break;
            } else {
                perror("fgets failed");
                continue;
            }
        }

        input[strcspn(input, "\n")] = 0;

        // Tokenize input
        int i = 0;
        char *token = strtok(input, " \t\n");
        while(token != NULL && i < MAX_ARG) {
            args[i++] = token;
            token = strtok(NULL, " \t\n");
        }
        args[i] = NULL;

        if(i == 0) continue; // empty input

        // Built-in commands
        if(strcmp(args[0], "exit") == 0) {
            printf("Have a nice day!\n");
            break;
        } else if(strcmp(args[0], "clear") == 0) {
            system("clear");
            print_banner();
            continue;
        } else if(strcmp(args[0], "help") == 0 || strcmp(args[0], ".h") == 0) {
            show_help();
            continue;
        } else if(strcmp(args[0], "cd") == 0) {
            if(args[1] == NULL) {
                perror("cd: Missing argument");
            } else if(chdir(args[1]) != 0) {
                perror("cd failed");
            }
            continue;
        }

        // Background process check
        int background = 0;
        if(i > 0 && strcmp(args[i-1], "&") == 0) {
            background = 1;
            args[i-1] = NULL;
        }

        pid_t pid = fork();

        if(pid == 0) {
            execvp(args[0], args);
            perror("execvp failed");
            exit(EXIT_FAILURE);
        } else if(pid > 0) {
            if(!background) {
                int status;
                waitpid(pid, &status, 0);
            } else {
                char full_cmd[1024] = "";
                for(int j = 0; j < i && args[j] != NULL; j++) {
                    strncat(full_cmd, args[j], sizeof(full_cmd) - strlen(full_cmd) - 1);
                    if(j < i - 1) strncat(full_cmd, " ", sizeof(full_cmd) - strlen(full_cmd) - 1);
                }
                add_bg_job(pid, full_cmd);
                printf("\n\033[32m[%d] %d running in background\033[0m\n", job_count, pid);
                fflush(stdout);
            }
        } else {
            perror("fork failed");
        }
    }

    return 0;
}
