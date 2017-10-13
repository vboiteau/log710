#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"

int getTableLength(ShellThreadTable shellThread) {
    int i = 0;
    ShellThreadTable iterator = shellThread;
    while(iterator != NULL) {
        if (iterator->next != NULL) {
            printf("break at %d\n", i);
            iterator = iterator->next;
            printf("break at %d\n", i);
        } else {
            iterator = NULL;
        }
        i++;
    }
    return i;
};

ShellThreadTable addToTable(ShellThreadTable shellThread, char **userInput, int pid, int id, char pName[]) {
    printf("id to add\t%d\n", id);
    ShellThreadTable iterator, temporaryShellThread;
    if (shellThread == NULL) {
        shellThread = (ShellThreadTable)malloc(sizeof(ShellThread));
        shellThread->userInputTable = userInput;
        shellThread->pid = pid;
        shellThread->id = id;
        shellThread->next = NULL;
        strcpy(shellThread->processName, pName);
    } else {
        iterator = shellThread;
        printf("Next task is causing segmentation fault.\n");
        int tableLength = getTableLength(shellThread);
        printf("The table contains %d rows.\n", tableLength);
        while(iterator->next!=NULL){
            iterator = iterator->next;
        }
        printf("The task hasn't caused segmentation fault.\n");
        temporaryShellThread = (ShellThreadTable)malloc(sizeof(ShellThread));
        temporaryShellThread->userInputTable = userInput;
        temporaryShellThread->pid = pid;
        temporaryShellThread->id = id;
        strcpy(temporaryShellThread->processName, pName);
        temporaryShellThread->next = NULL;
        iterator->next = temporaryShellThread;
    }
    return shellThread;
}

ShellThreadTable removeFromTable(ShellThreadTable startShellThread, int position) {
    printf("position to remove:\t%d\n", position);
    int i=1;
    ShellThreadTable iterator, temporaryShellThread;

    if (startShellThread != NULL) {
        iterator = startShellThread;

        if (position == 0) {
            startShellThread = startShellThread->next;
            /*iterator->next = NULL;*/
            free(iterator);
        } else {
            while(i++!=position-1 && iterator != NULL) {
                iterator = iterator->next;
            }
            if (iterator != NULL) {
                temporaryShellThread = iterator->next;
                iterator->next = temporaryShellThread->next;
                /*temporaryShellThread->next = NULL;*/
                free(temporaryShellThread);
            }

        }
    }
    return startShellThread;
}

int getPositionOfId(ShellThreadTable shellThread, int id) {
    int i = 0;
    ShellThreadTable iterator = shellThread;
    while(iterator != NULL) {
        printf("id to get\t%d\nid of iterator\t%d\n", id, iterator->id);
        if (iterator->id == id) {
           break; 
        }
        iterator = iterator->next;
        i++;
    }
    return i;
}

/*ShellThreadTable removeFromTableById(ShellThreadTable shellThread, int id) {*/
    /*ShellThreadTable iterator, temporaryShellThread;*/


    /*if (shellThread != NULL) {*/
        /*iterator = shellThread;*/

        /*if (iterator->id == id) {*/
            /*shellThread = shellThread->next;*/
            /*iterator->next = NULL;*/
            /*free(iterator);*/
        /*} else {*/
            /*while(iterator != NULL && iterator->id != id) {*/
                /*iterator = iterator->next;*/
            /*}*/
            /*if (iterator != NULL) {*/
                /*temporaryShellThread = iterator->next;*/
                /*if (temporaryShellThread != NULL) {*/
                    /*iterator->next = temporaryShellThread->next;*/
                    /*temporaryShellThread->next = NULL;*/
                    /*free(temporaryShellThread);*/
                /*}*/
            /*}*/

        /*}*/
    /*}*/
    /*return shellThread;*/
/*}*/

void printTable(ShellThreadTable shellThread) {
    int i = 0;
    ShellThreadTable iterator = shellThread;
    printf("Task currently in background consists in:\n");
    while(iterator != NULL) {
        printf("[%d]\t%d --- (%s)\n", i, iterator->pid, iterator->processName);
        iterator = iterator->next;
        i++;
    }
};
