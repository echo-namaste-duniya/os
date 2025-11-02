#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_WRITERS 2

// Shared variables
int reader_count = 0;
int shared_data = 0;

// Synchronization tools
pthread_mutex_t mutex;      // For reader_count access
pthread_mutex_t db;         // For database access
pthread_mutex_t reader_queue; // For reader priority

void *reader(void *param) {
    int id = *(int*)param;
    
    while(1) {
        // Enter reader queue
        pthread_mutex_lock(&reader_queue);
        
        // Update reader count
        pthread_mutex_lock(&mutex);
        reader_count++;
        if(reader_count == 1) {
            pthread_mutex_lock(&db); // First reader locks database
        }
        pthread_mutex_unlock(&mutex);
        
        pthread_mutex_unlock(&reader_queue);
        
        // Read data
        printf("Reader %d reads value: %d\n", id, shared_data);
        sleep(rand() % 2);
        
        // Update reader count
        pthread_mutex_lock(&mutex);
        reader_count--;
        if(reader_count == 0) {
            pthread_mutex_unlock(&db); // Last reader unlocks database
        }
        pthread_mutex_unlock(&mutex);
        
        // Random wait before next read
        sleep(rand() % 3);
    }
    return NULL;
}

void *writer(void *param) {
    int id = *(int*)param;
    
    while(1) {
        pthread_mutex_lock(&reader_queue); // Prevent new readers
        pthread_mutex_lock(&db);           // Get exclusive access
        
        // Write data
        shared_data++;
        printf("Writer %d writes value: %d\n", id, shared_data);
        sleep(rand() % 2);
        
        pthread_mutex_unlock(&db);
        pthread_mutex_unlock(&reader_queue);
        
        // Random wait before next write
        sleep(rand() % 3);
    }
    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];
    int reader_ids[NUM_READERS];
    int writer_ids[NUM_WRITERS];
    
    // Initialize mutexes
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&db, NULL);
    pthread_mutex_init(&reader_queue, NULL);
    
    printf("Readers-Writers Problem Simulation Started\n");
    printf("Number of readers: %d\n", NUM_READERS);
    printf("Number of writers: %d\n\n", NUM_WRITERS);
    
    // Create reader threads
    for(int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }
    
    // Create writer threads
    for(int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }
    
    // Join threads
    for(int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for(int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }
    
    // Cleanup
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&db);
    pthread_mutex_destroy(&reader_queue);
    
    return 0;
}