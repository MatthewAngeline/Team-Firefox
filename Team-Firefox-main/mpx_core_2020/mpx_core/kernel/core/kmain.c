/*
  ----- kmain.c -----

  Description..: Kernel main. The first function called after
      the bootloader. Initialization of hardware, system
      structures, devices, and initial processes happens here.
      
      Initial Kernel -- by Forrest Desjardin, 2013, 
      Modifications by:    Andrew Duncan 2014,  John Jacko 2017
      				Ben Smith 2018, and Alex Wilson 2019
*/

#include <stdint.h>
#include <string.h>
#include <system.h>

#include <core/io.h>
#include <core/serial.h>
#include <core/tables.h>
#include <core/interrupts.h>
#include <mem/heap.h>
#include <mem/paging.h>
#include "../../modules/R5/heapManager.h"

#include "modules/mpx_supt.h"
#include "modules/R1/Comhandle.h"
#include "modules/R4/infiniteCommands.h"
#include "modules/R6/serial.h"

char welcome[]={"R6"};
int menuCountPt=10;
void kmain(void)
{

   extern uint32_t magic;
   // Uncomment if you want to access the multiboot header
   // extern void *mbd;
   // char *boot_loader_name = (char*)((long*)mbd)[16];

  
   // 0) Initialize Serial I/O 
   // functions to initialize serial I/O can be found in serial.c
   // there are 3 functions to call
 	init_serial(COM1);
 	set_serial_in(COM1);
 	set_serial_out(COM1);
 	
   klogv("Starting MPX boot sequence...");
   klogv("Initialized serial I/O on COM1 device...");

   // 1) Initialize the support software by identifying the current
   //     MPX Module.  This will change with each module.
   // you will need to call mpx_init from the mpx_supt.c
 	mpx_init(IO_MODULE);
   // 2) Check that the boot was successful and correct when using grub
   // Comment this when booting the kernel directly using QEMU, etc.
   if ( magic != 0x2BADB002 ){
     //kpanic("Boot was not error free. Halting.");
   }
   
   // 3) Descriptor Tables -- tables.c
   //  you will need to initialize the global
   // this keeps track of allocated segments and pages
   klogv("Initializing descriptor tables...");
	init_gdt();


    // 4)  Interrupt vector table --  tables.c
    // this creates and initializes a default interrupt vector table
    // this function is in tables.c
    init_idt();
    init_irq();
    init_pic();
    sti();
    klogv("Interrupt vector table initialized!");
    com_open(1200);
    //sys_req(WRITE,DEFAULT_DEVICE,"Hello",&menuCountPt);
   com_write("hello\n",(int *)menuCountPt);
   //com_read(getUserInput(),(int*)menuCountPt);	
   // 5) Virtual Memory -- paging.c  -- init_paging
   //  this function creates the kernel's heap
   //  from which memory will be allocated when the program calls
   // sys_alloc_mem UNTIL the memory management module  is completed
   // this allocates memory using discrete "pages" of physical memory
   // NOTE:  You will only have about 70000 bytes of dynamic memory
   //
   init_paging();
   
   intializeHeap(50000);
   isEmpty();
   sys_set_malloc(allocateMemory);
   sys_set_free(freeMemory);
   
   klogv("Initializing virtual memory...");


   // 6) Call YOUR command handler -  interface method

   //comHand();
   	
   	//klogv("addingprocesses");
   	pcb* new_pcb = createPCB( "ComHandle", 1 , 9,"Not Suspended"); 
	context* cp = ( context*)( new_pcb -> stackHead ); 
	memset ( cp , 0, sizeof ( context )); 
	cp -> fs = 0x10 ;
	cp -> gs = 0x10 ;
	cp -> ds = 0x10 ;
	cp -> es = 0x10 ;
	cp -> cs = 0x8 ;
	cp -> ebp = ( u32int )( new_pcb -> stack );
	cp -> esp = ( u32int )( new_pcb -> stackHead ); 
	cp -> eip = ( u32int ) comHand;
	cp -> eflags = 0x202 ;


	new_pcb = createPCB( "ForeverIdle", 1 , 1,"Not Suspended"); 
	cp = ( context*)( new_pcb -> stackHead ); 
	memset ( cp , 0, sizeof ( context )); 
	cp -> fs = 0x10 ;
	cp -> gs = 0x10 ;
	cp -> ds = 0x10 ;
	cp -> es = 0x10 ;
	cp -> cs = 0x8 ;
	cp -> ebp = ( u32int )( new_pcb -> stack );
	cp -> esp = ( u32int )( new_pcb -> stackHead ); 
	cp -> eip = ( u32int ) foreverIdle;
	cp -> eflags = 0x202 ;

	
	new_pcb = createPCB( "AlarmAlarm", 1 , 1,"Not Suspended"); 
	cp = ( context*)( new_pcb -> stackHead ); 
	memset ( cp , 0, sizeof ( context )); 
	cp -> fs = 0x10 ;
	cp -> gs = 0x10 ;
	cp -> ds = 0x10 ;
	cp -> es = 0x10 ;
	cp -> cs = 0x8 ;
	cp -> ebp = ( u32int )( new_pcb -> stack );
	cp -> esp = ( u32int )( new_pcb -> stackHead ); 
	cp -> eip = ( u32int ) checkAlarm;
	cp -> eflags = 0x202 ;
	
	
	asm volatile ("int $60");
	cmcb* locator = getAlloc();
	
	while(locator != NULL){
	freeMemory((u32int*)locator->address);
	locator=locator->nextCMCB;
	}
	
	

   // 7) System Shutdown on return from your command handler
   klogv("Starting system shutdown procedure...");
   
   /* Shutdown Procedure */
   klogv("Shutdown complete. You may now turn off the machine. (QEMU: C-a x)");
   hlt();
}
