//#include <sys/types.h>
#include <unistd.h> //fork
#include <stdio.h>
#include <stdlib.h> 
#include <sys/wait.h> //wait
void main()
{
int status;
pid_t pid;
pid = fork();
if(pid == -1)
printf("\nERROR child not created");
else if (pid == 0) /* child process */
{
printf("\n I'm the child!");
exit(0);
}
else /* parent process */
{wait(&status);
printf("\n I'm the parent!");
printf("\n Child returned: %d\n", status);
}
}