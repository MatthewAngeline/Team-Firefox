#ifndef _USERCOMMANDSR3_H
#define _USERCOMMANDSR3_H

/*struct context {
u32int gs, fs, es, ds;
u32int edi, esi, ebp, esp, ebx, edx, ecx, eax;
u32int eip, cs, eflags;
};*/
void yield();
pcb* loadr3();



#endif
