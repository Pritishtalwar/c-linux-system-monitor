#include <stdio.h>
#include "process.h"
#include "proc_reader.h"

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
    printf("7. Exit\n");
    printf("========================================\n");
}

int main(void)
{
    int choice;

    do
    {
        showMenu();

        printf("Enter your choice: ");
        scanf("%d", &choice);

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

                searchProcess(query);
                break;
            }
            
            case 3:
            {
                int pid;

                printf("Enter PID: ");
                scanf("%d", &pid);

                showProcessDetails(pid);
                break;
            }

            case 4:
                printf("Kill Process selected.\n");
                break;

            case 5:
                printf("System Information selected.\n");
                break;

            case 6:
                printf("View Logs selected.\n");
                break;

            case 7:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 7);

    return 0;
}