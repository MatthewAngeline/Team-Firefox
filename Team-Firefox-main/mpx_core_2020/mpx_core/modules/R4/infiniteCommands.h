#ifndef _INFINITECOMMANDS_H
#define _INFINITECOMMANDS_H

void foreverIdle();
void checkAlarm();

void setAlarm();
void removealarm();

typedef struct alarm{
	char name[10];
	char message[50];
	int hour;
	int mins;
	int secs;
	

}alarm;

#endif

