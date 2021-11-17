#include "heapManager.h"
#include "../R1/Comhandle.h"
#include "../../include/system.h"
#include "../../include/string.h"
#include "../../include/mem/heap.h"
int beginMem;
cmcb* beginFree;
cmcb* beginAlloc;
char sentence[20];

cmcb* getAlloc(){
return beginAlloc;
}
//initialized the head 
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
//allocates memory of a specific size
u32int allocateMemory(u32int size){
	int count = 100;
	cmcb* locator = beginFree;
	int sential = 1;
	do{
	if(locator->size >= (u32int)size){
	if(locator->nextCMCB == NULL){
		cmcb* newFree = (cmcb*) (locator->address + (u32int)size);
		newFree->address = locator->address + (u32int)size + (u32int)sizeof(struct cmcb);
		newFree->size = locator->size - (u32int) size - sizeof(struct cmcb);
		locator->size = (u32int) size;
		newFree->type=0;
		locator->type=1;
		placeInAllocList(locator);
		placeInFreeList(newFree);
		}
		else{
			locator->type = 0;
			locator->prevCMCB->nextCMCB = locator->nextCMCB;
			locator->nextCMCB->prevCMCB = locator->prevCMCB;
			placeInAllocList(locator);
		}
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

//locates a specific cmcb with the given address 
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
int freeMemory(void* addr){
	int count = 100;
	cmcb* block = findCMCB((u32int)addr);
	if(block == NULL){
	sys_req(WRITE,DEFAULT_DEVICE,"This address does not exsist, try a different address",&count);
	return NULL;
	}
	block->type = 0;
	
	placeInFreeList(block);
	return 0;
	
}
//returns 1 if empty 0 if theres an item at the head
int isEmpty(){
	if(beginAlloc==NULL)return 1;
	else return 0;
}

//shows the free memory list
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
//shows the allocated memory list
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
//This adds the cmcb into the free list and removes it if its also located in the allocated list
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
		if(toAdd->prevCMCB == NULL){
			if(toAdd->nextCMCB == NULL){
				beginAlloc = NULL;
				
			}
			else{
				beginAlloc = toAdd->nextCMCB;
			}
		}
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
		toAdd = merge(toAdd);
		if(beginFree-> address >= toAdd->address){
		beginFree = toAdd;
		}
		
		return locator;
	}

}
//if 2 adjacent cmcbs are in the free memory it will automatically merge them together
cmcb* merge(cmcb* free){
	
	if(free->nextCMCB != NULL){
		if(free->address + free->size +(u32int)sizeof(struct cmcb) == free->nextCMCB->address){
		free->size = free->size+free->nextCMCB->size+(u32int)sizeof(struct cmcb);
		if(free->nextCMCB->nextCMCB != NULL){
			free->nextCMCB->nextCMCB->prevCMCB = free;
		}
		free->nextCMCB = free->nextCMCB->nextCMCB;
	}
	 
	 
	}
	if(free->prevCMCB != NULL){
		free = free->prevCMCB;
		if(free->address + free->size +(u32int)sizeof(struct cmcb) == free->nextCMCB->address){
		free->size = free->size+free->nextCMCB->size+(u32int)sizeof(struct cmcb);
		if(free->nextCMCB->nextCMCB != NULL){
			free->nextCMCB->nextCMCB->prevCMCB = free;
		}
		free->nextCMCB = free->nextCMCB->nextCMCB;
	}
	 
	
}
return free;

}
//places the cmcb into the allocated list.
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
		if(beginFree -> address == toAdd -> address){
		if(beginFree -> nextCMCB != NULL){
			beginFree = beginFree->nextCMCB;
		}
		else{
		beginFree = NULL;
		}
	}
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
	
return locator;
}

