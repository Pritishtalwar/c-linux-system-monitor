#include <stdio.h>
#include <time.h>

#include "logger.h"

void logMessage(const char *message)
{
    FILE *file;
    time_t currentTime;
    struct tm *localTime;

    file = fopen("logs/monitor.log", "a");

    if (file == NULL)
    {
        return;
    }

    currentTime = time(NULL);
    localTime = localtime(&currentTime);

    fprintf(file,
            "%04d-%02d-%02d %02d:%02d:%02d - %s\n",
            localTime->tm_year + 1900,
            localTime->tm_mon + 1,
            localTime->tm_mday,
            localTime->tm_hour,
            localTime->tm_min,
            localTime->tm_sec,
            message);

    fclose(file);
}
