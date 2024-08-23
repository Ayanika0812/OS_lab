#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    pid = fork();

    if (pid < 0) {
       
        write(2,"Error child not created",23);
        exit(-1);
    }

    if (pid == 0) {
    
        printf("\nbefore sleep\n");
        printf("Child process (PID: %d), Parent PID: %d\n", getpid(), getppid());

       //making child an orphan
        sleep(10);
         printf("\nafter sleep\n");
        printf("Child process (PID: %d), Parent PID after sleep: %d\n", getpid(), getppid());
        exit(0);
    } else {
    sleep(1);
        printf("Parent process (PID: %d)\n", getpid());
        exit(0);
    }

    return 0;
}

