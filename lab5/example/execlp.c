/*C program forking a separate process.*/
//#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include <sys/wait.h>
#include <stdlib.h> //exit
int main()
{
pid_t pid;
/* fork another process */
pid = fork();
if (pid < 0) { /* error occurred */
fprintf(stderr, "Fork Failed");
exit(-1);
}
else if (pid == 0) { /* child process */
execlp("/bin/ls", "ls", NULL);
}
else { /* parent process */
/* parent will wait for the child to complete */
wait (NULL);
printf ("Child Complete");
exit(0);
}
}