#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h> 

int main(int argc, char* argv[])
{
    int pid,fd;
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
    pid = fork();
    
  if(pid>0)   //parent 
  {
    printf("SZÜLŐ\tSzülő vagyok, irok a csobe!\n");
    printf("SZÜLŐ\tCsonyitas eredmenye: %d!\n",fid);
          fd=open("fifo.ftc",O_WRONLY);
          write(fd,"Hajra Fradi!\n",12);
          close(fd);
    printf("SZÜLŐ\tSzülő vagyok, beirtam, vegeztem!\n");
  }
  else // child
  {
    char s[1024]="Semmi";   
    printf("GYEREK\tCsonyitas eredmenye gyerekben: %d!\n",fid);
    fd=open("fifo.ftc",O_RDONLY);
    read(fd,s,sizeof(s));
    printf("GYEREK\tEzt olvastam a csobol: %s \n",s);
    close(fd);
    // remove fifo.ftc
    unlink("fifo.ftc");
  }   
  return 0; 
}