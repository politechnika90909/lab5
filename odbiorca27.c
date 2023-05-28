#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 256

int main() {
    int fd[2];
    pid_t childpid;
    char buffer[BUFFER_SIZE];

    if (pipe(fd) == -1) {
        perror("Pipe creation failed");
        exit(1);
    }

    childpid = fork();

    if (childpid == 0) {
        // potomek
        close(fd[1]);

        while (1) {
            memset(buffer, 0, BUFFER_SIZE);
            read(fd[0], buffer, BUFFER_SIZE);

            printf("odebrano: %s", buffer);

            if (strcmp(buffer, "exit\n") == 0) {
                break;
            }
        }

        close(fd[0]);
    } else {
        // rodzic
        close(fd[0]);

        while (1) {
            memset(buffer, 0, BUFFER_SIZE);
            fgets(buffer, BUFFER_SIZE, stdin);

            write(fd[1], buffer, strlen(buffer));

            if (strcmp(buffer, "exit\n") == 0) {
                break;
            }
        }

        close(fd[1]);
        wait(NULL);
    }

    return 0;
}
