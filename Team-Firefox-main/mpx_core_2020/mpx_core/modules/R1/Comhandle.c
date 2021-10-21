#include <../include/core/serial.h>
#include "Comhandle.h"
#include "../mpx_supt.h"
#include "../../include/string.h"
#include <../include/core/io.h>
#include "../R2/processQueues.h"
#include "../R2/userCommands.h"
#include "../R3/userCommandsR3.h"
#include "../R4/infiniteCommands.h"


int countPtr;
int menuCountPtr=99;
int quit=0;
int bufferTrack=0;

char userInput[100];
char time[10];
char date[10];
char MENU[]={"\nFirefox MPX\n0: help \n1: Set Date \n2: Set Time \n3: Display Date \n4: Display Time\n5: Version\n6: Shut Down \n7: Suspend \n8: Resume \n9: Set Priority \n10: Show PCB \n11: Show All \n12: Show Ready\n13: Show Block \n14: Create PCB \n15: Delete PCB \n16: Block \n17: Unblock \n18: Set Alarm \n19: LoadR3 \n20: Resume All\n21: Remove Alarm\nPlease enter your choice, one option at a time, entering only the number corresponding with the option:\n"};
char WRONGFORMAT[]={"Please insert the correct format\n"};
char CONFIRMATION[]={"Enter y + enter to shutdown press n + enter to go back to menu:\n"}; 
char VERSION[]={"4.0 \nCompletion Date:10/21/21\n"};




