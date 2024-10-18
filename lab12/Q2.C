#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main() {
    struct dirent *entry;
    DIR *dp;

    
    dp = opendir(".");
    if (dp == NULL) {
        perror("opendir failed");
        return EXIT_FAILURE;
    }
  
    while ((entry = readdir(dp)) != NULL) {
        
        if (entry->d_name[0] == '.') {
            continue;  
        }
        printf("%s\n", entry->d_name);
    }
    closedir(dp);
    return EXIT_SUCCESS;
}
