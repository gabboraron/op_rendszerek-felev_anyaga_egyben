#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h> 

int main(int argc, char* argv[])
{
    int pid,fd,fd2;
    printf("Fifo start!\n");
    int fid=mkfifo("fifo.ftc", S_IRUSR|S_IWUSR ); // creating named pipe file
    int fid2=mkfifo("fifo2.ftc", S_IRUSR|S_IWUSR ); // creating named pipe file
    // S_IWGRP, S_IROTH (other jog), file permission mode
    // the file name: fifo.ftc
    // the real fifo.ftc permission is: mode & ~umask 
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


    printf("Mkfifo vege, fork indul!\n");
    pid = fork();
    
  if(pid>0)   //parent 
  {
    char s[1024]="Semmi";   
    printf("Csonyitas eredmenye szuloben: %d!\n",fid);
    fd=open("fifo.ftc",O_RDONLY);
    read(fd,s,sizeof(s));
    printf("Ezt olvastam a csobol: %s \n",s);
    close(fd);
    // remove fifo.ftc
    unlink("fifo.ftc");


    printf("Csonyitas2 eredmenye szuloben: %d!\n",fid2);
    fd2=open("fifo2.ftc",O_RDONLY);
    read(fd2,s,sizeof(s));
    printf("Ezt olvastam a csobol: %s \n",s);
    close(fd2);
    // remove fifo2.ftc
    unlink("fifo2.ftc");

  }
  else // child
  {
    printf("Gyerek vagyok, irok a csobe!\n");
    printf("Csonyitas eredmenye: %d!\n",fid);
          fd=open("fifo.ftc",O_WRONLY);
          write(fd,"Hajra Fradi!\n",12);
          close(fd);
    printf("Gyerek vagyok, beirtam, az 1.-t!\n");


    printf("Gyerek vagyok, irok a 2. csobe!\n");
    printf("Csonyitas eredmenye: %d!\n",fid2);
          fd2=open("fifo2.ftc",O_WRONLY);
          write(fd2,"Hajra VASAS!\n",12);
          close(fd2);
    printf("Gyerek vagyok, beirtam a 2.at, vegeztem!\n");
  } 
    
    return 0; 
}