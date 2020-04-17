#ifndef _INTERFACE_H
#define _INTERFACE_H

void background(void);

void interface(struct ProcessInfo** processInfo, int sizeofProcess);

void showProcess(struct ProcessInfo** processInfo);

void header(void);

void showsizeProcess(int sizeofProcess);

void finalizeInterface(void);

#endif