#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>

#define FIFO_NAME "message_queue"
#define BUFFER_SIZE 256

// Convert string to uppercase
void to_uppercase(char *str) {
    for(int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: %s [sender|receiver]\n", argv[0]);
        exit(1);
    }
    
    // Create FIFO if it doesn't exist
    if(mkfifo(FIFO_NAME, 0666) == -1) {
        if(errno != EEXIST) {
            perror("mkfifo failed");
            exit(1);
        }
    }
    
    if(strcmp(argv[1], "sender") == 0) {
        // Sender Process
        printf("Sender Process Started\n");
        
        int fd = open(FIFO_NAME, O_WRONLY);
        if(fd == -1) {
            perror("Failed to open FIFO for writing");
            exit(1);
        }
        
        char buffer[BUFFER_SIZE];
        while(1) {
            printf("\nEnter message (or 'quit' to exit): ");
            fgets(buffer, BUFFER_SIZE, stdin);
            buffer[strcspn(buffer, "\n")] = '\0';  // Remove newline
            
            if(write(fd, buffer, strlen(buffer) + 1) == -1) {
                perror("Write to FIFO failed");
                break;
            }
            
            if(strcmp(buffer, "quit") == 0)
                break;
        }
        
        close(fd);
        
    } else if(strcmp(argv[1], "receiver") == 0) {
        // Receiver Process
        printf("Receiver Process Started\n");
        
        int fd = open(FIFO_NAME, O_RDONLY);
        if(fd == -1) {
            perror("Failed to open FIFO for reading");
            exit(1);
        }
        
        char buffer[BUFFER_SIZE];
        while(1) {
            ssize_t bytes_read = read(fd, buffer, BUFFER_SIZE);
            if(bytes_read <= 0) {
                perror("Read from FIFO failed");
                break;
            }
            
            if(strcmp(buffer, "quit") == 0)
                break;
            
            to_uppercase(buffer);
            printf("Received and converted: %s\n", buffer);
        }
        
        close(fd);
        
    } else {
        printf("Invalid argument. Use 'sender' or 'receiver'\n");
        exit(1);
    }
    
    // Clean up FIFO
    if(strcmp(argv[1], "receiver") == 0) {
        unlink(FIFO_NAME);
    }
    
    return 0;
}