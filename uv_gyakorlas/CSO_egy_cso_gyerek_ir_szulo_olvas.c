#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  int fd[2];
  pipe(fd);
  // You must read from fd[0] and write from fd[1]
  printf("INFO:\tReading from %d, writing to %d\n", fd[0], fd[1]);

  pid_t p = fork();
  if (p > 0) {
      char buf;
      int bytesread;
      /* I have a child therefore I am the parent*/
      wait(NULL);
      printf("PARR:\tI'm the parrent!\n");
      
      // read one byte at a time => read unknown length 'string'
      /*while ((bytesread = read(fd[0], &buf, 1)) > 0) {
          putchar(buf);
          if (buf == '!') break; // End of message
      }*/

      //read knowed length 'string'
      read(fd[0], &buf, 9);
      printf("buf: %s\n", &buf);

      /*don't forget your child*/      
  } else {      
      printf("CHILD:\tI'm the child!\n");
      write(fd[1],"Hi mom!",9);
  }
  return 0;
}