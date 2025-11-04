#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

#define SHM_SIZE 1024

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main() {
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    int semid = semget(key, 1, 0666 | IPC_CREAT);

    char *str = (char *)shmat(shmid, NULL, 0);

    // Initialize semaphore to 0 (Client waits)
    union semun sem_union;
    sem_union.val = 0;
    semctl(semid, 0, SETVAL, sem_union);

    printf("Server: Enter message to write into shared memory: ");
    fgets(str, SHM_SIZE, stdin);
    str[strcspn(str, "\n")] = '\0'; // remove newline

    printf("Server: Message written to shared memory.\n");

    // Signal client (increment semaphore)
    struct sembuf sb = {0, 1, 0};
    semop(semid, &sb, 1);

    // Wait before cleanup
    sleep(5);

    // Cleanup
    shmdt(str);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID, sem_union);
    printf("Server: Cleanup done.\n");

    return 0;
}
