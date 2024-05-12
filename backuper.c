#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

void copyFolder(const char *src, const char *dest) {
    char command[1024];
    sprintf(command, "cp -r %s %s", src, dest);
    system(command);
}

int main() {
    const char *sourceDir = "/home/user/source";
    const char *destinationDir = "/tmp/";

    copyFolder(sourceDir, destinationDir);

    return 0;
}
