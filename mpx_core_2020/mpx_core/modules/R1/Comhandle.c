#include <../include/core/serial.h>
#include "Comhandle.h"
#include "../mpx_supt.h"
#include "../../include/string.h"
int countPtr;
int menuCountPtr;
int quit=0;
int bufferTrack=0;

char userInput[100];
char MENU[]={"\nFirefox MPX\n0: help \n1: Set Date \n2: Set Time \n3: Display Date \n4: Display Time\n5: Version\n6: Shut Down \nPlease enter your choice:"};
char CONFIRMATION[]={"Enter y + enter to shutdown press n + enter to go back to menu"}; 
char VERSION[]={"1.1 \nCompletion Date:9/08/21"};
char HELP[]={};


int i=0;
//main file to run all the applications and used to create the menu driven logic.
int comHand(){

<<<<<<< HEAD
menuCountPtr=80;
=======
menuCountPtr=70;
//prints menu
>>>>>>> 08903ebfbb683c6a35dd98e483b0e0afb6a1de7b
sys_req(WRITE,DEFAULT_DEVICE,MENU,&menuCountPtr);

memset(userInput, '\0', 100);
countPtr=100;
//get input from polling
sys_req(READ,DEFAULT_DEVICE,userInput,&countPtr);
bufferTrack=countPtr;
while(!quit){
if(userInput[0] == 0){

}
//if 1 is pressed set date
if(userInput[0]==1){
}
//if 2 is pressed set time
if(userInput[0]==2){
}
//if 3 is pressed get Date
if(userInput[0]==3){
}
//if 4 is pressed get time
if(userInput[0]==4){
}
//if 5 is pressed get version
if(userInput[0]==5){
klogv("Entering the Version");
Version();
}
//if 6 is pressed shutdown
if(userInput[0]==6){
klogv("Got to shutdown protocol");
sys_req(WRITE,DEFAULT_DEVICE,CONFIRMATION,&menuCountPtr);
if(userInput[0]=='y')
quit=1;
else if(userInput[0]=='n'){
comHand();
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


