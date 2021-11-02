#include "heapManager.h"
#include "../R1/Comhandle.h"
#include "../../include/system.h"
#include "../../include/string.h"
#include "../../include/mem/heap.h"
int beginMem;
cmcb* beginFree;
cmcb* beginAlloc;


void intializeHeap(int size){
	beginMem = kmalloc(size+sizeof(struct cmcb));
	beginFree = (cmcb*) beginMem;
	beginFree->address = beginMem + sizeof(struct cmcb);
	beginFree->size = size;
	beginFree->type = 0;
	beginFree->nextCMCB = NULL;
	beginFree->prevCMCB = NULL;
}
//need to find out how to alloc (sysallocmem?)
u32int allocateMemory(int size){
	int count = 100;
	cmcb* locator = beginFree;
	int sential = 1;
	do{
	if(locator->size >= (u32int)size){
		cmcb* newFree = (cmcb*) locator->address + (u32int)size + sizeof(struct cmcb);
		newFree->size = locator->size - (u32int) size;
		locator->size = (u32int) size;
		newFree->address = locator->address + (u32int)size + sizeof(struct cmcb);
		newFree->type = 0;
		placeInList(beginFree, newFree);
		
		locator->type=1;
		sential = 0;
	}
	else{
		if(locator->nextCMCB == NULL) {
			sys_req(WRITE,DEFAULT_DEVICE,"No available CMCB's, could not allocate.\n",&count);
			return 0;
			}
		else locator = locator->nextCMCB;
	}
	} while(sential);
	placeInList(beginAlloc, locator);
	
	return locator->address;
}


cmcb* findCMCB(u32int addr){
	cmcb* locator = beginFree;
	while(locator!=NULL){
		if(locator->address==addr) return locator;
		else locator = locator->nextCMCB;
	}
	locator = beginAlloc;
	while(locator!=NULL){
		if(locator->address==addr)return locator;
		else locator = locator->nextCMCB;
	}
	return NULL;	

}

//still need to find out how to free mem (sysfreemem?)
u32int freeMemory(u32int addr){
	cmcb* block = findCMCB(addr);
	
	block->type = 0;
	
	placeInList(beginFree, block);
	return 0;
	
}

int isEmpty(){
	if(beginAlloc==NULL)return 1;
	else return 0;
}

//print u32int?
void showFreeMemory(){
	int count = 100;
	
	cmcb* block = beginFree;
	while(block!=NULL){
		char sentence[200];
		itoa((int)block->address, sentence);
		sys_req(WRITE,DEFAULT_DEVICE,sentence,&count);
		sys_req(WRITE,DEFAULT_DEVICE,"\nsize: ",&count);
		itoa((int)block->size, sentence);
		sys_req(WRITE,DEFAULT_DEVICE,sentence,&count);
		block=block->nextCMCB;
	}
	
}
//print u32int?
void showAllocatedMemory(){
	int count = 100;
	
	cmcb* block = beginAlloc;
	while(block!=NULL){
		char sentence[200];
		itoa((int)block->address, sentence);
		sys_req(WRITE,DEFAULT_DEVICE,sentence,&count);
		sys_req(WRITE,DEFAULT_DEVICE,"\nsize: ",&count);
		itoa((int)block->size, sentence);
		sys_req(WRITE,DEFAULT_DEVICE,sentence,&count);
		block=block->nextCMCB;
	}
	
}

cmcb* placeInList(cmcb* head, cmcb* toAdd){
	//organize other list for remove
	if(toAdd->prevCMCB != NULL) toAdd->prevCMCB->nextCMCB = toAdd->nextCMCB;
	if(toAdd->nextCMCB != NULL) toAdd->nextCMCB->prevCMCB = toAdd->prevCMCB;
	//add to head's lists
	cmcb* locator = head;
	while(locator != NULL){
		if(toAdd->address < locator->address){
			toAdd->nextCMCB = locator;
			toAdd->prevCMCB = locator->prevCMCB;
			if(locator->prevCMCB != NULL) locator->prevCMCB->nextCMCB = toAdd;
			locator->prevCMCB = toAdd;
			return toAdd;
		}
		if(locator->nextCMCB==NULL){
			locator->nextCMCB=toAdd;
			toAdd->prevCMCB = locator;
			toAdd->nextCMCB = NULL;
			return toAdd;
		}
		
	}
	
	return NULL;
}
