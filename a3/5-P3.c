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

    srand(time(NULL)); // generate random seed
    pthread_mutex_init(&mutex, NULL);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);
    counter = 0;
    //printf("numProducer: %d, numConsumer: %d\n", numProducer, numConsumer);

    pthread_t producers[numProducer];
    pthread_t consumers[numConsumer];

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
    if (counter < BUFFER_SIZE) {
        buffer[counter] = item;
        counter++;
        return 0;
    } else {
        return -1;
    }
}

int removeItem(buffer_item *item)
{
    if (counter > 0) {
        *item = buffer[counter - 1];
        counter--;
        return 0;
    } else {
        return -1;
    }
}

void *producer(void *param)
{
    buffer_item item; 
    while (1) {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        sleep(rand() % 5 + 1);
        item = rand();

        if (insertItem(item)) {
            fprintf(stderr, "Producer error\n");
        } else {
            printf("Producer produced %d\n", item);
        }
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

void *consumer(void *param) {
    buffer_item item;

    while (1) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        sleep(rand() % 5 + 1);

        if (removeItem(&item)) {
            fprintf(stderr, "Consumer error\n");
        } else {
            printf("Consumer consumed %d\n", item);
        }
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}
