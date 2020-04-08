#include <ncurses.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

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

struct ProcessInfo* GetProcessInfo(struct ProcessInfo *processInfo, char directoryName[256], char statFileName[256], struct dirent *entry)
{
  FILE *processInfoFile;
  char finalDirectoryName[256];
  char trash[256];
  strcat(finalDirectoryName, directoryName);
  strcat(finalDirectoryName, "/");
  strcat(finalDirectoryName,entry->d_name);
  strcat(finalDirectoryName,statFileName);
  //printf("%s", finalDirectoryName);
 
  processInfoFile = fopen(finalDirectoryName, "r");
  
  fscanf(processInfoFile,"%d", &(*processInfo).processID);
  fscanf(processInfoFile,"%s", trash);
  fscanf(processInfoFile,"%[A-Z]c", &(*processInfo).state);

  for (int i = 0; i < 15; i++)
  {
    fscanf(processInfoFile,"%s", trash);
  }

  fscanf(processInfoFile,"%d", &(*processInfo).priority);

  for (int i = 0; i < 3; i++)
  {
    fscanf(processInfoFile,"%s", trash);
  }

  fscanf(processInfoFile,"%d", &(*processInfo).startTime);
  processInfo->startTime /= sysconf(_SC_CLK_TCK);

  fclose(processInfoFile);

  finalDirectoryName[0] = '\0';

  return processInfo;

}

void listAllProcessesDirectory()
{
    DIR *pDir;
    struct dirent *entry;
    struct ProcessInfo* processInfoArray[99999];
    
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
        processInfoArrayIndex++;

        //printf("\n");
      }
    }
    printf("%d", processInfoArray[2]->processID);
}

