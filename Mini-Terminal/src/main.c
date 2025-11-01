#include <stdio.h>
#include <string.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


#define MAX_INPUT 1024
#define MAX_ARG 64
int main(){
    char input[MAX_INPUT];
    char *input_result; // To catch the return value of fgets
    char *args[MAX_ARG];

    while (1)
    {    
        char cwd[MAX_INPUT];

        if(getcwd(cwd, sizeof(cwd)) != NULL){
            printf("%s KAR_T> ", cwd);
        } else {
            perror("getcwd failed");
            printf("KAR_T> ");
        }
        fflush(stdout); // Ensures prompt displays immediately
        
        // Use input_result to store the return value of fgets
        input_result = fgets(input, sizeof(input), stdin);

        // 1. CHECK FOR ERROR/EOF (Ctrl+D)
        if (input_result == NULL) {
            if (feof(stdin)) {
                printf("\nExiting shell (EOF).\n");
                break;
            } else {
                perror("fgets failed");
                continue;
            }
        }
        
        // 2. REMOVE NEWLINE CHARACTER
        // fgets adds a '\n', which must be removed for clean processing (and strcmp)
        input[strcspn(input, "\n")] = 0; 

        // 3. HANDLE BUILT-IN COMMANDS (e.g., exit)
        if(strcmp(input, "exit") == 0){ 
            printf("Have a nice day!\n");
            break; // Exit the while loop
        }
        
        // 4. ECHO the input (for debugging)
       
        // 5. creating Token

        char *token = strtok(input, " \t\n");
        int i=0;

        while (token!=NULL && i<MAX_ARG)
        {
            args[i++]=token;
            token=strtok(NULL," \t\n");
        }

        args[i]=NULL;

        if(strcmp(args[0],"cd")==0){
            if(args[1]==NULL){
                perror("cd: Missing argument");
            }
            else if (chdir(args[1])!=0)
            {
                perror("cd failed");
            }
            continue;
        }

        pid_t pid=fork();

        if (pid == 0) {
            execvp(args[0],args);
            perror("execvp failed");
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            int status;
            waitpid(pid,&status,0);
            // printf("Exist status: %d\n",WEXITSTATUS(status));
        } else {
            perror("fork failed");
        }
        
        // if (strlen(input) > 0) {
        //     printf("You entered: [%s]\n", input);
        // }
    }
    
    return 0;
}