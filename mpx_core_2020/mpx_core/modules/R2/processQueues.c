#include "processQueues.h"
//#include <stddef.h>
#include "../mpx_supt.h"
#include "../include/system.h"
#include "../../include/string.h"
#include <../include/core/io.h>

int queueCountPtr=80;

queue readyQueue = {0,NULL,NULL};
queue blockedQueue = {0,NULL,NULL};
queue* getReadyQueue(){
	return &readyQueue;
}
queue* getBlockedQueue(){
	return &blockedQueue;
}
pcb* allocatePCB(){
	//int size = 1024;
	pcb *temp = sys_alloc_mem(sizeof(pcb));
	return temp;
}

int freePCB(pcb* PCB){
	
	if(sys_free_mem(PCB) ==-1){
		return 0;
	}
	return 1;

}

pcb* setupPCB(char name[], int pcbClass, int priority){
	pcb* p = allocatePCB();
	if(findPCB(name) == NULL && strlen(name)>=8 ){
		strcpy(p->name,name);

		}
	else{
		sys_req(WRITE,DEFAULT_DEVICE,"Invalid input1\n",&queueCountPtr);
		return NULL;

}
	if(pcbClass == 0 || pcbClass == 1){
	p->pcbClass = pcbClass;
	}
	else{
		sys_req(WRITE,DEFAULT_DEVICE,"Invalid input2\n",&queueCountPtr);
		return NULL;
	}
	if(priority>=0 && priority <10){
	p->priority = priority;
	}
	else{
		sys_req(WRITE,DEFAULT_DEVICE,"Invalid input3\n",&queueCountPtr);
		return NULL;
	}
	strcpy(p->state,"Ready");
	
	strcpy(p->status,"Not Suspended");
	
	return p;
	
}

pcb* findPCB(char name[]){
	queue* q = getReadyQueue();
	pcb* locator;
	if(q->head != NULL){
		locator = q->head;
		//is the same
		if(strcmp(locator->name,name)==0){
			return locator;
			sys_req(WRITE,DEFAULT_DEVICE,"return1\n",&queueCountPtr);
		}
		while(locator->nextPCB != NULL){
			locator = locator->nextPCB;
			if(strcmp(locator->name,name)==0){
				return locator;
				sys_req(WRITE,DEFAULT_DEVICE,"return2\n",&queueCountPtr);
			}
		}
		
	}
	q = getBlockedQueue();
	if(q->head != NULL){ //if q is not empty
		locator = q->head;
		//is the same (works as a do while)
		if(strcmp(locator->name,name)==0){
			//sys_req(WRITE,DEFAULT_DEVICE,"return3\n",&queueCountPtr);
			return locator;
		}
		while(locator->nextPCB != NULL){ //while not at tail
			locator = locator->nextPCB;
			if(strcmp(locator->name,name)==0){
				//sys_req(WRITE,DEFAULT_DEVICE,"return4\n",&queueCountPtr);
				return locator;
			}
		}
		
	}
	sys_req(WRITE,DEFAULT_DEVICE,"Invalid input5\n",&queueCountPtr);
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
		int cond =1;
		pcb* temp = q->tail;
		while(cond){
		//sys_req(WRITE,DEFAULT_DEVICE,"loop\n",&queueCountPtr);
			if(PCB->priority <= temp->priority){	
				PCB->nextPCB = temp->nextPCB;
				PCB->prevPCB = temp;
				temp->nextPCB->prevPCB = PCB;
				temp->nextPCB = PCB;
				cond = 0;
				//break;
				}
			else if(temp->prevPCB != NULL){
				temp = temp->prevPCB;
				sys_req(WRITE,DEFAULT_DEVICE,"loop\n",&queueCountPtr);
				
			}
			//new PCB is Head
			else{
				temp->prevPCB = PCB;
				PCB->nextPCB = temp;
				q->head = PCB;
				cond = 0;
				//break;
				
			}
		}
	}
	strcpy(PCB->state,"Ready");
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
	strcpy(PCB->state,"Blocked");
}

//returns 0 on error, 1 on success
int removeFromQueue(pcb* PCB){
	queue* q;
	pcb* locator;
	if(strcmp(PCB->state,"Ready")==0){
	//sys_req(WRITE,DEFAULT_DEVICE,"getting ready\n",&queueCountPtr);
		q = getReadyQueue();
	}
	else if (strcmp(PCB->state,"Blocked")==0){
	//sys_req(WRITE,DEFAULT_DEVICE,"getting blocked\n",&queueCountPtr);
		q = getBlockedQueue();
	}
	if(q->head != NULL)locator = q->head;
	else return 0;
	

	while(1){
		if(locator->name == PCB->name){
			if(locator == q->head){
				locator->nextPCB->prevPCB = NULL;
				q->head = locator->nextPCB;
				locator->nextPCB = NULL;
				
			}
			else if (locator == q->tail){
				locator->prevPCB->nextPCB = NULL;
				q->tail = locator->prevPCB;
				locator->prevPCB = NULL;
			}
			
			else{
				locator->prevPCB->nextPCB = locator->nextPCB;
				locator->nextPCB->prevPCB = locator->prevPCB;
				locator->nextPCB = NULL;
				locator->prevPCB = NULL;
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

