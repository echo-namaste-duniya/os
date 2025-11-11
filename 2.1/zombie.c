#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }

    // Child process
    else if (pid == 0) {
        printf("Child process started (PID: %d)\n", getpid());
        printf("Child exiting now...\n");
        exit(0);  // Child finishes immediately
    }

    // Parent process
    else {
        printf("Parent process (PID: %d), child PID: %d\n", getpid(), pid);
        printf("Parent sleeping for 10 seconds...\n");
        sleep(10); // Child becomes zombie during this time

        // Check process status in another terminal
        // Command: ps -l | grep <your-username>

        printf("Parent now exiting after sleep.\n");
    }

    return 0;
}
