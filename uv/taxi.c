#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h> 

int main(int argc, char **argv){
  //int fd[2];
  //pipe(fd);

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


  pid_t child2;
  pid_t child=fork();
  if (child>0)
  {
    child2 = fork();
    if(child2>0){
      wait(NULL);
     // wait(NULL);
      printf("KÖZPONT\tSzülő vagyok, várok...\n");

      /*char buf;
      int bytesread;
      while ((bytesread = read(fd[0], &buf, 1)) > 0) {
          putchar(buf);
          if (buf == '!') break; // End of message
      }*/

      char s[1024]="Semmi";   
      fd=open("fifo.ftc",O_RDONLY);
      read(fd,s,sizeof(s));
      printf("KÖZPONT\tEzt a címet kaptam: %s \n",s);
      close(fd);
      // remove fifo.ftc
      unlink("fifo.ftc");
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
    //printf("TAXI\tGyerek1 vagyok.\n");
  }
  return 0;
}