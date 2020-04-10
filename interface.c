#include<stdio.h>
#include<string.h>
#include<ncurses.h>
#include<stdlib.h>

#define FUNDO    1
#define COR_MENU 2  
#define CURSOR   3

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

void header(){
	ProcessInfo *p;
	
	p = (ProcessInfo *)calloc(1, sizeof(ProcessInfo));
	
	p->processID = 5;
	strcpy(p->userName, "Felipe");
	p->priority = 8;
	p->state = 'S';
	p->cpuPercentage = 10;
	p->startTime = 20;
	strcpy(p->commandLine, "teste");
	
	
    printw("MyTop\n");
    printw("-----------------------------------------------------------\n");
    printw("PID\tUSER\tPR\tS\t%%CPU\tTIME\tCOMMAND\n");
    printw("-----------------------------------------------------------\n");
    printw("%i\t %s\t %i\t %c\t %i\t %i\t %s\n", (*p).processID, p->userName, p->priority, p->state, p->cpuPercentage, p->startTime, p->commandLine);
}

void background(){
    start_color();   
    //definição de pares de cores para a interface
    init_pair(FUNDO,    COLOR_GREEN,   COLOR_BLACK);
    init_pair(COR_MENU, COLOR_BLUE,   COLOR_YELLOW);
    init_pair(CURSOR,   COLOR_YELLOW, COLOR_BLUE);
    init_pair(4,        COLOR_BLACK,  COLOR_YELLOW);
    init_pair(11,       COLOR_YELLOW, COLOR_BLACK);
    init_pair(12,       COLOR_RED,    COLOR_BLACK);

    bkgd (COLOR_PAIR(FUNDO));
}

void showProcess(struct ProcessInfo** processInfo){
    for(int i = 0; i < 15; i++){
        printw("%d\t%s\n", processInfo[i]->processID, processInfo[i]->state);
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
