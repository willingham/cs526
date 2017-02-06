#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_LINE 80 // The maximum length command 
#define HISTORY_SIZE 10 


void parseCommand(char commandBuffer[], char *args[], int *background)
{
    int comLength;
    
    comLength = read(STDIN_FILENO, commandBuffer, MAX_LINE);
}

int main(void)
{
    char commandBuffer[MAX_LINE];
	char *args[MAX_LINE/2 + 1]; // command line arguments
	int should_run = 1; // flag to determine when to exit program
    int background = 0; // flag to run in background
    pid_t pid;
	while (should_run) {
        background = 0;
        printf("osh>"); fflush(stdout);
        parseCommand(commandBuffer, args, &background);
        /**
        * After reading user input, the steps are:
        * (1) fork a child process using fork()
        * (2) the child process will invoke execvp()
        * (3) if command included &, parent will invoke wait() */
    }
    return 0;
}
