/*
Implementação simples de uso de Threads
*/

#include <pthread.h>                                                            
#include <stdio.h>
#include <stdlib.h>                                                              
#include <unistd.h>    
int _count=0;

void *__sumCount(void *_count){
		int _countLocal=*(int*)_count;
		_countLocal++;
		printf("Count: %d\n",_countLocal);
		pthread_yield(NULL); 
		*(int *)_count=_countLocal;
}

int main(){
	pthread_t _thread;
	int i=0;	
	while(i<100){
	if(pthread_create(&_thread,NULL,__sumCount,&_count)!=0){
		perror("pthread_create() error");      
    	exit(1);    
	}
	i++;
}
	sleep(1);
	pthread_exit(NULL);
	exit(0);
}
	
