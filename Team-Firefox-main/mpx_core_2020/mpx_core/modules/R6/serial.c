#include "serial.h"
#include <core/serial.h> 
#include <core/io.h>
#include <core/tables.h> 
#include <string.h>

u32int dev = COM1;
int level = 4;

struct dcb serial_dcb = {
	.ring_s = sizeof(*serial_dcb.ring/sizeof(unsigned char))
};

u32int original_idt_entry; 

void set_int(int bit,int on){
if(on){
outb(dev+1,inb(dev+1) | (1<<bit));
}
else{
outb(dev+1, inb(dev+1) & -(1<<bit));
}
}
void input_h(){

char i = inb(dev);
outb(dev,i);

}
void top_handler(){

	if(serial_dcb.open){
		cli();
		int type = inb(dev+2);
		int bit1 = type>>1&1;
		int bit2 = type>>2&1;
		if(!bit1 && !bit2){
		//modum
		inb(dev+6);
		}
		else if(bit1 && !bit2){
		//output handler
		 
		}
		else if(!bit1&&bit2){
		//input handler
		input_h();
		}
		else if(bit1 &&bit2){
		inb(dev+5);
		}

		sti();
	}
	outb(0x20,0x20);
}

int com_open(int baud_rate){
	//error checking 
	//is baud rate valid
	//port is not already open
	
	cli();
	serial_dcb.open = 1;
	serial_dcb.events = 1;
	serial_dcb.dcb_status = 0;
	
	original_idt_entry = idt_get_gate(0x24);
	idt_set_gate(0x24,(u32int)top_handler,0x08,0x8e);
	
	long brd = 115200/(long)baud_rate;
	
	//disable the ints
	outb(dev +1,0b00000000);
	//set line control register
	outb(dev +3,0b10000000);
	//lsb baud rate 
	outb(dev+0,brd);
	
	//msb baud_rate
	outb(dev+1,brd >> 8);
	
	//lock divisor;
	
	outb(dev+3,0b00000011);
	
	//enable FIFO
	outb(dev+2,0b11000111);
	
	//enable PIC 
	outb(0x21,inb(0x21) & ~(1<<level));
	
	outb(dev +1, 0b00000001);
	(void) inb(dev);
	
	sti();
	
	return 0;
}
int com_write(){
return 0;
}

int com_read(){
return 0;
}

