#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/resource.h>

void create_processes(void) 
{
  int count = 0;

  printf("c programmering är för horungar\n");
  
  while(count < 10) 
  {
    pid_t pid;
    pid = fork();

    printf("jag hatar mitt liv\n");

    if(pid == 0) /* Child */
    {
      printf("Child \n");
      execv("/usr/b21leowa/documents/deadline/deadline", NULL);
      exit(0);
    } 
    else
    {
      /* Parent */
      printf("Parent\n\n");
      wait(NULL);
      printf("kuk fitta hora");
    }

    printf("=========================================\n\n\n");

    /* Increment count */
    count++;
  }
}

int main (int argc, char ** argv)
{
  create_processes();
  return 0;
}