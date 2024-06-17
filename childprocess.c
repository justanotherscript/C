/*

In this example, the fork() function creates a new process, and the execl() function executes the another_program program in the child process. The wait() function is used to wait for the child process to finish.

Remember to replace another_program.c and another_program with the actual names of the program you want to run.

Now, go forth and ninja-ly execute those C programs!

*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) { // child process
        execl("./another_program", "another_program", (char *)NULL);
        perror("execl");
        exit(1);
    } else { // parent process
        wait(NULL);
    }

    return 0;
}
