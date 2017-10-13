#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"
#include <err.h>

// Implementation of the singly-linked list based on the templates on rosetta code.
// http://rosettacode.org/wiki/Singly-linked_list/Element_removal#C

ShellThreadTable addToTable(ShellThreadTable table, char **userInput, int pid, int id, char pName[]) {
    printf("id to add\t%d\n", id);
    ShellThreadTable iterator, temporaryThread;
    if (table == NULL) {
        table = (ShellThreadTable)malloc(sizeof(ShellThread));
        table->userInputTable = userInput;
        table->pid = pid;
        table->id = id;
        table->next = NULL;
        strcpy(table->processName, pName);
    } else {
        iterator = table;
        printf("Next task is causing segmentation fault.\n");
        int tableLength = getTableLength(table);
        printf("The table contains %d rows.\n", tableLength);
        while(iterator->next!=NULL){
            iterator = iterator->next;
        }
        printf("The task hasn't caused segmentation fault.\n");
        temporaryThread = (ShellThreadTable)malloc(sizeof(ShellThread));
        temporaryThread->userInputTable = userInput;
        temporaryThread->pid = pid;
        temporaryThread->id = id;
        strcpy(temporaryThread->processName, pName);
        temporaryThread->next = NULL;
        iterator->next = temporaryThread;
    }
    return table;
}

ShellThreadTable removeFromTable(ShellThreadTable table, int position) {
    printf("position to remove:\t%d\n", position);
    int i=1;
    ShellThreadTable iterator, temporaryThread;

    if (table != NULL) {
        iterator = table;

        if (position == 0) {
            table = table->next;
            iterator->next = NULL;
            free(iterator);
        } else {
            while(i++!=position-1 && iterator != NULL) {
                iterator = iterator->next;
            }
            if (iterator != NULL) {
                temporaryThread = iterator->next;
                iterator->next = temporaryThread->next;
                temporaryThread->next = NULL;
                free(temporaryThread);
            }
        }
    }
    return table;
}

int getPositionOfId(ShellThreadTable table, int id) {
    int i = 0;
    ShellThreadTable iterator = table;
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

int getTableLength(ShellThreadTable table) {
    int i = 0;
    ShellThreadTable iterator = table;
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

char * tableToString(ShellThreadTable table) {
    char printedTable[512] = "";
    char tableHeader[64] = "===\tBackground Tasks Table\t===\n";
    if (strlen(tableHeader) + 1 > sizeof(printedTable) - strlen(printedTable)) {
        err(1, "onstack would be truncated");
    }
    strncat(printedTable, tableHeader, sizeof(printedTable) - strlen(printedTable) - 1);
    /*ShellThreadTable iterator = table;*/
    /*for(int i = 0;*/
            /*iterator != NULL;*/
            /*iterator = iterator->next, i++) {*/
        /*char tableRow[64];*/
        /*[>snprintf(tableRow, sizeof(tableRow), "")<]*/
    /*}*/

    return strdup(printedTable);
}

void printTable(ShellThreadTable table) {
    int i = 0;
    ShellThreadTable iterator = table;
    printf("Task currently in background consists in:\n");
    while(iterator != NULL) {
        printf("[%d]\t%d --- (%s)\n", i, iterator->pid, iterator->processName);
        iterator = iterator->next;
        i++;
    }
};
