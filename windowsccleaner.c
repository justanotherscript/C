#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

int main() {
    printf("Welcome to diskclean. This Program works in Windows 10 and 11\n");
    printf("All opened Programs should be closed before you continue\n");
    printf("When the program has finished, 'ready' will be shown.\n");
    printf("Press 'c' key to continue...\n");

    while (1) {
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'c') {
                break;
            }
        }
    }

    const char *tmp_path = "C:\\Windows\\Temp";
    struct dirent *entry;
    DIR *dp = opendir(tmp_path);

    if (dp == NULL) {
        perror("opendir");
        return EXIT_FAILURE;
    }

    while ((entry = readdir(dp)) != NULL) {
        char file_path[512];
        snprintf(file_path, sizeof(file_path), "%s\\%s", tmp_path, entry->d_name);

        if (entry->d_type == DT_REG) {
            FILE *file = fopen(file_path, "r+");
            if (file) {
                fclose(file);
                continue; // File is still in use
            }
            if (remove(file_path) == 0) {
                printf("Deleted: %s\n", file_path);
            } else {
                perror("remove");
            }
        }
    }

    closedir(dp);
    return EXIT_SUCCESS;
}
