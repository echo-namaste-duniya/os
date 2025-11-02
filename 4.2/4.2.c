#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_CHAIRS 3     // Number of chairs in hallway
#define NUM_STUDENTS 10  // Total number of students

// Synchronization tools
sem_t ta_sem;          // For TA's sleep/wakeup
sem_t student_sem;     // For student waiting
pthread_mutex_t mutex; // For chairs access

int waiting = 0;       // Number of students waiting

void *ta_actions(void *param) {
    while(1) {
        // TA is sleeping, waiting for a student
        printf("TA is sleeping\n");
        sem_wait(&ta_sem);
        
        // Critical section
        pthread_mutex_lock(&mutex);
        if(waiting > 0) {
            waiting--;
            // Signal student to enter office
            sem_post(&student_sem);
        }
        pthread_mutex_unlock(&mutex);
        
        // Help student (simulation)
        printf("TA is helping a student\n");
        sleep(rand() % 5);
    }
    return NULL;
}

void *student_actions(void *param) {
    int id = *(int*)param;
    while(1) {
        pthread_mutex_lock(&mutex);
        
        if(waiting < NUM_CHAIRS) {  // If there are empty chairs
            waiting++;
            printf("Student %d sat in hallway. Students waiting = %d\n", id, waiting);
            pthread_mutex_unlock(&mutex);
            
            // Wake up TA
            sem_post(&ta_sem);
            
            // Wait to be helped
            sem_wait(&student_sem);
            
            printf("Student %d is getting help from the TA\n", id);
            sleep(rand() % 5);  // Getting help
        } else {
            pthread_mutex_unlock(&mutex);
            printf("Student %d will try later\n", id);
        }
        
        // Student programs for a while before next visit
        sleep(rand() % 10);
    }
    return NULL;
}

int main() {
    pthread_t ta_thread;
    pthread_t student_threads[NUM_STUDENTS];
    int student_ids[NUM_STUDENTS];
    
    // Initialize semaphores and mutex
    sem_init(&ta_sem, 0, 0);      // TA is initially sleeping
    sem_init(&student_sem, 0, 0);  // No student is initially being helped
    pthread_mutex_init(&mutex, NULL);
    
    printf("Teaching Assistant Office Hours Simulation Started\n");
    printf("Number of chairs in hallway: %d\n", NUM_CHAIRS);
    printf("Number of students: %d\n\n", NUM_STUDENTS);
    
    // Create TA thread
    pthread_create(&ta_thread, NULL, ta_actions, NULL);
    
    // Create student threads
    for(int i = 0; i < NUM_STUDENTS; i++) {
        student_ids[i] = i + 1;
        pthread_create(&student_threads[i], NULL, student_actions, &student_ids[i]);
    }
    
    // Join threads
    pthread_join(ta_thread, NULL);
    for(int i = 0; i < NUM_STUDENTS; i++) {
        pthread_join(student_threads[i], NULL);
    }
    
    // Cleanup
    sem_destroy(&ta_sem);
    sem_destroy(&student_sem);
    pthread_mutex_destroy(&mutex);
    
    return 0;
}