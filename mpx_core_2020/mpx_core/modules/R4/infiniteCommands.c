#include "infiniteCommands.h"
#include "../../include/string.h"
#include "../mpx_supt.h"

void foreverIdle() {
char message[30];
  int count=0;
	
	memset( message, '\0', sizeof(message));
	strcpy(message, "IDLE Forever Executing.\n");
	count = strlen(message);
  
  while(1){
	sys_req( WRITE, DEFAULT_DEVICE, message, &count);
	sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
  }
}


