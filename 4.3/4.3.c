#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define FILENAME "numbers.txt"

// Synchronization tools
sem_t ready;          // Binary semaphore for value ready
pthread_mutex_t mutex; // Mutex for file access

void *producer(void *param) {
    FILE *file;
    int number;
    
    while(1) {
        // Generate random number between 0 and 9
        number = rand() % 10;
        
        // Get mutex lock for file access
        pthread_mutex_lock(&mutex);
        
        // Write to file
        file = fopen(FILENAME, "w");
        if(file != NULL) {
            fprintf(file, "%d", number);
            fclose(file);
            printf("Producer wrote: %d\n", number);
        }
        
        pthread_mutex_unlock(&mutex);
        
        // Signal that value is ready
        sem_post(&ready);
        
        // Sleep for random time (0-100ms)
        usleep((rand() % 101) * 1000);
    }
    return NULL;
}

void *consumer(void *param) {
    FILE *file;
    int number;
    
    while(1) {
        // Wait for value to be ready
        sem_wait(&ready);
        
        // Get mutex lock for file access
        pthread_mutex_lock(&mutex);
        
        // Read from file
        file = fopen(FILENAME, "r");
        if(file != NULL) {
            fscanf(file, "%d", &number);
            fclose(file);
            printf("Consumer read: %d\n", number);
        }
        
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;
    
    // Initialize random seed
    srand(time(NULL));
    
    // Initialize synchronization tools
    sem_init(&ready, 0, 0);      // Initially, no value is ready
    pthread_mutex_init(&mutex, NULL);
    
    printf("Producer-Consumer File I/O Simulation Started\n\n");
    
    // Create threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);
    
    // Join threads
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);
    
    // Cleanup
    sem_destroy(&ready);
    pthread_mutex_destroy(&mutex);
    
    return 0;
}