#include "userCommandsR3.h"
#include "../../include/system.h"
#include "../R2/processQueues.h"
#include "../mpx_supt.h"
#include "../R2/userCommands.h"
#include <../include/core/serial.h>
#include <../include/core/io.h>
#include "../../include/string.h"
#include "processes.h"

//temp for R3
void yield(){
	asm volatile ("int $60");
}

pcb* loadr3(){
	int i = 1;
	char* procName[5]={"TestName1","TestName2","TestName3","TestName4","TestName5"};
	pcb* new_pcb;
	while(i<=5) {
	new_pcb = createPCB( procName[i-1] , 1 , 1, "Suspended"); 
	context* cp = ( context*)( new_pcb -> stackHead ); 
	memset ( cp , 0, sizeof ( context )); 
	cp -> fs = 0x10 ;
	cp -> gs = 0x10 ;
	cp -> ds = 0x10 ;
	cp -> es = 0x10 ;
	cp -> cs = 0x8 ;
	cp -> ebp = ( u32int )( new_pcb -> stack );
	cp -> esp = ( u32int )( new_pcb -> stackHead ); 
	
	if(i == 1) {
	cp -> eip = ( u32int ) proc1;
	}
	else if(i == 2) {
	cp -> eip = ( u32int ) proc2;
	}
	else if(i == 3) {
	cp -> eip = ( u32int ) proc3;
	}
	else if(i == 4) {
	cp -> eip = ( u32int ) proc4;
	}
	else if(i == 5) {
	cp -> eip = ( u32int ) proc5;
	}
	cp -> eflags = 0x202 ;
	i++;
	}
	
	
	
	
	
	
	
	return new_pcb ;
}
