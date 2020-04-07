#include<stdio.h>
#include<ncurses.h>
#include<stdlib.h>

void interface(void){
    initscr();

    printw("MyTop\n");
    printw("PID\tUSER\tPR\tS\tCPU\tTIME\tCOMMAND");

    refresh();

    getch();

    endwin();
}