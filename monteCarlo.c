/*
Implementação do problema de problema MonteCarlo utilizando comunicação inter-processos
gcc monteCarlo.c -o montecarlo -lm -W -Wall
*/

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>

int createPoints(int pointsPerChild);
void calculatePi(int nChild, int nPoints);
void sig_handler(int signo,siginfo_t *si,void *context);

int numberChild=0;

int main(){
  unsigned int nPoints = 0; 
  unsigned int nChild = 0;
  printf("Introduza numero de points a lancar: ");
  scanf("%d", &nPoints);
  printf("Introduza numero filhos: ");
  scanf("%d", &nChild);
  calculatePi(nChild,nPoints);
  return 0;
}

void calculatePi(int nChild, int nPoints){
  struct sigaction sVal;
  int fd[2];
  pipe(fd);
  int pointsIn;
  int pointsPerChild=(nPoints/nChild);
  int totalIn=0;
  double pi;  
  int nAnswers=0;
  int controlChild=1;
  int controlFather=1;
  sVal.sa_flags = SA_SIGINFO;
  sVal.sa_sigaction = sig_handler;
  int answer;      
  sigaction(SIGUSR2,&sVal,NULL);


  for (int i = 0; i < nChild; ++i){
    pid_t pid = fork();
    if(pid==0){
      printf("Child Proccess : %d\n",i+1);
      close(fd[0]);
      pointsIn = (createPoints(pointsPerChild));
      write(fd[1],&pointsIn,(sizeof(pointsIn)));      
      exit(0);

    }else{

      wait(NULL);
      close(fd[1]);
      read(fd[0],&answer,sizeof(answer));
      totalIn=totalIn+answer;
      nAnswers++;
      if(nAnswers==nChild){
         pi=(double)totalIn/(double)nPoints*4.0;
         for (int i = 0; i < nChild; ++i)
         {
            kill(getpid(),SIGUSR2);     
         }
         printf("Pi: %g\n",pi);

       }
    }
  }
}
int createPoints(int pointsPerChild){
   int i=0;
   double x, y;              
   double d;
   unsigned int pointsInPerChild = 0; 
   srand48(getpid());
   for (i = pointsPerChild; i > 0 ; i--) {
      x =drand48();
      printf("x: %.10f  ",x);
      y =drand48();
      printf("y: %.10f  ",y);
      d = sqrt(x*x + y*y);
      printf("Distance: %.10f\n",d);
      if (d <= 1.0) {
          pointsInPerChild++;
      }  
      
    }
    return pointsInPerChild;
}

void sig_handler(int signo,siginfo_t *si,void *context){
  if(signo == SIGUSR2){
       printf("Child: %d - Finalizado \n",numberChild);
       numberChild++;   
  }
}



