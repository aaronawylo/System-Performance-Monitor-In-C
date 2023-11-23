//
// Created by Mika on 2023-11-14.
//

#ifndef PROCESS_H
#define PROCESS_H



typedef struct {
    unsigned long processId;
    char processName[260];
    char status[10];
} ProcessInfo;

void PrintProcesses(ProcessInfo *processList, int maxProcesses, int *numProcesses);

#endif // PROCESS_H
