#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h> 

#define BUFSIZE 1024
#define MAX_ARGS 16

void free_args(char **args, int argc) {
    for (int i = 0; i < argc; i++) {
        free(args[i]);
    }
    free(args);
}

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
                    free_args(args, argc);
                    break;
                }

                // Recherche de l'indice du caractère '>'
                int redirect_index = -1;
                for (int i = 0; i < argc; i++) {
                    if (strcmp(args[i], ">") == 0) {
                        redirect_index = i;
                        break;
                    }
                }

                if (redirect_index != -1 && redirect_index == argc - 2) {
                    // Redirection de la sortie standard vers un fichier
                    char *output_file = args[redirect_index + 1];
                    args[redirect_index] = NULL; 
                    int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
                    if (fd == -1) {
                        perror("Erreur lors de l'ouverture du fichier");
                        free_args(args, argc);
                        continue;
                    }

                    if (fork() == 0) {
                        // Redirection de la sortie standard vers le fichier
                        dup2(fd, STDOUT_FILENO);
                        close(fd);

                        // Exécution de la commande
                        execvp(args[0], args);
                        perror("Exec failed");
                        exit(EXIT_FAILURE);
                    } else {
                        wait(NULL);
                    }
                } else {
                    // Exécution normale sans redirection
                    if (fork() == 0) {
                        execvp(args[0], args);
                        perror("Exec failed");
                        exit(EXIT_FAILURE);
                    } else {
                        wait(NULL);
                    }
                }

                // Libérer la mémoire allouée pour les arguments
                free_args(args, argc);
                args = NULL;
            }
        }
    }

    return 0;
}
