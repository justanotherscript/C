#include<stdio.h>
#include<unistd.h>

int main(void) {

if (access("~/.vimrc", F_OK) != -1) {

printf("File exists!\n");
} else {

printf("File not exists.\n You aint based man\n");
}


return 0;

}
