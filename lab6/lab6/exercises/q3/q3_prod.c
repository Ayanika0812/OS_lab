#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_NAME "/tmp/myfifo"

int main() {
    // Create the FIFO if it doesn't exist
    if (access(FIFO_NAME, F_OK) == -1) {
        if (mkfifo(FIFO_NAME, 0666) != 0) {
            perror("mkfifo failed");
            exit(EXIT_FAILURE);
        }
    }

    // Open FIFO for writing
    int fifo_fd = open(FIFO_NAME, O_WRONLY);
    if (fifo_fd < 0) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    while (1) {
        printf("Enter message to send (or 'x' to exit): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';  // Remove trailing newline character
        
        if (strcmp(buffer, "x") == 0) {
            break;
        }
        
        if (write(fifo_fd, buffer, strlen(buffer) + 1) < 0) {
            perror("write failed");
            close(fifo_fd);
            exit(EXIT_FAILURE);
        }
    }

    close(fifo_fd);
    return 0;
}

/*

output -

./a.out
Enter message to send (or 'x' to exit): Hi Consumer
Enter message to send (or 'x' to exit): how r u doing?       
Enter message to send (or 'x' to exit): x

*/
