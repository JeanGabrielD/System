#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 1024


int parse_line(char *s, char **argv[]){
    char word[1024];
    int nbWord = 0;
    int wordIndex = 0;
    for(int i=0; i < sizeof(s) ; i++){
        if(s[i] == ' '){
            *argv[nbWord] = word;
            nbWord++;
            wordIndex = 0;  
        }
        else{
            word[wordIndex] = s[i];
            wordIndex++;
        }

    }
    write(STDOUT_FILENO, word, sizeof(word));

    write(STDOUT_FILENO, "parsing réussi", 16);
    *argv[nbWord] = word;
    *argv[nbWord + 1] = NULL;
    write(STDOUT_FILENO, "parsing réussi", 16);
    return 1;
}

int main(int argc , char * argv[]){

    char prompt[] = {'$', ' '};
    char buffer[2];
    //int commandLength = 0;
    //int finalLength;
    //int n;
    //char **args[1024];
    

    while(1){
        write(STDOUT_FILENO,(void*) prompt, 2);
        read(STDIN_FILENO, (void*) buffer, 2);
        
       /* for(int i= 0 ;i<1024;i++){
            if(buffer[i] - '\0' == 0){
                finalLength = commandLength;
            }
            else{
                commandLength++;
            }
        }

        char bufferOut[finalLength];*/

        //for (int i = 0; i < finalLength; i++){
            /*bufferOut[i] = buffer[i];
        }*/
        
        /*if(buffer == "exit"){
            exit(0);
        }*/
        write(STDOUT_FILENO, (void*) "écriture réussie\n", 20);
        
        if(fork() == 0){
            execlp(buffer, buffer, (char*) NULL);
        }
        /*if (n != -1){
            parse_line(buffer, args);
        
        }

        if(fork() == 0){
            execvp(*args[0], *args);
        }*/

    }

    return 1;

}