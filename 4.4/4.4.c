#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_CHAIRS 5      // Number of waiting room chairs
#define NUM_CUSTOMERS 10  // Total number of customers

// Synchronization tools
sem_t customers;     // Counts waiting customers
sem_t barber;       // Barber's state (0=idle, 1=busy)
pthread_mutex_t mutex; // Mutex for seats access

int waiting = 0;    // Number of customers waiting

void *barber_actions(void *param) {
    while(1) {
        // Wait for a customer
        sem_wait(&customers);
        
        // Get mutex to update waiting count
        pthread_mutex_lock(&mutex);
        waiting--;
        
        // Signal barber is ready for customer
        sem_post(&barber);
        pthread_mutex_unlock(&mutex);
        
        // Cut hair (simulation)
        printf("Barber is cutting hair\n");
        sleep(rand() % 3);
        printf("Barber finished haircut\n");
    }
    return NULL;
}

void *customer_actions(void *param) {
    int id = *(int*)param;
    
    while(1) {
        pthread_mutex_lock(&mutex);
        
        if(waiting < NUM_CHAIRS) {
            waiting++;
            printf("Customer %d sat in waiting room. Customers waiting = %d\n", id, waiting);
            
            // Signal customer arrival
            sem_post(&customers);
            pthread_mutex_unlock(&mutex);
            
            // Wait for barber
            sem_wait(&barber);
            printf("Customer %d is getting haircut\n", id);
            
        } else {
            pthread_mutex_unlock(&mutex);
            printf("Customer %d left - waiting room full\n", id);
        }
        
        // Customer waits before next haircut
        sleep(rand() % 10 + 10);
    }
    return NULL;
}

int main() {
    pthread_t barber_thread;
    pthread_t customer_threads[NUM_CUSTOMERS];
    int customer_ids[NUM_CUSTOMERS];
    
    // Initialize semaphores and mutex
    sem_init(&customers, 0, 0);  // Initially no customers
    sem_init(&barber, 0, 0);     // Barber starts idle
    pthread_mutex_init(&mutex, NULL);
    
    printf("Sleeping Barber Problem Simulation Started\n");
    printf("Number of waiting room chairs: %d\n", NUM_CHAIRS);
    printf("Number of customers: %d\n\n", NUM_CUSTOMERS);
    
    // Create barber thread
    pthread_create(&barber_thread, NULL, barber_actions, NULL);
    
    // Create customer threads
    for(int i = 0; i < NUM_CUSTOMERS; i++) {
        customer_ids[i] = i + 1;
        pthread_create(&customer_threads[i], NULL, customer_actions, &customer_ids[i]);
    }
    
    // Join threads
    pthread_join(barber_thread, NULL);
    for(int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_join(customer_threads[i], NULL);
    }
    
    // Cleanup
    sem_destroy(&customers);
    sem_destroy(&barber);
    pthread_mutex_destroy(&mutex);
    
    return 0;
}