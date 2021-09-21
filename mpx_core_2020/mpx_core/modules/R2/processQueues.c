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

//pcb* findPCB(char name[]);

void addToReadyQueue(pcb* PCB){
	queue* a = getReadyQueue();
	a->count++;
	//means only entry
	if(a->count == 1){
		a->head = PCB;
		a->tail = PCB;
	}
	//low priority
	else if(PCB->priority <= a->tail->priority){
		a->tail->nextPCB = PCB;
		PCB->prevPCB = a->tail;
		a->tail = PCB;
	}
	else{
		pcb* temp = a->tail;
		while(PCB->priority > temp->priority){
			if(temp->prevPCB != a->head){
				temp = temp->prevPCB;
			}
			//new PCB is Head
			else{
				PCB->nextPCB = a->head;
				a->head = PCB;
			}
		}
	}
}
void addToBlockedQueue(pcb* PCB){
	queue* a = getBlockedQueue();
	a->count++;
	//only entry
	if(a->count == 1){
		a->head = PCB;
		a->tail = PCB;
	}
	else{
		a->tail->nextPCB = PCB;
		PCB->prevPCB = a->tail;
		a->tail = PCB;
	}
}

//int removeFromQueue(struct pcb PCB);

