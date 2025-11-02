#include <stdio.h>
#include "help.h"

// Actual definition
command_help commands[NUM_COMMANDS] = {
    {"ls", "List files in the current directory"},
    {"cd", "Change directory"},
    {"exit", "Exit the shell"},
    {"sleep", "Pause execution for specified seconds"},
    {"pwd", "Print working directory"},
    {"touch", "Create an empty file"},
    {"mkdir", "Create a new directory"},
    {"rm", "Delete a file"},
    {"rmdir", "Delete an empty directory"},
    {"help/.h", "Show this help panel"},
    {"cat<file>", "List files & directories"},
    {"grep <pattern> <file>", "Search text in file"},
    {"sleep <n>","Pause for n seconds"},
    {"echo <text>","Print text to console"},
    {"date","Show current date & time"},
    {"whoami","Show current user"},
    {"ps","List running processes"}
};

void show_help() {
    printf("\n\033[35m===== Mini-Terminal Help =====\033[0m\n\n");
    for(int i = 0; i < NUM_COMMANDS; i++) {
        printf("\033[33m%-15s\033[36m \t\t: %s\033[0m\n", commands[i].name, commands[i].description);
    }
    printf("\n\033[35m==============================\033[0m\n\n");
}