#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

typedef struct elem{
    struct elem *next;
    char ** userInputTable;
    int pid;
    int id;
    char processName[];
} ShellThread;

typedef ShellThread* ShellThreadTable;

int getTableLength(ShellThreadTable shellTable);

ShellThreadTable addToTable(ShellThreadTable shellThread, char **userInput, int pid, int id, char pName[]);

ShellThreadTable removeFromTable(ShellThreadTable startShellThread, int position); 


int getPositionOfId(ShellThreadTable shellThread, int id); 

void printTable(ShellThreadTable shellThread); 

#endif
