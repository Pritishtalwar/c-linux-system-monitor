#ifndef PROC_READER_H
#define PROC_READER_H

typedef struct
{
    int pid;
    char name[256];
    char state[64];
    int parent_pid;
    unsigned long memory_kb;
    int threads;
} ProcessInfo;

int readProcessInfo(int pid, ProcessInfo *info);

void showProcessDetails(int pid);

#endif