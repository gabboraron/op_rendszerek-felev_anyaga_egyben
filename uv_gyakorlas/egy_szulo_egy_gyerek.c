#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

/*int main(){
  pid_t child=fork();
  if (child>0)
  {
    wait();
    printf("Szülő vagyok, gyereket megvárom!\n");
  }
  else{
    printf("Gyerek vagyok \n");
  }
  return 0;
}*/

int main(){
  pid_t child=fork();
  if (child>0)
  {
    printf("Szülő vagyok, én futok hamarabb!\n");
  }
  else{
    wait();
    printf("Gyerek vagyok, szülőt várok! \n");
  }
  return 0;
}