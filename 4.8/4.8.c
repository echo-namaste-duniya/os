#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_RESOURCES 10   // Total number of licenses
#define NUM_PROCESSES 8    // Number of processes trying to use licenses

// Shared data
int available_resources = MAX_RESOURCES;

// Synchronization tools
pthread_mutex_t mutex;
sem_t resources;

// Function to decrease available resources
int decrease_count(int count) {
    int success = 0;
    pthread_mutex_lock(&mutex);
    
    if(available_resources >= count) {
        available_resources -= count;
        success = 1;
        printf("Allocated %d resources. Remaining: %d\n", 
               count, available_resources);
    } else {
        printf("Failed to allocate %d resources (only %d available)\n", 
               count, available_resources);
    }
    
    pthread_mutex_unlock(&mutex);
    return success;
}

// Function to increase available resources
void increase_count(int count) {
    pthread_mutex_lock(&mutex);
    
    available_resources += count;
    printf("Released %d resources. Now available: %d\n", 
           count, available_resources);
    
    pthread_mutex_unlock(&mutex);
}

void *process(void *param) {
    int id = *(int*)param;
    
    while(1) {
        // Request random number of resources (1-5)
        int requested = rand() % 5 + 1;
        
        printf("Process %d requesting %d resources\n", id, requested);
        
        // Try to acquire resources
        while(!decrease_count(requested)) {
            printf("Process %d waiting for resources...\n", id);
            sleep(1);
        }
        
        // Use the resources (simulate work)
        printf("Process %d using %d resources\n", id, requested);
        sleep(rand() % 3 + 1);
        
        // Release the resources
        increase_count(requested);
        printf("Process %d released %d resources\n", id, requested);
        
        // Wait before next request
        sleep(rand() % 2);
    }
    return NULL;
}

int main() {
    pthread_t processes[NUM_PROCESSES];
    int process_ids[NUM_PROCESSES];
    
    // Initialize random seed
    srand(time(NULL));
    
    // Initialize synchronization tools
    pthread_mutex_init(&mutex, NULL);
    sem_init(&resources, 0, MAX_RESOURCES);
    
    printf("Resource Management System Started\n");
    printf("Total resources (licenses): %d\n", MAX_RESOURCES);
    printf("Number of processes: %d\n\n", NUM_PROCESSES);
    
    // Create process threads
    for(int i = 0; i < NUM_PROCESSES; i++) {
        process_ids[i] = i + 1;
        pthread_create(&processes[i], NULL, process, &process_ids[i]);
    }
    
    // Join threads
    for(int i = 0; i < NUM_PROCESSES; i++) {
        pthread_join(processes[i], NULL);
    }
    
    // Cleanup
    pthread_mutex_destroy(&mutex);
    sem_destroy(&resources);
    
    return 0;
}