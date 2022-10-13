#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/resource.h>

int main(int argc, char const *argv[])
{
  int i = 0;
  int NUMBER_OF_PROCESSES = 15;
  char *args[] = { NULL };
  printf(">> Creating child processes...\n\n");
  
  while(i < NUMBER_OF_PROCESSES) {
    if (fork() == 0) {
      printf("Creating process [%d]\n", i);
      execv("/usr/b21leowa/documents/deadline/deadline", args);
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