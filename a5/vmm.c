//////////////////////////////////////////
// CS526: Operating Systems
// Assignment 5: Virtual Memory Manager
//
// Author: Thomas Willingham
// Date 2017 April
//////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PAGE_TABLE_ENTRY_SIZE 256
#define PAGE_SIZE 256
#define TLB_ENTRY_SIZE 16
#define FRAME_SIZE 256
#define LOGICAL_MEMORY_SIZE 66536
#define PHYSICAL_FRAME_COUNT (PHYSICAL_MEMORY_SIZE / FRAME_SIZE)

void doStartup(int argc, char *argv[]);
void getPhysicalMemorySize();
void doVirtualMemorySimulation();
void doTeardown();

typedef struct page
{
    int frame;
    bool valid;
} page;

typedef struct tlbEntry
{
    int frame;
    int page;
    bool valid;
} tlbEntry;

FILE *backingStore = NULL;;
FILE *addresses = NULL;
unsigned int PHYSICAL_MEMORY_SIZE = 65536; //32768;

int main(int argc, char *argv[])
{
    doStartup(argc, argv);
    printf("Startup done\n");
    doVirtualMemorySimulation();
    doTeardown();
}

void getPhysicalMemorySize() {
    char physicalMemorySizeOption[2] = {0, '\0'};
    printf("Physical address space size: \n");
    printf("        (default) 0 = 65536\n");
    printf("                  1 = 32768\n");
    fflush(stdout);
    fgets(physicalMemorySizeOption, 2, stdin);
    if (physicalMemorySizeOption[0] == '1') PHYSICAL_MEMORY_SIZE = 32768;
}

void doStartup(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "usage: <program> <address input file>");
        exit(0);
    }

    printf("############################################\n");
    printf("#               Starting the               #\n"); 
    printf("#   Super Awesome Virtual Memory Manager   #\n");
    printf("############################################\n");

    getPhysicalMemorySize();

    addresses = fopen(argv[1], "r");
    backingStore = fopen("BACKING_STORE.bin", "r");
}

void doVirtualMemorySimulation()
{
    int totalAccesses = 0;
    int totalFaults = 0;
    int totalHits = 0;

    const page pageDefault = { .valid = false};
    char memoryPhysical[PHYSICAL_MEMORY_SIZE];

    page pageTable[PAGE_TABLE_ENTRY_SIZE];
    int free[PHYSICAL_FRAME_COUNT];

    for (int i=0; i < PAGE_TABLE_ENTRY_SIZE; i++) {
        pageTable[i] = pageDefault;
    }

    tlbEntry tlb[TLB_ENTRY_SIZE];
    char line[256];
    int nextFrame = 0, tlbHead = 0, tlbSize = 0;
    int logicalAddress = 0, pageNum = 0, pageOff = 0, frameNum = -1;

    while (fgets(line, sizeof(line), addresses) != NULL) {
        logicalAddress = atoi(line);
        pageNum = (logicalAddress & 0x0000FFFF) >> 8;
        pageOff = (logicalAddress & 0x000000FF);

        frameNum = -1;

        if (tlbSize > 0) {
            int cur = 0;
            for (int i=0; i < tlbSize; i++) {
                cur = (i + tlbHead) % tlbSize;
                if (tlb[cur].valid && tlb[cur].page == pageNum) {
                    totalHits++;
                    frameNum = tlb[cur].frame;
                }
            }
        }
        if (frameNum == -1) {
            if (pageTable[pageNum].valid) {
                frameNum = pageTable[pageNum].frame;
            } else {
                totalFaults++;
                frameNum = nextFrame;

                if (free[frameNum] != 0) {
                    for (int i=0; i < PAGE_TABLE_ENTRY_SIZE; i++) {
                        if (pageTable[i].valid && pageTable[i].frame == frameNum) {
                            pageTable[i].valid = false;
                            break;
                        }
                    }
                    for (int i=0; i < tlbSize; i++) {
                        if (tlb[i].valid && tlb[i].frame == frameNum) {
                            tlb[i].valid = false;
                            break;
                        }
                    }
                }
                fseek(backingStore, PAGE_TABLE_ENTRY_SIZE * pageNum, SEEK_SET);
                fread(&memoryPhysical[FRAME_SIZE * frameNum], 1, PAGE_TABLE_ENTRY_SIZE, backingStore);

                pageTable[pageNum].frame = frameNum;
                pageTable[pageNum].valid = 1;
                free[frameNum] = 1;
                nextFrame++;
                nextFrame = nextFrame % PHYSICAL_FRAME_COUNT;
            }

            tlbEntry newEntry = 
            {
                .page = pageNum,
                .frame = frameNum,
                .valid = true
            };

            tlb[tlbHead] = newEntry;
            tlbHead++;
            tlbHead = tlbHead % TLB_ENTRY_SIZE;
            if (tlbSize < TLB_ENTRY_SIZE) tlbSize++;
        }

        int physicalAddress = (FRAME_SIZE * frameNum) + pageOff;
        char value = memoryPhysical[physicalAddress];
        printf("Virtual address: %d Physical address: %d Value: %d\n", logicalAddress, physicalAddress, value);
        totalAccesses++;
    }

    double faultRate = totalFaults * 1.0 / totalAccesses;
    double hitRate = totalHits * 1.0 / totalAccesses;

    printf("Number of Translated Addresses = %d\n", totalAccesses);
    printf("Page Faults = %d\n", totalFaults);
    printf("Page Fault Rate = %.3f\n", faultRate);
    printf("TLB Hits = %d\n", totalHits);
    printf("TLB Hit Rate = %.3f\n",  hitRate);
}

void doTeardown()
{
    fclose(addresses);
    fclose(backingStore);
}