//main file to run all the applications and used to create the menu driven logic.
int comHand(){

menuCountPtr=80;

menuCountPtr=70;
//prints menu
sys_req(WRITE,DEFAULT_DEVICE,MENU,&menuCountPtr);
memset(userInput, '\0', 100);
countPtr=100;
//get input from polling and stores it into userInput
sys_req(READ,DEFAULT_DEVICE,userInput,&countPtr);
bufferTrack=countPtr;
while(!quit){
//if 0 is stored into the userInput at location 0
if(userInput[0] == '0'){
sys_req(WRITE,DEFAULT_DEVICE, "Option 1: Set Date. \n\t Allows the user to set the Date should the listed date be incorrect.\nOption 2: Set Time. \n\t Allows the user to set the Time should the listed date be incorrect.\nOption 3: Display Date. \n\t Prints the date that is set to the screen.\nOption 4: Display Time. \n\t Prints the time that is set to the screen.\nOption 5: Version. \n\t Prints the currect Version of the project to the screen.\nOption 6: Shut Down. \n\t Begins the shutdown protocol for the system.\nOption 7: Suspend \n\t Sets a user given process to Suspended \nOption 8: Resume \n\t Sets a user given process to Not Suspended \nOption 9: Set Priority\n\t Changes priority of a user given process \nOption 10: Show PCB \n\t Shows the PCB of a user given process \nOption 11: Show All \n\t Shows all PCBs \nOption 12: Show Ready \n\t Shows all PCBs within the ready queue \nOption 13: Show Blocked \n\t Shows all PCBs within the blocked queue \nOption 14: Create PCB \n\t Creates a PCB when the user gives the PCB name, the class, and the priority \nOption 15: Delete PCB \n\t Deletes a user given PCB \nOption 16: Block \n\t Changes a user given process state to blocked and moves PCB to blocked queue \nOption 17: Unblock \n\t Changes a user given process state to ready and moves PCB to ready queue\nOption 18: Set Alarm \n\t Allows the use create an alarm that will display a message when the time entered is reached.\nOption 19: LoadR3\n\t Loads in processes given in R3\nOption 20: Resume All\n\t Sets all processes to Not Suspended\nOption 21: Remove Alarm\n\t Cancels an alarm that has the same name that the user inputs",&countPtr);

clearInput();
}
//if 1 is stored into the userInput at location 0
else if(userInput[0]=='1'){
	//if 0 is stored into the userInput at location 1
	//Show PCB
	if(userInput[1]=='0'){
	sys_req(WRITE,DEFAULT_DEVICE,"Enter process name\n",&menuCountPtr);
	clearInput();
	sys_req(READ,DEFAULT_DEVICE,userInput,&countPtr);
	showPCB(userInput);
	clearInput();
	}
	//if 1 is stored into the userInput at location 1
	//Show All
	else if(userInput[1]=='1'){
	showAll();
	clearInput();
	}
	//if 2 is stored into the userInput at location 1
	//Show Ready
	else if(userInput[1]=='2'){
	showReady();
	clearInput();
	}
	//if 3 is stored into the userInput at location 1
	//Show Blocked
	else if(userInput[1]=='3'){
	showBlocked();
	clearInput();
	}
	//if 4 is stored into the userInput at location 1
	//Create PCB
	else if(userInput[1]=='4'){
	sys_req(WRITE,DEFAULT_DEVICE,"Enter process name, a class number (0 or 1), a priority number (name,#,#) and Suspended or Not Suspended\n",&menuCountPtr);
	clearInput();
	sys_req(READ,DEFAULT_DEVICE,userInput,&countPtr);
	char* token = strtok(userInput, ",");
	int count = 0;
	char* input[4];
	while(count<4) {
	input[count]=token;
	token = strtok(NULL, ",");
	count++;
	}
	int class = atoi(input[1]);
	int priority = atoi(input[2]);

	createPCB(input[0],class,priority,input[3]);
	clearInput();
	}
	//if 5 is stored into the userInput at location 1
	//Delete PCB
	else if(userInput[1]=='5'){
	sys_req(WRITE,DEFAULT_DEVICE,"Enter process name\n",&menuCountPtr);
	clearInput();
	sys_req(READ,DEFAULT_DEVICE,userInput,&countPtr);
	deletePCB(userInput);
	clearInput();
	}
	//if 6 is stored into the userInput at location 1
	//Block
	else if(userInput[1]=='6'){
	sys_req(WRITE,DEFAULT_DEVICE,"Enter process name\n",&menuCountPtr);
	clearInput();
	sys_req(READ,DEFAULT_DEVICE,userInput,&countPtr);
	block(userInput);
	clearInput();
	}
	//if 7 is stored into the userInput at location 1
	//Unblock
	else if(userInput[1]=='7'){
	sys_req(WRITE,DEFAULT_DEVICE,"Enter process name\n",&menuCountPtr);
	clearInput();
	sys_req(READ,DEFAULT_DEVICE,userInput,&countPtr);
	unblock(userInput);
	clearInput();
	}
	else if(userInput[1]=='8'){
	clearInput();
	setAlarm();
	}
	else if(userInput[1]=='9'){
	clearInput();
	loadr3();
	}
	//set date
	else{
	Setdate();
	clearInput();
	}
}
//if 2 is  is stored into the userInput at location 0
// set time
else if(userInput[0]=='2'){
if(userInput[1]=='0'){
resumeAll();
	clearInput();
	
	}
	else if(userInput[1]=='1'){
	removealarm();
}
else{
	setTime();
	clearInput();

}
}
//if 3 is stored into the userInput at location 0
//get date
else if(userInput[0]=='3'){
	getDate();
	clearInput();
	
}
//if 4 is stored into the userInput at location 0
//get time
else if(userInput[0]=='4'){
	getTime();
	clearInput();
	
}
//if 5 is stored into the userInput at location 0
//get version
else if(userInput[0]=='5'){
	Version();
	clearInput();
	
}
//if 6 is stored into the userInput at location 0
//shut down
else if(userInput[0]=='6'){
	//sends the confirmation to the terminal 
	sys_req(WRITE,DEFAULT_DEVICE,CONFIRMATION,&menuCountPtr);
	memset(userInput, '\0', 100);
	countPtr=100;
	clearInput();
	//get input from polling
	sys_req(READ,DEFAULT_DEVICE,userInput,&countPtr);
	bufferTrack=countPtr;
	//if y is stores it at location 0 enter this statement
	if(userInput[0]=='y'){
	quit=1;
	break;
	}
	//if n is stores it at location 0 enter this statement
	else if(userInput[0]=='n'){
			clearInput();
			comHand();
			
	}
}
//if 7 is stored into the userInput at location 0
//Suspend
else if(userInput[0]=='7'){
	sys_req(WRITE,DEFAULT_DEVICE,"Enter process name\n",&menuCountPtr);
	clearInput();
	sys_req(READ,DEFAULT_DEVICE,userInput,&countPtr);
	Suspend(userInput);
	clearInput();
	
}
//if 8 is stored into the userInput at location 0
//Resume
else if(userInput[0]=='8'){
	sys_req(WRITE,DEFAULT_DEVICE,"Enter process name\n",&menuCountPtr);
	clearInput();
	sys_req(READ,DEFAULT_DEVICE,userInput,&countPtr);
	Resume(userInput);
	clearInput();
	
}
//if 9 is stored into the userInput at location 0
//Set Priority
else if(userInput[0]=='9'){
	sys_req(WRITE,DEFAULT_DEVICE,"Enter process name and new priority number (name,#)\n",&menuCountPtr);
	clearInput();
	sys_req(READ,DEFAULT_DEVICE,userInput,&countPtr);
	char* token = strtok(userInput, ",");

	int count = 0;
	char* input[2];
	while(count<2) {
	input[count]=token;
	token = strtok(NULL, ",");
	count++;
	}
	int priority = atoi(input[1]);

	setPriority(input[0],priority);
	clearInput();
	

}
else{
comHand();
}
sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
}

return 0;
}


