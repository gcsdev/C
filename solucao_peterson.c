/*
Implementação da Solução de Peterson
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define FALSE 0
#define TRUE 1
#define N 2

void __enter_region(int process);
void __leave_region(int process);
__critical_region(void *id);
int _turn=0;
int _interested[N];
int _count=0;

int main(){
	pthread_t _thread;
	
	for (int _i = 0; _i < 2; ++_i)
	{
		if(pthread_create(&_thread,NULL,__critical_region,(void *)_i)!=0){
			perror("pthread_create() error");      
    		exit(1);
		}
	}
	pthread_exit(NULL);
	exit(0);
}

void __enter_region(int process){
	int _other;
	_other=1-process;
	_interested[process]=TRUE;
	_turn=process;
	if(_turn==process && _interested[_other]==FALSE){
		puts("Entrou região crítica");
		__sumCount();
	}
	while(_turn=process && _interested[_other]==TRUE){}
}

void __leave_region(int process){
	_interested[process]=FALSE;
	puts("Processo deixou a região crítica");
	
}


int __critical_region(void *id){
	int _id=(int)id;
	while(TRUE){
		__enter_region(_id);
		__leave_region(_id);	
		sleep(3);
	}
}
void __sumCount(){
		(_count)++;
		printf("Count: %d\n",_count);
		pthread_yield(NULL); 
}
