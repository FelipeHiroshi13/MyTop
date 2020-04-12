#include<stdio.h>
#include<string.h>
#include<ncurses.h>
#include<stdlib.h>

#define FUNDO    1
#define COR_MENU 2  
#define CURSOR   3

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

void header(){	
    printw("MyTop\n");
    printw("-----------------------------------------------------------\n");
    printw("PID\tUSER\tPR\tS\t%%CPU\tTIME\tCOMMAND\n");
    printw("-----------------------------------------------------------\n");
}

void background(){
    start_color();   
    init_pair(FUNDO,    COLOR_GREEN,   COLOR_BLACK);
    init_pair(COR_MENU, COLOR_BLUE,   COLOR_YELLOW);
    init_pair(CURSOR,   COLOR_YELLOW, COLOR_BLUE);
    init_pair(4,        COLOR_BLACK,  COLOR_YELLOW);
    init_pair(11,       COLOR_YELLOW, COLOR_BLACK);
    init_pair(12,       COLOR_RED,    COLOR_BLACK);

    bkgd (COLOR_PAIR(FUNDO));
}

void showProcess(struct ProcessInfo** processInfo){
   for(int i = 0; i < 3; i++){
        printw("%d\t%s\t%d\t%c\t%0.2lf\t\t%s\n", processInfo[i]->processID, processInfo[i]->userName, processInfo[i]->priority, processInfo[i]->state,
                                                        processInfo[i]->cpuPercentage, processInfo[i]->commandLine);
    }
}

void interface(struct ProcessInfo** processInfo){
    initscr();
    background();
    header();

    showProcess(processInfo);

    refresh();

    getch();

    endwin();
}