//allows the user to set the date with a given date. Needs to not allow for dates greater then the given number of dates within a month. 
void Setdate(){
	cli();
	
	memset(date, '\0', 50);
	countPtr=50;
	//get input from polling

		char clock[] = {"Please enter the current time in MM/DD/YY format.\n"};
		sys_req(WRITE,DEFAULT_DEVICE,clock,&menuCountPtr);
		
		sys_req(READ,DEFAULT_DEVICE,date,&menuCountPtr);
	//stores the string into a token from the start of date until the first /
		char *token = strtok(date, "/");
		int mon = atoi(token);
	//stores the next part of the date until the next /
		token = strtok(NULL,"/");
		int day = atoi(token);
	//stores the next part of the date until the return carriage
		token = strtok(NULL, "/r");
	//converts the token into an interger
		int yr= atoi(token);
			int cond = 1;
			int recursive_check = 1;
	//while loop to verify that the conditions are met
		while(cond){
			//checks if its a valid month within the Geogarian Calander
			if (mon < 13 && mon>0){
				//if its January, March, May, August, October or december and verifies that it has at least 31 days and more than 0 days.
				if((mon == 1 || mon == 3 || mon == 5|| mon == 7|| mon == 8|| mon == 10|| mon == 12) && day <= 31 && day >0){
						cond = 0;
						
				}//checks if febuary on a normal calander year has 28 days or more than 0 days
				else if ((mon ==2) && day <= 28 && day > 0){
					cond = 0;
					
				}
				
				//Checks if its a leap year but the intel system does not have it and then it switches to the day after.
				
				else if (mon == 2 && yr%4 == 0 && yr != 0 && day <= 29 && day > 0){
					cond = 0;
				}					
				//checks the other valid months to see if they have 30 days or more than 0
				else if ((mon == 4 || mon == 6 || mon == 9|| mon == 11) && day <= 30 && day >0){
					cond = 0;
				}
				//if all conditions fail inputs a fail sequence
				else{
					sys_req(WRITE,DEFAULT_DEVICE,"Invalid input\n",&menuCountPtr);
					Setdate();
					recursive_check = 0;
					cond = 0;
					
				}
				//is it a valid year greater than 0 less than 100
				if(yr >=0 && yr <=99){
				cond = 0;
				}
				else{
					//if not it sends an error message to the user
					sys_req(WRITE,DEFAULT_DEVICE,"Invalid year\n",&menuCountPtr);
					Setdate();
					recursive_check = 0;
					cond = 0;
				}

			}
			else{
				//invalid date try again...
				sys_req(WRITE,DEFAULT_DEVICE,"Invalid input\n",&menuCountPtr);
					Setdate();
					cond = 0;
					recursive_check  = 0;
			}
		}			//is it inside of the recursive call if not stores the information 
					if(recursive_check == 1){
						unsigned int monB = decToBCD(mon);
						unsigned int dayB = decToBCD(day);
						unsigned int yrB = decToBCD(yr);
						outb(0x70, 0x08);
						outb(0x71, monB);
						outb(0x70, 0x07);
						outb(0x71, dayB);
						outb(0x70, 0x09);
						outb(0x71, yrB);
					}
	//reenables interrupts
		sti();

}
//displays the date to the user that they have given. could have a set date if nothing has been added yet.
void getDate(){

	menuCountPtr=100;
	//retrieves day from 0x07
	outb(0x70, 0x07);
	unsigned char Bday = inb(0x71);
	int day_2 = BCDToDec(Bday);
	//retrieves month from 0x08
	outb(0x70, 0x08);
	unsigned char Bmon= inb(0x71);
	int mon = BCDToDec(Bmon);
	//retrieves year from 0x09
	outb(0x70, 0x09);
	unsigned char Byr = inb(0x71);
	int yr = BCDToDec(Byr);
	
	char day_Ptr[3];
	char mon_Ptr[3];
	char yr_Ptr[3];
	//converts day,month year into characters
	itoa(day_2,day_Ptr);
	itoa(mon,mon_Ptr);
	itoa(yr,yr_Ptr);
	//displays date to terminal seperated by /
	sys_req(WRITE,DEFAULT_DEVICE,mon_Ptr,&menuCountPtr);
	sys_req(WRITE,DEFAULT_DEVICE,"/",&menuCountPtr);
	sys_req(WRITE,DEFAULT_DEVICE,day_Ptr,&menuCountPtr);
	sys_req(WRITE,DEFAULT_DEVICE,"/",&menuCountPtr);
	sys_req(WRITE,DEFAULT_DEVICE,yr_Ptr,&menuCountPtr);
	sys_req(WRITE,DEFAULT_DEVICE,"\n",&menuCountPtr);

}
//allows the user to set the time that they would like their system to read. 1-12 unless military time then 1-24 should be the hours and 1-59 for minutes and seconds. 
void setTime(){
	menuCountPtr = 100;
	cli();
	
	memset(time, '\0', 50);
	countPtr=50;
	//get input from polling
	int cond = 1;
	while(cond){
		char clock[] = {"Please enter the current time in HH:MM:SS format.\n"};
		sys_req(WRITE,DEFAULT_DEVICE,clock,&menuCountPtr);
		sys_req(READ,DEFAULT_DEVICE,time,&countPtr);
		
		char *token = strtok(time, ":");
		int hr = atoi(token);
			
		token = strtok(NULL,":");
		int min = atoi(token);

		token = strtok(NULL, "/r");
		int sec= atoi(token);
	//is it a valid time hrs less than 24, minutes less than 60 and greater than or equal to 0 and same with seconds.
			if (hr < 24 && hr>=0 &&min <60 && min>=0 && sec<60 && sec>=0){
				cond = 0;
				unsigned int hrsB = decToBCD(hr);
				unsigned int minB = decToBCD(min);
				unsigned int secB = decToBCD(sec);
				outb(0x70, 0x04);
				outb(0x71, hrsB);
				outb(0x70, 0x02);
				outb(0x71, minB);
				outb(0x70, 0x00);
				outb(0x71, secB);
		}
		else{
			//if not display an error message
			sys_req(WRITE,DEFAULT_DEVICE,"Invalid input\n",&menuCountPtr);
			setTime();
			cond = 0;
		}
	}
	//reenables interrupts
		sti();


}
//displays the set time that the user has set. If they havent added anything yet it will display a preset time. 

