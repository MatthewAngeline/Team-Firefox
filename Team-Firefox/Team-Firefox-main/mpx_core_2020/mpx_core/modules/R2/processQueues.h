#ifndef _PROCESSQUEUES_H
#define _PROCESSQUEUES_H


typedef struct pcb{
	char name[50];
	int pcbClass;
	//higher number = higher priority
	int priority;
	char state[50];
	char status[50];
	unsigned char stack[1024];
	unsigned char* stackBase;
	unsigned char* stackHead;
	//next is towards tail of queue, prev is towards head
	struct pcb* nextPCB;
	struct pcb* prevPCB;
}pcb;


typedef struct queue{
	int count;
	struct pcb* head;
	struct pcb* tail;
}queue;

queue* getReadyQueue();
queue* getBlockedQueue();
queue* getSuspendedBlockedQueue();
queue* getSuspenedReadyQueue();

pcb* allocatePCB();

int freePCB(pcb* PCB);

pcb* setupPCB(char name[], int pcbClass, int priority, char status[]);

pcb* findPCB(char name[]);

void addToReadyQueue(pcb* PCB);

void addToBlockedQueue(pcb* PCB);

void addToSuspendedBlockedQueue(pcb* PCB);

void addToSuspendedReadyQueue(pcb* PCB);

pcb* removeFromQueue(pcb* PCB);

#endif




