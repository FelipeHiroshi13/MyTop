#ifndef _BACKEND_H
#define _BACKEND_H

struct ProcessInfo** listAllProcessesDirectory(int *sizeLisProcess);

struct ProcessInfo ** recalculaCPU(struct ProcessInfo ** processInfoArray, int *sizeLisProcess);

struct ProcessInfo** reListProcess(int * sizlistProcess);

#endif