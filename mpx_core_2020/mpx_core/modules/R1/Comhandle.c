#include <../include/core/serial.h>
#include "Comhandle.h"
#include "../mpx_supt.h"
#include "../../include/string.h"
#include <../include/core/io.h>


int countPtr;
int menuCountPtr;
int quit=0;
int bufferTrack=0;

char userInput[100];
char time[10];
char MENU[]={"\nFirefox MPX\n0: help \n1: Set Date \n2: Set Time \n3: Display Date \n4: Display Time\n5: Version\n6: Shut Down \nPlease enter your choice, one option at a time, entering only the number corresponding with the option:\n"};
char WRONGFORMAT[]={"Please insert the correct format\n"};

char CONFIRMATION[]={"Enter y + enter to shutdown press n + enter to go back to menu:\n"}; 
char VERSION[]={"1.1 \nCompletion Date:9/08/21\n"};
char HELP[]={};



//main file to run all the applications and used to create the menu driven logic.
int comHand(){


menuCountPtr=80;

menuCountPtr=70;
//prints menu
sys_req(WRITE,DEFAULT_DEVICE,MENU,&menuCountPtr);

memset(userInput, '\0', 100);
countPtr=100;
//get input from polling
sys_req(READ,DEFAULT_DEVICE,userInput,&countPtr);
bufferTrack=countPtr;


while(!quit){
if(userInput[0] == '0'){
klogv("made it to help");
Help();
clearInput();
comHand();
}
//if 1 is pressed set date
if(userInput[0]=='1'){
klogv("made it to set date");
clearInput();
comHand();
}
//if 2 is pressed set time
if(userInput[0]=='2'){
klogv("made it to set time");
setTime();
clearInput();
comHand();
}
//if 3 is pressed get Date
if(userInput[0]=='3'){
klogv("made it to get date");
clearInput();
comHand();
}
//if 4 is pressed get time
if(userInput[0]=='4'){
klogv("made it to get time");
getTime();
clearInput();
comHand();
}
//if 5 is pressed get version
if(userInput[0]=='5'){
klogv("Entering the Version");

Version();
clearInput();
comHand();
}
//if 6 is pressed shutdown
if(userInput[0]=='6'){
klogv("Got to shutdown protocol");

sys_req(WRITE,DEFAULT_DEVICE,CONFIRMATION,&menuCountPtr);
memset(userInput, '\0', 100);
countPtr=100;
clearInput();
//get input from polling
sys_req(READ,DEFAULT_DEVICE,userInput,&countPtr);
bufferTrack=countPtr;
if(userInput[0]=='y'){
quit=1;
break;
}
else if(userInput[0]=='n'){
comHand();
clearInput();
}


}
}

return 0;
}


//allows the user to set the date with a given date. Needs to not allow for dates greater then the given number of dates within a month. 
void Setdate(){


//polling();
}
//displays the date to the user that they have given. could have a set date if nothing has been added yet.
void getDate(){

}
//allows the user to set the time that they would like their system to read. 1-12 unless military time then 1-24 should be the hours and 1-59 for minutes and seconds. 
void setTime(){
	char clock[] = {"Please enter the current time in HH:MM:SS format.\n"};
	sys_req(WRITE,DEFAULT_DEVICE,clock,&menuCountPtr);
	cli();
	
	memset(time, '\0', 50);
	countPtr=50;
	//get input from polling
	sys_req(READ,DEFAULT_DEVICE,time,&countPtr);
	

	unsigned int hrsB = (time[0] * 10 + time[1])-6;
	unsigned int minB = (time[3] * 10 + time[4])-10;
	unsigned int secB = (time[6] * 10 + time[7])+7;

	outb(0x70, 0x04);
	outb(0x71, hrsB);
	outb(0x70, 0x02);
	outb(0x71, minB);
	outb(0x70, 0x00);
	outb(0x71, secB);

	sti();



//polling();
}
//displays the set time that the user has set. If they havent added anything yet it will display a preset time. 

