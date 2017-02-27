//////////////////////////////////////
// Thomas Willingham
// Date: 2017 Feb
// Operating Systems Ch. 5, Proj. 3
/////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define BUFFER_SIZE 5

typedef int buffer_item;
buffer_item buffer[BUFFER_SIZE];
pthread_mutex_t mutex;
sem_t full, empty;
int counter;

void *producer(void *param);
int insertItem(buffer_item item);
void *consumer(void *param);
int removeItem(buffer_item *item);

int main(int argc, char **argv)
{
    if (argc != 4) {
        printf("usage: <program> <sleep time> <number of producer> <number of consumer>\n");
        exit(1);
    }
    int sleepTime = atoi(argv[1]);
    int numProducer = atoi(argv[2]);
    int numConsumer = atoi(argv[3]);

    time_t t;
    srand((unsigned) time(&t)); // generate random seed
    pthread_mutex_init(&mutex, NULL);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);
    counter = 0;

    pthread_t *producers = malloc(sizeof(pthread_t)*numProducer);
    pthread_t *consumers = malloc(sizeof(pthread_t)*numConsumer);

    for (int i=0; i < numProducer; i++) {
        pthread_create(&producers[i], NULL, producer, NULL);
    }
    for (int i=0; i < numConsumer; i++) {
        pthread_create(&consumers[i], NULL, consumer, NULL);
    }

    sleep(sleepTime);
    return 0;
}

int insertItem(buffer_item item)
{
    int success;
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);
    if (counter < BUFFER_SIZE) {
        buffer[counter] = item;
        counter++;
        success = 0;
    } else {
        success = -1;
    }
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
    return success;
}

int removeItem(buffer_item *item)
{
    int success;
    sem_wait(&full);
    pthread_mutex_lock(&mutex);
    if (counter > 0) {
        *item = buffer[counter - 1];
        counter--;
        success = 0;
    } else {
        success = -1;
    }
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    return success;
}

void *producer(void *param)
{
    buffer_item item; 
    while (1) {
        sleep(rand() % 5 + 1);
        item = rand();

        if (insertItem(item)) {
            fprintf(stderr, "Producer error: Buffer full\n");
        } else {
            printf("Producer produced %d\n", item);
        }
    }
}

void *consumer(void *param) {
    buffer_item item;

    while (1) {
        sleep(rand() % 5 + 1);

        if (removeItem(&item)) {
            fprintf(stderr, "Consumer error: Buffer empty\n");
        } else {
            printf("Consumer consumed %d\n", item);
        }
    }
}
