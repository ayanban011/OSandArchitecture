#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
 int main() {

  pid_t process;
  int count= 0;

  switch(process= fork()) {

    case -1:
      printf("Fork error!\n\n");
      exit(1);
    break;

    case 0: //child
      printf("Process CHILD: PID= %d, Value= %d \n", getpid(), process);
      printf("Coounter NOT increased: %d\n", count);
      printf("Increase counter...\n");
      sleep(2);
      count= count + 2;
      printf("Counter increased: %d\n\n", count);
      exit(0);
    break;

    default: //parent
      wait(0);
      printf("Process PARENT: PID= %d, Value= %d\n", getpid(), process);
      printf("Counter value: %d\n\n", count);
    break;
  }
  return 0;
}


