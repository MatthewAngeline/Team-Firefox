#ifndef _COMHANDLE_H
#define _COMHANDLE_H

int getGlobal();

int comHand();

void Setdate();

void getDate();

void setTime();

void getTime();

void Version();


void clearInput(char userInput[]);

unsigned int decToBCD(int num);

int BCDToDec(unsigned char num);

void itoa(int num, char* str);

#endif
