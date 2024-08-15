/*Write a program to list the files given as arguments, stopping every 20 lines until a key is hit. (a simple version of more UNIX utility)*/

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define LINES_PER_PAGE 20

void display_page(char *buffer, int bytes_read) {
    int line_count = 0;
    for (int i = 0; i < bytes_read; i++) {
        write(1, &buffer[i], 1);
        if (buffer[i] == '\n') {
            line_count++;
            if (line_count == LINES_PER_PAGE) {
                write(1, "--More--", 8);
                char ch;
                read(0, &ch, 1);  // Wait for user input
                if (ch == 'q') exit(0);  // Exit if 'q' is pressed
                line_count = 0;
            }
        }
    }
}

int main(int argc, char *argv[]) {
	for (int i = 1; i < argc; i++) {
		int fd = open(argv[i], O_RDONLY);
		if (fd == -1) {
		    perror("Error opening file");
		    exit(1);
		}
		char buffer[BUFFER_SIZE];
		ssize_t bytes_read;
		while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0){
		    display_page(buffer, bytes_read);
		}
		if (bytes_read == -1) {
		    perror("Error reading file");
		    close(fd);
		    exit(1);
		}
		close(fd);
    	}
    	return 0;
}


