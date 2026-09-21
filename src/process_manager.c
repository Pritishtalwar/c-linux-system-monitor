#include <stdio.h>
#include <signal.h>
#include <errno.h>

#include "process_manager.h"
#include "process.h"


/* =========================================================
   SEND SIGNAL TO PROCESS
   ========================================================= */

int sendProcessSignal(int pid, int signal_number)
{
    if (pid <= 0)
    {
        return 0;
    }

    if (kill(pid, signal_number) == 0)
    {
        return 1;
    }

    return 0;
}


/* =========================================================
   PROCESS MANAGEMENT MENU
   ========================================================= */

void showProcessManagement(void)
{
    int pid;
    int choice;
    int signal_number;

    printf("\n");
    printf("========================================\n");
    printf("        PROCESS MANAGEMENT\n");
    printf("========================================\n");

    pid = getIntegerInput("Enter PID: ");

    if (pid <= 0)
    {
        printf("Invalid PID. PID must be greater than 0.\n");
        return;
    }

    printf("\n");
    printf("1. Terminate Process (SIGTERM)\n");
    printf("2. Kill Process (SIGKILL)\n");
    printf("3. Stop Process (SIGSTOP)\n");
    printf("4. Continue Process (SIGCONT)\n");

    choice = getIntegerInput("Enter signal choice: ");

    switch (choice)
    {
        case 1:
            signal_number = SIGTERM;
            break;

        case 2:
            signal_number = SIGKILL;
            break;

        case 3:
            signal_number = SIGSTOP;
            break;

        case 4:
            signal_number = SIGCONT;
            break;

        default:
            printf("Invalid signal choice.\n");
            return;
    }

    if (sendProcessSignal(pid, signal_number))
    {
        printf("Signal sent successfully to PID %d.\n", pid);
    }
    else
    {
        printf("Failed to send signal to PID %d.\n", pid);

        if (errno == ESRCH)
        {
            printf("Process does not exist.\n");
        }
        else if (errno == EPERM)
        {
            printf("Permission denied.\n");
        }
        else if (errno == EINVAL)
        {
            printf("Invalid signal.\n");
        }
        else
        {
            printf("An unexpected error occurred.\n");
        }
    }
}