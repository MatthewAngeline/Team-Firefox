#ifndef _HEAPMANAGER_H
#define _HEAPMANAGER_H

#include "../mpx_supt.h"
typedef struct cmcb{
	//type 1 is alloc, 0 is free
	int type;
	u32int address;
	//size in bytes
	u32int size;
	char name[50];
	struct cmcb* nextCMCB;
	struct cmcb* prevCMCB;
}cmcb;

void merge(cmcb* free);
cmcb* placeInFreeList(cmcb* toAdd);
cmcb* placeInAllocList(cmcb* toAdd);
cmcb* findCMCB(u32int addr);
void intializeHeap(int size);

u32int allocateMemory(int size);

u32int freeMemory(u32int addr);

int isEmpty();

void showFreeMemory();

void showAllocatedMemory();
#endif
