#include <../include/core/serial.h>

#include <stdint.h>
#include <string.h>

#include <core/io.h>
#include <core/serial.h>
#include "../../modules/mpx_supt.h"
int bufferCount=0;
int * keyboard_polling(char *buffer, int *count){



	bufferCount=0;
	buffer[bufferCount]='\0';
	int inputCount=0;
	//bufferCount tracks cursor, inputCount tracks total input characters
	while(1){
	
	
//if key is pressed store character into the buffer
		
		if(inb(COM1+5)&1){
		//logic for each key stroke
		char letter = inb(COM1);
		
		
		//ascii for enter 0x0D will halt the key when pressed and wont update the buffer array
		if(letter==0x0D){
		serial_print("\n");
		break;
		}
		//ascii for backspace is 127, will remove one item from buffer count and then deletes the item on screen
		else if(letter==127){
				
			if(bufferCount==0){
				serial_print("\033[2K");
				serial_print("Cannot backspace characters not from input.\n");
				
				serial_print(buffer);
				bufferCount=inputCount;
			}
			else{
				int i = bufferCount-1;
				//rearranges buffer
				while(i<inputCount){
					buffer[i]=buffer[i+1];
					buffer[i+1]='\0';
					i++;
				}
				
				bufferCount=bufferCount-1;
				inputCount=inputCount-1;
				
				//reprints
				serial_print("\033[2K");
				i=bufferCount;
				while(i>-1){
				serial_print("\b");
				i--;
				}
				serial_print(buffer);
				i=inputCount;
				while(i>bufferCount) {
					serial_print("\b");
					i--;
				}
		

			}
		}
		
		
		//Checks arrows
		else if(letter=='\033'){
		inb(COM1);
		letter = inb(COM1);
			if(letter=='A'){//Up arrow
			serial_print("\033[A");
			}
			else if(letter=='B'){//Down arrow
			serial_print("\033[B");
			}
			else if(letter=='C'){//Right arrow
				if(bufferCount<inputCount){
					bufferCount=bufferCount+1;
					serial_print("\033[C");
				}
			}
			else if(letter=='D'){//Left arrow 'D'
				if(bufferCount>0){
				bufferCount=bufferCount-1;
				serial_print("\033[D");
				}
			}
		}
	        //ascii for left arrow is 75
	

		//delete ascii is 0x7f
		else if(letter==0x7e){
			if(inputCount>bufferCount+1){
				serial_print(" \b");
				inputCount=inputCount-1;
				//reference variable
				int i = bufferCount;
				//rearranges buffer
				while(i<inputCount){
					buffer[i]=buffer[i+1];
					buffer[i+1]='\0';
					i++;
				}
				i=bufferCount;
				//moves cursor to left side
				while(i>0){
				serial_print("\b");
				i--;
				}
				//ESC[k - clear line escape code
				//clears line and prints buffer
				serial_print("\033[2K");
				serial_print(buffer);
				i=inputCount;
				//puts cursor in the same spot
				while(i>bufferCount){
				serial_print("\b");
				i--;
				}
				
			}
			//deleting last character in line
			else if(inputCount-1==bufferCount){
			buffer[bufferCount]='\0';
			inputCount=inputCount-1;
			serial_print(" \b");
			}
				

			
			//serial_print("del");
		}
		
//buffer fills and breaks out - should be 100 using for tests
		else if(bufferCount==100){
		serial_print("\n");
		break;
		}
		
		//regular characters
		else{
		buffer[bufferCount] = letter;

		serial_print(&buffer[bufferCount]);
		bufferCount=bufferCount+1;
		inputCount=inputCount+1;
			
		}

		

		
	
		}
		}


return count;


}
