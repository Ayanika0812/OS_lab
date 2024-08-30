#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_NAME "/tmp/myfifo"

int main() {
    // Open FIFO for reading
    int fifo_fd = open(FIFO_NAME, O_RDONLY);
    if (fifo_fd < 0) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    while (1) {
        ssize_t bytes_read = read(fifo_fd, buffer, sizeof(buffer));
        if (bytes_read < 0) {
            perror("read failed");
            close(fifo_fd);
            exit(EXIT_FAILURE);
        }
        
        if (bytes_read == 0) {
            // End of input (e.g., writer closed the pipe)
            break;
        }
        
        printf("Received: %s\n", buffer);
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

./a.out
Received: Hi Consumer
Received: how r u doing?


*/
