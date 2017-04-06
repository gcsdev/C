/*
Implementação utilizando fork 2
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

void main (void){
	int fd =open("./file1",O_RDONLY);
    if(fd==-1){
  	   perror("Open");
  	   exit(-1);		  
  	}
 	char buffer;
 	int count=1;
 	pid_t pid =fork();
 	if(pid==0){
 		pid_t pid1 =fork();
 		//chlid's child proccess
 		if(pid1==0){
 	  		printf("PID do Neto %d\n", getpid());
 	  		printf("FORK do Neto %d\n",pid1);
 			read(fd,&buffer,count);
 	        printf("Read:%c\n",buffer);
 		//child process	
 		}else{
 			wait(NULL);
 			printf("PID do Filho %d\n", getpid());
 	  		printf("FORK do Filho %d\n",pid1);
 			read(fd,&buffer,count);
 	        printf("Read:%c\n",buffer);
 		}
 	//father proccess
 	}else{
	  wait(NULL);
 	  printf("PID do Pai %d\n", getpid());
 	  printf("FORK do pai %d\n",pid);
 	  read(fd,&buffer,count);
 	  printf("Read:%c\n",buffer);
	}
}
