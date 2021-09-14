#include "processQueues.h"


struct pcb {
	char name[50];
	char class[50];
	//higher number = higher priority
	int priority;
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

void addToQueue(queue Q, pcb PCB){
	Q.count++;
	//means only entry
	if(Q.head==Q.tail){
		Q.head = PCB;
		Q.tail = PCB;
	}
	//low priority
	else if(PCB.priority <= Q.tail.priority){
		Q.tail.nextPCB = PCB;
		PCB.prevPCB = Q.tail;
		Q.tail = PCB;
	}
	else{
		pcb temp = Q.tail;
		while(PCB.priority > temp.priority){
			if(temp.prevPCB != Q.head){
				temp = temp.prevPCB;
			}
			//new PCB is Head
			else{
				PCB.nextPCB = Q.head;
				Q.head = PCB;
			}
		}
	}
}

/*void removeFromQueue(queue Q){

}*/
