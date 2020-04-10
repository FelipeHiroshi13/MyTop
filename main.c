#include <stdio.h>
#include "backend.h"
#include "interface.h"


typedef struct ProcessInfo
{
  int processID;
  char userName[99];
  int priority;
  char state;
  int cpuPercentage;
  int startTime;
  char commandLine[99];
}ProcessInfo;

int main (void){
	struct ProcessInfo **processInfo;

	processInfo = listAllProcessesDirectory();

  interface(processInfo);

  return 0;
}

	
