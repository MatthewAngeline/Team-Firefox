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

pcb* loadr3(char name[], int func){
	pcb* new_pcb = createPCB( name , 1 , 1); 
	context* cp = ( context*)( new_pcb -> stackHead ); 
	memset ( cp , 0, sizeof ( context )); 
	cp -> fs = 0x10 ;
	cp -> gs = 0x10 ;
	cp -> ds = 0x10 ;
	cp -> es = 0x10 ;
	cp -> cs = 0x8 ;
	cp -> ebp = ( u32int )( new_pcb -> stack );
	cp -> esp = ( u32int )( new_pcb -> stackHead ); 
	if (func == 1) {
	cp -> eip = ( u32int ) proc1;
	}
	else if (func == 2) {
	cp -> eip = ( u32int ) proc2;
	}
	else if (func == 3) {
	cp -> eip = ( u32int ) proc3;
	}
	else if (func == 4) {
	cp -> eip = ( u32int ) proc4;
	}
	else if (func == 5) {
	cp -> eip = ( u32int ) proc5;
	}
	else {
	// error message
	}
	
	cp -> eflags = 0x202 ;
	return new_pcb ;
}
