#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 256

// Function to reverse a string
void reverse_string(char *str) {
    int length = strlen(str);
    for(int i = 0; i < length/2; i++) {
        char temp = str[i];
        str[i] = str[length-1-i];
        str[length-1-i] = temp;
    }
}

int main() {
    int pipefd[2];  // Pipe file descriptors
    pid_t pid;
    char buffer[BUFFER_SIZE];
    
    // Create pipe
    if(pipe(pipefd) == -1) {
        perror("Pipe creation failed");
        exit(1);
    }
    
    pid = fork();
    if(pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    
    if(pid > 0) {  // Parent process
        // Close read end of pipe
        close(pipefd[0]);
        
        printf("Enter a string: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';  // Remove newline
        
        // Write string to pipe
        if(write(pipefd[1], buffer, strlen(buffer) + 1) == -1) {
            perror("Write to pipe failed");
            exit(1);
        }
        
        printf("Parent sent: %s\n", buffer);
        
        // Close write end of pipe
        close(pipefd[1]);
        
    } else {  // Child process
        // Close write end of pipe
        close(pipefd[1]);
        
        // Read string from pipe
        ssize_t bytes_read = read(pipefd[0], buffer, BUFFER_SIZE);
        if(bytes_read == -1) {
            perror("Read from pipe failed");
            exit(1);
        }
        
        // Reverse the string
        reverse_string(buffer);
        
        printf("Child received and reversed: %s\n", buffer);
        
        // Close read end of pipe
        close(pipefd[0]);
        exit(0);
    }
    
    return 0;
}