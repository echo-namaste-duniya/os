#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

bool isPrime(int n) {
    if(n <= 1) return false;
    if(n <= 3) return true;
    
    if(n % 2 == 0 || n % 3 == 0) return false;
    
    for(int i = 5; i * i <= n; i += 6) {
        if(n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

void generatePrimes(int count) {
    int found = 0;
    int num = 2;
    
    while(found < count) {
        if(isPrime(num)) {
            printf("%d ", num);
            found++;
        }
        num++;
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    // Check if exactly one argument is provided
    if(argc != 2) {
        printf("Usage: %s count\n", argv[0]);
        return 1;
    }

    // Convert argument to integer and validate
    int count = atoi(argv[1]);
    if(count <= 0) {
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
        printf("Child process generating first %d prime numbers:\n", count);
        generatePrimes(count);
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