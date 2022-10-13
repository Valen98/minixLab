#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/resource.h>

int main(int argc, char const *argv[])
{
  int i;
  int j;
  int NUMBER_OF_PROCESSES = 5;
  char *args[] = { NULL };
  i = 0;
  j=130;
  printf(">> Creating child processes...\n\n");
  
  while(i < NUMBER_OF_PROCESSES) {
    if (fork() == 0) {
      printf("Creating process [%d]\n", i);
      setdl(j);
      j = j-20;
      exit(0);
    }
    i++;
  }

  i = 0;
  while(i < NUMBER_OF_PROCESSES) {
    wait(NULL);
    i++;
  }

  printf("finished\n\n");

  return 0;
}