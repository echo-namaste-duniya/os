#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_READERS 8  // Number of clients checking flight info
#define NUM_WRITERS 3  // Number of clients making reservations
#define NUM_FLIGHTS 5  // Number of flights in database

// Shared data structure
struct Flight {
    int id;
    int available_seats;
};

struct Flight flights[NUM_FLIGHTS];
int reader_count = 0;

// Synchronization tools
pthread_mutex_t mutex;       // For reader_count access
pthread_mutex_t db;          // For database access
pthread_mutex_t reader_queue; // For reader priority

void initialize_flights() {
    for(int i = 0; i < NUM_FLIGHTS; i++) {
        flights[i].id = i + 1;
        flights[i].available_seats = 20; // Each flight has 20 seats initially
    }
}

void *flight_info_reader(void *param) {
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
        
        // Read flight information
        printf("Client %d checking flight information:\n", id);
        for(int i = 0; i < NUM_FLIGHTS; i++) {
            printf("Flight %d: %d seats available\n", 
                   flights[i].id, flights[i].available_seats);
        }
        printf("\n");
        sleep(rand() % 2);
        
        // Update reader count
        pthread_mutex_lock(&mutex);
        reader_count--;
        if(reader_count == 0) {
            pthread_mutex_unlock(&db); // Last reader unlocks database
        }
        pthread_mutex_unlock(&mutex);
        
        // Random wait before next check
        sleep(rand() % 3);
    }
    return NULL;
}

void *reservation_writer(void *param) {
    int id = *(int*)param;
    
    while(1) {
        pthread_mutex_lock(&reader_queue); // Prevent new readers
        pthread_mutex_lock(&db);           // Get exclusive access
        
        // Make reservation
        int flight = rand() % NUM_FLIGHTS;
        if(flights[flight].available_seats > 0) {
            flights[flight].available_seats--;
            printf("Client %d booked a seat on Flight %d. Seats remaining: %d\n",
                   id, flights[flight].id, flights[flight].available_seats);
        } else {
            printf("Client %d could not book Flight %d: No seats available\n",
                   id, flights[flight].id);
        }
        sleep(rand() % 2);
        
        pthread_mutex_unlock(&db);
        pthread_mutex_unlock(&reader_queue);
        
        // Random wait before next booking attempt
        sleep(rand() % 5);
    }
    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];
    int reader_ids[NUM_READERS];
    int writer_ids[NUM_WRITERS];
    
    // Initialize flights database
    initialize_flights();
    
    // Initialize mutexes
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&db, NULL);
    pthread_mutex_init(&reader_queue, NULL);
    
    printf("Airline Reservation System Simulation Started\n");
    printf("Number of flights: %d\n", NUM_FLIGHTS);
    printf("Number of clients checking flights: %d\n", NUM_READERS);
    printf("Number of clients making reservations: %d\n\n", NUM_WRITERS);
    
    // Create reader threads
    for(int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, flight_info_reader, &reader_ids[i]);
    }
    
    // Create writer threads
    for(int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, reservation_writer, &writer_ids[i]);
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