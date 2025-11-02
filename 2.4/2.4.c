#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void generateSequence(int n) {
    printf("%d ", n);
    while(n != 1) {
        if(n % 2 == 0) {
            n = n / 2;
        } else {
            n = 3 * n + 1;
        }
        printf("%d ", n);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    // Check if exactly one argument is provided
    if(argc != 2) {
        printf("Usage: %s positive_integer\n", argv[0]);
        return 1;
    }

    // Convert argument to integer and validate
    int n = atoi(argv[1]);
    if(n <= 0) {
        printf("Error: Please provide a positive integer\n");
        return 1;
    }

    pid_t pid = fork();

    if(pid < 0) {
        printf("Fork failed!\n");
        return 1;
    }
    else if(pid == 0) {
        // Child process
        printf("Child process generating sequence starting from %d:\n", n);
        generateSequence(n);
        exit(0);
    }
    else {
        // Parent process
        printf("Parent waiting for child to complete...\n");
        wait(NULL);
        printf("Parent process completed\n");
    }

    return 0;
}