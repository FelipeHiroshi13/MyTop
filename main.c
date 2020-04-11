#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "backend.h"
#include "interface.h"

typedef char string[256];

typedef struct ProcessInfo
{
  int processID;
  char userName[99];
  int priority;
  char state;
  int cpuPercentage;
  long int startTime;
  string commandLine;
}ProcessInfo;

int main (void){
	struct ProcessInfo **processInfo;

	processInfo = listAllProcessesDirectory();

  //interface(processInfo);

  return 0;
}

	
