#include "kernel/types.h"
#include "user/user.h"
int p[2];
char *argv[2];

int
main(void)
{
    
    argv[0] = "pingpong";
    argv[1] = 0;
    pipe(p);
    if (fork() == 0)
    {
        close(p[1]);
        char byte;
        read(p[0], &byte, 1);
        fprintf(2, "%d: received ping \n", getpid());
        write(p[0], &byte, 1);
        close(p[0]);
        exit(0);
    }
    else
    {
        close(p[0]);
        char byte = 'o';
        write(p[1], &byte, 1);
        wait(0);
        close(p[1]);
        fprintf(2, "%d: received pong \n", getpid());
        read(p[1], &byte, 1);
        exit(0);
    }
}