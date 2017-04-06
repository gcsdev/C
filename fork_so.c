/*
Implementação utilizando fork - fork 1
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>

void childProcess(pid_t pid);
void parentProcess(pid_t pid);               

void  main(void)
{
     pid_t pid = fork();
     if (pid == 0) 
          childProcess(pid);
     else 
     	  parentProcess(pid);
}

void childProcess(pid_t pid)
{
     printf("PID from child, value = %d\n", pid);
}
void  parentProcess(pid_t pid)
{
	 wait(NULL);
     printf("PID is from parent, value = %d\n", pid);
}

