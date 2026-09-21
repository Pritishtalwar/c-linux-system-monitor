#define _DEFAULT_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "process_metrics.h"


/* =========================================================
   READ PROCESS CPU TICKS
   ========================================================= */

static int readProcessTicks(int pid, unsigned long long *ticks)
{
    char path[512];
    char line[1024];
    FILE *file;

    unsigned long long utime;
    unsigned long long stime;

    if (ticks == NULL || pid <= 0)
    {
        return 0;
    }

    snprintf(path,
             sizeof(path),
             "/proc/%d/stat",
             pid);

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

    /*
     * Process name can contain spaces and parentheses.
     * Therefore, find the final ')' of the comm field.
     */
    char *closingBracket = strrchr(line, ')');

    if (closingBracket == NULL)
    {
        return 0;
    }

    /*
     * After ')' comes:
     *
     * state
     * ppid
     * pgrp
     * session
     * tty_nr
     * tpgid
     * flags
     * minflt
     * cminflt
     * majflt
     * cmajflt
     * utime
     * stime
     */

    if (sscanf(closingBracket + 2,
               "%*c "
               "%*d %*d %*d %*d %*d "
               "%*u %*u %*u %*u %*u "
               "%llu %llu",
               &utime,
               &stime) != 2)
    {
        return 0;
    }

    *ticks = utime + stime;

    return 1;
}


/* =========================================================
   READ TOTAL CPU TICKS
   ========================================================= */

static int readTotalCpuTicks(unsigned long long *totalTicks)
{
    FILE *file;
    char line[512];

    unsigned long long user;
    unsigned long long nice;
    unsigned long long system;
    unsigned long long idle;
    unsigned long long iowait;
    unsigned long long irq;
    unsigned long long softirq;
    unsigned long long steal;

    if (totalTicks == NULL)
    {
        return 0;
    }

    file = fopen("/proc/stat", "r");

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
        return 0;
    }

    *totalTicks =
        user +
        nice +
        system +
        idle +
        iowait +
        irq +
        softirq +
        steal;

    return 1;
}


/* =========================================================
   GET PROCESS MEMORY
   ========================================================= */

unsigned long getProcessMemory(int pid)
{
    char path[512];
    char line[512];

    FILE *file;

    unsigned long memory = 0;

    if (pid <= 0)
    {
        return 0;
    }

    snprintf(path,
             sizeof(path),
             "/proc/%d/status",
             pid);

    file = fopen(path, "r");

    if (file == NULL)
    {
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strncmp(line, "VmRSS:", 6) == 0)
        {
            sscanf(line + 6, "%lu", &memory);
            break;
        }
    }

    fclose(file);

    return memory;
}


/* =========================================================
   CALCULATE PROCESS CPU USAGE
   ========================================================= */

double getProcessCpuUsage(int pid)
{
    unsigned long long firstProcessTicks;
    unsigned long long secondProcessTicks;

    unsigned long long firstTotalTicks;
    unsigned long long secondTotalTicks;

    long clockTicks;

    if (!readProcessTicks(pid, &firstProcessTicks))
    {
        return -1.0;
    }

    if (!readTotalCpuTicks(&firstTotalTicks))
    {
        return -1.0;
    }

    /*
     * Measure CPU activity over 200 milliseconds.
     */
    usleep(200000);

    if (!readProcessTicks(pid, &secondProcessTicks))
    {
        return -1.0;
    }

    if (!readTotalCpuTicks(&secondTotalTicks))
    {
        return -1.0;
    }

    clockTicks = sysconf(_SC_CLK_TCK);

    if (clockTicks <= 0)
    {
        return -1.0;
    }

    unsigned long long processDelta =
        secondProcessTicks - firstProcessTicks;

    unsigned long long totalDelta =
        secondTotalTicks - firstTotalTicks;

    if (totalDelta == 0)
    {
        return 0.0;
    }

    /*
     * Process CPU time relative to total system CPU time.
     */
    double cpuUsage =
        ((double)processDelta /
         (double)totalDelta) * 100.0;

    return cpuUsage;
}