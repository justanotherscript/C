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

    // Checking for linux OS with 
    // __linux__ macro 
    if ( __linux__ ) {
        printf("Hey, it seems that you"
                "are working on a Linux OS.\n");
        printf("The backup now starts...\n");


    const char *sourceDir = "/home/user/source";
    const char *destinationDir = "/tmp/";

    copyFolder(sourceDir, destinationDir);
    printf("Mission complete\n");
    
    return 0;

    } else {

printf("Your platform isnt supported, good luck");
        exit(1);

    }



}
