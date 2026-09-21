#include <stdio.h>
#include <string.h>

#include "proc_reader.h"
#include "process_metrics.h"


/* =========================================================
   READ PROCESS INFORMATION FROM /proc/[PID]/status
   ========================================================= */

int readProcessInfo(int pid, ProcessInfo *info)
{
    char path[512];
    char line[512];
    FILE *file;

    if (info == NULL || pid <= 0)
    {
        return 0;
    }

    /*
     * Initialize structure fields.
     */
    info->pid = pid;
    info->name[0] = '\0';
    info->state[0] = '\0';
    info->parent_pid = 0;
    info->memory_kb = 0;
    info->threads = 0;

    snprintf(path,
             sizeof(path),
             "/proc/%d/status",
             pid);

    file = fopen(path, "r");

    /*
     * Process may have disappeared between
     * directory scanning and opening its status file.
     */
    if (file == NULL)
    {
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        /* Process name */
        if (strncmp(line, "Name:", 5) == 0)
        {
            sscanf(line + 5,
                   " %255[^\n]",
                   info->name);
        }

        /* Process state */
        else if (strncmp(line, "State:", 6) == 0)
        {
            sscanf(line + 6,
                   " %63[^\n]",
                   info->state);
        }

        /* Parent PID */
        else if (strncmp(line, "PPid:", 5) == 0)
        {
            sscanf(line + 5,
                   "%d",
                   &info->parent_pid);
        }

        /* Resident memory */
        else if (strncmp(line, "VmRSS:", 6) == 0)
        {
            sscanf(line + 6,
                   "%lu",
                   &info->memory_kb);
        }

        /* Number of threads */
        else if (strncmp(line, "Threads:", 8) == 0)
        {
            sscanf(line + 8,
                   "%d",
                   &info->threads);
        }
    }

    fclose(file);

    /*
     * If important information could not be read,
     * consider the process information invalid.
     */
    if (info->name[0] == '\0')
    {
        return 0;
    }

    return 1;
}


/* =========================================================
   DISPLAY PROCESS DETAILS
   ========================================================= */

void showProcessDetails(int pid)
{
    ProcessInfo info;
    double cpuUsage;

    if (pid <= 0)
    {
        printf("Invalid PID.\n");
        return;
    }

    if (!readProcessInfo(pid, &info))
    {
        printf("\n");
        printf("Error: Unable to read process %d\n", pid);
        printf("The process may have ended or permission may be denied.\n");
        return;
    }

    /*
     * CPU usage is measured separately from
     * basic process information.
     */
    cpuUsage = getProcessCpuUsage(pid);

    printf("\n");
    printf("========================================\n");
    printf("          PROCESS DETAILS\n");
    printf("========================================\n");

    printf("PID        : %d\n", info.pid);
    printf("Name       : %s\n", info.name);
    printf("State      : %s\n", info.state);
    printf("Parent PID : %d\n", info.parent_pid);
    printf("Memory     : %lu KB\n", info.memory_kb);

    if (cpuUsage < 0)
    {
        printf("CPU Usage  : N/A\n");
    }
    else
    {
        printf("CPU Usage  : %.2f%%\n", cpuUsage);
    }

    printf("Threads    : %d\n", info.threads);

    printf("========================================\n");
}