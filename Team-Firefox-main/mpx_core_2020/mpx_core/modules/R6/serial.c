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

	//char i = inb(dev);
	//outb(dev,i);

	}
	
int output_h(){
	if(serial_dcb.dcb_status != 1){
	return -2;
	}
	else if(serial_dcb.out_x != serial_dcb.out_s){
	char i = inb(dev);
	outb(dev,i);
	return -1;
	}
	else{
	serial_dcb.dcb_status = 0;
	serial_dcb.events = 1;
	return serial_dcb.out_x;
	}


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
		klogv("got to output interrupt");
		//output handler
		while(bit1 == 1){
		output_h();
		if(serial_dcb.out_s == serial_dcb.out_x){
		bit1=0;
		}
		}
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
	if(serial_dcb.open == 1){
	return -103;
	}

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
int com_write(char * buf_p,int * count){
		if(buf_p == NULL){
		return -402;
		}
		if(count == NULL){
		return -403;
		}
		if(serial_dcb.open !=1){
		return -401;
		}
		if(serial_dcb.dcb_status != 0){
		return -404;
		}
	 klogv("Got to Com Write");
		serial_dcb.out = (unsigned char*)buf_p;
		serial_dcb.out_x = 0;
		serial_dcb.out_s = *count;
		
				
		//status @ 1 equals write
		serial_dcb.dcb_status = 1;
		serial_dcb.events = 0;
		
		outb(dev,serial_dcb.out[serial_dcb.out_x]);
		
		
		outb(COM1+1, inb(COM1+1) | 0x02);
		serial_dcb.out_x = serial_dcb.out_x + 1;
return 0;
}

int com_read(char * buf_p,int * count){
	//checks to make sure the buffer is valid, the count is valid, the device is open and the device is idle
	if(buf_p == NULL){
	return -302;
	}
	if(count == NULL){
	return -303;
	}
	if(serial_dcb.open !=1){
	return -301;
	}
	if(serial_dcb.dcb_status != 0){
	return -304;
	}
	
	serial_dcb.in =(unsigned char*) buf_p;
	serial_dcb.in_x = 0;
	serial_dcb.in_s = *count;
	//status @ 2 equals read
	serial_dcb.dcb_status = 2;
	serial_dcb.events = 0;
	
	return 0;
	}
