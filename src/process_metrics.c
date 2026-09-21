#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "process_metrics.h"

static int readProcessTicks(int pid, unsigned long long *ticks)
{
    char path[512];
    char line[1024];
    FILE *file;

    snprintf(path, sizeof(path), "/proc/%d/stat", pid);

    file = fopen(path, "r");

    if (file == NULL)
    {
        return 0;
    }

    if (fgets(line, sizeof(line), file) == NULL)
    {
        fclose(file);
        return 0;
    }

    fclose(file);

    char *closingBracket = strrchr(line, ')');

    if (closingBracket == NULL)
    {
        return 0;
    }

    unsigned long long utime;
    unsigned long long stime;

    if (sscanf(closingBracket + 2,
               "%*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %llu %llu",
               &utime,
               &stime) != 2)
    {
        return 0;
    }

    *ticks = utime + stime;

    return 1;
}

unsigned long getProcessMemory(int pid)
{
    char path[512];
    char line[512];
    FILE *file;
    unsigned long memory = 0;

    snprintf(path, sizeof(path), "/proc/%d/status", pid);

    file = fopen(path, "r");

    if (file == NULL)
    {
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "VmRSS: %lu", &memory) == 1)
        {
            break;
        }
    }

    fclose(file);

    return memory;
}

double getProcessCpuUsage(int pid)
{
    unsigned long long firstProcessTicks;
    unsigned long long secondProcessTicks;

    unsigned long long firstTotalTicks;
    unsigned long long secondTotalTicks;

    if (!readProcessTicks(pid, &firstProcessTicks))
    {
        return -1.0;
    }

    FILE *file;
    char line[512];

    file = fopen("/proc/stat", "r");

    if (file == NULL)
    {
        return -1.0;
    }

    if (fgets(line, sizeof(line), file) == NULL)
    {
        fclose(file);
        return -1.0;
    }

    fclose(file);

    unsigned long long user;
    unsigned long long nice;
    unsigned long long system;
    unsigned long long idle;
    unsigned long long iowait;
    unsigned long long irq;
    unsigned long long softirq;
    unsigned long long steal;

    if (sscanf(line,
               "cpu %llu %llu %llu %llu %llu %llu %llu %llu",
               &user,
               &nice,
               &system,
               &idle,
               &iowait,
               &irq,
               &softirq,
               &steal) < 4)
    {
        return -1.0;
    }

    firstTotalTicks =
        user + nice + system + idle +
        iowait + irq + softirq + steal;

    usleep(200000);

    if (!readProcessTicks(pid, &secondProcessTicks))
    {
        return -1.0;
    }

    file = fopen("/proc/stat", "r");

    if (file == NULL)
    {
        return -1.0;
    }

    if (fgets(line, sizeof(line), file) == NULL)
    {
        fclose(file);
        return -1.0;
    }

    fclose(file);

    if (sscanf(line,
               "cpu %llu %llu %llu %llu %llu %llu %llu %llu",
               &user,
               &nice,
               &system,
               &idle,
               &iowait,
               &irq,
               &softirq,
               &steal) < 4)
    {
        return -1.0;
    }

    secondTotalTicks =
        user + nice + system + idle +
        iowait + irq + softirq + steal;

    unsigned long long processDelta =
        secondProcessTicks - firstProcessTicks;

    unsigned long long totalDelta =
        secondTotalTicks - firstTotalTicks;

    if (totalDelta == 0)
    {
        return 0.0;
    }

    double cpuUsage =
        ((double)processDelta / (double)totalDelta) * 100.0;

    return cpuUsage;
}