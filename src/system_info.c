#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

#include "system_info.h"

void showSystemInfo(void)
{
    FILE *file;
    char line[512];

    printf("\n");
    printf("========================================\n");
    printf("          SYSTEM INFORMATION\n");
    printf("========================================\n");

    /* CPU Information */
    file = fopen("/proc/cpuinfo", "r");

    if (file != NULL)
    {
        while (fgets(line, sizeof(line), file) != NULL)
        {
            if (strncmp(line, "model name", 10) == 0)
            {
                printf("CPU        : %s", strchr(line, ':') + 2);
                break;
            }
        }

        fclose(file);
    }
    else
    {
        printf("CPU        : Unable to read\n");
    }

    /* Memory Information */
    file = fopen("/proc/meminfo", "r");

    if (file != NULL)
    {
        while (fgets(line, sizeof(line), file) != NULL)
        {
            if (strncmp(line, "MemTotal:", 9) == 0)
            {
                printf("Memory     : %s", strchr(line, ':') + 2);
            }

            if (strncmp(line, "MemAvailable:", 13) == 0)
            {
                printf("Available  : %s", strchr(line, ':') + 2);
                break;
            }
        }

        fclose(file);
    }
    else
    {
        printf("Memory     : Unable to read\n");
    }

    /* System Uptime */
        /* Running Process Count */
    DIR *directory;
    struct dirent *entry;
    int processCount = 0;

    directory = opendir("/proc");

    if (directory != NULL)
    {
        while ((entry = readdir(directory)) != NULL)
        {
            if (isdigit(entry->d_name[0]))
            {
                processCount++;
            }
        }

        closedir(directory);

        printf("Processes   : %d\n", processCount);
    }
    else
    {
        printf("Processes   : Unable to read\n");
    }
    file = fopen("/proc/uptime", "r");

    if (file != NULL)
    {
        double uptime;

        if (fscanf(file, "%lf", &uptime) == 1)
        {
            int hours = (int)uptime / 3600;
            int minutes = ((int)uptime % 3600) / 60;

            printf("Uptime     : %d hours %d minutes\n",
                   hours, minutes);
        }

        fclose(file);
    }
    else
    {
        printf("Uptime     : Unable to read\n");
    }

    printf("========================================\n");
}