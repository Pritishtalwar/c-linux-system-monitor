#include <stdio.h>

#include "process.h"
#include "proc_reader.h"
#include "system_info.h"
#include "process_manager.h"
#include "logger.h"

void showMenu(void)
{
    printf("\n");
    printf("========================================\n");
    printf("        LINUX SYSTEM MONITOR\n");
    printf("========================================\n");
    printf("1. Show Running Processes\n");
    printf("2. Search Process\n");
    printf("3. Process Details\n");
    printf("4. Kill Process\n");
    printf("5. System Information\n");
    printf("6. View Logs\n");
    printf("7. Live System Monitor\n");
    printf("8. Sort Processes\n");
    printf("9. Exit\n");
    printf("========================================\n");
}

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
            case 1:
                listProcesses();
                break;

            case 2:
            {
                char query[256];

                printf("Enter process name to search: ");
                scanf("%255s", query);

                while (getchar() != '\n');

                searchProcess(query);
                logMessage("Process search performed");
                break;
            }

            case 3:
            {
                int pid;

                pid = getIntegerInput("Enter PID: ");

                showProcessDetails(pid);
                logMessage("Process details viewed");
                break;
            }

            case 4:
                showProcessManagement();
                logMessage("Process management viewed");
                break;

            case 5:
                showSystemInfo();
                logMessage("System information viewed");
                break;

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

                printf("\n========================================\n");
                printf("              LOGS\n");
                printf("========================================\n");

                while (fgets(line, sizeof(line), logFile) != NULL)
                {
                    printf("%s", line);
                }

                fclose(logFile);
                break;
            }

            case 7:
                monitorProcesses();
                logMessage("Live system monitor used");
                break;

            case 8:
                sortProcesses();
                logMessage("Process sorting performed");
                break;
            
            case 9:
                logMessage("System monitor exited");
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 9);

    return 0;
}