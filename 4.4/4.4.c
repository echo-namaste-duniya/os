#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t db;
pthread_mutex_t rc_mutex;
int readcount = 0;

void *reader(void *arg) {
    int id = *(int *)arg;

    pthread_mutex_lock(&rc_mutex);
    readcount++;
    if (readcount == 1)
        sem_wait(&db);
    pthread_mutex_unlock(&rc_mutex);

    printf("Reader %d viewed reservation.\n", id);
    sleep(1);

    pthread_mutex_lock(&rc_mutex);
    readcount--;
    if (readcount == 0)
        sem_post(&db);
    pthread_mutex_unlock(&rc_mutex);

    return NULL;
}

void *writer(void *arg) {
    int id = *(int *)arg;

    sem_wait(&db);
    printf("Writer %d updated reservation.\n", id);
    sleep(2);
    sem_post(&db);

    return NULL;
}

int main() {
    pthread_t r[3], w[2];
    int rid[3] = {1, 2, 3}, wid[2] = {1, 2};

    sem_init(&db, 0, 1);
    pthread_mutex_init(&rc_mutex, NULL);

    // Create threads
    pthread_create(&r[0], NULL, reader, &rid[0]);
    pthread_create(&w[0], NULL, writer, &wid[0]);
    pthread_create(&r[1], NULL, reader, &rid[1]);
    pthread_create(&w[1], NULL, writer, &wid[1]);
    pthread_create(&r[2], NULL, reader, &rid[2]);

    // Wait for threads to finish
    for (int i = 0; i < 3; i++)
        pthread_join(r[i], NULL);
    for (int i = 0; i < 2; i++)
        pthread_join(w[i], NULL);

    sem_destroy(&db);
    pthread_mutex_destroy(&rc_mutex);

    return 0;
}
