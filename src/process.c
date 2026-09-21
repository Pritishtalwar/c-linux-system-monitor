#include <stdio.h>
#include <dirent.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "process.h"
#include "proc_reader.h"
#include "process_metrics.h"


/* =========================================================
   SHOW ALL RUNNING PROCESSES
   ========================================================= */

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
    printf("================================================\n");
    printf("              RUNNING PROCESSES\n");
    printf("================================================\n");

    printf("%-10s %-30s\n",
           "PID",
           "PROCESS NAME");

    printf("------------------------------------------------\n");

    while ((entry = readdir(directory)) != NULL)
    {
        if (!isdigit((unsigned char)entry->d_name[0]))
        {
            continue;
        }

        int pid = atoi(entry->d_name);

        ProcessInfo info;

        /*
         * A process can disappear while we are reading /proc.
         * readProcessInfo() handles that situation.
         */
        if (!readProcessInfo(pid, &info))
        {
            continue;
        }

        printf("%-10d %-30s\n",
               info.pid,
               info.name);
    }

    printf("================================================\n");

    closedir(directory);
}


/* =========================================================
   SORTING COMPARATORS
   ========================================================= */

static int compareByPid(const void *a, const void *b)
{
    const ProcessInfo *processA = (const ProcessInfo *)a;
    const ProcessInfo *processB = (const ProcessInfo *)b;

    if (processA->pid < processB->pid)
    {
        return -1;
    }

    if (processA->pid > processB->pid)
    {
        return 1;
    }

    return 0;
}


static int compareByMemory(const void *a, const void *b)
{
    const ProcessInfo *processA = (const ProcessInfo *)a;
    const ProcessInfo *processB = (const ProcessInfo *)b;

    /*
     * Higher memory usage should appear first.
     */

    if (processA->memory_kb < processB->memory_kb)
    {
        return 1;
    }

    if (processA->memory_kb > processB->memory_kb)
    {
        return -1;
    }

    return 0;
}


/* =========================================================
   SORT PROCESSES
   ========================================================= */

void sortProcesses(void)
{
    DIR *directory;
    struct dirent *entry;

    /*
     * Store process information before sorting.
     */
    ProcessInfo processes[1024];

    int count = 0;
    int choice;

    directory = opendir("/proc");

    if (directory == NULL)
    {
        printf("Error: Unable to open /proc\n");
        return;
    }

    while ((entry = readdir(directory)) != NULL)
    {
        if (!isdigit((unsigned char)entry->d_name[0]))
        {
            continue;
        }

        /*
         * Prevent array overflow.
         */
        if (count >= 1024)
        {
            break;
        }

        int pid = atoi(entry->d_name);

        if (readProcessInfo(pid, &processes[count]))
        {
            count++;
        }
    }

    closedir(directory);

    if (count == 0)
    {
        printf("No processes found.\n");
        return;
    }

    printf("\n");
    printf("========================================\n");
    printf("          SORT PROCESSES\n");
    printf("========================================\n");

    printf("1. Sort by PID\n");
    printf("2. Sort by Memory Usage\n");

    choice = getIntegerInput("Enter your choice: ");

    switch (choice)
    {
        case 1:

            qsort(processes,
                  count,
                  sizeof(ProcessInfo),
                  compareByPid);

            break;

        case 2:

            qsort(processes,
                  count,
                  sizeof(ProcessInfo),
                  compareByMemory);

            break;

        default:

            printf("Invalid choice.\n");
            return;
    }

    printf("\n");
    printf("================================================\n");
    printf("              SORTED PROCESSES\n");
    printf("================================================\n");

    printf("%-10s %-30s %-15s\n",
           "PID",
           "PROCESS NAME",
           "MEMORY (KB)");

    printf("------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        printf("%-10d %-30s %-15lu\n",
               processes[i].pid,
               processes[i].name,
               processes[i].memory_kb);
    }

    printf("================================================\n");
}


/* =========================================================
   SEARCH PROCESS
   ========================================================= */

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
        if (!isdigit((unsigned char)entry->d_name[0]))
        {
            continue;
        }

        int pid = atoi(entry->d_name);

        ProcessInfo info;

        if (!readProcessInfo(pid, &info))
        {
            continue;
        }

        if (strstr(info.name, query) != NULL)
        {
            printf("PID: %-8d Name: %s\n",
                   info.pid,
                   info.name);

            found = 1;
        }
    }

    if (!found)
    {
        printf("No matching process found.\n");
    }

    closedir(directory);
}


/* =========================================================
   LIVE SYSTEM MONITOR
   ========================================================= */

void monitorProcesses(void)
{
    char input[20];

    while (1)
    {
        /*
         * Clear terminal screen.
         */
        printf("\033[2J");
        printf("\033[H");

        printf("===============================================================\n");
        printf("                    LIVE SYSTEM MONITOR\n");
        printf("===============================================================\n");

        listProcesses();

        printf("\n");
        printf("Press ENTER to refresh or type q and press ENTER to exit: ");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        if (input[0] == 'q' || input[0] == 'Q')
        {
            break;
        }
    }
}


/* =========================================================
   INTEGER INPUT VALIDATION
   ========================================================= */

int getIntegerInput(const char *prompt)
{
    int value;
    int result;

    while (1)
    {
        printf("%s", prompt);

        result = scanf("%d", &value);

        if (result == 1)
        {
            /*
             * Remove remaining characters from input buffer.
             */
            while (getchar() != '\n');

            return value;
        }

        printf("Invalid input. Please enter a number.\n");

        /*
         * Clear invalid input.
         */
        while (getchar() != '\n');
    }
}