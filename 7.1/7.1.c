#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 256

// Function to reverse case of each character
void reverse_case(char *str) {
    for(int i = 0; str[i] != '\0'; i++) {
        if(isupper(str[i]))
            str[i] = tolower(str[i]);
        else if(islower(str[i]))
            str[i] = toupper(str[i]);
    }
}

int main() {
    int pipe1[2];  // Parent to Child pipe
    int pipe2[2];  // Child to Parent pipe
    pid_t pid;
    char buffer[BUFFER_SIZE];
    
    // Create pipes
    if(pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Pipe creation failed");
        exit(1);
    }
    
    pid = fork();
    if(pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    
    if(pid > 0) {  // Parent process
        // Close unused pipe ends
        close(pipe1[0]);  // Close read end of pipe1
        close(pipe2[1]);  // Close write end of pipe2
        
        while(1) {
            printf("\nEnter a message (or 'quit' to exit): ");
            fgets(buffer, BUFFER_SIZE, stdin);
            buffer[strcspn(buffer, "\n")] = '\0';  // Remove newline
            
            // Send message to child
            write(pipe1[1], buffer, strlen(buffer) + 1);
            
            if(strcmp(buffer, "quit") == 0)
                break;
            
            // Read modified message from child
            read(pipe2[0], buffer, BUFFER_SIZE);
            printf("Received modified message: %s\n", buffer);
        }
        
        // Close remaining pipe ends
        close(pipe1[1]);
        close(pipe2[0]);
        
    } else {  // Child process
        // Close unused pipe ends
        close(pipe1[1]);  // Close write end of pipe1
        close(pipe2[0]);  // Close read end of pipe2
        
        while(1) {
            // Read message from parent
            read(pipe1[0], buffer, BUFFER_SIZE);
            
            if(strcmp(buffer, "quit") == 0)
                break;
            
            // Reverse case of each character
            reverse_case(buffer);
            
            // Send modified message back to parent
            write(pipe2[1], buffer, strlen(buffer) + 1);
        }
        
        // Close remaining pipe ends
        close(pipe1[0]);
        close(pipe2[1]);
        exit(0);
    }
    
    return 0;
}