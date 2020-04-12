#include <stdio.h>
#include <ncurses.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/mman.h>
#include <time.h> 
#include <sys/types.h>
#include <pwd.h>


#define SHM_KEY 0x1234

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

/*
* Calcula o tempo do um processo no CPU (sTime)
* Calcula o tempo de processo do user (uTime)
* sum Soma de todas as linhas da CPU 
*/

void calculateTime(FILE *fp, FILE *processFile, double *sTime, double *uTime, double *sum){
  char trash[256];
  fscanf(fp,"%s", trash);
  for (int i = 0; i < 4; i++)
    {
      fscanf(fp,"%s", trash);
      *sum += atoi(trash);
    }

    for (int i = 0; i < 13; i++)
    {
      fscanf(processFile,"%s", trash);
    }

    fscanf(processFile,"%lf", &(*uTime));
    fscanf(processFile,"%lf", &(*sTime));

    fclose(fp);
    fclose(processFile);

}

/*
* Calcula a porcentagem da CPU em cada processo
*/

int CalculateCpuPercentage(string finalDirectoryName)
{
  int nb = sysconf(_SC_NPROCESSORS_ONLN);
  double finalPercentage;
  FILE* fp = fopen("/proc/stat","r");
  FILE* processFile = fopen(finalDirectoryName,"r");
  ///char str[100];
  char* token;
  double sum = 0, lastSum = 0;
  double sTimeAfter = 0, sTimeBefore = 0;
  double uTimeAfter = 0, uTimeBefore = 0;

  //const char d[2] = " ";

  calculateTime(fp, processFile, &sTimeAfter, &uTimeAfter, &sum);

  sleep(1);

  fp = fopen("/proc/stat","r");
  processFile = fopen(finalDirectoryName,"r");

  calculateTime(fp, processFile, &sTimeBefore, &uTimeBefore, &lastSum);

  finalPercentage = nb*(100 * (((sTimeAfter + uTimeAfter) - (sTimeBefore + uTimeBefore)) / (sum - lastSum)));

  if(strcmp(finalDirectoryName, "/proc/824/stat") == 0)
    printf("%.2lf\n", finalPercentage);
  return finalPercentage;
}

char * getUid(char directoryName[256]){
  FILE *processInfoFile;
  struct passwd pd;
  struct passwd* pwdptr=&pd;
  struct passwd* tempPwdPtr;
  char pwdbuffer[200];
  int  pwdlinelen = sizeof(pwdbuffer);
  char trash[256];
  char uid[50];

  strcat(directoryName, "/status");
  printf("%s\n", directoryName);
  processInfoFile = fopen(directoryName, "r");
  for (int i = 0; i < 20; i++)
  {
    fscanf(processInfoFile,"%s", trash);
  }
  fscanf(processInfoFile,"%s", uid);

  getpwuid_r(1000,pwdptr,pwdbuffer,pwdlinelen,&tempPwdPtr);
  return pd.pw_name;
  
}

/*
* Realiza uma leitura de cada informacao do processo
*/

struct ProcessInfo* GetProcessInfo(struct ProcessInfo *processInfo, char directoryName[256], char statFileName[256], struct dirent *entry)
{
  FILE *processInfoFile;
  char finalDirectoryName[256] = "";
  char directoryStatus[256] = "";
  char trash[256];
  //char buffer;
  strcat(finalDirectoryName, directoryName);
  strcat(finalDirectoryName, "/");
  strcat(finalDirectoryName,entry->d_name);

  strcpy(directoryStatus, finalDirectoryName);

  strcpy((*processInfo).userName, getUid(directoryStatus));

  strcat(finalDirectoryName,statFileName);

  //CalculateCpuPercentage(finalDirectoryName);
 
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
  directoryStatus[0] = '\0';

  return processInfo;

}

/*
* Cria a array de processos
*/

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


void* create_shared_memory(size_t size) 
{
  // Our memory buffer will be readable and writable:
  int protection = PROT_READ | PROT_WRITE;

  // The buffer will be shared (meaning other processes can access it), but
  // anonymous (meaning third-party processes cannot obtain an address for it),
  // so only this process and its children will be able to use it:
  int visibility = MAP_SHARED | MAP_ANONYMOUS;

  // The remaining parameters to `mmap()` are not important for this use case,
  // but the manpage for `mmap` explains their purpose.
  return mmap(NULL, size, protection, visibility, -1, 0);
}


// void pidProcess() {
  
//   int parent_message = 33;  // parent process will write this message
//   int child_message = 23; // child process will then write this one
//   ProcessInfo processInfo[15];

//   processInfo[0].priority = 10;

//   void* shmem = create_shared_memory(sizeof(ProcessInfo*) * 15);

//   memcpy(shmem, &processInfo[0], sizeof(ProcessInfo));
  
//   int pid = fork();

//   if (pid == 0) 
//   {
//     printf("Child read: %d\n", ((ProcessInfo*)&shmem[0])->priority);
//     processInfo[0].priority = 20;
//     memcpy(shmem, &processInfo[0], sizeof(ProcessInfo));
//     printf("Child wrote: %d\n", ((ProcessInfo*)&shmem[0])->priority);

//   } else 
//   {
//     printf("Parent read: %d\n", ((ProcessInfo*)&shmem[0])->priority);
//     sleep(1);
//     printf("After 1s, parent read: %d\n", ((ProcessInfo*)&shmem[0])->priority);
//   }
// }