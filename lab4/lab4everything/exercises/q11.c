#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // to use strstr
#include <unistd.h>

#define BUFFER_SIZE 1024


int main(int argc, char *argv[]) {
 

  const char *search_word = argv[1];
  const char *filename = argv[2];
  int fd = open(filename, O_RDONLY);
  if (fd < 0) {

    printf("Error opening a file");
    exit(1);
  }

  char buffer[BUFFER_SIZE];
  int bytes_read;
  while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
    char *line_strt = buffer;
    for (int i = 0; i < bytes_read; i++) {
      if (buffer[i] == '\n' || i == bytes_read - 1) {
        int line_length = &buffer[i] - line_strt;
        if (strstr(line_strt, search_word) != NULL) {
          write(1, line_strt, line_length);
          write(1, "\n", 1);
        }
        line_strt = &buffer[i + 1];
      }
    }
  }

  if (bytes_read < 0) {
    printf("Error reading file\n");
  }

  close(fd);
  exit(0);
}
