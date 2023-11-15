#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 256

int main(void) {
    char input[MAX_INPUT_SIZE];
    char *prompt = "#cisfun$ ";

    while (1) {
        printf("%s", prompt);

        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            if (feof(stdin)) {
                printf("\n");  // Handle Ctrl+D
                break;
            }
        }

        // Remove the trailing newline character, if present
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }

        pid_t child_pid = fork();
        if (child_pid == -1) {
            perror("fork");
            continue;
        }

        if (child_pid == 0) {
            // In the child process
            if (execlp(input, input, (char *)NULL) == -1) {
                perror("execlp");
                exit(1);
            }
        } else {
            // In the parent process
            int status;
            wait(&status);

            if (WIFEXITED(status)) {
                if (WEXITSTATUS(status) == 127) {
                    printf("%s: command not found\n", input);
                }
            }
        }
    }

    return 0;
}

