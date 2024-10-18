#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> //directory manipulation functions
#include <sys/stat.h> //file status functions and structures
#include <pwd.h> //user information
#include <grp.h> // group information
#include <time.h> // time-related functions
#include <string.h>

// retrieves and prints detailed information about a specified file
void print_file_info(const char *filename) {
    struct stat file_stat;  //structure to hold file status information

    // Get file status
    if (lstat(filename, &file_stat) < 0) {
        perror("lstat");
        return;
    }

    // Print file type and permissions
    printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
    printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
    printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
    printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
    printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
    printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
    printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
    printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
    printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
    printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");
    printf(" ");

    // Print number of links
    printf("%lu ", file_stat.st_nlink);

    // Print owner and group
    struct passwd *pw = getpwuid(file_stat.st_uid); //ownername
    struct group  *gr = getgrgid(file_stat.st_gid);  //group
    printf("%s %s ", pw->pw_name, gr->gr_name);

    // Print size in bytes
    printf("%lld ", (long long)file_stat.st_size);

    // Print last modification time
    char timebuf[80];
    struct tm *tm_info = localtime(&file_stat.st_mtime);
    strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", tm_info);
    printf("%s ", timebuf);

    // Print file name
    printf("%s\n", filename);
}

int main() {
    DIR *dir;
    struct dirent *entry;

    // Open the current directory
    if ((dir = opendir(".")) == NULL) { //open current directory
        perror("opendir");
        return EXIT_FAILURE;
    }

    // Read directory entries
    while ((entry = readdir(dir)) != NULL) {
        // Skip the "." and ".." entries
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            print_file_info(entry->d_name);
        }
    }

    closedir(dir);
    return EXIT_SUCCESS;
}

