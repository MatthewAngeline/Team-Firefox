#ifndef _HEAPMANAGER_H
#define _HEAPMANAGER_H

#include "../mpx_supt.h"
typedef struct cmbc{
	//type 1 is alloc, 0 is free
	int type;
	u32int address;
	//size in bytes
	u32int size;
	char name[50];
	struct cmbc* nextCMBC;
	struct cmbc* prevCMBC;
}cmbc;



void intializeHeap(int size);

u32int allocateMemory(int size);

u32int freeMemory(cmbc* mem);

int isEmpty();

void showFreeMemory();

void showAllocatedMemory();
#endif
