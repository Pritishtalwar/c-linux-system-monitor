#include <stdio.h>
#include <string.h>

#include "process.h"
#include "proc_reader.h"
#include "system_info.h"
#include "process_manager.h"
#include "logger.h"


/* =========================================================
   DISPLAY MAIN MENU
   ========================================================= */

void showMenu(void)
{
    printf("\n");
    printf("========================================\n");
    printf("        LINUX SYSTEM MONITOR\n");
    printf("========================================\n");

    printf("1. Show Running Processes\n");
    printf("2. Search Process\n");
    printf("3. Process Details\n");
    printf("4. Process Management\n");
    printf("5. System Information\n");
    printf("6. View Logs\n");
    printf("7. Live System Monitor\n");
    printf("8. Sort Processes\n");
    printf("9. Exit\n");

    printf("========================================\n");
}


/* =========================================================
   MAIN FUNCTION
   ========================================================= */

int main(void)
{
    int choice;

    logMessage("System monitor started");

    do
    {
        showMenu();

        choice = getIntegerInput("Enter your choice: ");

        switch (choice)
        {
            /* -------------------------------------------------
               1. SHOW RUNNING PROCESSES
               ------------------------------------------------- */

            case 1:

                listProcesses();

                logMessage("Running processes viewed");

                break;


            /* -------------------------------------------------
               2. SEARCH PROCESS
               ------------------------------------------------- */

            case 2:
            {
                char query[256];

                printf("Enter process name to search: ");

                if (fgets(query, sizeof(query), stdin) != NULL)
                {
                    query[strcspn(query, "\n")] = '\0';

                    if (strlen(query) == 0)
                    {
                        printf("Search query cannot be empty.\n");
                        break;
                    }

                    searchProcess(query);

                    logMessage("Process search performed");
                }

                break;
            }


            /* -------------------------------------------------
               3. PROCESS DETAILS
               ------------------------------------------------- */

            case 3:
            {
                int pid;

                pid = getIntegerInput("Enter PID: ");

                if (pid <= 0)
                {
                    printf("Invalid PID. PID must be greater than 0.\n");
                    break;
                }

                showProcessDetails(pid);

                logMessage("Process details viewed");

                break;
            }


            /* -------------------------------------------------
               4. PROCESS MANAGEMENT
               ------------------------------------------------- */

            case 4:

                showProcessManagement();

                logMessage("Process management performed");

                break;


            /* -------------------------------------------------
               5. SYSTEM INFORMATION
               ------------------------------------------------- */

            case 5:

                showSystemInfo();

                logMessage("System information viewed");

                break;


            /* -------------------------------------------------
               6. VIEW LOGS
               ------------------------------------------------- */

            case 6:
            {
                FILE *logFile;
                char line[512];

                logFile = fopen("logs/monitor.log", "r");

                if (logFile == NULL)
                {
                    printf("No logs available yet.\n");
                    break;
                }

                printf("\n");
                printf("========================================\n");
                printf("              LOGS\n");
                printf("========================================\n");

                while (fgets(line, sizeof(line), logFile) != NULL)
                {
                    printf("%s", line);
                }

                fclose(logFile);

                break;
            }


            /* -------------------------------------------------
               7. LIVE SYSTEM MONITOR
               ------------------------------------------------- */

            case 7:

                logMessage("Live system monitor started");

                monitorProcesses();

                logMessage("Live system monitor closed");

                break;


            /* -------------------------------------------------
               8. SORT PROCESSES
               ------------------------------------------------- */

            case 8:

                sortProcesses();

                logMessage("Process sorting performed");

                break;


            /* -------------------------------------------------
               9. EXIT
               ------------------------------------------------- */

            case 9:

                logMessage("System monitor exited");

                printf("\n");
                printf("Exiting Linux System Monitor...\n");

                break;


            /* -------------------------------------------------
               INVALID OPTION
               ------------------------------------------------- */

            default:

                printf("Invalid choice. Please try again.\n");

                break;
        }

    } while (choice != 9);

    return 0;
}