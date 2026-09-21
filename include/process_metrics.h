#ifndef PROCESS_METRICS_H
#define PROCESS_METRICS_H

unsigned long getProcessMemory(int pid);
double getProcessCpuUsage(int pid);

#endif