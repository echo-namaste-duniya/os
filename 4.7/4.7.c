#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

#define NUM_READERS 10    // Number of clients searching flights
#define NUM_WRITERS 5     // Number of clients booking flights
#define NUM_FLIGHTS 10    // Number of flights
#define NUM_CITIES 5      // Number of cities
#define MAX_SEATS 50      // Maximum seats per flight

// Cities in the system
const char* cities[] = {"New York", "London", "Tokyo", "Paris", "Dubai"};

// Flight structure
typedef struct {
    int id;
    char source[20];
    char destination[20];
    int available_seats;
    float price;
} Flight;

// Shared data
Flight flights[NUM_FLIGHTS];
int reader_count = 0;

// Synchronization tools
pthread_mutex_t mutex;        // For reader_count access
sem_t db;                    // For database access
pthread_mutex_t reader_queue; // For reader priority

void initialize_flights() {
    for(int i = 0; i < NUM_FLIGHTS; i++) {
        flights[i].id = i + 1;
        strcpy(flights[i].source, cities[rand() % NUM_CITIES]);
        do {
            strcpy(flights[i].destination, cities[rand() % NUM_CITIES]);
        } while(strcmp(flights[i].source, flights[i].destination) == 0);
        flights[i].available_seats = rand() % MAX_SEATS + 1;
        flights[i].price = (float)(rand() % 1000) + 200.0f;
    }
}

void display_flight(Flight* flight) {
    printf("Flight %d: %s -> %s, Seats: %d, Price: $%.2f\n",
           flight->id, flight->source, flight->destination,
           flight->available_seats, flight->price);
}

void *flight_searcher(void *param) {
    int id = *(int*)param;
    
    while(1) {
        // Enter reader queue
        pthread_mutex_lock(&reader_queue);
        
        // Update reader count
        pthread_mutex_lock(&mutex);
        reader_count++;
        if(reader_count == 1) {
            sem_wait(&db); // First reader locks database
        }
        pthread_mutex_unlock(&mutex);
        
        pthread_mutex_unlock(&reader_queue);
        
        // Search flights
        printf("\nClient %d searching flights:\n", id);
        // Simulate searching by criteria
        char* search_city = cities[rand() % NUM_CITIES];
        printf("Searching flights to/from %s:\n", search_city);
        
        for(int i = 0; i < NUM_FLIGHTS; i++) {
            if(strcmp(flights[i].source, search_city) == 0 ||
               strcmp(flights[i].destination, search_city) == 0) {
                display_flight(&flights[i]);
            }
        }
        sleep(rand() % 2);
        
        // Update reader count
        pthread_mutex_lock(&mutex);
        reader_count--;
        if(reader_count == 0) {
            sem_post(&db); // Last reader unlocks database
        }
        pthread_mutex_unlock(&mutex);
        
        // Random wait before next search
        sleep(rand() % 4);
    }
    return NULL;
}

void *flight_booker(void *param) {
    int id = *(int*)param;
    
    while(1) {
        pthread_mutex_lock(&reader_queue); // Prevent new readers
        sem_wait(&db);                    // Get exclusive access
        
        // Try to book a flight
        int flight_idx = rand() % NUM_FLIGHTS;
        Flight* flight = &flights[flight_idx];
        
        if(flight->available_seats > 0) {
            flight->available_seats--;
            printf("\nClient %d booked Flight %d:\n", id, flight->id);
            display_flight(flight);
        } else {
            printf("\nClient %d failed to book Flight %d (No seats available)\n",
                   id, flight->id);
        }
        
        sleep(rand() % 2);
        
        sem_post(&db);
        pthread_mutex_unlock(&reader_queue);
        
        // Random wait before next booking attempt
        sleep(rand() % 6);
    }
    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];
    int reader_ids[NUM_READERS];
    int writer_ids[NUM_WRITERS];
    
    // Seed random number generator
    srand(time(NULL));
    
    // Initialize flights
    initialize_flights();
    
    // Initialize synchronization tools
    pthread_mutex_init(&mutex, NULL);
    sem_init(&db, 0, 1);
    pthread_mutex_init(&reader_queue, NULL);
    
    printf("Advanced Airline Reservation System Started\n");
    printf("Number of Flights: %d\n", NUM_FLIGHTS);
    printf("Number of Searching Clients: %d\n", NUM_READERS);
    printf("Number of Booking Clients: %d\n\n", NUM_WRITERS);
    
    // Display initial flight data
    printf("Initial Flight Information:\n");
    for(int i = 0; i < NUM_FLIGHTS; i++) {
        display_flight(&flights[i]);
    }
    printf("\n");
    
    // Create reader threads
    for(int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, flight_searcher, &reader_ids[i]);
    }
    
    // Create writer threads
    for(int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, flight_booker, &writer_ids[i]);
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
    sem_destroy(&db);
    pthread_mutex_destroy(&reader_queue);
    
    return 0;
}