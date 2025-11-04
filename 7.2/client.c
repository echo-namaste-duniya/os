#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

#define SHM_KEY 0x1234
#define SEM_KEY 0x5678
#define SIZE 128

int main() {
    int shmid, semid;
    char *shmaddr;

    // Access existing shared memory
    shmid = shmget(SHM_KEY, SIZE, 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach to shared memory
    shmaddr = (char *)shmat(shmid, NULL, 0);
    if (shmaddr == (char *)-1) {
        perror("shmat");
        exit(1);
    }

    // Access semaphore
    semid = semget(SEM_KEY, 1, 0666);
    if (semid == -1) {
        perror("semget");
        exit(1);
    }

    // Wait until server signals (decrement semaphore)
    struct sembuf sb = {0, -1, 0};
    semop(semid, &sb, 1);

    printf("Client read from shared memory: %s\n", shmaddr);

    // Detach and clean up
    shmdt(shmaddr);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);
    printf("Shared memory and semaphore cleaned up.\n");

    return 0;
}
