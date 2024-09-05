#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int main() {
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    char *str = (char *)shmat(shmid, (void *)0, 0);

    pid_t pid = fork();

    if (pid == 0) {  // Child process
        // Wait for alphabet from parent
        while (str[0] == '\0');
        char received = str[0];
        printf("Child received: %c\n", received);
        // Respond with the next alphabet
        str[0] = received + 1;
    } else {  // Parent process
        printf("Parent: Enter an alphabet: ");
        scanf("%c", str);
        // Wait for response from child
        sleep(2);
        printf("Parent received: %c\n", str[0]);
    }

    shmdt(str);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
