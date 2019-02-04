#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber){
  //printf("Signal with number %i has arrived\n",signumber);
  printf("Kopogás %i has arrived\n",signumber);
}

int main(){

  sigset_t sigset;
  sigemptyset(&sigset);
  sigaddset(&sigset,SIGTERM);
  sigprocmask(SIG_BLOCK,&sigset,NULL); 
    
  signal(SIGTERM,handler); //signal and handler is connetcted
  signal(SIGUSR1,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP),
                           //handler = SIG_DFL - back to default behavior
 
  pid_t banya, child;
  child=fork();
  if (child>0)
  {
    pause(); //waits till a signal arrive SIGTERM is blocked, so it gets SIGUSR1
    sigprocmask(SIG_UNBLOCK,&sigset,NULL);
    
    int status;
    wait(&status);
    printf("Parent process ended\n");
  }
  else
  {
    printf("Waits 3 seconds, then send a SIGTERM %i signal (it is blocked)\n",SIGTERM);
    sleep(3);
    kill(getppid(),SIGTERM);
    printf("Waits 3 seconds, then send a SIGUSR1 %i signal (it is not blocked)\n", SIGUSR1);
    sleep(3);
    kill(getppid(),SIGUSR1);
    printf("Child process ended\n");  
  }
  return 0;
}