#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_LINE 80 // The maximum length command 
#define HISTORY_SIZE 10 
int main(void)
{
	char *args[MAX_LINE/2 + 1]; // command line arguments
	int should_run = 1; // flag to determine when to exit program
    int background = 0; // flag to run in background
    pid_t pid;
	while (should_run) {
        background = 0;
        printf("osh>"); fflush(stdout);
        /**
        * After reading user input, the steps are:
        * (1) fork a child process using fork()
        * (2) the child process will invoke execvp()
        * (3) if command included &, parent will invoke wait() */
    }
    return 0;
}
