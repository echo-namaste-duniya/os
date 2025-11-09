#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

#define SHM_KEY 0x1234
#define SIZE 100

int main() {
    while(1){
    int shmid;
    char *shmaddr;

    // Create shared memory segment
    shmid = shmget(SHM_KEY, SIZE, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    // Attach shared memory
    shmaddr = (char *)shmat(shmid, NULL, 0);
    if (shmaddr == (char *)-1) {
        perror("shmat");
        exit(1);
    }
    
    // Write message
    printf("Enter message to send to client: ");
    fgets(shmaddr, SIZE, stdin);

    printf("Server: Message written to shared memory.\n");
    printf("Server: Waiting for client to read...\n");

    sleep(5);  // Give client time to read
    
    // Detach shared memory
    shmdt(shmaddr);

    printf("Server: Done.\n");
}
    return 0;
}
