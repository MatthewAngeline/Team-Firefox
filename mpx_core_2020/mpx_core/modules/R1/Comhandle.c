#include "../../kernel/core/serial.c"
int *count;
char buffer[100];
//main file to run all the applications and used to create the menu driven logic.
void comHand(){
klogv("Entered Comhandler Successful");
polling(buffer,count);

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

}
//displays hard coded information about each of the modules that have been added that the user can use. 
void Help(){

}


