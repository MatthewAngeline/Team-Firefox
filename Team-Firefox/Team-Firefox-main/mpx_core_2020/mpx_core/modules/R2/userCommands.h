#ifndef _USERCOMMANDS_H
#define _USERCOMMANDS_H


//temp commands
pcb * createPCB(char name[], int pcbClass, int priority, char status[]);
void deletePCB(char name[]);
void block(char name[]);
void unblock(char name[]);





//perm commands
void showPCB(char processName[]);
void showReady();
void showBlocked();
void showSuspendedBlocked();
void showSuspendedReady();
void showAll();
void Suspend(char name[]);
void Resume(char name[]);
void resumeAll();
void setPriority(char name[],int PCBpriority);
void deleteAllPCB(char name[]);



#endif
