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
  double cpuPercentage;
  long int startTime;
  double sTime;
  double uTime;
  double sum;
  string commandLine;
}ProcessInfo;

int main (void){
	struct ProcessInfo **processInfo;

	processInfo = listAllProcessesDirectory();
   
  interface(processInfo);
  while(1){
    sleep(1);
    recalculaCPU(processInfo);
    showProcess(processInfo);
  }
  return 0;
}

	
