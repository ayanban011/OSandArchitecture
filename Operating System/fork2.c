#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
int main(int argc,char*argv[]) 
{ 
    int n=atoi(argv[0]);
    for(int i=0;i<n;i++) // loop will run n times (n=5) 
    { 
        if(fork() == 0) 
        { 
            printf("[son] pid %d from [parent] pid %d\n",getpid(),getppid()); 
            exit(0); 
        } 
    } 
    for(int i=0;i<n;i++) // loop will run n times (n=5) 
    wait(NULL); 
      
} 

