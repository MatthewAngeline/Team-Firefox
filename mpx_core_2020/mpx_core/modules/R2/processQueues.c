#include "processQueues.h"
#include "../R3/userCommandsR3.h"
#include "../mpx_supt.h"
#include "../include/system.h"
#include "../../include/string.h"
#include <../include/core/io.h>

int queueCountPtr=80;
//initializes the queues
queue readyQueue = {0,NULL,NULL};
queue blockedQueue = {0,NULL,NULL};

//returns pointer to the ready queue
queue* getReadyQueue(){
	return &readyQueue;
}

//returns pointer to the blocked queue
queue* getBlockedQueue(){
	return &blockedQueue;
}

// allocates memory for the new PCB
pcb* allocatePCB(){
	//int size = 1024;
	pcb *temp = sys_alloc_mem(sizeof(pcb));
	return temp;
}

// free memory given to the associated PCB
int freePCB(pcb* PCB){
	
	if(sys_free_mem(PCB) ==-1){
		return 0;
	}
	return 1;

}

// Creates an empty PCB and allocates its name, class and priority.
pcb* setupPCB(char name[], int pcbClass, int priority){
	pcb* p = allocatePCB();
	strcpy(p->name,name);
	p->pcbClass = pcbClass;
	p->priority = priority;
	p->nextPCB=NULL;
	p->prevPCB=NULL;
	p->stackHead = p->stackBase + 1024 - sizeof(context);
	strcpy(p->state,"Ready");
	strcpy(p->status,"Not Suspended");
	
	return p;
	
}

// Searches both the queues for a given PCB and returns the PCB if it finds it
pcb* findPCB(char name[]){
	queue* q = getReadyQueue();
	pcb* locator;
	//checks ready queue
	if(q->head != NULL){
		locator = q->head;
		//is the same
		if(strcmp(locator->name,name)==0){
			return locator;
		}
		//loops through the queue until found or each PCB in queue is visited
		while(locator->nextPCB != NULL){
			locator = locator->nextPCB;
			//is the same
			if(strcmp(locator->name,name)==0){
				return locator;
			}
		}
		
	}
	//checks blocked queue
	q = getBlockedQueue();
	if(q->head != NULL){
		locator = q->head;
		//is the same
		if(strcmp(locator->name,name)==0){
			return locator;
		}
		//loops through the queue until found or each PCB is visisted
		while(locator->nextPCB != NULL){ 
			locator = locator->nextPCB;
			//is the same
			if(strcmp(locator->name,name)==0){
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
	if(q->head == NULL){
		q->head = PCB;
		q->tail = PCB;
	}
	//lower priority than tail - inserted at tail
	else if(PCB->priority <= q->tail->priority){
		q->tail->nextPCB = PCB;
		PCB->prevPCB = q->tail;
		q->tail = PCB;
	}
	//higher priority than tail
	else{
		int cond =1;
		pcb* temp = q->tail;
		while(cond){
			//Found the location and inserts it, changing the condition to break the loop
			if(PCB->priority <= temp->priority){	
				PCB->nextPCB = temp->nextPCB;
				PCB->prevPCB = temp;
				temp->nextPCB->prevPCB = PCB;
				temp->nextPCB = PCB;
				cond = 0;
				}
			//moves to another PCB with higher or equal priority
			else if(temp->prevPCB != NULL){
				temp = temp->prevPCB;
				
			}
			//new PCB must be Head
			else{
				temp->prevPCB = PCB;
				PCB->nextPCB = temp;
				q->head = PCB;
				cond = 0;
			
				
			}
		}
	}
	strcpy(PCB->state,"Ready");
}
void addToBlockedQueue(pcb* PCB){
	queue* q = getBlockedQueue();
	q->count++;
	//it is the only entry
	if(q->count == 1){
		q->head = PCB;
		q->tail = PCB;
	}
	//adds to the tail of the queue
	else{
		q->tail->nextPCB = PCB;
		PCB->prevPCB = q->tail;
		q->tail = PCB;
	}
	strcpy(PCB->state,"Blocked");
}

//returns 0 on error, 1 on success
int removeFromQueue(pcb* PCB){
	queue* q;
	pcb* locator;
	//finds the correct queue based off of the PCB state
	if(strcmp(PCB->state,"Ready")==0){
		q = getReadyQueue();
	}
	else if (strcmp(PCB->state,"Blocked")==0){
		q = getBlockedQueue();
	}
	if(q->head != NULL)locator = q->head;
	//returns 0 if there is no PCB in queue
	else return 0;
	
	//loops until finding the PCB
	while(1){
		
		if(locator->name == PCB->name){
			//PCB is the head
			if(locator == q->head){
				locator->nextPCB->prevPCB = NULL;
				q->head = locator->nextPCB;
				locator->nextPCB = NULL;
				
			}
			//pcb is the tail
			else if (locator == q->tail){
				locator->prevPCB->nextPCB = NULL;
				q->tail = locator->prevPCB;
				locator->prevPCB = NULL;
			}
			//pcb is in the interior of the queue
			else{
				locator->prevPCB->nextPCB = locator->nextPCB;
				locator->nextPCB->prevPCB = locator->prevPCB;
				locator->nextPCB = NULL;
				locator->prevPCB = NULL;
			}
			//decrements count once removed
			q->count--;
			//returns 1 as a success
			return 1;
		}
		//PCB not found yet
		else{
			if(locator->nextPCB != NULL){
				locator=locator->nextPCB;
			}
			//returns error if not found and there is no other PCBs
			else{
				return 0;
			}
		}
		
	}

}

