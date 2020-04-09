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
	p->userName = "Alguém";
	p->priority = 8;
	p->state = 'S';
	p->cpuPercentage = 10;
	p->startTime = 20;
	p->commandLine = "teste123";
	
	
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

void interface(){
    initscr();
    background();
    header();
    

    refresh();

    getch();

    endwin();
}
