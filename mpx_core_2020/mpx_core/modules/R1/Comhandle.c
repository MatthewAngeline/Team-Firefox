#include <../include/core/serial.h>
#include "Comhandle.h"
#include "../mpx_supt.h"
#include "../../include/string.h"


int countPtr;
int menuCountPtr;
int quit=0;
int bufferTrack=0;

char userInput[100];

//char MENU[]={"\n\n\n0: help \n1: Set Date \n2: Set Time \n3: Display Date \n4: Display Time\n5: Version\n6: Shut Down \n Please enter only the single digit corresponding with the option you desire and then press 'Enter'\n"};

char MENU[]={"\nFirefox MPX\n0: help \n1: Set Date \n2: Set Time \n3: Display Date \n4: Display Time\n5: Version\n6: Shut Down \nPlease enter your choice:\n"};

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
//polling();
}
//displays the set time that the user has set. If they havent added anything yet it will display a preset time. 
void getTime(){

}

//displays a hard code information about the current module for this on It should read "We are currently on version R.1 or 1.1. 
void Version(){
menuCountPtr=100;
sys_req(WRITE,DEFAULT_DEVICE,VERSION,&menuCountPtr);
comHand();
}
//displays hard coded information about each of the modules that have been added that the user can use. 
void Help(){

}
void clearInput(){
int i=0;
while(i<5){
userInput[i]='\0';
i=i+1;
}
i=0;
}


