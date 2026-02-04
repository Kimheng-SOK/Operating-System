#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t p = fork();

    if (p < 0) {
        perror("fork failed");
        return 1;
    }
    else if (p == 0) {
        printf("Child process running ls...\n");
        execl("/bin/ls", "ls", "-l", NULL);

        perror("exec failed");
        exit(1);
    }
    else {
        printf("Parent process waiting for child...\n");
        wait(NULL);
        printf("Child finished.\n");
    }

    return 0;
}
