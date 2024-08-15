/*Write a program to copy character-by character copy is accomplished using calls to the functions referenced in stdio.h*/

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 1  //set to 1 for each character

int main(int argc, char *argv[]) {
    //if (argc != 3) {
    //    write(2, "Usage: ./copyfile <source> <destination>\n", 42);
    //   exit(1);
    //}

    char *source_file = argv[1];
    char *dest_file = argv[2];

    int src_fd = open(source_file, O_RDONLY);
    if (src_fd < 0) {
        write(2, "Error opening source file\n", 26);
        exit(1);
    }

    int dest_fd = open(dest_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    // Opens the destination file for writing. Creates it if it does not exist, and truncates it if it does. Sets file permissions to 0644
    if (dest_fd < 0) {
        write(2, "Error opening destination file\n", 31);
        close(src_fd);
        exit(1);
    }

    char buffer[BUFFER_SIZE];
    int bytes_read;
    int i=0;
    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        if (write(dest_fd, buffer, bytes_read) != bytes_read) {
            write(2, "Error writing to destination\n", 30);
            close(src_fd);
            close(dest_fd);
            exit(1);
        }
    }
    write(1, "Successfully copied contents\n", 30);

    if (bytes_read < 0) {
        write(2, "Error reading source\n", 21);
    }

    close(src_fd);
    close(dest_fd);
    return 0;
}

