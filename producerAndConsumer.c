// Implementação Produtor e Consumidor com POSIX MESSAGE QUEUE
//gcc -o producer producerAndConsumer.c -lrt


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <sys/wait.h>

//simple message
struct message {
	char msgText [200];
};

int sendMessage(int qid, int pri, const char text[]){
	int send = mq_send(qid,text,strlen(text)+1,pri);
	if(send==-1){
		perror("mq_send");
	} 	
	return send;
}

void producer (mqd_t qid){
	//priority message - low
	sendMessage(qid,1,"This is my first message");	
	sendMessage(qid,1,"This is my second message");
	//priority message - hight
	sendMessage(qid,3,"Last message");
}

void consumer (mqd_t qid){
	
	struct mq_attr mattr;
	//producer is finished at this point
	do{
		u_int pri;
		struct message msg;
		ssize_t lenght;

		lenght=mq_receive(qid,(char *)&msg, sizeof(msg), &pri);
		if(lenght==-1){
			perror("mq_receive");
			break;
		}
		printf("got pri %d '%s' lenght=%d\n", pri, msg.msgText, lenght);
		// Check for more messages in the queue.
		int r = mq_getattr(qid, &mattr);
		if (r == -1) {
			perror("mq_getattr");
			break;
		}
	} while (mattr.mq_curmsgs); // Stop when no more messages
}

int main(int argc, char *argv[]){
	// Allow up to 10 messages before blocking.
	// Message size is 200 bytes (see above).
	struct mq_attr mattr = {
		.mq_maxmsg = 10,
		.mq_msgsize = sizeof(struct message)
	};
	mqd_t mqid = mq_open("/myq",
						O_CREAT | O_RDWR,
						S_IREAD | S_IWRITE,
						&mattr);

	if (mqid == (mqd_t) -1) {
		perror("mq_open");
		exit(1);
	}
	// Fork a producer process, we'll be the consumer.
	pid_t pid = fork();
	if (pid == 0) {
		producer(mqid);
		mq_close(mqid);
		exit(0);
	}else {
	// Wait for the producer to send all messages so
		int status;
		wait(&status);
		consumer(mqid);
		mq_close(mqid);
	}
	mq_unlink("/myq");
	return 0;
}
