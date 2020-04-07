#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface.h"

typedef char String[20];

int main (int argc, char **argv){
	FILE *f;
	String pathFile;
	
	listAllProcessesDirectory();
    interface();
    
	if(argc != 2){
		printf("Error 1\n");
	}
	else{
		strcpy(pathFile, (argv[1]));
		
		f = fopen(pathFile, "r");
		
		if(f == NULL){
			printf("Error 2\n");
		}
		else{
			
			fclose(f);	
		}
	}

    return 0;
}

	
