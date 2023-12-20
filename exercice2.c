#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#define BUFSIZE 1024

int parse_line(char *s, char **argv[]) {
    char word[BUFSIZE];
    int nbWord = 0;
    int wordIndex = 0;

    // Allouer de l'espace pour le tableau d'arguments
    *argv = (char **)malloc((BUFSIZE / 2) * sizeof(char *));
    if (*argv == NULL) {
        perror("Allocation echouee");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; s[i] != '\0'; i++) {
        if (isspace((unsigned char)s[i]) || s[i] == '\n') {
            word[wordIndex] = '\0';
            if (wordIndex > 0) {
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
                 printf("affichons les commandes dans le tableu\n");
                for (int i = 0; i < argc+1; i++) {
                    printf(" | %s", args[i]);
                }
                printf("\n");
               
                if (strcmp(args[0], "exit") == 0) {
                    printf("Exiting the shell\n");
                    break;
                }

                if (fork() == 0) {
                    execvp(args[0], args);
                    perror("Exec failed");
                    exit(EXIT_FAILURE);
                    for (int i = 0; i < argc+1; i++) {
                    free(args[i]);
                }
                } else {
                    wait(NULL);
                }

                // Libérer la mémoire allouée pour les arguments
                for (int i = 0; i < argc+1; i++) {
                    free(args[i]);
                }
                free(args);
                args = NULL;
            }
        }
    }

    return 0;
}