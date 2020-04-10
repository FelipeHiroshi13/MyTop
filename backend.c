#include <ncurses.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> 

typedef char string[256];

typedef struct ProcessInfo
{
  int processID;
  char userName[99];
  int priority;
  char state;
  int cpuPercentage;
  long int startTime;
  string commandLine;
}ProcessInfo;

int comp (const void * a, const void * b) 
{
   ProcessInfo *processInfoA = (ProcessInfo *)a;
   ProcessInfo *processInfoB = (ProcessInfo *)b;

  return ( processInfoB->processID - processInfoB->processID );
}

struct ProcessInfo* GetProcessInfo(struct ProcessInfo *processInfo, char directoryName[256], char statFileName[256], struct dirent *entry)
{
  FILE *processInfoFile;
  char finalDirectoryName[256];
  char trash[256];
  char buffer;
  strcat(finalDirectoryName, directoryName);
  strcat(finalDirectoryName, "/");
  strcat(finalDirectoryName,entry->d_name);
  strcat(finalDirectoryName,statFileName);
  //printf("%s", finalDirectoryName);
 
  processInfoFile = fopen(finalDirectoryName, "r");

  //fseek(processInfoFile, 3, processInfoFile);
  fscanf(processInfoFile,"%d", &(*processInfo).processID);
  //printf("-->%d\n", processInfo->state);
  fscanf(processInfoFile,"%s", (*processInfo).commandLine);
  fscanf(processInfoFile," %c", &(*processInfo).state);

  for (int i = 0; i < 15; i++)
  {
    fscanf(processInfoFile,"%s", trash);
  }


  fscanf(processInfoFile,"%d", &(*processInfo).priority);

  for (int i = 0; i < 3; i++)
  {
    fscanf(processInfoFile,"%s", trash);
  }

  fscanf(processInfoFile,"%ld", &(*processInfo).startTime);

  //printf("%ld\n", processInfo->startTime);
  
  processInfo->startTime /= sysconf(_SC_CLK_TCK);
  
  
  time_t curTime; 
  curTime = time(NULL); 
  //printf("%ld\n", curTime);
  curTime -= processInfo->startTime;
  processInfo->startTime = curTime;

  fclose(processInfoFile);

  finalDirectoryName[0] = '\0';

  return processInfo;

}

struct ProcessInfo** listAllProcessesDirectory()
{
    DIR *pDir;
    struct dirent *entry;
    struct ProcessInfo* processInfoArray[99999];
    struct ProcessInfo** ptr = &processInfoArray[0];

    char directoryName[256] = "/proc";
    char statFileName[256] = "/stat";
    
    pDir = opendir(directoryName);
    int processInfoArrayIndex = 0;
    while((entry = readdir(pDir)) != NULL)
    {
      if(isdigit(entry->d_name[0]))
      {
        struct ProcessInfo *processInfo = malloc (sizeof (ProcessInfo));
        processInfoArray[processInfoArrayIndex] = GetProcessInfo(processInfo, directoryName, statFileName, entry);
        //printf("%s", directoryName);
        //printf("%s", entry->d_name);
        //printf("-->%d\n", processInfoArray[processInfoArrayIndex]->processID);
        processInfoArrayIndex++;
        //printf("\n");
      }
    }
    //printf("%ld\n", (processInfoArray)[3]->startTime);
    return ptr;
}

