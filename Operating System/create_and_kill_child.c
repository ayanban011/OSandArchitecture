#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/user.h>
 
int main(int argc, char *argv[])
{
 
int s, signal;
pid_t p;
 
p = fork();
 
if(p == 0)
{
 
if(execl(argv[1], argv[1], (char *)0) != 0) { printf ("excel error!\n"); return -1; }
 
}
 
if(waitpid(p, &s, WNOHANG) > 0)
{
 
kill(p, SIGKILL);
 
}
 
if(WIFSIGNALED(s))
{
 
signal = WTERMSIG(s);
 
fprintf(stdout, "child stopped due to signal %d\n", signal);
 
return -1;
 
}
 
return 0;
 
}
