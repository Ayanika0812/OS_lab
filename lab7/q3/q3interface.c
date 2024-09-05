#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main() {
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    char *str = (char *)shmat(shmid, (void *)0, 0);

    while (1) {
        printf("Enter your message: ");
        fgets(str, 1024, stdin);

        if (strncmp(str, "exit", 4) == 0)
            break;
    }

    shmdt(str);
    return 0;
}
