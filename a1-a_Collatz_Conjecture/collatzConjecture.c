//////////////////////////////
// Collatz Conjecture
// Author: Thomas Willingham
// Date: 2017 February
/////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int n = 0; // initialize counter 
    pid_t pid; // process variable

    if (argc == 2) { // check for starting number & convert to int
        n = atoi(argv[1]);
    } else {
        printf("Error: You must enter a single positive integer.\n");
        exit(1);
    }

    if (n <= 0) { // verify number is greater than 0
        printf("Error: Input must be greater than 0\n");
        exit(1);
    }
    
    pid = fork(); // duplicate the process

    if (pid == 0) { // check if process was created
        printf("%d ", n); 
        while (n != 1) { // loop for collatz until n=1
            if (n % 2 == 0) {
                n = n / 2;
            } else {
                n = (3 * n) + 1;
            }
            printf("%d ", n);
        }

        printf("\nChild Done!\n");
    } else {
        printf("Waiting on child...");
        int s;
        wait(&s);
        printf("Done!\n");
    }
    return 0;
}
