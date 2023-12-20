#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFSIZE 1024
#define MAX_ARGS 16

int parse_line(char *s, char **argv[]) {
    char word[BUFSIZE];
    int nbWord = 0;
    int wordIndex = 0;

    // Allouer de l'espace pour le tableau d'arguments
    *argv = (char **)malloc((MAX_ARGS + 1) * sizeof(char *));
    if (*argv == NULL) {
        perror("Allocation echouee");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; s[i] != '\0'; i++) {
        if (isspace((unsigned char)s[i]) || s[i] == '\n') {
            word[wordIndex] = '\0';
            s[i] = '\0';
            if (wordIndex > 0 && nbWord < MAX_ARGS) {
                // Ignorer les espaces supplémentaires entre les mots
                (*argv)[nbWord] = malloc(strlen(word) + 1);
                if ((*argv)[nbWord] == NULL) {
                    perror("Allocation echouee");
                    exit(EXIT_FAILURE);
                }
                strcpy((*argv)[nbWord], word);
                nbWord++;
                wordIndex = 0;
            }
        } else if (s[i] == '|') {
            // Handle the pipe symbol as a separate argument
            word[wordIndex] = '\0';
            s[i] = '\0';
            if (wordIndex > 0 && nbWord < MAX_ARGS) {
                (*argv)[nbWord] = malloc(strlen(word) + 1);
                if ((*argv)[nbWord] == NULL) {
                    perror("Allocation echouee");
                    exit(EXIT_FAILURE);
                }
                strcpy((*argv)[nbWord], word);
                nbWord++;
                wordIndex = 0;
            }
            // Add the pipe symbol as a separate argument
            char pipe_arg[2] = "|";
            (*argv)[nbWord] = malloc(2);
            strcpy((*argv)[nbWord], pipe_arg);
            nbWord++;
        } else {
            word[wordIndex] = s[i];
            wordIndex++;
        }
    }

    (*argv)[nbWord] = NULL;

    return nbWord;
}


int main(int argc, char *argv[]) {
    int n;
    char prompt[] = {'$'};
    char buffer[BUFSIZE];
    char **args = NULL;

    while (1) {
        write(STDOUT_FILENO, (void *)prompt, 1);
        n = read(STDIN_FILENO, (void *)buffer, BUFSIZE);

        if (n != -1) {
            int argc = parse_line(buffer, &args);

            if (argc > 0) {
                if (strcmp(args[0], "exit") == 0) {
                    printf("Exiting the shell\n");
                    break;
                }

                int pipe_index = -1;
                for (int i = 0; i < argc; i++) {
                    if (strcmp(args[i], "|") == 0) {
                        pipe_index = i;
                        break;
                    }
                }

                if (pipe_index != -1) {
                    // Handle the case where a pipe is present
                    args[pipe_index] = NULL; // Set the pipe as the end of the first command
                    int pipe_fd[2];
                    pipe(pipe_fd);

                    if (fork() == 0) {
                        // Child process: set up output redirection
                        close(pipe_fd[0]); // Close unused read end
                        dup2(pipe_fd[1], STDOUT_FILENO); // Redirect stdout to the pipe
                        close(pipe_fd[1]); // Close write end

                        execvp(args[0], args);
                        perror("Exec failed");
                        exit(EXIT_FAILURE);
                    } else {
                        // Parent process: wait for the child and set up input redirection
                        wait(NULL);
                        close(pipe_fd[1]); // Close unused write end
                        dup2(pipe_fd[0], STDIN_FILENO); // Redirect stdin to the pipe
                        close(pipe_fd[0]); // Close read end

                        // Move to the second part of the command
                        for (int i = pipe_index + 1; i < argc; i++) {
                            args[i - (pipe_index + 1)] = args[i];
                        }
                        argc -= (pipe_index + 1);
                    }
                }

                // Execute the command
                if (fork() == 0) {
                    execvp(args[0], args);
                    perror("Exec failed");
                    exit(EXIT_FAILURE);
                } else {
                    wait(NULL);
                }

                // Libérer la mémoire allouée pour les arguments
                for (int i = 0; i < argc; i++) {
                    free(args[i]);
                }
                free(args);
                args = NULL;
            }
        }
    }

    return 0;
}

