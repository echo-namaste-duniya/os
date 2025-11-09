#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_KEY 0x1234
#define SIZE 100

int main() {
    while(1){
    int shmid;
    char *shmaddr;

    // Wait to ensure server writes first
    sleep(5);

    // Access existing shared memory
    shmid = shmget(SHM_KEY, SIZE, 0666);
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
    // Read message
    printf("Client: Message from server -> %s\n", shmaddr);
    
    // Detach shared memory
    shmdt(shmaddr);
    sleep(5);
    }
    return 0;
}
