/*
Implementação - Chaveamento Obrigatório
*/

#include <pthread.h>                                                            
#include <stdio.h>
#include <stdlib.h>                                                              
#include <unistd.h>  

void *__tryAccess(void *args);
void __critical_region(int id);
void __noncritical_region(int id);
int _turn =0;
int _count=0;

int main(){	
	pthread_t _thread;
	int _i=0;
	for (int _i = 0; _i < 2; ++_i){
		if(pthread_create(&_thread,NULL,__tryAccess,(void *)_i)!=0){
			perror("pthread_create() error");      
    		exit(1);
		}
	}
	pthread_exit(NULL);
	exit(0);
}

void *__tryAccess(void *id){
	int _id = (int)id;
if(id==0){
	while(1){
		while(_turn!=0)
			__critical_region(_id);
		_turn=1;
		__noncritical_region(_id);
	}
}else{
	while(1){
		while(_turn!=1);
			__critical_region(_id);
		_turn=0;
		__noncritical_region(_id);
	}
}
}

void __critical_region(int id){
	printf("id: %d - Critical Region!\n",id);
	__sumCount();
	sleep(3);
}

void __noncritical_region(int id){
	printf("id: %d - Non Critical Region!\n",id);
	sleep(3);
}
void __sumCount(){
		(_count)++;
		printf("Count: %d\n",_count);
		pthread_yield(NULL); 
}




