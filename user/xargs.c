#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
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
    int argsc = argc;

    for (int i = 0; i < argc; i++) {
        args[i] = argv[i + 1];
    }

    while (read(0, &temp, 1) == 1) {
        if (temp == ' ') {
            args[argsc++ - 1] = buffer;
            memset(buffer, 0, sizeof(buffer));
            ptr = buffer;
            continue;
        }
        if (temp == '\n') {
            args[argsc++ - 1] = buffer;
            int pid = fork();
            if (pid > 0) {
                wait(0);
                memset(buffer, 0, sizeof(buffer));
                ptr = buffer;
                argsc = argc;
                for (int i = argc; i < MAXARG; i++) {
                    args[i] = 0;
                }
                continue;
            } else {
                exec(args[0], args);
            }
        }
        *ptr = temp;
        ptr++;
    }

    return 0;
}