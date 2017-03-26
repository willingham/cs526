/////////////////////////////////////
// CS526: Operating Systems
// Assignment 4: Sleeping TA
//
// Author: Thomas Willingham
// Date: 2017 March
/////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define NUM_CHAIRS 3
#define MAX_SLEEP 10

int numStudents;              // entered by user
int chairs[NUM_CHAIRS];       // the chairs to keep track of
int curStudent = 0;           // current chair for a student to sit in
int curChair = 0;             // current chair for a ta to look at
int numStudentsInChairs = 0;  // total number filled chairs

pthread_mutex_t mutex;        // main mutex
pthread_t thread_ta;          // thread for TA process
pthread_t *thread_st;         // thread for student process
sem_t sem_ta, sem_st;         // semaphores for TA and student

void *teachingAssistant();
void *student(void *ST);


int main(int argc, char **argv)
{
    if (argc != 2) {   // check for correct running of the program
        printf("Usage: <program> <number of students>\n");
        exit(1);
    } else { 
        numStudents = atoi(argv[1]);
        printf("Creating %d student threads\n", numStudents);
    }

    thread_st = (pthread_t*)malloc(sizeof(pthread_t)*numStudents);      // allocate memory for student threads
    pthread_mutex_init(&mutex, NULL);
    sem_init(&sem_ta, 0, 0);            // init ta semaphore to 0
    sem_init(&sem_st, 0, 1);            // init student semaphore to 1
    time_t t;
    srand((unsigned) time(&t));         // seed random

    pthread_create(&thread_ta, NULL, teachingAssistant, NULL);  // create ta thread
    for (int i=0; i<numStudents; i++) pthread_create(&thread_st[i], NULL, student, (void*)(int) i+1);  // create student threads
    
    pthread_join(thread_ta, NULL);  // join ta thread
    for (int i=0; i<numStudents; i++) pthread_join(thread_st[i], NULL);  // join student threads

    return 0;
}


void *teachingAssistant()
{
    while (1) {
        sem_wait(&sem_ta);  // wait until TA is woken up
        pthread_mutex_lock(&mutex);  // get a lock
        printf("      TA - is teaching student %d.\n", chairs[curChair]);
        numStudentsInChairs--;
        chairs[curChair] = 0;  // 'empty' the current chair
        curChair = (curChair % NUM_CHAIRS) + 1;  // determine the next chair
        printf("STUDENTS - %d, %d, and %d are waiting in chairs.\n", chairs[0], chairs[1], chairs[2]);
        pthread_mutex_unlock(&mutex);
        sleep((rand() % MAX_SLEEP) + 1);  // sleep between 1 and MAX_SLEEP seconds
        printf("      TA - finished teaching student.\n");
        sem_post(&sem_st);  // signal student
    }
}


void *student(void *id)
{
    printf(" STUDENT - %d is programming.\n", (int)id);
    while (1) {
        sleep((rand() % MAX_SLEEP) + 1);  // sleep between 1 and MAX_SLEEP seconds
        pthread_mutex_lock(&mutex);       // get a lock
        if (NUM_CHAIRS <= numStudentsInChairs) {
            pthread_mutex_unlock(&mutex);  // unlock if no available chairs
            printf("  CHAIRS - full; STUDENT %d programming.\n", (int)id);
        } else {
            chairs[curStudent] = (int)id;   // 'sitting down' in the current chair
            numStudentsInChairs++;
            printf(" STUDENT - %d is sitting in a chair.\n", chairs[curStudent]);
            printf("STUDENTS - %d, %d, and %d are waiting in chairs.\n", chairs[0], chairs[1], chairs[2]);
            curStudent = (curStudent % NUM_CHAIRS) + 1;  // determine next student
            pthread_mutex_unlock(&mutex);
            sem_post(&sem_ta);
            sem_wait(&sem_st);
        }
    }
}
