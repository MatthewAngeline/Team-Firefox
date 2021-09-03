/*
  ----- serial.c -----

  Description..: Contains methods and variables used for
    serial input and output.
*/

#include <stdint.h>
#include <string.h>

#include <core/io.h>
#include <core/serial.h>
#include "../../modules/mpx_supt.h"

#define NO_ERROR 0


#define BACKSPACE 127;
#define ENTER 0X0D;
#define LEFT_ARROW 0;75;
#define RIGHT_ARROW 0;77;
#define UP_ARROW 10;
#define DOWN_ARROW 13;


int bufferCount=0;
// Active devices used for serial I/O
int serial_port_out = 0;
int serial_port_in = 0;


/*
  Procedure..: init_serial
  Description..: Initializes devices for user interaction, logging, ...
*/
int init_serial(int device)
{
  outb(device + 1, 0x00); //disable interrupts
  outb(device + 3, 0x80); //set line control register
  outb(device + 0, 115200/9600); //set bsd least sig bit
  outb(device + 1, 0x00); //brd most significant bit
  outb(device + 3, 0x03); //lock divisor; 8bits, no parity, one stop
  outb(device + 2, 0xC7); //enable fifo, clear, 14byte threshold
  outb(device + 4, 0x0B); //enable interrupts, rts/dsr set
  (void)inb(device);      //read bit to reset port
  return NO_ERROR;
}

/*
  Procedure..: serial_println
  Description..: Writes a message to the active serial output device.
    Appends a newline character.
*/
int serial_println(const char *msg)
{
  int i;
  for(i=0; *(i+msg)!='\0'; i++){
    outb(serial_port_out,*(i+msg));
  }
  outb(serial_port_out,'\r');
  outb(serial_port_out,'\n');  
  return NO_ERROR;
}

/*
  Procedure..: serial_print
  Description..: Writes a message to the active serial output device.
*/
int serial_print(const char *msg)
{
  int i;
  for(i=0; *(i+msg)!='\0'; i++){
    outb(serial_port_out,*(i+msg));
  }
  if (*msg == '\r') outb(serial_port_out,'\n');
  return NO_ERROR;
}

/*int (*student_read)(char *buffer, int *count);
  Procedure..: set_serial_out
  Description..: Sets serial_port_out to the given device address.
    All serial output, such as that from serial_println, will be
    directed to this device.
*/
int set_serial_out(int device)
{
  serial_port_out = device;
  return NO_ERROR;
}

/*
  Procedure..: set_serial_in
  Description..: Sets serial_port_in to the given device address.
    All serial input, such as console input via a virtual machine,
    QEMU/Bochs/etc, will be directed to this device.
*/
int set_serial_in(int device)
{
  serial_port_in = device;
  return NO_ERROR;
}

int *polling(char *buffer, int *count){
// insert your code to gather keyboard input via the technique of polling.
// You must validat each key and handle special keys such as delete, back space, and
// arrow keys
	//klogv("got to polling from sys_req(read..");
	buffer[bufferCount]='\0';
	while(1){
	
	
//if key is pressed store character into the buffer
		
		if(inb(COM1+5)&1){
		//store the key in buffer
		char letter= inb(COM1);
		buffer[bufferCount]=letter;
				
		//logic for each key stroke		
		if(bufferCount==5){
		break;
		}
		//ascii for enter 0x0D will halt the key when pressed and wont update the buffer array
		if(letter==0x0D){
		break;
		}
		//ascii for backspace is 127, will remove one item from buffer count and then deletes the item on screen
		else if(letter==127){	
		bufferCount=bufferCount-1;
		serial_print("\b \b");
		}
		//ascii for down array is 13 and will drop down to a new line. not sure how it affects the buffer because in theory it should create a new line so should we reset buffer back to 0? 
		else if(letter==13){
		serial_print("\r \r");
		}
		else if(letter==10){
		serial_print("\n \n");
		}
		
		//NEED TO ASK TA SAM about the codes for left and right arrow because idk how to get those to work 
		//if(inb(COM1)==RIGHT_ARROW){
		//bufferCount++;
		
	      //  }
		//if(inb(COM1)==LEFT_ARROW){
		//bufferCount--;
		
		//}
		//delete ascii is 0x7f
		//if(letter==0x7f){
		
		//}
		else{
		
		serial_print(&buffer[bufferCount]);
		bufferCount=bufferCount+1;
		}
		}

		
		}
		klogv("Leaving While Loop");


return count;
}

