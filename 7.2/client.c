#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

#define SHM_SIZE 1024

int main() {
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, SHM_SIZE, 0666);
    int semid = semget(key, 1, 0666);

    char *str = (char *)shmat(shmid, NULL, 0);

    printf("Client: Waiting for server to write message...\n");

    // Wait for server (decrement semaphore)
    struct sembuf sb = {0, -1, 0};
    semop(semid, &sb, 1);

    printf("Client: Message read from shared memory: %s\n", str);

    shmdt(str);
    return 0;
}
