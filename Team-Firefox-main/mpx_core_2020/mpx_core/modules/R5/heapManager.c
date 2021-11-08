#include "heapManager.h"
#include "../R1/Comhandle.h"
#include "../../include/system.h"
#include "../../include/string.h"
#include "../../include/mem/heap.h"
int beginMem;
cmcb* beginFree;
cmcb* beginAlloc;
char sentence[20];



void intializeHeap(int size){
	beginMem = kmalloc(size+sizeof(struct cmcb));
	beginFree = (cmcb*) beginMem;
	beginAlloc = NULL;
	beginFree->address = beginMem + sizeof(struct cmcb);
	beginFree->size = (u32int) size;
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
		newFree->address = locator->address + (u32int)size + (u32int)sizeof(struct cmcb);
		newFree->size = locator->size - (u32int) size - sizeof(struct cmcb);
		locator->size = (u32int) size;
		newFree->type=0;
		locator->type=1;
		placeInAllocList(locator);
		placeInFreeList(newFree);
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
	int count = 100;
	cmcb* block = findCMCB(addr);
	if(block == NULL){
	sys_req(WRITE,DEFAULT_DEVICE,"This address does not exsist, try a different address",&count);
	return NULL;
	}
	block->type = 0;
	
	placeInFreeList(block);
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
		itoa((int)block->address, sentence);
		sys_req(WRITE,DEFAULT_DEVICE,sentence,&count);
		sys_req(WRITE,DEFAULT_DEVICE," size: ",&count);
		itoa((int)block->size, sentence);
		sys_req(WRITE,DEFAULT_DEVICE,sentence,&count);
		sys_req(WRITE,DEFAULT_DEVICE,"\n",&count);
		block=block->nextCMCB;
	}
	
}
//print u32int?
void showAllocatedMemory(){
	int count = 100;
	cmcb* block = beginAlloc;
	
	while(block!=NULL){
		itoa((int)block->address, sentence);
		sys_req(WRITE,DEFAULT_DEVICE,sentence,&count);
		sys_req(WRITE,DEFAULT_DEVICE," size: ",&count);
		itoa((int)block->size, sentence);
		sys_req(WRITE,DEFAULT_DEVICE,sentence,&count);
		sys_req(WRITE,DEFAULT_DEVICE,"\n",&count);
		block=block->nextCMCB;
	}
	
}
cmcb* placeInFreeList(cmcb* toAdd){
	cmcb* locator = beginFree;
	if(locator == NULL){
	
		if(toAdd->prevCMCB != NULL) toAdd->prevCMCB->nextCMCB = toAdd->nextCMCB;
		if(toAdd->nextCMCB != NULL) toAdd->nextCMCB->prevCMCB = toAdd->prevCMCB;
		
		toAdd->prevCMCB = NULL;
		toAdd->nextCMCB = NULL;
		beginFree = toAdd;
		return beginFree;
	}
	else{
		if(toAdd->prevCMCB != NULL) toAdd->prevCMCB->nextCMCB = toAdd->nextCMCB;
		if(toAdd->nextCMCB != NULL) toAdd->nextCMCB->prevCMCB = toAdd->prevCMCB;
		//add to head's lists
		//cmcb* locator = head;
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
			else{
			locator = locator->nextCMCB;
			}
		}
		merge(toAdd);
		if(beginFree-> address < toAdd->address){
		beginFree = toAdd;
		}
		
		return locator;
	}

}
void merge(cmcb* free){

	if(free->prevCMCB->address + free->prevCMCB->size + (u32int)sizeof(struct cmcb) == free->address){
	free->size = free->size + free->prevCMCB->size;
	free->prevCMCB->nextCMCB = free->nextCMCB; 
	free = free->prevCMCB;
	
	}
	if(free->address + free->size +(u32int)sizeof(struct cmcb) == free->nextCMCB->address){
	free->size = free->size+free->nextCMCB->size;
	free->nextCMCB = free->nextCMCB->nextCMCB;
	
	}

}
cmcb* placeInAllocList(cmcb* toAdd){
	cmcb* locator = beginAlloc;
	if(locator == NULL){
	
		if(toAdd->prevCMCB != NULL) toAdd->prevCMCB->nextCMCB = toAdd->nextCMCB;
		if(toAdd->nextCMCB != NULL) toAdd->nextCMCB->prevCMCB = toAdd->prevCMCB;
		
		toAdd->prevCMCB = NULL;
		toAdd->nextCMCB = NULL;
		beginAlloc = toAdd;
		
		beginFree = NULL;
		return beginAlloc;
	}
	else{
		if(toAdd->prevCMCB != NULL) toAdd->prevCMCB->nextCMCB = toAdd->nextCMCB;
		if(toAdd->nextCMCB != NULL) toAdd->nextCMCB->prevCMCB = toAdd->prevCMCB;
		//add to head's lists
		//cmcb* locator = head;
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
			else{
			locator = locator->nextCMCB;
			}
		}
		
	}
	if(beginFree -> address == toAdd -> address){
		if(beginFree -> nextCMCB != NULL){
			beginFree = beginFree->nextCMCB;
		}
		else{
		beginFree = NULL;
		}
	}
return locator;
}

