//////////////////////////////
//  Unix shell
//  Author: Thomas Willingham
//  Date: 2017 February
/////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#define MAX_LINE 80 // The maximum length command 
#define HISTORY_SIZE 10 

int parseHistoryNumber(char *bfrCommand, int commandNumber)
{
    int len = strlen(bfrCommand);
    if (len < 2) {
        return -1;
    } else if (bfrCommand[1] == '!') {
        return commandNumber; // commandNumber = 0
    } else {
        int result = atoi(bfrCommand+1);
        if (result == 0) {
            return -1;
        } else {
            return result;
        }
    }
}

void runCommand(char bfrCommand[MAX_LINE+1])
{
    char *args[MAX_LINE/2 + 1];
    char *arg;
    int argNum = 0;

    arg = strtok(bfrCommand, " \n");

    while (arg != NULL) {
        args[argNum] = malloc((strlen(arg)+1)*sizeof(char));
        strcpy(args[argNum], arg);
        argNum++;
        arg = strtok(NULL, " \n");
    }
    args[argNum] = NULL;

    if (argNum > 0) {
        int shouldWait = 1;
        if (argNum > 1 && strcmp(args[argNum-1], "&") == 0) {
            shouldWait = 0;
            argNum--;
            args[argNum] = NULL;
        }

        pid_t pid = fork();

        if (pid < 0) {
            perror("Error forking process.\n");
            exit(1);
        } else {
            if (pid == 0) {
                if (execvp(args[0], args) == -1) {
                    perror("Execution error.\n");
                    exit(1);
                }
            } else if (shouldWait) {
                int status;
                waitpid(pid, &status, 0);
            }
        }
    }
}

int main(void)
{
    char bfrCommand[MAX_LINE + 1];
    char history[HISTORY_SIZE][MAX_LINE+1];
	//char *args[MAX_LINE/2 + 1]; // command line arguments
	int shouldRun = 1; // flag to determine when to exit program
    int background = 0; // flag to run in background
    int commandNumber = 0;
    pid_t pid;
	while (shouldRun) {
        background = 0;
        printf("osh>"); fflush(stdout);
        int shouldRunCommand = 1;
        fgets(bfrCommand, MAX_LINE, stdin);
        if (strlen(bfrCommand) > 1) {
            if (bfrCommand[0] == '!') {
                int historyNumber = parseHistoryNumber(bfrCommand, commandNumber); // TODO get history number
                if (commandNumber != -1) {
                    if (commandNumber == 0) {
                        printf("History is empty.\n");
                        shouldRunCommand = 1;
                    } else if (historyNumber < 1 || historyNumber <= commandNumber-10 || historyNumber > commandNumber) {
                        printf("Specified command does not exist.\n");
                        shouldRunCommand = 0;
                    } else {
                        strcpy(bfrCommand, history[historyNumber%HISTORY_SIZE]);
                        printf("%s", bfrCommand);
                    }
                } else {
                    printf("Error in history command.\n");
                    shouldRunCommand = 0;
                }
            } else if (strcmp(bfrCommand, "history\n") == 0) {
                int lastSavedCommand = commandNumber - 9;
                if (lastSavedCommand < 1) {
                    lastSavedCommand = 1;
                }
                for (int i = commandNumber; i >= lastSavedCommand; i--) {
                    printf("%d %s", i, history[i%HISTORY_SIZE]);
                }
                shouldRunCommand = 0;
            } else if (strcmp(bfrCommand, "exit\n") == 0) {
                shouldRun = 0;
                shouldRunCommand = 0;
            }

            if (shouldRunCommand) {
                commandNumber++;
                strcpy(history[commandNumber%HISTORY_SIZE], bfrCommand);
                runCommand(bfrCommand);
            }
        }
                

        /**
        * After reading user input, the steps are:
        * (1) fork a child process using fork()
        * (2) the child process will invoke execvp()
        * (3) if command included &, parent will invoke wait() */
    }
    return 0;
}
