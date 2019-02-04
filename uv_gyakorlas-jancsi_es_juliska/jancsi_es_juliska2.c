#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

void handler(int signumber){
  printf("Handled signal nr: %i \n",signumber);
}

int main(){
  /*random number */
  srand(time(NULL)); //random number
  int r = rand() % 2; //random number between 0 and 1 
  char original1[7]="rdnr: ";
  original1[6]=r+'0';
  r = rand() % 2; 
  char original2[7]="rdnr: ";
  original2[6]=r+'0';

  /*pipe, named pipe*/
    int pid,fd,fd2;
    printf("Fifo start!\n");
    int fid=mkfifo("fifo.ftc", S_IRUSR|S_IWUSR ); // creating named pipe file
    int fid2=mkfifo("fifo2.ftc", S_IRUSR|S_IWUSR );

    if (fid==-1)  //error handling
    {
      printf("Error at fid number: %i",errno);
      exit(EXIT_FAILURE);
    }
    if (fid2==-1)  //error handling
    {
      printf("Error at fid2 number: %i",errno);
      exit(EXIT_FAILURE);
    }
    printf("Pipe system OK!\n\n");

  /*signal*/
  sigset_t sigset;
  sigemptyset(&sigset); //empty signal set
  sigaddset(&sigset,SIGTERM); //SIGTERM is in set
  //sigfillset(&sigset); //each signal is in the set
  sigprocmask(SIG_BLOCK,&sigset,NULL); //signals in sigset will be blocked
               //parameters, how: SIG_BLOCK, SIG_UNBLOCK, SIG_SETMASK -   ;
               //2. parameter changes the signalset to this if it is not NULL,
               //3.parameter if it is not NULL, the formerly used set is stored here
    
  signal(SIGTERM,handler); //signal and handler is connetcted
  signal(SIGUSR1,handler); 

  pid_t child2;
  pid_t child=fork();
  if (child>0)
  {
    child2 = fork();
    if(child2>0){
      //wait(NULL);
      //wait(NULL);
      printf("Parent, wainting for signal...\n");

      sigsuspend(&sigset);
      sigprocmask(SIG_UNBLOCK,&sigset,NULL);
      
      /*int status;
      wait(&status);*/
/*
      int sigcounter = 0;
      if(sigwait(&sigset) == SIGUSR1) {
        ++sigcounter;
      }
      if(sigwait(&sigset) == SIGTERM) {
        ++sigcounter;
      }*/
      printf("Parent got 2 signal!\n\n");
      
      //if(sigcounter == 2){
      printf("Parent will send 2 random number: %s and %s\n", original1, original2);
      fd=open("fifo.ftc",O_WRONLY);
          write(fd,original1,12);
          close(fd);
      fd2=open("fifo2.ftc",O_WRONLY);
          write(fd2,original2,12);
          close(fd2);
      printf("Parent has sent the numbers!\n");
    //}
    }
    else{
      printf("I'm child2.\n");
      printf("child2 signalnr: %i (it is not blocked)\n", SIGUSR1);
      kill(getppid(),SIGUSR1);
      
      /*get pipe*/
      sleep(5);
      char s[1024]="nothn";  
     
      printf("got on pipe, in child2: %d!\n",fid);
      fd=open("fifo.ftc",O_RDONLY);
      read(fd,s,sizeof(s));
      printf("got this on pipe, by child2: %s \n",s);
      close(fd);
      // remove fifo.ftc
      unlink("fifo.ftc");
    }
  }
  else
  {
    printf("I'm child1.\n");
    printf("child1 signal nr: %i (it is blocked)\n",SIGTERM);
    sleep(3);
    kill(getppid(),SIGTERM);

    //sleep(5);
    /*get pipe*/
    char s[1024]="nothn";
    printf("Got this on pipe, by child1: %d!\n",fid2);
    fd2=open("fifo2.ftc",O_RDONLY);
    read(fd2,s,sizeof(s));

    printf("got this in pipe fd2: %s by child1 \n",s);
    close(fd2);
    // remove fifo2.ftc
    unlink("fifo2.ftc");
  }
  return 0;
}