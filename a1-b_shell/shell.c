#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_LINE 80 // The maximum length command 
#define HISTORY_SIZE 10 

int parseHistoryNumber(char *bfrCommand, int commandNumber)
{
    int len = strlen(bfrCommand);
    if (length[0] < 2) {
        return -1;
    } else if (bfrCommand[1] == '!') {
        return commandNumber; // commandNumber = 0
    } else {
        int result = atoi(bfrCommand[1]);
        if (result == 0) {
            return -1;
        } else {
            return result;
        }
    }
}

void parseCommand(char bfrCommand[], char *args[], int *background)
{
    fgets(bfrCommand, MAX_LINE, stdin);
    if (strlen(bfrCommand) > 1) {
        if (bfrCommand[0] == '!') {
            int commandNumber; // TODO get history number
            
        }
    }
}

int main(void)
{
    char bfrCommand[MAX_LINE + 1];
	char *args[MAX_LINE/2 + 1]; // command line arguments
	int should_run = 1; // flag to determine when to exit program
    int background = 0; // flag to run in background
    pid_t pid;
	while (should_run) {
        background = 0;
        printf("osh>"); fflush(stdout);
        parseCommand(bfrCommand, args, &background);
                

        /**
        * After reading user input, the steps are:
        * (1) fork a child process using fork()
        * (2) the child process will invoke execvp()
        * (3) if command included &, parent will invoke wait() */
    }
    return 0;
}
