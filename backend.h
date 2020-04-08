#ifndef _BACKEND_H
#define __BACKEND_H

void listAllProcessesDirectory(void);
struct ProcessInfo* GetProcessInfo(struct ProcessInfo *processInfo, char directoryName[256], char statFileName[256], struct dirent *entry);

#endif