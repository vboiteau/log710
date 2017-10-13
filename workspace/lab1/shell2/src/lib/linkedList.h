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


ShellThreadTable addToTable(ShellThreadTable table, char **userInput, int pid, int id, char pName[]);
ShellThreadTable removeFromTable(ShellThreadTable table, int position); 


int getTableLength(ShellThreadTable table);
int getPositionOfId(ShellThreadTable table, int id); 

void printTable(ShellThreadTable table); 
char* tableToString(ShellThreadTable table);
#endif
