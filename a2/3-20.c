#include <stdio.h>
#include <stdlib.h>

#define MIN_PID 300
#define MAX_PID 5000

int *pids;
int numPidsAllocated;
int nextPid;


int allocate_map()
{
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
    if (numPidsAllocated == MAX_PID-MIN_PID+1) return -1;
    while (pids[nextPid-MIN_PID]) {
        nextPid++;
        if (nextPid > MAX_PID) nextPid = MIN_PID;
    }
    pids[nextPid] = 1;
    numPidsAllocated++;
    return nextPid;
}

void release_pid(int pid) {
    if (pid >= MIN_PID && pid <= MAX_PID && pids[pid-MIN_PID]) {
        pids[pid-MIN_PID] = 0;
        numPidsAllocated--;
    }
}
