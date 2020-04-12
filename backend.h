#ifndef _BACKEND_H
#define _BACKEND_H

struct ProcessInfo** listAllProcessesDirectory(void);

struct ProcessInfo ** recalculaCPU(struct ProcessInfo ** processInfoArray);

#endif