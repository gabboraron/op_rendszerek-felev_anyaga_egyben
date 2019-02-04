#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <string.h> 


  struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
  }; 

  // sendig a message
  int kuld( int uzenetsor, char *cim ) 
  { 
       const struct uzenet uz = { 5, cim }; 
       int status; 
       
       status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
       if ( status < 0 ) 
          perror("msgsnd"); 
       return 0; 
  } 
       
  // receiving a message. 
  int fogad( int uzenetsor ) 
  { 
       struct uzenet uz; 
       int status; 
       status = msgrcv(uzenetsor, &uz, 1024, 5, 0 );        
       if ( status < 0 ) 
            perror("msgsnd"); 
       else
        printf("TAXI\tA kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
       return uz.mtype; 
  }

int main(int argc, char **argv){
  //int fd[2];
  //pipe(fd);

  /* CSÖVEK */
  int fd;
  printf("Fifo start!\n");
  int fid=mkfifo("fifo.ftc", S_IRUSR|S_IWUSR ); // creating named pipe file
  // S_IWGRP, S_IROTH (other jog), file permission mode
  // the file name: fifo.ftc
  // the real fifo.ftc permission is: mode & ~umask 
  if (fid==-1)  //error handling
  {
    printf("Error number: %i",errno);
    exit(EXIT_FAILURE);
  }
  printf("Mkfifo vege, fork indul!\n\n");

  /*ÜZENETSOR*/ 
  int uzenetsor, status; 
  key_t kulcs; 
  kulcs = ftok(argv[0],1); 
  printf ("A kulcs: %d\n",kulcs);
  uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
  if ( uzenetsor < 0 ) { 
    perror("msgget"); 
    return 1; 
  }

  /*MAIN*/
  pid_t child2;
  pid_t child=fork();
  if (child>0)
  {
    child2 = fork();
    if(child2>0){
     // wait(NULL);
     // wait(NULL);
      printf("KÖZPONT\tSzülő vagyok, várok...\n");

      /*char buf;
      int bytesread;
      while ((bytesread = read(fd[0], &buf, 1)) > 0) {
          putchar(buf);
          if (buf == '!') break; // End of message
      }*/
/*A*/
      char s[1024]="Semmi";   
      fd=open("fifo.ftc",O_RDONLY);
      read(fd,s,sizeof(s));
      printf("KÖZPONT\tEzt a címet kaptam: %s \n",s);
      close(fd);
      // remove fifo.ftc
      unlink("fifo.ftc");

/*B*/
      kuld( uzenetsor, s );
      wait( NULL );
      status = msgctl( uzenetsor, IPC_RMID, NULL ); 
      if ( status < 0 ) 
        perror("msgctl"); 
      return 0; 
    }
    else{
      printf("UTAS\tGyerek2 vagyok, küldök\n");

      fd=open("fifo.ftc",O_WRONLY);
      write(fd,argv[1],sizeof(argv[1]));
      close(fd);

      //int idx = 0;
    //  write(fd[1], argv[1],sizeof(argv[1]));
      //for (idx = 0; idx < argc; ++idx)
      //{
        //printf("argv[%d]: %s\n", idx, argv[idx]);
    //    printf("UTAS\tEz a lakcímem: %s\n", argv[1]);
      //}
    }
  }
  else
  {
    printf("TAXI\tGyerek1 vagyok, várok...\n");
    int tel =  fogad( uzenetsor ); 
  }
  return 0;
}