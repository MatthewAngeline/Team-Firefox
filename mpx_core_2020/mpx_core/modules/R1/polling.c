char *buffer[100];
int *count=0;

void polling(){
	while(1){
//if key is pressed store character into the buffer
		//
		//if(inb(COM1+5)==00001101){
		//return(*buffer);
		//}
		if(inb(COM1+5)){
			buffer[count]=inb(COM1+5);
			serial_print(*buffer[count]);
			count++;
		}
		
		}
		
//void return(*buffer){

//}
//void backspace(*buffer){}
//void delete(*buffer){}

//void leftarrow(*buffer){
//ascii 75 
//move the arrow over to one spot behind, and move whatever is currently in the buffer one space ahead leaving the spot open for a new character to be placed in location 

}

//void uparrow(*buffer){
//ascii 72
//}
//void downarrow(*buffer){
//ascii 80
//}


//void rightarrow(*buffer){
//ascii 77
//}
