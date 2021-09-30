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
//Suspend looks for a pcb with the given name and sets its status to suspended.
void Suspend(char name[]){
	pcb* temp = findPCB(name);

	//checks to see if there is a pcb with that name, if find returns NULL then it displays the wrong name message
	if(temp == NULL){
	sys_req(WRITE,DEFAULT_DEVICE,WRONGNAME,&nameCountPtr);
	}

	else{
	strcpy(temp->status,"Suspended"); 
	}

}

//Resume looks for a pcb with a given name and sets its status to Not Suspended
void Resume(char name[]){
	pcb* temp = findPCB(name);

	//checks to see if there is a pcb with that name, if find returns NULL then it displays the wrong name message
	if(temp == NULL){
		sys_req(WRITE,DEFAULT_DEVICE,WRONGNAME,&nameCountPtr);
	}
	//else it copies the not suspended into the status location of the PCB
	else{
	strcpy(temp->status,"Not Suspended"); 
	}



}

void setPriority(char name[],int PCBpriority){
pcb* temp = findPCB(name);
	//checks to ensure that the value the user inserted is between 0 and 9 and ensures that the PCB exsists within the queues.
	if((PCBpriority < 0 || PCBpriority > 9) && temp == NULL){
	 
	 sys_req(WRITE,DEFAULT_DEVICE,WRONGNAME,&nameCountPtr);

	}
	//removes the PCB from the queue and sets its priority and reinserts it into the correct queue
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
	//verifies that the name is a valid pcb
	pcb* temp = findPCB(processName);
	if(temp == NULL){
	sys_req(WRITE,DEFAULT_DEVICE,"PCB does not exist\n",&userCountPtr);
	}
	else{
		char* strPtr;
		char print[100];
		strPtr = print;
		//copies the name of the pcb and prints it to the screen
		strcpy(print, temp->name);
		sys_req(WRITE,DEFAULT_DEVICE,print,&userCountPtr);
		sys_req(WRITE,DEFAULT_DEVICE,"\n",&userCountPtr);
		//copies the class of the pcb and prints it to the screen
		itoa(temp->pcbClass, strPtr);
		strcpy(print, strPtr);
		sys_req(WRITE,DEFAULT_DEVICE,print,&userCountPtr);
		sys_req(WRITE,DEFAULT_DEVICE,"\n",&userCountPtr);
		//changes the priority to a character and prints it to the screen
		itoa(temp->priority, strPtr);
		strcpy(print, strPtr);
		sys_req(WRITE,DEFAULT_DEVICE,print,&userCountPtr);
		sys_req(WRITE,DEFAULT_DEVICE,"\n",&userCountPtr);
		//copies the state of the pcb and prints it to the screen
		strcpy(print, temp->state);
		sys_req(WRITE,DEFAULT_DEVICE,print,&userCountPtr);
		sys_req(WRITE,DEFAULT_DEVICE,"\n",&userCountPtr);
		//copies the status of the pcb and prints it to the screen
		strcpy(print, temp->status);
		
		sys_req(WRITE,DEFAULT_DEVICE,print,&userCountPtr);
		sys_req(WRITE,DEFAULT_DEVICE,"\n",&userCountPtr);
		
		

	}
	}
	
void showReady(){
	//returns the pointer of the ready queue 
	queue* temp = getReadyQueue();
	pcb* PCB = temp->head;
	   while (PCB != NULL){
		   //calls showPCB on each of the items within the ready queue until the current PCB is null. 
		showPCB(PCB->name);
		PCB = PCB->nextPCB;
	
}
}
void showBlocked(){
	//returns a pointer to the blocked queue 
queue* temp = getBlockedQueue();
pcb* PCB = temp->head;
	while(PCB !=NULL){
		//calls showPCB on each of the items within the blocked queue until the current PCB is NULL.
	   showPCB(PCB->name);
	   PCB = PCB->nextPCB;

}
}

void showAll(){
	//calls show ready and show blocked to display both queues to the user.
	showReady();
	showBlocked();
}

//----------------Temp Methods------------------
void createPCB(char name[], int pcbClass,int priority){
	//takes in the parameters given and calls setupPCB then adds the information to the ready queue.
	pcb* temp = setupPCB(name,pcbClass,priority);
	addToReadyQueue(temp);
}

void deletePCB(char name[]){
	//locates the PCB and removes it from the queue then frees the memory allocated to that PCB.
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
	//finds the pcb with the given name if not found it returns the wrong name 
	pcb* temp = findPCB(name);
	if(temp == NULL){
		sys_req(WRITE,DEFAULT_DEVICE,WRONGNAME,&nameCountPtr);
	}
	else{
		//removes the current pcb from the ready queue
		removeFromQueue(temp);
		//updates it state to be blocked
		strcpy(temp->state,"Blocked");
		//sets the next and previous to null
		temp->nextPCB = NULL;
		temp->prevPCB = NULL;
		//adds it to the blocked queue
		addToBlockedQueue(temp);
	}
}

void unblock(char name[]){
//locates the pcb with the given name
pcb* temp = findPCB(name);
	
	if(temp == NULL){
		//gives the user a wrong name message to tell them that name doesnt exsist
		sys_req(WRITE,DEFAULT_DEVICE,WRONGNAME,&nameCountPtr);
		
	}
		else{
				 //removes the current PCB from its queue
		removeFromQueue(temp);
			//changes it state to ready
		strcpy(temp->state,"Ready");
			//sets both next and prev to null
		temp->nextPCB = NULL;
		temp->prevPCB = NULL;
			//adds it back into the ready queue
		addToReadyQueue(temp);


		}



}


