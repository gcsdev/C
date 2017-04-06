/*
Implementação utilizando execve
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>


void childProcess(pid_t pid);
void parentProcess(pid_t pid);               

void  main(void)
{
	 char *argv[] = { "/bin/sh", "-c", "ls", 0 };
     char *envp[] =
    {
        "HOME=/",
        "PATH=/bin:/usr/bin",
        "TZ=UTC0",
        0
    };

	 pid_t pid = fork();
     if (pid == 0){ 
          childProcess(pid);
      	  int number = execve(argv[0],&argv[0],envp);
     }else{ 
     	  parentProcess(pid);
     	}
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