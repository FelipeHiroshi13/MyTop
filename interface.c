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
  double startTime;
  double sTime;
  double uTime;
  double sum;
  string commandLine;
}ProcessInfo;



void header(){	
    move(2,0);
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

void showsizeProcess(int sizeofProcess){
    move(0,0);
    printw("MyTop\n");
    printw("Tarefas: %d total\n",sizeofProcess);
    clrtoeol();
}

void showProcess(struct ProcessInfo** processInfo){
    double sec;
    int h, m, s;
    
    move(5,0);
    clrtoeol();

    for(int i = 0; i < 15; i++){
        sec = processInfo[i]->startTime;
	    m = sec/60;
	    s = (sec - (m*60));
        h = (int)sec % 100;
        printw("%d\t%-0.6s\t%d\t%c\t%0.2lf\t%0.2d:%0.2d.%0.2d %s\n", processInfo[i]->processID, processInfo[i]->userName, processInfo[i]->priority, processInfo[i]->state,processInfo[i]->cpuPercentage, m, s, h, processInfo[i]->commandLine);
    }
    refresh();
}

void interface(struct ProcessInfo** processInfo, int sizeofProcess){
    initscr();
    background();
    showsizeProcess(sizeofProcess);
    header();

    showProcess(processInfo);

    refresh();
}

void finalizeInterface(){
    endwin();
    exit(0);
}
