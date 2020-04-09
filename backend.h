#ifndef _BACKEND_H
#define _BACKEND_H

struct ProcessInfo** listAllProcessesDirectory(void);
struct ProcessInfo* GetProcessInfo(struct ProcessInfo *processInfo, char directoryName[256], char statFileName[256], struct dirent *entry);

#endif