#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
#include <sys/wait.h> 
#include <errno.h> 
#include <sys/time.h>

/*
void handler(int sign, siginfo_t* info,void* nonused) {
	printf("%i\n", sign);
}*/

int SIGNUM = 0;

void handler(int sign) {
	printf("Kopog\n");
}

void handler2(int sign) {
	printf("jelzes jancsinak\n");
	SIGNUM++;
}

int main()
{
	int status;
	
	
	int fd[2];
	int fd2[2];
	
	int jp1[2];
	int jp2[2];
	int jp3[2];
	int ujp[2];
	
	pipe(fd);
	pipe(fd2);
	pipe(jp1);
	pipe(jp2);
	pipe(jp3);
	pipe(ujp);
	
	signal(SIGTERM, handler);
	signal(SIGUSR2, handler2);
	
	pid_t pid, pid2;
	
	pid=fork(); 
	if (pid<0){perror("The fork calling was not succesful\n"); exit(1);} 
	if (pid>0) 
	{
		if(pid2=fork()==0)
		{
			close(fd2[0]);
			close(jp1[1]);
			close(jp2[1]);
			close(jp3[1]);
			close(ujp[1]);
			kill(getppid(), SIGTERM);
			int suti;
			if(read(fd2[1], &suti, sizeof(suti)) >0) {
				printf("Pipe error\n");
			}
			if(suti==0) {
				printf("kalacs\n");
			}
			else{
				printf("suti\n");
			}
			close(fd2[1]);
			if(SIGNUM==1) {
				char * msg = "Elso";
				write(jp1[0], &msg, sizeof(msg));
				printf("%s jelzes\n", msg);
			}
			pause();
			if(SIGNUM==2) {
				char * msg = "Masokid";
				write(jp1[0], &msg, sizeof(msg));
				printf("%s jelzes\n", msg);
			}
			//pause();
			if(SIGNUM==3) {
				char * msg = "Harmadik";
				write(jp1[0], &msg, sizeof(msg));
				printf("%s jelzes\n", msg);
			}
			int uj = rand()%3-1;
			write(ujp[0], &uj, sizeof(uj));
			exit(0);
		}
		else
		{			
			close(fd[1]);
			close(fd2[1]);
			close(ujp[0]);
			pause();
			srand(time(NULL));
			int r=rand()%2;
			write(fd[0], &r, sizeof(r));
			write(fd2[0], &r, sizeof(r));
			
			waitpid(pid, &status, 0);
			waitpid(pid2, &status, 0);
			
			int uj;
			read(ujp[1], &uj, sizeof(uj));
			printf("%i\n", uj);
			
			close(fd[0]);
			close(fd2[0]);
			printf("Szulo vege\n");
			exit(0);
		}
	}
	else
	{
		close(fd[0]);
		kill(getppid(), SIGTERM);
		int suti;
		if(read(fd[1], &suti, sizeof(suti)) >0) {
			printf("Pipe error\n");
		}
		if(suti==0) {
				printf("kalacs\n");
		}
		else{
			printf("suti\n");
		}
		close(fd[1]);
		kill(pid2, SIGUSR2);
		sleep(1);
		kill(pid2, SIGUSR2);
		sleep(1);
		kill(pid2, SIGUSR2);
		sleep(1);
		exit(0);
	}
	return 0;
}