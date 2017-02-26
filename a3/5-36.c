#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define MIN_PID 300
#define MAX_PID 5000

int *pids;
int numPidsAllocated;
int nextPid;

pthread_mutex_t pidMapMutex;

int allocate_map()
{
    pthread_mutex_init(&pidMapMutex, NULL);
    numPidsAllocated = 0;
    nextPid = MIN_PID;
    pids = malloc(sizeof(int)*(MAX_PID-MIN_PID+1));
    if (!pids) return -1;

    for (int i=0; i<=(MAX_PID-MIN_PID); i++) {
        pids[i] = 0;
    }
    return 1;
}

int allocate_pid()
{
    pthread_mutex_lock(&pidMapMutex);
    if (numPidsAllocated == MAX_PID-MIN_PID+1) return -1;
    while (pids[nextPid-MIN_PID]) {
        nextPid++;
        if (nextPid > MAX_PID) nextPid = MIN_PID;
    }
    pids[nextPid-MIN_PID] = 1;
    numPidsAllocated++;
    pthread_mutex_unlock(&pidMapMutex);
    return nextPid;
}

void release_pid(int pid) {
    pthread_mutex_lock(&pidMapMutex);
    if (pid >= MIN_PID && pid <= MAX_PID && pids[pid-MIN_PID]) {
        pids[pid-MIN_PID] = 0;
        numPidsAllocated--;
    }
    pthread_mutex_unlock(&pidMapMutex);
}

void *requestPid(void *p)
{
    int pid = allocate_pid();
    printf("Thread started with pid %d\n", pid);
    sleep(rand()%10 + 1);
    printf("Thread releasing pid %d\n", pid);
    release_pid(pid);
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("usage: <program> <number of threads>\n");
        return 1;
    }
    int numThreads = atoi(argv[1]);
    time_t t;
    allocate_map();
    srand((unsigned) time(&t));
    pthread_t *threads = malloc(sizeof(pthread_t)*numThreads);
    for (int i=0; i < numThreads; i++) {
       pthread_create(&threads[i], NULL, requestPid, NULL);
    }
    for (int i=0; i < numThreads; i++) {
       pthread_join(threads[i], NULL);
    }
    return 1;
}


