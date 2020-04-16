#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
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
  double startTime;
  double sTime;
  double uTime;
  double sum;
  string commandLine;
}ProcessInfo;

int main (void){
	struct ProcessInfo **processInfo;
  int sizeListProcess = 0;

  processInfo = listAllProcessesDirectory(&sizeListProcess); 

  while(1){
    processInfo = listAllProcessesDirectory(&sizeListProcess); 
    recalculaCPU(processInfo, &sizeListProcess);
    interface(processInfo,sizeListProcess);
    //processInfo = reListProcess(&sizeListProcess);
    //showsizeProcess(sizeListProcess);
    // processInfo = listAllProcessesDirectory(&sizeListProcess);
    //recalculaCPU(processInfo, &sizeListProcess);
  }
  return 0;
}

	
