#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

int main(){

  sigset_t sigset;
  sigemptyset(&sigset); //empty signal set
  sigaddset(&sigset,SIGTERM); //SIGTERM is in set
  //sigfillset(&sigset); //each signal is in the set
  sigprocmask(SIG_BLOCK,&sigset,NULL); //signals in sigset will be blocked
               //parameters, how: SIG_BLOCK, SIG_UNBLOCK, SIG_SETMASK -   ;
               //2. parameter changes the signalset to this if it is not NULL,
               //3.parameter if it is not NULL, the formerly used set is stored here
    
  signal(SIGTERM,handler); //signal and handler is connetcted
  //signal(SIGUSR1,handler); 

  pid_t child=fork();
  if (child>0)
  {
    wait();
    printf("Szülő vagyok signalt várok\n");

    sigprocmask(SIG_UNBLOCK,&sigset,NULL); //ha az unblock kimarad akkor nem biztosított a megérkezés sorrendje! 
    int status;
    wait(&status);
    printf("Parent process ended\n");
  }
  else
  {
    printf("Gyerek vagyok.\n");
    printf("Waits 2 seconds, then send a SIGTERM %i signal (it is blocked)\n",SIGTERM);
    sleep(2);
    kill(getppid(),SIGTERM);
  }
  return 0;
}