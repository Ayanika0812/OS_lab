#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_NAME "/tmp/q3Fifo"

int main() {
    if (access(FIFO_NAME, F_OK) == -1) {
        int res = mkfifo(FIFO_NAME, 0777);
        if (res != 0) {
            fprintf(stderr, "Could not create fifo %s\n", FIFO_NAME);
            exit(EXIT_FAILURE);
        }
    }

    int c_pid = fork();

    if (c_pid < 0) {
        perror("Child not created");
        exit(-1);
    }

    if (c_pid == 0) {  // Child process
        char buffer[256];
        while (1) {
        printf("\n in child process \n");
            int pipe = open(FIFO_NAME, O_WRONLY);
            printf("Child: ");
            fgets(buffer, sizeof(buffer), stdin);  // Use fgets() to read input
            buffer[strcspn(buffer, "\n")] = '\0';  // Removing trailing newline character
            if (strcmp(buffer, "x") == 0)
                break;
            write(pipe, buffer, strlen(buffer) + 1);  // Write the null-terminated string
            close(pipe);

            pipe = open(FIFO_NAME, O_RDONLY);
            read(pipe, buffer, sizeof(buffer));
            printf("Parent says: %s\n", buffer);
            close(pipe);
        }
        exit(0);
    } else {  // Parent process
        char buffer[256];
        while (1) {
        printf("\n in parent process \n");
            int pipe = open(FIFO_NAME, O_RDONLY);
            read(pipe, buffer, sizeof(buffer));
            if (strcmp(buffer, "x") == 0)
                break;
            printf("Child says: %s\n", buffer);
            close(pipe);

            pipe = open(FIFO_NAME, O_WRONLY);
            printf("Parent: ");
            fgets(buffer, sizeof(buffer), stdin);  // Use fgets() to read input
            buffer[strcspn(buffer, "\n")] = '\0';  // Remove trailing newline character
            write(pipe, buffer, strlen(buffer) + 1);  // Write the null-terminated string
            close(pipe);
        }
        exit(0);
    }
}
