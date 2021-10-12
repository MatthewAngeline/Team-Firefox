#ifndef _USERCOMMANDSR3_H
#define _USERCOMMANDSR3_H


#include "userCommandsR3.h"
#include "../../include/system.h"
#include "../R2/processQueues.h"
#include "../mpx_supt.h"
#include "../R2/userCommands.h"


typedef struct {
u32int gs, fs, es, ds;
u32int edi, esi, ebp, esp, ebx, edx, ecx, eax;
u32int eip, cs, eflags;
}context;

void yield();
pcb* loadr3();



#endif
