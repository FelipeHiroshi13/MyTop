#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
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
	char out;
	
  do{
    processInfo = listAllProcessesDirectory(&sizeListProcess); 
    sleep(1);
    recalculaCPU(processInfo, &sizeListProcess);
    interface(processInfo,sizeListProcess);
    
    nodelay(stdscr, TRUE);
    if(out == 'q')
    	finalizeInterface();
    out = getch();
    
  }while(1);
  
  return 0;
}

	
