#include <stdio.h>
#include <dirent.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "process.h"
#include "proc_reader.h"

void listProcesses(void)
{
    DIR *directory;
    struct dirent *entry;

    directory = opendir("/proc");

    if (directory == NULL)
    {
        printf("Error: Unable to open /proc\n");
        return;
    }

    printf("\n");
    printf("========================================\n");
    printf("        RUNNING PROCESSES\n");
    printf("========================================\n");
    printf("%-10s %-30s\n", "PID", "PROCESS NAME");
    printf("----------------------------------------\n");

    while ((entry = readdir(directory)) != NULL)
    {
        if (isdigit(entry->d_name[0]))
        {
            char path[512];
            char processName[256];
            FILE *file;

            snprintf(path, sizeof(path),
                     "/proc/%s/comm", entry->d_name);

            file = fopen(path, "r");

            if (file != NULL)
            {
                if (fgets(processName, sizeof(processName), file) != NULL)
                {
                    processName[strcspn(processName, "\n")] = '\0';

                    printf("%-10s %-30s\n",
                           entry->d_name,
                           processName);
                }

                fclose(file);
            }
        }
    }

    closedir(directory);
}

void searchProcess(const char *query)
{
    DIR *directory;
    struct dirent *entry;
    int found = 0;

    directory = opendir("/proc");

    if (directory == NULL)
    {
        printf("Error: Unable to open /proc\n");
        return;
    }

    printf("\n");
    printf("========================================\n");
    printf("          PROCESS SEARCH\n");
    printf("========================================\n");

    while ((entry = readdir(directory)) != NULL)
    {
        if (!isdigit(entry->d_name[0]))
        {
            continue;
        }

        int pid = atoi(entry->d_name);
        ProcessInfo info;

        if (readProcessInfo(pid, &info))
        {
            if (strstr(info.name, query) != NULL)
            {
                printf("PID: %-8d Name: %s\n",
                       info.pid,
                       info.name);

                found = 1;
            }
        }
    }

    if (!found)
    {
        printf("No matching process found.\n");
    }

    closedir(directory);
}