void getTime(){
	//retrieves hrs from 0x04
	menuCountPtr=100;
	outb(0x70, 0x04);
	unsigned char Bhrs = inb(0x71);
	//converts it to decimal 
	int hrs_2 = BCDToDec(Bhrs);
	//retrieves minutes from 0x02
	outb(0x70, 0x02);
	unsigned char Bmin = inb(0x71);
	//converts it to decimal
	int min = BCDToDec(Bmin);
	//stores seconds from 0x00
	outb(0x70, 0x00);
	unsigned char Bsec = inb(0x71);
	//converts it to decimal
	int sec = BCDToDec(Bsec);
	
	char hr_Ptr[3];
	char min_Ptr[3];
	char sec_Ptr[3];
	//converts interger to a character 
	itoa(hrs_2,hr_Ptr);
	itoa(min,min_Ptr);
	itoa(sec,sec_Ptr);
	//writes the date into teriminal seperated by :
	sys_req(WRITE,DEFAULT_DEVICE,hr_Ptr,&menuCountPtr);
	sys_req(WRITE,DEFAULT_DEVICE,":",&menuCountPtr);
	sys_req(WRITE,DEFAULT_DEVICE,min_Ptr,&menuCountPtr);
	sys_req(WRITE,DEFAULT_DEVICE,":",&menuCountPtr);
	sys_req(WRITE,DEFAULT_DEVICE,sec_Ptr,&menuCountPtr);
	
}


