#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_CHAIRS 3  // Number of chairs in the hallway

sem_t students;    // Counts waiting students
sem_t ta;          // TA availability semaphore
pthread_mutex_t mutex; // Mutex to protect waiting_students counter

int waiting_students = 0;

// ----------------------------------
// TA Function
// ----------------------------------
void *ta_work(void *arg) {
    while (1) {
        // Wait for a student to arrive
        sem_wait(&students);

        // Protect waiting_students variable
        pthread_mutex_lock(&mutex);
        waiting_students--;
        printf("TA starts helping a student. Students waiting: %d\n", waiting_students);
        pthread_mutex_unlock(&mutex);

        // Signal that TA is busy (available to one student)
        sem_post(&ta);

        // Simulate helping a student
        sleep(rand() % 3 + 1);

        printf("TA finished helping a student and is ready for the next one.\n\n");
    }
    return NULL;
}

// ----------------------------------
// Student Function
// ----------------------------------
void *student_activity(void *arg) {
    int id = *(int *)arg;

    while (1) {
        // Student is programming (not at TA office)
        sleep(rand() % 5 + 1);

        pthread_mutex_lock(&mutex);
        if (waiting_students < MAX_CHAIRS) {
            waiting_students++;
            printf("Student %d is waiting. Students waiting: %d\n", id, waiting_students);

            // Student notifies TA that they need help
            sem_post(&students);
            pthread_mutex_unlock(&mutex);

            // Wait until TA is ready
            sem_wait(&ta);
            printf("Student %d is getting help from TA.\n", id);
        } else {
            // No chairs available
            pthread_mutex_unlock(&mutex);
            printf("Student %d found no empty chair and will come back later.\n", id);
        }
    }
    return NULL;
}

// ----------------------------------
// Main Function
// ----------------------------------
int main() {
    srand(time(NULL));

    int num_students;
    printf("Enter number of students: ");
    scanf("%d", &num_students);

    pthread_t ta_thread;
    pthread_t student_threads[num_students];
    int student_ids[num_students];

    // Initialize semaphores and mutex
    sem_init(&students, 0, 0);
    sem_init(&ta, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    // Create TA thread
    pthread_create(&ta_thread, NULL, ta_work, NULL);

    // Create student threads
    for (int i = 0; i < num_students; i++) {
        student_ids[i] = i + 1;
        pthread_create(&student_threads[i], NULL, student_activity, &student_ids[i]);
    }

    // Join threads (infinite loop, for simulation)
    pthread_join(ta_thread, NULL);
    for (int i = 0; i < num_students; i++) {
        pthread_join(student_threads[i], NULL);
    }

    // Cleanup
    sem_destroy(&students);
    sem_destroy(&ta);
    pthread_mutex_destroy(&mutex);

    return 0;
}
