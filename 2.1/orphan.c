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
        sleep(5); // Child waits, parent exits first
        printf("Child process (PID: %d), Parent PID now: %d\n", getpid(), getppid());
        printf("Child is now an orphan adopted by init/systemd!\n");
    }

    // Parent process
    else {
        printf("Parent process (PID: %d), child PID: %d\n", getpid(), pid);
        printf("Parent exiting immediately...\n");
        exit(0);  // Parent exits early
    }

    return 0;
}
