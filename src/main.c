#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CMD_LEN 1024
#define MAX_ARGS 64

int main() {
    char input[MAX_CMD_LEN];
    char *args[MAX_ARGS];
    char *token;
    pid_t pid;
    int status;

    while (1) {
        printf("mysh> ");
        fflush(stdout);

        if (!fgets(input, MAX_CMD_LEN, stdin)) {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) == 0) continue;

        int i = 0;
        token = strtok(input, " ");
        while (token != NULL && i < MAX_ARGS - 1) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (strcmp(args[0], "exit") == 0) break;

        if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL || chdir(args[1]) != 0) {
                perror("cd");
            }
            continue;
        }

        pid = fork();
        if (pid < 0) {
            perror("fork");
            continue;
        }

        if (pid == 0) {
            execvp(args[0], args);
            perror("execvp");
            exit(1);
        } else {
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}
