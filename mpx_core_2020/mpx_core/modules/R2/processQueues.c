#include "processQueues.h"
#include <stddef.h>


/*struct pcb {
	char name[50];
	char class[50];
	//higher number = higher priority 0-9
	int priority;
	//Ready, blocked, running and suspended, not suspended
	char state[50];
	unsigned char* stack;
	//next is towards tail of queue, prev is towards head
	pcb* nextPCB;
	pcb* prevPCB;
};

struct stack{
	unsigned char* head;
	unsigned char* base;
	
};
struct queue{
	int count;
	pcb* head;
	pcb* tail;
};
*/
queue readyQueue = {0,NULL,NULL};
queue blockedQueue = {0,NULL,NULL};
queue* getReadyQueue(){
	return &readyQueue;
}
queue* getBlockedQueue(){
	return &blockedQueue;
}
//int allocatePCB();

//int freePCB(struct pcb PCB);

//pcb* setupPCB(char name[], char pcbClass[], int priority);

pcb* findPCB(char name[]){
	queue* q = getReadyQueue();
	pcb* locator;
	if(q->head != NULL){
		locator = q->head;
		//is the same
		if(locator->name==name){
			return locator;
		}
		while(locator->nextPCB != NULL){
			locator = locator->nextPCB;
			if(locator->name==name){
				return locator;
			}
		}
		
	}
	q = getBlockedQueue();
	if(q->head != NULL){
		locator = q->head;
		//is the same
		if(locator->name==name){
			return locator;
		}
		while(locator->nextPCB != NULL){
			locator = locator->nextPCB;
			if(locator->name==name){
				return locator;
			}
		}
		
	}
	return NULL;
}

void addToReadyQueue(pcb* PCB){
	queue* q = getReadyQueue();
	q->count++;
	//means only entry
	if(q->count == 1){
		q->head = PCB;
		q->tail = PCB;
	}
	//low priority
	else if(PCB->priority <= q->tail->priority){
		q->tail->nextPCB = PCB;
		PCB->prevPCB = q->tail;
		q->tail = PCB;
	}
	else{
		pcb* temp = q->tail;
		while(PCB->priority > temp->priority){
			if(temp->prevPCB != q->head){
				temp = temp->prevPCB;
			}
			//new PCB is Head
			else{
				PCB->nextPCB = q->head;
				q->head = PCB;
			}
		}
	}
}
void addToBlockedQueue(pcb* PCB){
	queue* q = getBlockedQueue();
	q->count++;
	//only entry
	if(q->count == 1){
		q->head = PCB;
		q->tail = PCB;
	}
	else{
		q->tail->nextPCB = PCB;
		PCB->prevPCB = q->tail;
		q->tail = PCB;
	}
}

//returns 0 on error, 1 on success
int removeFromQueue(pcb* PCB){
	char readyNotSuspend[] = "ReadyNotSuspended";
	char readySuspend[] = "ReadySuspended";
	char blockedNotSuspend[] = "BlockedNotSuspended";
	char blockedSuspend[] = "BlockedSuspended";
	queue* q;
	pcb* locator;
	if(PCB->state == readyNotSuspend || PCB->state == readySuspend){
		q = getReadyQueue();
	}
	else if (PCB->state == blockedNotSuspend || PCB->state == blockedSuspend){
		q = getBlockedQueue();
	}
	if(q->head != NULL)locator = q->head;
	else return 0;
	

	while(1){
		if(locator->name == PCB->name){
			if(locator == q->head){
				locator->nextPCB->prevPCB = NULL;
				q->head = locator->nextPCB;
				
			}
			else if (locator == q->tail){
				locator->prevPCB->nextPCB = NULL;
				q->tail = locator->prevPCB;
				
			}
			
			else{
				locator->prevPCB->nextPCB = locator->nextPCB;
				locator->nextPCB->prevPCB = locator->prevPCB;
			}
			q->count--;
			return 1;
		}
		else{
			if(locator->nextPCB != NULL){
				locator=locator->nextPCB;
			}
			else{
				return 0;
			}
		}
		
	}

}

