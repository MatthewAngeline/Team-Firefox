#ifndef _USERCOMMANDS_H
#define _USERCOMMANDS_H


//temp commands
void createPCB(char name[], int pcbClass, int priority);
void deletePCB(char name[]);
void block(char name[]);
void unblock(char name[]);





//perm commands
void showPCB(char processName[]);
void showReady();
void showBlocked();
void showAll();
void Suspend(char name[]);
void Resume(char name[]);
void setPriority(char name[],int PCBpriority);



#endif
