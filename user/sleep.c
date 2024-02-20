#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("The number of arguments does not match\n");
        exit(0);
    }
    int time = atoi(argv[1]);
    if (time <= 0) {
        printf("Sleep time must be greater than 0\n");
        exit(0);
    }
    sleep(time);
    exit(0);
}