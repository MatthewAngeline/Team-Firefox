#include "processQueues.h"
//#include <stddef.h>
#include "../mpx_supt.h"
#include "../include/system.h"
#include "../../include/string.h"
#include <../include/core/io.h>
#include "userCommands.h"
#include "../R1/Comhandle.h"
char WRONGNAME[]= "Invalid Name, Reselect the option to try again \n";
int nameCountPtr=99;
int userCountPtr = 100;

void Suspend(char name[]){
	pcb* temp = findPCB(name);


	if(temp == NULL){
	sys_req(WRITE,DEFAULT_DEVICE,WRONGNAME,&nameCountPtr);
	}

	else{
	strcpy(temp->status,"Suspended"); 
	}

}


void Resume(char name[]){
	pcb* temp = findPCB(name);


	if(temp == NULL){
		sys_req(WRITE,DEFAULT_DEVICE,WRONGNAME,&nameCountPtr);
	}

	else{
	strcpy(temp->status,"Not Suspended"); 
	}



}

void setPriority(char name[],int PCBpriority){
pcb* temp = findPCB(name);

	if((PCBpriority < 0 || PCBpriority > 9) && temp == NULL){
	 
	 sys_req(WRITE,DEFAULT_DEVICE,WRONGNAME,&nameCountPtr);

	}
	else{
	  removeFromQueue(temp);
	  temp->priority = PCBpriority;
	}
	if(strcmp(temp->state,"Ready")==0){
	  addToReadyQueue(temp);
	}
	else{
	  addToBlockedQueue(temp);
	}
}

void showPCB(char processName[]){
	pcb* temp = findPCB(processName);
	if(temp == NULL){
	sys_req(WRITE,DEFAULT_DEVICE,"PCB does not exist\n",&userCountPtr);
	}
	else{
		char* strPtr;
		char print[100];
		strPtr = print;
		strcpy(print, temp->name);
		sys_req(WRITE,DEFAULT_DEVICE,"Name:",&userCountPtr);
		sys_req(WRITE,DEFAULT_DEVICE,print,&userCountPtr);
		sys_req(WRITE,DEFAULT_DEVICE,"\n",&userCountPtr);
		sys_req(WRITE,DEFAULT_DEVICE,"Class:",&userCountPtr);
		if(temp->pcbClass==0){
		strcpy(print,"Application");
		
		}
		else if(temp->pcbClass==1){
		strcpy(print,"System");
		
		}
		
		strcpy(print, strPtr);
		sys_req(WRITE,DEFAULT_DEVICE,print,&userCountPtr);
		sys_req(WRITE,DEFAULT_DEVICE,"\n",&userCountPtr);
		sys_req(WRITE,DEFAULT_DEVICE,"Priority:",&userCountPtr);
		itoa(temp->priority, strPtr);
		strcpy(print, strPtr);
		sys_req(WRITE,DEFAULT_DEVICE,print,&userCountPtr);
		sys_req(WRITE,DEFAULT_DEVICE,"\n",&userCountPtr);
		sys_req(WRITE,DEFAULT_DEVICE,"State:",&userCountPtr);
		strcpy(print, temp->state);
		sys_req(WRITE,DEFAULT_DEVICE,print,&userCountPtr);
		sys_req(WRITE,DEFAULT_DEVICE,"\n",&userCountPtr);
		sys_req(WRITE,DEFAULT_DEVICE,"status:",&userCountPtr);
		strcpy(print, temp->status);
		
		sys_req(WRITE,DEFAULT_DEVICE,print,&userCountPtr);
		sys_req(WRITE,DEFAULT_DEVICE,"\n",&userCountPtr);
		
		

	}
	}
	
void showReady(){
	sys_req(WRITE,DEFAULT_DEVICE,"\nReady Queue: \n",&userCountPtr);
	queue* temp = getReadyQueue();
	pcb* PCB = temp->head;
	   while (PCB != NULL){
		showPCB(PCB->name);
		PCB = PCB->nextPCB;
	
}
}
void showBlocked(){
	sys_req(WRITE,DEFAULT_DEVICE,"\nBlocked Queue: \n",&userCountPtr);
queue* temp = getBlockedQueue();
pcb* PCB = temp->head;
	while(PCB !=NULL){
	   showPCB(PCB->name);
	   PCB = PCB->nextPCB;

}
}

void showAll(){
	showReady();
	showBlocked();
}

//----------------Temp Methods------------------
void createPCB(char name[], int pcbClass,int priority){
	pcb* temp = setupPCB(name,pcbClass,priority);
	addToReadyQueue(temp);
}

void deletePCB(char name[]){
pcb* temp = findPCB(name);
	if(temp == NULL){
		sys_req(WRITE,DEFAULT_DEVICE,WRONGNAME,&nameCountPtr);
	}
	else{
		removeFromQueue(temp);
	}
	freePCB(temp);


}

void block(char name[]){
	pcb* temp = findPCB(name);
	if(temp == NULL){
		sys_req(WRITE,DEFAULT_DEVICE,WRONGNAME,&nameCountPtr);
	}
	else{
		removeFromQueue(temp);
		strcpy(temp->state,"Blocked");
		temp->nextPCB = NULL;
		temp->prevPCB = NULL;
		addToBlockedQueue(temp);
	}
}

void unblock(char name[]){

pcb* temp = findPCB(name);
	if(temp == NULL){
		sys_req(WRITE,DEFAULT_DEVICE,WRONGNAME,&nameCountPtr);
		
	}
		else{
				 
		removeFromQueue(temp);
		strcpy(temp->state,"Ready");
		temp->nextPCB = NULL;
		temp->prevPCB = NULL;
		addToReadyQueue(temp);


		}



}


