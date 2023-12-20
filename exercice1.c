#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 1024


int main(int argc , char * argv[]){

    int n;
    char prompt[] = {'$'};
    char buffer[BUFSIZE];

    while(1){
        write(STDOUT_FILENO,(void*) prompt, 1);
        n = read(STDIN_FILENO, (void*) buffer, BUFSIZE);
        
	char command[n + 1];
	strncpy(command, buffer, n);
	command[n] = '\0'; 

	write(STDOUT_FILENO, (void*) command, sizeof(command));

	size_t command_length = strlen(command);
	if(command_length > 0 && command[command_length - 1] == '\n'){
		command[command_length - 1] = '\0';
	}

	if(strcmp(command, "exit") == 0){
		printf("exiting the shell\n");
		break;
	}
        
	//processus fils, qui execute la commande
        if(fork() == 0){
            execl("/bin/sh","/bin/sh",  "-c", command, (char*) NULL);
        }
	else{
		wait(NULL);
	}

    }
    return 1;
}
