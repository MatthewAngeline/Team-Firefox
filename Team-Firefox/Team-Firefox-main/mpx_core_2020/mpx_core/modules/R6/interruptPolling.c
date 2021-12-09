#include "interruptPolling.h"
#include "../include/system.h"
int count2 = 0;
iocbQueue ioQ = {NULL,NULL};
dcb com1;
char uBuf[100];
char iBuf[100];

dcb* getCom(){
	com1.allocStatus = 0;
	com1.currentOp =0;
	com1.eventFlag=0;
	com1.userBuffer=uBuf;
	com1.internBuffer=iBuf;
	com1.bufferCount=0;
	com1.transferred=0;
	com1.total=0;
	com1.count=&count2;
	return &com1;
}
iocbQueue* getIOCBQueue(){
	return &ioQ;
}
void addToSchedule(dcb* device, iocb* toAdd){
	iocbQueue* q = getIOCBQueue();
	if(isBusy(device)){
		if(q->head == NULL) {
			q->head = toAdd;
			q->tail = toAdd;
		}
		else{
			q->tail->nextPtr = toAdd;
			q->tail = toAdd;
		}
	}
	else{
		passToDevice(device,toAdd);
	}
}
int isBusy(dcb* device){
	if(device->allocStatus == 0) return 0;
	else return 1;
}
void passToDevice(dcb* device, iocb* toAdd){
	device->allocStatus = 1;
	
	device = device;
	toAdd = toAdd;
}
