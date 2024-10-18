#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

void list_files(const char *base_path, int depth) {
    struct dirent *entry;
    struct stat statbuf;
    char path[1024];

    // Open the directory
    DIR *dir = opendir(base_path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    // Iterate over directory entries
    while ((entry = readdir(dir)) != NULL) {
        // Skip the "." and ".." entries
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            // Construct the full path
            snprintf(path, sizeof(path), "%s/%s", base_path, entry->d_name);

            // Get file status
            if (lstat(path, &statbuf) == -1) {
                perror("lstat");
                continue;
            }

            // Print the file name with indentation based on depth
            for (int i = 0; i < depth; i++) {
                printf("    "); // Indentation for subdirectories
            }
            printf("%s\n", entry->d_name);

            // If it's a directory, recurse into it
            if (S_ISDIR(statbuf.st_mode)) {
                list_files(path, depth + 1);
            }
        }
    }

    // Close the directory
    closedir(dir);
}

int main() {
    // Start listing files from the current directory
    list_files(".", 0);
    return 0;
}
/*
./a.out
q11.c
Q2.C
lab11
    EXAMPLE
        Q.C
    exerciseslab11
        q2.c
        q1full.c
        a.out
        q3.c
        q1.c
a.out
q1.c
*/
