#include <stdio.h>
#include <string.h>

#include "proc_reader.h"

int readProcessInfo(int pid, ProcessInfo *info)
{
    char path[512];
    char line[512];
    FILE *file;

    if (info == NULL)
    {
        return 0;
    }

    info->pid = pid;
    info->name[0] = '\0';
    info->state[0] = '\0';
    info->parent_pid = 0;
    info->memory_kb = 0;
    info->threads = 0;

    snprintf(path, sizeof(path), "/proc/%d/status", pid);

    file = fopen(path, "r");

    if (file == NULL)
    {
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strncmp(line, "Name:", 5) == 0)
        {
            sscanf(line + 5, "%255s", info->name);
        }
        else if (strncmp(line, "State:", 6) == 0)
        {
            sscanf(line + 6, " %63[^\n]", info->state);
        }
        else if (strncmp(line, "PPid:", 5) == 0)
        {
            sscanf(line + 5, "%d", &info->parent_pid);
        }
        else if (strncmp(line, "VmRSS:", 6) == 0)
        {
            sscanf(line + 6, "%lu", &info->memory_kb);
        }
        else if (strncmp(line, "Threads:", 8) == 0)
        {
            sscanf(line + 8, "%d", &info->threads);
        }
    }

    fclose(file);

    return 1;
}

void showProcessDetails(int pid)
{
    ProcessInfo info;

    if (!readProcessInfo(pid, &info))
    {
        printf("Error: Unable to read process %d\n", pid);
        printf("The process may have ended or permission may be denied.\n");
        return;
    }

    printf("\n========================================\n");
    printf("        PROCESS DETAILS\n");
    printf("========================================\n");

    printf("PID        : %d\n", info.pid);
    printf("Name       : %s\n", info.name);
    printf("State      : %s\n", info.state);
    printf("Parent PID : %d\n", info.parent_pid);
    printf("Memory     : %lu KB\n", info.memory_kb);
    printf("Threads    : %d\n", info.threads);

    printf("========================================\n");
}