#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5  // Size of the pizza counter

// Shared buffer and variables
int pizza_counter[BUFFER_SIZE];
int in = 0, out = 0;

// Synchronization tools
sem_t empty;      // Counts empty buffer slots
sem_t full;       // Counts full buffer slots
pthread_mutex_t mutex;  // Ensures mutual exclusion to critical section

void *simpson_produce(void *arg) {
    int pizza_no = 1;
    
    while(1) {
        // Wait for empty slot
        sem_wait(&empty);
        
        // Get mutex lock
        pthread_mutex_lock(&mutex);
        
        // Produce pizza
        pizza_counter[in] = pizza_no;
        printf("Mr. Simpson baked Pizza #%d\n", pizza_no);
        
        // Move insertion point
        in = (in + 1) % BUFFER_SIZE;
        pizza_no++;
        
        // Release mutex lock
        pthread_mutex_unlock(&mutex);
        
        // Signal full slot
        sem_post(&full);
        
        // Random wait to simulate baking time
        sleep(rand() % 3);
    }
    return NULL;
}

void *joey_consume(void *arg) {
    while(1) {
        // Wait for full slot
        sem_wait(&full);
        
        // Get mutex lock
        pthread_mutex_lock(&mutex);
        
        // Consume pizza
        printf("Joey consumed Pizza #%d\n", pizza_counter[out]);
        
        // Move removal point
        out = (out + 1) % BUFFER_SIZE;
        
        // Release mutex lock
        pthread_mutex_unlock(&mutex);
        
        // Signal empty slot
        sem_post(&empty);
        
        // Random wait to simulate eating time
        sleep(rand() % 2);
    }
    return NULL;
}

int main() {
    pthread_t simpson_thread, joey_thread;
    
    // Initialize synchronization tools
    sem_init(&empty, 0, BUFFER_SIZE);  // Initially, all slots are empty
    sem_init(&full, 0, 0);             // Initially, no slots are full
    pthread_mutex_init(&mutex, NULL);   // Initialize mutex
    
    printf("Pizza Counter Simulation Started\n");
    printf("Buffer Size: %d\n\n", BUFFER_SIZE);
    
    // Create producer and consumer threads
    pthread_create(&simpson_thread, NULL, simpson_produce, NULL);
    pthread_create(&joey_thread, NULL, joey_consume, NULL);
    
    // Join threads (this program runs indefinitely)
    pthread_join(simpson_thread, NULL);
    pthread_join(joey_thread, NULL);
    
    // Cleanup
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    
    return 0;
}