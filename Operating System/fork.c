#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h> 
#define N 5


int main (void)
{
    int pid;
    int i;
    int status;
	for (int i=0; i<N; i++) 
	{
		pid=fork();
		if(pid<0)
			//error
		else if(pid==0) //child
			execlp("\bin\ls","ls",NULL);
		else //parent
		{
			wait(NULL);
			exit(0);
		}	
	}
}