//displays a hard code information about the current module for this on It should read "We are currently on version R.1 or 1.1. 
void Version(){
menuCountPtr=100;
	//writes a prerecorded version update to terminal
sys_req(WRITE,DEFAULT_DEVICE,VERSION,&menuCountPtr);
comHand();
}
//displays hard coded information about each of the modules that have been added that the user can use. 
void Help(){
	//displays this message to terminal
char HELP[] = {"Option 1: Set Date. \n\t Allows the user to set the Date should the listed date be incorrect.\nOption 2: Set Time. \n\t Allows the user to set the Time should the listed date be incorrect.\nOption 3: Display Date. \n\t Prints the date that is set to the screen.\nOption 4: Display Time. \n\t Prints the time that is set to the screen.\nOption 5: Version. \n\t Prints the currect Version of the project to the screen.\nOption 6: Shut Down. \n\t Begins the shutdown protocol for the system.\nOption 7: Suspend \n\t Sets a user given process to Suspended \nOption 8: Resume \n\t Sets a user given process to Not Suspended \nOption 9: Set Priority\n\t Changes priority of a user given process \nOption 10: Show PCB \n\t Shows the PCB of a user given process \nOption 11: Show All \n\t Shows all PCBs \nOption 12: Show Ready \n\t Shows all PCBs within the ready queue \nOption 13: Show Blocked \n\t Shows all PCBs within the blocked queue \nOption 14: Create PCB \n\t Creates a PCB when the user gives the PCB name, the class, and the priority \nOption 15: Delete PCB \n\t Deletes a user given PCB \nOption 16: Block \n\t Changes a user given process state to blocked and moves PCB to blocked queue \nOption 17: Unblock \n\t Changes a user given process state to ready and moves PCB to ready queue"};
sys_req(WRITE,DEFAULT_DEVICE,HELP,&menuCountPtr);
}

void clearInput(){
int i=0;
	//changes all values in buffer to null terminals to empty the buffer.
while(i<50){
userInput[i]='\0';
i=i+1;
}
	//resets i to be 0 at the end so it can be reused 
i=0;
}

//param - decimal number - returns BCD
//utilized from internet: https://stackoverflow.com/questions/35370200/converting-decimal-to-bcd user: user3528438
unsigned int decToBCD(int num){
	
	unsigned int ones = 0; 
	unsigned int tens = 0; 
	unsigned int temp = 0; 
	ones = num%10;
	temp = num/10;
	tens = temp<<4;
	return (tens + ones);


}
//param - BCD - returns decimal

//Hayhurst code:
int BCDToDec(unsigned char num){

    unsigned char d=  num & 0x0F ;
    int c = (num >> 4);
    int dec = (c * 10) + d;
    return dec;
	
}

//Converts integer to string data type
void itoa(int num, char* str){
	int i = 0;
		
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



