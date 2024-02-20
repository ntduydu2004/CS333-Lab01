#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define BUFFER_SIZE 1024

int main(int argc, char*argv[]) {
    if (argc < 2) {
        printf("Usage: xargs command\n");
        exit(1);
    }

    // Temp variable to store the input one by one character
    char temp;
    // Buffer to store the input
    char buffer[BUFFER_SIZE];
    // Array to store the arguments
    char* args[MAXARG];
    // Pointer to the buffer
    char* ptr = buffer;

    // Read the input from the standard input
    // Read one character at a time
    // If return 1, then read successfully
    // If return 0, then no more input
    while (read(0, &temp, 1) == 1) {
        // End of the input when newline is read
        if (temp == '\n') {
            // Copies the command-line arguments
            for (int i = 0; i < argc; i++) {
                args[i] = argv[i + 1];
            }
            // Null-terminate the arguments
            args[argc - 1] = buffer;
            
            // Print the arguments
            for (int i = 0; i < argc - 1; i++) {
                printf("%s\n", args[i]);
            }
            // Clear the buffer
            memset(buffer, 0, sizeof(buffer));
            // Reset the pointer to the buffer
            ptr = buffer;
            // Fork a new process
            int pid = fork();
            // Parent process
            if (pid > 0) {
                // Wait for the child process to finish
                wait(0);
                // Clear the arguments
                memset(args, 0, sizeof(args));
                // Continue to the next input
                continue;
            } else if (pid == 0) {
                // Child process
                // Execute the command
                exec(args[0], args);
            }
        }
        // Store the input into the buffer
        *ptr = temp;
        // Move the pointer to the next position
        ptr++;
    }
    
    return 0;
}
