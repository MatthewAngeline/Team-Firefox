#include "interruptPolling.h"
#include "../include/system.h"
iocbQueue ioQ = {NULL,NULL};
iocbQueue* getIOCBQueue(){
	return &ioQ;
}
void addToSchedule(dcb* device,iocbQueue* q, iocb* toAdd){
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
	//passToDevice();
	}
}
int isBusy(dcb* device){
	if(device->allocStatus == 0) return 0;
	else return 1;
}
void passToDevice(dcb* device, iocb* toAdd){
	device = device;
	toAdd = toAdd;
}
