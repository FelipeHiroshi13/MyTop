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
  double cpuPercentage;
  double startTime;
  double sTime;
  double uTime;
  double sum;
  string commandLine;
}ProcessInfo;


/*
* Funcao para deslocar na posicao certa do documento
*/
void  desloca(int n, FILE *processFile){
  char trash [256];
  for(int i = 0; i < n; i++){
    fscanf(processFile,"%s", trash);
  }
}

/*
* Calcula o tempo do um processo no CPU (sTime)
* Calcula o tempo de processo do user (uTime)
* sum Soma de todas as linhas da CPU 
*/

void calculateTime(FILE *fp, FILE *processFile, double *sTime, double *uTime, double *sum)
{
  char value[256];

  fscanf(fp,"%s", value);

  for (int i = 0; i < 4; i++)
  {
    fscanf(fp,"%s", value);
    (*sum) += (double)(atoi(value));
  }
  
  //fseek(processFile, 14*sizeof(char), SEEK_SET);
  desloca(13, processFile);
  
  fscanf(processFile,"%lf", &(*uTime));
  fscanf(processFile,"%lf", &(*sTime));
  //printf("calculateTimeTest3\n");
}


/*
* Calcula a porcentagem da CPU em cada processo
*/

void calculateInicialTimers(struct ProcessInfo *processInfo, string finalDirectoryName)
{
  FILE* fp = fopen("/proc/stat","r");
  FILE* processFile = fopen(finalDirectoryName,"r");

  double sum = 0;
  double sTimeBefore = 0, uTimeBefore = 0;

  //fseek(processFile, 14*sizeof(char), SEEK_SET);
  calculateTime(fp, processFile, &sTimeBefore, &uTimeBefore, &sum);

  (*processInfo).sTime = sTimeBefore;
  (*processInfo).uTime = uTimeBefore;
  (*processInfo).sum = sum;
  
  fclose(fp);
  fclose(processFile);
}

/*
* Calcula a porcentagem final de cada processo, no qual é preciso calcular os valores 
* iniciais com calculateInicialTimers e retorna a porcentagem.
*
*/

void CalculateProcessExecutionTime(struct ProcessInfo *processInfo, string finalDirectoryName, long unsigned * processStartTime)
{
  FILE* processFile = fopen(finalDirectoryName,"r");
  FILE* uptimeFile = fopen("/proc/uptime", "r");
  double upTime;
  double time;

  fscanf(uptimeFile,"%lf", &upTime);

  desloca(21, processFile);

  fscanf(processFile,"%lu", processStartTime);

  // if(strcmp(finalDirectoryName, "/proc/6143/stat") == 0)
  //   printf("--->%lu\n", processStartTime);
  //time = (double)((*processStartTime)/sysconf(_SC_CLK_TCK));

  (*processInfo).startTime = ((* processInfo).sTime + (*processInfo).uTime)/sysconf(_SC_CLK_TCK);

  //printf("-->%lf\n", time);

  fclose(processFile);
  fclose(uptimeFile);

}

double calculateFinalTimers(struct ProcessInfo *processInfo, string finalDirectoryName)
{
  int nb = sysconf(_SC_NPROCESSORS_ONLN);
  double finalPercentage;
  FILE* fp = fopen("/proc/stat","r");
  FILE* processFile = fopen(finalDirectoryName,"r");
  double sum = 0, lastSum = 0, sumSec;
  double sTimeBefore = 0, uTimeBefore = 0;
  double sTimeAfter = 0, uTimeAfter = 0;
  
  double uptime = 0;
  double total_time = 0;
  long unsigned startime = 0;
  double seconds = 0;

  FILE* fupTime = fopen("/proc/uptime","r");

  fscanf(fupTime, "%lf", &uptime);

 // printf("FINALL \n", uptime);

  sTimeBefore = (*processInfo).sTime;
  uTimeBefore = (*processInfo).uTime;
  lastSum = (*processInfo).sum;

  calculateTime(fp, processFile, &sTimeAfter, &sTimeAfter, &sum);
 
  CalculateProcessExecutionTime(processInfo, finalDirectoryName, &startime);

  finalPercentage = nb * 100 * (((sTimeAfter + uTimeAfter) - (sTimeBefore  + uTimeBefore))/sysconf(_SC_CLK_TCK)) / (lastSum - sum);

  (*processInfo).sTime = sTimeAfter;
  (*processInfo).uTime = uTimeAfter;
  (*processInfo).sum = sum;

  fclose(fp);
  fclose(fupTime);
  fclose(processFile);

  return finalPercentage;
}

