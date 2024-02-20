#include "kernel/types.h"
#include "user/user.h"
int p1[2], p2[2];
char *argv[2];

int
main(void)
{
    
    argv[0] = "pingpong";
    argv[1] = 0;
    pipe(p1);
    pipe(p2);
    if (fork() == 0)
    {
        close(p1[1]);
        char byte;
        read(p1[0], &byte, 1);
        fprintf(2, "%d: received ping\n", getpid());
        close(p1[0]);
        close(p2[0]);
        write(p2[1], &byte, 1);
        close(p2[1]);
        exit(0);
    }
    else
    {
        close(p1[0]);
        char byte = 'o';
        write(p1[1], &byte, 1);
        close(p1[1]);
        wait(0);
        fprintf(2, "%d: received pong\n", getpid());
        close(p2[1]);
        read(p2[0], &byte, 1);
        close(p2[0]);
        exit(0);
    }
}