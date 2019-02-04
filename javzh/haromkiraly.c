#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [1024]; 
}; 

// sendig a message
int kuld( int uzenetsor ) 
{ 
     const struct uzenet uz = { 5, "Hajra Fradi!" }; 
     int status; 
     
     status = msgsnd(uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
      // a 3. param ilyen is lehet: sizeof(uz.mtext)
      // a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// receiving a message. 
int fogad( int uzenetsor ) 
{ 
     struct uzenet uz; 
     int status; 
      // az utolso parameter(0) az uzenet azonositoszama
      // ha az 0, akkor a sor elso uzenetet vesszuk ki
      // ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
      // vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, &uz, 1025, 5, 0 ); 
     
     /*if ( status < 0 ) 
        perror("msgget"); 
     else*/
      printf("A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
     return 0; 
} 


int main(int argc, char* argv[]){
  int uzenetsor, status; 
  key_t kulcs; 
  kulcs = ftok(argv[0],1); 
  printf ("A kulcs: %d\n",kulcs);
  uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 

  const struct uzenet uz = { 5, "Hajra Fradi!" };

  if ( uzenetsor < 0 ) { 
    perror("msgget"); 
    return 1; 
  }  

  pid_t child3;
  pid_t child2;
  pid_t child=fork();
  if (child>0)
  {
    child2 = fork();
    if(child2>0){
      child3 = fork();
      if(child3>0){
        wait(NULL);
        printf("Szülő vagyok várok\n");
        return fogad( uzenetsor ); 
      }
      else
      {
        printf("Gyerek3 vagyok küldök\n");  
      }
    }
    else{
      printf("Gyerek2 vagyok küldök\n");
    }
  }
  else
  {
    printf("Gyerek1 vagyok.\n");
    kuld(uzenetsor);
  
    status = msgctl(uzenetsor, IPC_RMID, NULL ); 
    if ( status < 0 ) 
      perror("msgctl");
    return 0; 
  }
  return 0;
}