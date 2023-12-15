#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 1024



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

    int n;
    char prompt[] = {'$'};
    char buffer[BUFSIZE];
    //int commandSize = 0;
    //int commandLength = 0;
    //int finalLength;
    //int n;
    //char **args[1024];
    

    while(1){
        write(STDOUT_FILENO,(void*) prompt, 1);
        n = read(STDIN_FILENO, (void*) buffer, BUFSIZE);
        
	char command[n];
	for(int i = 0; i < n; i++){
		command[i] = buffer[i];
	}

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
        //write(STDOUT_FILENO, (void*) "écriture réussie\n", 20);
	write(STDOUT_FILENO, (void*) command, sizeof(command));
        
	//processus fils, qui execute la commande
        if(fork() == 0){
            execl("/bin/sh","/bin/sh",  "-c", command, (char*) NULL);
        }
	else{
		wait(NULL);
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
