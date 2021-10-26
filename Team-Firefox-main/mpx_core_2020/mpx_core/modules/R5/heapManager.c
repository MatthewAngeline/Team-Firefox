#include "heapManager.h"
#include "../../include/system.h"
#include "../../include/mem/heap.h"
int beginMem;
cmbc* beginFree;
cmbc* beginAlloc;

void intializeHeap(int size){
	beginMem = kmalloc(size+sizeof(struct cmbc));
	beginFree->address = beginMem + sizeof(struct cmbc);
	beginFree->size = size;
	beginFree->type = 0;
	beginFree->nextCMBC = NULL;
	beginFree->prevCMBC = NULL;
}

u32int allocateMemory(int size){
	cmbc* locator = beginFree;
	int sential = 1;
	do{
	if(beginFree->size <= (u32int)size){
		
	}
	else{
		if(locator->nextCMBC == NULL) return 0;
		else locator = locator->nextCMBC;
	}
	} while(sential);
}

u32int freeMemory(cmbc* mem){
	
}

int isEmpty(){

}

void showFreeMemory(){

}

void showAllocatedMemory(){

}
