#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>

#define SHM_SIZE 1024
#define SHM_KEY 12345
#define SEM_KEY 54321

// Structure for shared memory segment
typedef struct {
    int written;  // Flag to indicate if data is written
    char message[SHM_SIZE];
} shared_memory;

// Union for semaphore operations
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

// Initialize semaphore
int init_semaphore(int sem_id, int value) {
    union semun sem_union;
    sem_union.val = value;
    return semctl(sem_id, 0, SETVAL, sem_union);
}

// P (wait) operation
void sem_wait(int sem_id) {
    struct sembuf sb = {0, -1, 0};
    semop(sem_id, &sb, 1);
}

// V (signal) operation
void sem_signal(int sem_id) {
    struct sembuf sb = {0, 1, 0};
    semop(sem_id, &sb, 1);
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: %s [server|client]\n", argv[0]);
        exit(1);
    }
    
    // Create shared memory segment
    int shm_id = shmget(SHM_KEY, sizeof(shared_memory), IPC_CREAT | 0666);
    if(shm_id == -1) {
        perror("Shared memory creation failed");
        exit(1);
    }
    
    // Attach shared memory segment
    shared_memory *shm = (shared_memory *)shmat(shm_id, NULL, 0);
    if(shm == (void *)-1) {
        perror("Shared memory attachment failed");
        exit(1);
    }
    
    // Create semaphore
    int sem_id = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if(sem_id == -1) {
        perror("Semaphore creation failed");
        exit(1);
    }
    
    if(strcmp(argv[1], "server") == 0) {
        // Server Process
        printf("Server Process Started\n");
        
        // Initialize semaphore
        if(init_semaphore(sem_id, 1) == -1) {
            perror("Semaphore initialization failed");
            exit(1);
        }
        
        while(1) {
            // Wait for semaphore
            sem_wait(sem_id);
            
            printf("\nEnter message (or 'quit' to exit): ");
            fgets(shm->message, SHM_SIZE, stdin);
            shm->message[strcspn(shm->message, "\n")] = '\0';  // Remove newline
            
            shm->written = 1;  // Set written flag
            
            // Signal semaphore
            sem_signal(sem_id);
            
            if(strcmp(shm->message, "quit") == 0)
                break;
            
            sleep(1);  // Give client time to read
        }
        
        // Cleanup
        shmdt(shm);
        shmctl(shm_id, IPC_RMID, NULL);
        semctl(sem_id, 0, IPC_RMID);
        
    } else if(strcmp(argv[1], "client") == 0) {
        // Client Process
        printf("Client Process Started\n");
        
        while(1) {
            // Wait for semaphore
            sem_wait(sem_id);
            
            if(shm->written) {
                printf("Received message: %s\n", shm->message);
                shm->written = 0;  // Reset written flag
                
                if(strcmp(shm->message, "quit") == 0)
                    break;
            }
            
            // Signal semaphore
            sem_signal(sem_id);
            
            usleep(100000);  // Small delay to prevent busy waiting
        }
        
        // Detach from shared memory
        shmdt(shm);
        
    } else {
        printf("Invalid argument. Use 'server' or 'client'\n");
        exit(1);
    }
    
    return 0;
}