#include <stdio.h>
#include <string.h> 
#include <stdlib.h> 
#include <unistd.h>

#define MAX_INPUT 1024

int main(){
    char input[MAX_INPUT];
    char *input_result; // To catch the return value of fgets

    while (1)
    {    
        printf("KAR_T>");
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
        if (strlen(input) > 0) {
            printf("You entered: [%s]\n", input);
        }

        // --- Next step: Tokenization and Execution Logic will go here ---
    }
    
    return 0;
}