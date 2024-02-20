#include "kernel/types.h"
#include "user/user.h"
void process(int p, int rd, int wd){
    fprintf(2, "%d\n", p);
    while(1){
        int num;
        if (read(rd, &num, sizeof(int)) <= 0){
            close(rd);
            return;
        }
        if (num % p != 0){
            write(wd, &num, sizeof(int));
        }
    }
}
int p[2];
char *argv[2];

int
main(void){
    argv[0] = "primes";
    argv[1] = 0;
    pipe(p);
    if (fork() == 0){
        close(p[0]);
        for (int i = 2; i <= 35; i ++){
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);
        exit(0);
    }
    else{
        wait(0);
        while(1){
            int p2[2];
            pipe(p2);
            close(p[1]);
            int prime;
            int n = read(p[0], &prime, sizeof(int));
            if (n <= 0){
                close(p[0]);
                exit(0);
            }
            process(prime, p[0], p2[1]);
            close(p[0]);
            p[0] = p2[0];
            p[1] = p2[1];
            
        }
    }
    exit(0);
}