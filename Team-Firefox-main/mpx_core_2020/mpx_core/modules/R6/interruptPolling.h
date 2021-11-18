#ifndef _INTERRUPTPOLLING_H
#define _INTERRUPTPOLLING_H

#include "../R2/processQueues.h"
typedef struct dcb{
	int allocStatus;
	//vals 0=free   1=busy
	int currentOp;
	//vals 0=   1=
	int eventFlag;
	char userBuffer[100];
	char internBuffer[100];
}dcb;

typedef struct iocb{
	pcb* process;
	//device
	int operation;
	//vals 0=read 1=write
	char buffer[100];
	int count;
	struct iocb* nextPtr;
}iocb;

typedef struct iocbQueue{
	iocb* head;
	iocb* tail;
}iocbQueue;
iocbQueue* getIOCBQueue();
void addToSchedule(dcb* device, iocbQueue* q, iocb* toAdd);
int isBusy(dcb* device);
void passToDevice(dcb* device, iocb* toAdd);
#endif
