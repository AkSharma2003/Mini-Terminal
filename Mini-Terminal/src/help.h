#ifndef HELP_H
#define HELP_H
typedef struct {
    char *name;
    char *description;
} command_help;

#define NUM_COMMANDS 20

// Only declaration here
extern command_help commands[NUM_COMMANDS];

void show_help();

#endif