/*
* Pega o uid do arquivo status de cada processo para localizar o userName
*
*/
char * getUid(char directoryName[256]){
  FILE *processInfoFile;
  struct passwd pd;
  struct passwd* pwdptr=&pd;
  struct passwd* tempPwdPtr;
  char pwdbuffer[200];
  int  pwdlinelen = sizeof(pwdbuffer);
  char uid[50];

  strcat(directoryName, "/status");
  processInfoFile = fopen(directoryName, "r");
  
  desloca(20, processInfoFile);
  fscanf(processInfoFile,"%s", uid);
  //printf("%s\n", uid);

  getpwuid_r(atoi(uid),pwdptr,pwdbuffer,pwdlinelen,&tempPwdPtr);
  fclose(processInfoFile);
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
  //char buffer;
  strcat(finalDirectoryName, directoryName);
  strcat(finalDirectoryName, "/");
  strcat(finalDirectoryName,entry->d_name);

  strcpy(directoryStatus, finalDirectoryName);
  strcpy((*processInfo).userName, getUid(directoryStatus));
  
  strcat(finalDirectoryName,statFileName);

  calculateInicialTimers(processInfo, finalDirectoryName);
 
  processInfoFile = fopen(finalDirectoryName, "r");

  //fseek(processInfoFile, 3, processInfoFile);
  fscanf(processInfoFile,"%d", &(*processInfo).processID);
  
  //printf("-->%d\n", processInfo->state);
  fscanf(processInfoFile,"%s", (*processInfo).commandLine);
  fscanf(processInfoFile," %c", &(*processInfo).state);

  desloca(14, processInfoFile);

  fscanf(processInfoFile,"%d", &(*processInfo).priority);

  desloca(3, processInfoFile);

  //fscanf(processInfoFile,"%ld", &(*processInfo).startTime);


  fclose(processInfoFile);

  finalDirectoryName[0] = '\0';
  directoryStatus[0] = '\0';

  return processInfo;

}

/*
* Ordena os processos de acordo com a % de CPU
*
*/
void selection_sort_decrescente(struct ProcessInfo ** processoInfo, int tam) {
  
    int i, j, min;
    ProcessInfo *swap = malloc (sizeof (ProcessInfo));
        for (i = 0; i < (tam-1); i++){
            min = i;
          
            for (j = (i+1); j < tam; j++) {
                if(processoInfo[j]->cpuPercentage > processoInfo[min]->cpuPercentage){
                    min = j;
                }
            }
            if (i != min) {
              swap = processoInfo[i];
              processoInfo[i] = processoInfo[min];
              processoInfo[min] = swap;
            }
          
        }
}

int sizeProcess(){
  DIR *pDir;
  struct dirent *entry;
  int size = 0;

  pDir = opendir("/proc");
  while((entry = readdir(pDir)) != NULL)
  {
    if(isdigit(entry->d_name[0]))
    {
      size++;
    }
  }
  closedir(pDir);

  return size;
}

/*
* Cria a array de processos
*/

struct ProcessInfo** listAllProcessesDirectory(int * sizlistProcess)
{
    DIR *pDir;
    struct dirent *entry;
    struct ProcessInfo* processInfoArray[99999];
    struct ProcessInfo** ptr = &processInfoArray[0];

    char directoryName[256] = "/proc";
    char statFileName[256] = "/stat";

    int processInfoArrayIndex = 0;

    pDir = opendir(directoryName);

    while((entry = readdir(pDir)) != NULL)
    {
      if(isdigit(entry->d_name[0]))
      {
        struct ProcessInfo *processInfo = malloc (sizeof (ProcessInfo));
        processInfoArray[processInfoArrayIndex] = GetProcessInfo(processInfo, directoryName, statFileName, entry);
        processInfoArrayIndex++;
      }
    }

    (*sizlistProcess) = processInfoArrayIndex;
    

    while(sizeProcess() != (*sizlistProcess)){
      int processInfoArrayIndex = 0;

      pDir = opendir("/proc");

      while((entry = readdir(pDir)) != NULL)
      {
        if(isdigit(entry->d_name[0]))
        {
          struct ProcessInfo *processInfo = malloc (sizeof (ProcessInfo));
          processInfoArray[processInfoArrayIndex] = GetProcessInfo(processInfo, directoryName, statFileName, entry);
          processInfoArrayIndex++;
        }
      }

      (*sizlistProcess) = processInfoArrayIndex;
      closedir(pDir);
      sleep(1);
    }

    closedir(pDir);

    (*sizlistProcess) = processInfoArrayIndex;
    
    return ptr;
}

struct ProcessInfo ** recalculaCPU(struct ProcessInfo ** processInfoArray, int * sizlistProcess){
    struct dirent *entry;
    DIR *pDir;
    char directoryName[256] = "/proc";
    char statFileName[10] = "/stat";
    //printf("%s", directoryName);
   
    pDir = opendir(directoryName);

    int processInfoArrayIndex = 0;

    struct ProcessInfo** ptr = &processInfoArray[0];

    processInfoArrayIndex = 0;
    while((entry = readdir(pDir)) != NULL)
    {
      strcpy(directoryName, "/proc");
      if(isdigit(entry->d_name[0]))
        {
          strcat(directoryName, "/");
          strcat(directoryName,entry->d_name);
          strcat(directoryName, statFileName);
          processInfoArray[processInfoArrayIndex]->cpuPercentage = calculateFinalTimers(processInfoArray[processInfoArrayIndex], directoryName);
          processInfoArrayIndex++;
        }
      directoryName[0] = '\0';
    }

    selection_sort_decrescente(ptr, processInfoArrayIndex);

    (*sizlistProcess) = processInfoArrayIndex;
    closedir(pDir);
    return ptr;

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