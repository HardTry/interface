#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
//#include <sys/ipc.h>
//#include <sys/shm.h>
#include <linux/shm.h>

int main(void) {
  //int id = shmget((key_t)1234, 1024, IPC_CREAT|0666);
  //printf("%d\n", id);
  printf("%u %d\n", SHMMAX, SHMMIN);
  return 0;
}

