#include <ncurses.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include<stdio.h>
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

int CalculateCpuPercentage(string finalDirectoryName)
{
  FILE* fp = fopen("/proc/stat","r");
  FILE* processFile = fopen(finalDirectoryName,"r");
  char str[100];
  char* token;
  char trash[256];
  long int sum = 0, lastSum = 0;
  long int sTimeAfter = 0, sTimeBefore = 0;
  long int finalPercentage;
  const char d[2] = " ";
  fgets(str,100,fp);
  token = strtok(str,d);
  
  while(token != NULL)
  {
    token = strtok(NULL,d);
    if(token!=NULL)
    {
      lastSum += atoi(token);
    }
  }

  for (int i = 0; i < 15; i++)
  {
    fscanf(processFile,"%s", trash);
  }

  fscanf(processFile,"%ld", &sTimeBefore);
  //printf("%ld\n", sTimeBefore);
  //printf("%ld\n", lastSum);

  sleep(1);

  fclose(fp);
  fclose(processFile);

  fp = fopen("/proc/stat","r");
  processFile = fopen(finalDirectoryName,"r");

  fgets(str,100,fp);
  token = strtok(str,d);

  while(token != NULL)
  {
    token = strtok(NULL,d);
    if(token!=NULL)
    {
      sum += atoi(token);
    }
  }

  for (int i = 0; i < 14; i++)
  {
    fscanf(processFile,"%s", trash);
  }

  fscanf(processFile,"%ld", &sTimeAfter);

  //printf("%ld\n", sTimeAfter);
  //printf("%ld\n", sum);
  finalPercentage = 100 * (sTimeAfter - sTimeBefore) / (sum - lastSum);
  printf("%ld\n", finalPercentage);
  fclose(fp);
  fclose(processFile);
  return finalPercentage;
}



struct ProcessInfo* GetProcessInfo(struct ProcessInfo *processInfo, char directoryName[256], char statFileName[256], struct dirent *entry)
{
  FILE *processInfoFile;
  char finalDirectoryName[256];
  char trash[256];
  char buffer;
  char commandline[99];
  
  strcat(finalDirectoryName, directoryName);
  strcat(finalDirectoryName, "/");
  strcat(finalDirectoryName,entry->d_name);
  strcat(finalDirectoryName,statFileName);
  //printf("%s", finalDirectoryName);

  //CalculateCpuPercentage(finalDirectoryName);
 
  processInfoFile = fopen(finalDirectoryName, "r");

  //fseek(processInfoFile, 3, processInfoFile);
  fscanf(processInfoFile,"%d", &(*processInfo).processID);
  //printf("-->%d\n", processInfo->state);
  fscanf(processInfoFile,"%s", commandline);

  strcpy((*processInfo).commandLine, commandline);

  fscanf(processInfoFile," %c", &(*processInfo).state);

  for (int i = 0; i < 14; i++)
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
        processInfoArrayIndex++;
      }
    }

    printf("--->%s\n", processInfoArray[0]->commandLine);
    
    return ptr;
}

