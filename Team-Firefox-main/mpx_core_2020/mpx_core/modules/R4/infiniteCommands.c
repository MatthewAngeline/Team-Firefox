#include "infiniteCommands.h"
#include "../../include/string.h"
#include "../mpx_supt.h"
#include "../R1/Comhandle.h"
#include <../include/core/serial.h>
#include <../include/core/io.h>

char userInput[100];

alarm* alarmList[30];
int count = 100;

void foreverIdle() {
char message[30];
  int count=0;
	
	memset( message, '\0', sizeof(message));
	strcpy(message, "IDLE Forever Executing.\n");
	count = strlen(message);
  
  while(1){
 	 
	sys_req( WRITE, DEFAULT_DEVICE, message, &count);
	sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
	}
  
}

void checkAlarm(){
	//klogv("Checking Alarm");
	while(1){
	outb(0x70, 0x04);
	unsigned char Bhrs = inb(0x71);
	//converts it to decimal 
	int hrs = BCDToDec(Bhrs);
	//retrieves minutes from 0x02
	outb(0x70, 0x02);
	unsigned char Bmin = inb(0x71);
	//converts it to decimal
	int min = BCDToDec(Bmin);
	//stores seconds from 0x00
	outb(0x70, 0x00);
	//unsigned char Bsec = inb(0x71);
	//converts it to decimal
	//int sec = BCDToDec(Bsec);
	
	alarm* ref = alarmList[0];
	int i = 0;
	sys_req(WRITE, DEFAULT_DEVICE, "checking alarms\n", &count);
	while(i < 30){
	
		if(ref != NULL && hrs == ref->hour && min == ref->mins ){//&& sec == ref->secs
			char mess[30];
			strcpy(mess,ref->message);
			sys_req( WRITE, DEFAULT_DEVICE, mess, &count);
			sys_req( WRITE, DEFAULT_DEVICE, "\n", &count);
			//sys_req( WRITE, DEFAULT_DEVICE, "detected alarm", &count);
			alarmList[i] = NULL;
			sys_free_mem(ref); 
		}
		i++;
		ref = alarmList[i];
		
		
	}
	//klogv("endAlarm");
	sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
}
}

void setAlarm(){
	alarm* new = sys_alloc_mem(sizeof(alarm));
	int valid = 0;
	
	while(!valid) {
	sys_req(WRITE,DEFAULT_DEVICE,"Name:",&count);
	sys_req(READ,DEFAULT_DEVICE,userInput,&count);
	strcpy(new->name, userInput);
	clearInput();
	
	sys_req(WRITE,DEFAULT_DEVICE,"Message:",&count);
	sys_req(READ,DEFAULT_DEVICE,userInput,&count);
	strcpy(new->message, userInput);
	clearInput();
	
	sys_req(WRITE,DEFAULT_DEVICE,"Time (Hour):",&count);
	sys_req(READ,DEFAULT_DEVICE,userInput,&count);
	new->hour = atoi(userInput);
	clearInput();
	
	sys_req(WRITE,DEFAULT_DEVICE,"Time (Min):",&count);
	sys_req(READ,DEFAULT_DEVICE,userInput,&count);
	new->mins = atoi(userInput);
	clearInput();
	
	sys_req(WRITE,DEFAULT_DEVICE,"Time (Sec):",&count);
	sys_req(READ,DEFAULT_DEVICE,userInput,&count);
	new->secs = atoi(userInput);
	clearInput();
	
	valid = checkInput(new->name, new->hour, new->mins, new->secs);
	}
	
	alarm* temp = alarmList[0];
	int i = 0;
	while (i<30){
		if(temp == NULL){
			alarmList[i] = new;
			break;
		}
		else{
			i++;
			temp = alarmList[i];
			
		}
	}
	if (i >= 30){
		sys_req( WRITE, DEFAULT_DEVICE, "This alarm is not added because the alarm list is full", &count);
	}
}

void removealarm(){
sys_req(WRITE, DEFAULT_DEVICE, "Enter the name of the alarm you wish to remove:\n",&count);
clearInput();
sys_req(READ, DEFAULT_DEVICE, userInput, &count);
alarm* ref = alarmList[0];
int i = 0;
while (i<30){

	if (strcmp(ref->name,userInput)==0){
		alarmList[i] = NULL;
		sys_free_mem(ref); 
		sys_req(WRITE, DEFAULT_DEVICE, "Alarm removed\n",&count);
		break;
	}
	else{
		if(i == 29){
		sys_req(WRITE, DEFAULT_DEVICE, "Alarm not found\n",&count);
			break;
		}
		i++;
		ref = alarmList[i];
		
	}
}
	
}

// returns 0 if invalid, 1 if valid
int checkInput(char name[], int hr, int min, int sec) {
	if(hr < 24 && hr >= 0 && min < 60 && min >= 0 && sec < 60 && sec >= 0){
		int i = 0;
		while (i<30){
		if (strcmp(alarmList[i]->name,name)==0){
			sys_req(WRITE, DEFAULT_DEVICE, "Name taken\n\n",&count);
			return 0;
		}
		i++;
		}
		return 1;
	}
	else{
	sys_req(WRITE, DEFAULT_DEVICE, "Invalid time\n\n",&count);
	return 0;
	}
}






