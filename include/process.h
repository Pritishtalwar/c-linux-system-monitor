#ifndef PROCESS_H
#define PROCESS_H

void listProcesses(void);
void searchProcess(const char *query);
void monitorProcesses(void);
int getIntegerInput(const char *prompt);
void sortProcesses(void);

#endif