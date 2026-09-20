#include <stdio.h>
#include <dirent.h>
#include <ctype.h>

#include "process.h"

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

    printf("\nPID\tProcess Directory\n");
    printf("-----------------------------\n");

    while ((entry = readdir(directory)) != NULL)
    {
        if (isdigit(entry->d_name[0]))
        {
            printf("%s\n", entry->d_name);
        }
    }

    closedir(directory);
}