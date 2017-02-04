#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int n = 0;
    pid_t pid;

    if (argc == 2) {
        n = atoi(argv[1]);
    } else {
        printf("Error: You must enter a single positive integer.\n");
        exit(1);
    }

    if (n <= 0) {
        printf("Error: Input must be greater than 0\n");
        exit(1);
    }
    
    pid = fork();

    if (pid == 0) {
        printf("%d ", n);
        while (n != 1) {
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
