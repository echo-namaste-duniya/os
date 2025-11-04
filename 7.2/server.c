#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

#define SHM_KEY 0x1234
#define SEM_KEY 0x5678
#define SIZE 128

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main() {
    int shmid, semid;
    char *shmaddr;

    // Create shared memory
    shmid = shmget(SHM_KEY, SIZE, IPC_CREAT | 0666);
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

    // Create semaphore
    semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        exit(1);
    }

    // Initialize semaphore to 0 (client waits)
    union semun sem_union;
    sem_union.val = 0;
    semctl(semid, 0, SETVAL, sem_union);

    printf("Enter a message to write in shared memory: ");
    fgets(shmaddr, SIZE, stdin);
    shmaddr[strcspn(shmaddr, "\n")] = '\0';

    printf("Message written to shared memory.\n");

    // Signal client (increment semaphore)
    struct sembuf sb = {0, 1, 0};
    semop(semid, &sb, 1);

    printf("Server signaled client and is exiting...\n");

    shmdt(shmaddr);
    return 0;
}
