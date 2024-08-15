/* Write a program to print the lines of a file that contain a word given as the program argument (a simple version of grep UNIX utility) */

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        const char *usage_msg = "Usage: ./q1 <filename> <word>\n";
        write(2, usage_msg, strlen(usage_msg));  //Guides the user on how to run  the program in case number of params is wrong
        exit(1); //exit with 1 to show error
    }

    const char *filename = argv[1];
    const char *search_word = argv[2];

    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        const char *error_msg = "Error opening file\n";
        write(2, error_msg, strlen(error_msg));
        exit(1);
    }

    char buffer[BUFFER_SIZE];
    int bytes_read;
    char line[BUFFER_SIZE];
    int line_len = 0;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n' || i == bytes_read - 1) {
                line[line_len] = '\0';  // Null-terminate the line
                if (strstr(line, search_word)) {
                    write(1, line, line_len);
                    write(1, "\n", 1);
                }
                line_len = 0;  // Reset for the next line
            } 
            else {
                line[line_len++] = buffer[i];
            }
        }
    }

    if (bytes_read == -1) {
        const char *read_error_msg = "Error reading file\n";
        write(2, read_error_msg, strlen(read_error_msg));
        close(fd);
        exit(1);
    }

    close(fd);
    return 0;
}

