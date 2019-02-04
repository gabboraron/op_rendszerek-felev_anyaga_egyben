#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

int main(){

    
  signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                           // SIGTERM erkezesere felvesszuk a handler nevu fuggvenyt
                           // 
  signal(SIGUSR1,handler); //handler = SIG_DFL - back to default behavior 
  
  pid_t child=fork();  

  if (child>0) //szulo folyamat
  { 
    pause(); //waits till a signal arrive 
    printf("Signal arrived\n",SIGTERM);
    int status;   //
    wait(&status);//varjuk hogy befejezodjon a foylamat
    printf("Parent process ended\n");
    
    //uj gyerek
    pid_t child = fork();
    if (child>0) 
    {
      pause();
      printf("child 2 arrived\n",
      printf("parrent 2 ended\n");    
  }
  else //gyerek folyamat
  {
    printf("Waits 3 seconds, then send a SIGTERM %i signal\n",SIGTERM);
    sleep(3);
    kill(getppid(),SIGTERM); 
    //1. parameter the pid number of process, we send the signal
    // 		if -1, then eacho of the processes of the same uid get the signal
    // 		we kill our bash as well! The connection will close
    //2. parameter the name or number of signal
    printf("Child process ended\n");  
  }
  return 0;
}