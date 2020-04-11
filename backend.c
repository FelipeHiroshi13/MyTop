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

int CalculateCpuPercentage(string finalDirectoryName)
{
  int nb = sysconf(_SC_NPROCESSORS_ONLN);
  double finalPercentage;
  if(strcmp(finalDirectoryName, "/proc/824/stat") == 0)
  {
    while(1){
      FILE* fp = fopen("/proc/stat","r");
      FILE* processFile = fopen(finalDirectoryName,"r");
      char str[100];
      char* token;
      char trash[256];
      double sum = 0, lastSum = 0;
      double sTimeAfter = 0, sTimeBefore = 0;
      double uTimeAfter = 0, uTimeBefore = 0;
    
      const char d[2] = " ";

      fscanf(fp,"%s", trash);

      for (int i = 0; i < 4; i++)
      {
        fscanf(fp,"%s", trash);
        //printf("%s\n", trash);
        lastSum += atoi(trash);
      }

      for (int i = 0; i < 13; i++)
      {
        fscanf(processFile,"%s", trash);
      }

      fscanf(processFile,"%lf", &uTimeBefore);
      fscanf(processFile,"%lf", &sTimeBefore);
      if(strcmp(finalDirectoryName, "/proc/824/stat") == 0)
      {
        // printf("System Time Before:%lf\n", sTimeBefore);
        // printf("User Time Before:%lf\n", uTimeBefore);
        // printf("Cpu Time Before:%lf\n", lastSum);
      }

      fclose(fp);
      fclose(processFile);

      sleep(1);


      fp = fopen("/proc/stat","r");
      processFile = fopen(finalDirectoryName,"r");

      fscanf(fp,"%s", trash);

      for (int i = 0; i < 4; i++)
      {
        fscanf(fp,"%s", trash);
        //printf("%s\n", trash);
        sum += atoi(trash);
      }

      for (int i = 0; i < 13; i++)
      {
        fscanf(processFile,"%s", trash);
      }

      fscanf(processFile,"%lf", &uTimeAfter);
      fscanf(processFile,"%lf", &sTimeAfter);
      if(strcmp(finalDirectoryName, "/proc/824/stat") == 0)
      {
        printf("System Time After: %lf\n", sTimeAfter);
        printf("User Time After: %lf\n", uTimeAfter);
        printf("Cpu Time After: %lf\n", sum);
      }
      finalPercentage = nb*(100 * (((sTimeAfter + uTimeAfter) - (sTimeBefore + uTimeBefore)) / (sum - lastSum)));

      /*if(strcmp(finalDirectoryName, "/proc/201/stat") == 0)
      {
        printf("top :%lf\n", finalPercentage);
      }*/
      if(strcmp(finalDirectoryName, "/proc/824/stat") == 0)
        printf("%.2lf\n", finalPercentage);
      fclose(fp);
      fclose(processFile);
    }
  }
  return finalPercentage;
  
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

  CalculateCpuPercentage(finalDirectoryName);
 
  processInfoFile = fopen(finalDirectoryName, "r");

  //fseek(processInfoFile, 3, processInfoFile);
  fscanf(processInfoFile,"%d", &(*processInfo).processID);
  //printf("-->%d\n", processInfo->state);
  fscanf(processInfoFile,"%s", (*processInfo).commandLine);
  fscanf(processInfoFile," %c", &(*processInfo).state);

  for (int i = 0; i < 14; i++)
  {
    fscanf(processInfoFile,"%s", trash);
  }


  fscanf(processInfoFile,"%d", &(*processInfo).priority);
  //printf("%d\n", (*processInfo).priority);

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
    return ptr;
}

