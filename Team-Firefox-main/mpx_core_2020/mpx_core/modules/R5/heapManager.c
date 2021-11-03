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
	beginAlloc = NULL;
	beginFree->address = beginMem + sizeof(struct cmcb);
	beginFree->size = size;
	beginFree->type = 0;
	beginFree->nextCMCB = NULL;
	beginFree->prevCMCB = NULL;
}

u32int allocateMemory(int size){
	int count = 100;
	cmcb* locator = beginFree;
	int sential = 1;
	do{
	if(locator->size >= (u32int)size){
		cmcb* newFree = (cmcb*) (locator->address + (u32int)size);
		newFree->address = locator->address + (u32int)size + sizeof(struct cmcb);
		newFree->size = locator->size - (u32int) size - sizeof(struct cmcb);
		locator->size = (u32int) size;
		newFree->type=0;
		locator->type=1;
		placeInList(beginFree, newFree);
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
	
	/*char sentence[100];
	itoa(beginFree->address, sentence);
	sys_req(WRITE,DEFAULT_DEVICE,sentence,&count);
	itoa(beginAlloc->address, sentence);
	sys_req(WRITE,DEFAULT_DEVICE,"\n",&count);
	sys_req(WRITE,DEFAULT_DEVICE,sentence,&count);*/
	
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
	// if list is empty
	cmcb* locator;
	if(head == beginFree) locator = beginFree;
	else locator = beginAlloc;
	if(locator == NULL) {
		toAdd->prevCMCB = NULL;
		
		toAdd->nextCMCB = NULL;
		if(head == beginFree) {
		beginFree = toAdd;
		}
		else {
		beginAlloc = toAdd;
		}
		
		/*locator = toAdd;
		locator->prevCMCB = NULL;
		locator->nextCMCB = NULL;*/
	}
	else {
		//organize other list for remove
		if(toAdd->prevCMCB != NULL) toAdd->prevCMCB->nextCMCB = toAdd->nextCMCB;
		if(toAdd->nextCMCB != NULL) toAdd->nextCMCB->prevCMCB = toAdd->prevCMCB;
		//add to head's lists
		
		while(locator != NULL){
			if(toAdd->address < locator->address){
				toAdd->nextCMCB = locator;
				toAdd->prevCMCB = locator->prevCMCB;
				if(locator->prevCMCB != NULL) locator->prevCMCB->nextCMCB = toAdd;
				locator->prevCMCB = toAdd;
				break;
			}
			if(locator->nextCMCB==NULL){
				locator->nextCMCB=toAdd;
				toAdd->prevCMCB = locator;
				toAdd->nextCMCB = NULL;
				break;
			}
			
		}
	}
	return toAdd;
}