void getTime(){
	menuCountPtr=100;
	outb(0x70, 0x04);
	unsigned char Bhrs = inb(0x71);
	int hrs_2 = BCDToDec(Bhrs);

	outb(0x70, 0x02);
	unsigned char Bmin = inb(0x71);
	int min = BCDToDec(Bmin);
	
	outb(0x70, 0x00);
	unsigned char Bsec = inb(0x71);
	int sec = BCDToDec(Bsec);



	int hr = hrs_2-4;
	int mi  = min;
	int se = sec;
	
	char hr_Ptr[5];
	char min_Ptr[5];
	char sec_Ptr[5];
	
	itoa(hr,hr_Ptr);
	itoa(mi,min_Ptr);
	itoa(se,sec_Ptr);
	
	sys_req(WRITE,DEFAULT_DEVICE,hr_Ptr,&menuCountPtr);
	sys_req(WRITE,DEFAULT_DEVICE,min_Ptr,&menuCountPtr);
	sys_req(WRITE,DEFAULT_DEVICE,sec_Ptr,&menuCountPtr);
	
}


//displays a hard code information about the current module for this on It should read "We are currently on version R.1 or 1.1. 
void Version(){
menuCountPtr=100;
sys_req(WRITE,DEFAULT_DEVICE,VERSION,&menuCountPtr);
comHand();
}
//displays hard coded information about each of the modules that have been added that the user can use. 
void Help(){
char HELP[] = {"Option 1: Set Date. \n\t Allows the user to set the Date should the listed date be incorrect.\nOption 2: Set Time. \n\t Allows the user to set the Time should the listed date be incorrect.\nOption 3: Display Date. \n\t Prints the date that is set to the screen.\nOption 4: Display Time. \n\t Prints the time that is set to the screen.\nOption 5: Version. \n\t Prints the currect Version of the project to the screen.\nOption 6: Shut Down. \n\t Begins the shutdown protocol for the system.\n"};
sys_req(WRITE,DEFAULT_DEVICE,HELP,&menuCountPtr);
}
void clearInput(){
int i=0;
while(i<5){
userInput[i]='\0';
i=i+1;
}
i=0;
}

//param - decimal number - returns BCD
unsigned int decToBCD(int num){

/*
	if (num == 0) return 0000;
	else if (num == 1) return 0001;
	else if (num == 2) return 0010;
	else if (num == 3) return 0011;
	else if (num == 4) return 0100;
	else if (num == 5) return 0101;
	else if (num == 6) return 0110;
	else if (num == 7) return 0111;
	else if (num == 8) return 1000;
	else return 1001;
	*/
	
	
	unsigned int ones = 0; 
	unsigned int tens = 0; 
	unsigned int temp = 0; 
	ones = num%10;
	temp = num/10;
	tens = temp<<4;
	return (tens + ones);



}
//param - BCD - returns decimal


int BCDToDec(unsigned char num){
	
	/*
	if (num == 0000) return '0';
	else if (num == 0001) return '1';
	else if (num == 0010) return '2';
	else if (num == 0011) return '3';
	else if (num == 0100) return '4';
	else if (num == 0101) return '5';
	else if (num == 0110) return '6';
	else if (num == 0111) return '7';
	else if (num == 1000) return '8';
	else return '9';
	*/
	


    unsigned char d=  num & 0x0f;
    int c = (num >> 4);
    int dec = (c * 10) + d;

    return dec;
	

}

void itoa(int num, char* str){
		int i = 0;
	//	int b = 1;
		
		while(num != 0){
			int a = num%10;
			str[i++] = a + '0';
			num = num/10;
			}
		str[i] = '\0';
		
		    int x,y;
    char temp;

    for( y= 0, x = i-1; y < x; y++, x--){
        temp = str[y];
        str[y] = str[x];
        str[x] = temp;
    }

    if(str[0] == '\0'){
        strcpy(str,"0");
    }

		
}



