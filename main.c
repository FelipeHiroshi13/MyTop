#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface.h"

typedef char String[20];

int main (int argc, char **argv){
	FILE *fs, *fu; 		// pointers fileStat, fileUptime
	String pathStat;	
	String pathUptime;
	
	listAllProcessesDirectory();
    interface();
    
	if(argc != 2){
		printf("Error 1\n");
	}
	else{
		strcpy(pathStat, (argv[1]));
		strcpy(pathUptime, (argv[2]));
		
		fs = fopen(pathStat, "r");
		
		if(fs == NULL){
			printf("Error 2\n");
		}
		else{
			
			// PID (find in stat[0])
			
			// USER
			
			// PR 
			
			// S (find in stat[1])
			
			// %CPU (this section need some calculus with stat[14:17, 22] and uptime[0])
			
			// TIME ( idem %cpu )
			
			// COMMAND (find in stat[2])
			
			fclose(fs);	
		}
	}

    return 0;
}

	
