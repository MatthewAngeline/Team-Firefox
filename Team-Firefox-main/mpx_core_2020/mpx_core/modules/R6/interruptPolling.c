#include "interruptPolling.h"
#include "../include/system.h"
int count = 0;
iocbQueue ioQ = {NULL,NULL};
dcb com1 = {0,0,0,NULL,NULL,0,0,0,&count};

dcb* getCom(){
	return com1;
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
	device->status = 1;
	
	device = device;
	toAdd = toAdd;
}
