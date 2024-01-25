#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if (argc != 2){
    fprintf(2, "Invalid! There is no ticks argument for sleep! \n");
    exit(0);
  }
  int ticks = atoi(argv[1]);
  if (ticks <= 0){
    fprintf(2, "Invalid! The number of ticks must be positive! \n");
    exit(0);
  }
  sleep(ticks);
  exit(0);
}