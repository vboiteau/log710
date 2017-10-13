#include <sys/types.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#define _GNU_SOURCE

pthread_mutex_t threadIdLock;
pthread_mutex_t shellThreadTableLock;
int lastThreadID = 1;

int incrementLastThreadID(){
    pthread_mutex_lock(&threadIdLock);
    int temp;
    lastThreadID ++;
    temp = lastThreadID;
    pthread_mutex_unlock(&threadIdLock);
    return temp;
}



// Implementation of the singly-linked list based on the templates on rosetta code.
// http://rosettacode.org/wiki/Singly-linked_list/Element_removal#C
typedef struct elem{
    struct elem *next;
    char ** userInputTable;
    int pid;
    int id;
    char processName[];
} ShellThread;

typedef ShellThread* ShellThreadTable;

ShellThreadTable addToTable(ShellThreadTable shellThread, char **userInput, int pid, int id, char pName[]) {
    ShellThreadTable iterator, temporaryShellThread;

    if (shellThread == NULL) {
        shellThread = (ShellThreadTable)malloc(sizeof(ShellThread));
        shellThread->userInputTable = userInput;
        shellThread->pid = pid;
        shellThread->id = id;
        strcpy(shellThread->processName, pName);
    } else {
        iterator = shellThread;
        while(iterator->next!=NULL){
            iterator = iterator->next;
        }
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
    int i=1;
    ShellThreadTable iterator, temporaryShellThread;

    if (startShellThread != NULL) {
        iterator = startShellThread;

        if (position == 0) {
            startShellThread = startShellThread->next;
            iterator->next = NULL;
            //free(iterator);
        } else {
            while(i++!=position-1 && iterator != NULL) {
                iterator = iterator->next;
            }
            if (iterator != NULL) {
                temporaryShellThread = iterator->next;
                iterator->next = temporaryShellThread->next;
                temporaryShellThread->next = NULL;
                //free(temporaryShellThread);
            }

        }
    }
    return startShellThread;
}

ShellThreadTable removeFromTableById(ShellThreadTable shellThread, int id) {
    ShellThreadTable tmp = shellThread;
    ShellThreadTable prev = NULL;

    while (tmp->id != id && tmp->next != NULL)
    {
        prev = tmp;
        tmp = tmp->next;
    }
    if (tmp->id == id)
    {
        if (prev)
        {
            prev->next = tmp->next;
        }
        else
        {
            shellThread = tmp->next;
        }
    }
    return shellThread;
}

int getTableLength(ShellThreadTable shellThread) {
    int i = 0;
    ShellThreadTable iterator = shellThread;
    while(iterator != NULL) {
        iterator = iterator->next;
        i++;
    }
    return i;
};

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

ShellThreadTable startShellThread = NULL;

void safeAddtoTable(ShellThreadTable shellThread, char **userInput, int pid, int id, char pName[]) {
    pthread_mutex_lock(&shellThreadTableLock);
    startShellThread = addToTable(shellThread, userInput, pid, id, pName);
    pthread_mutex_unlock(&shellThreadTableLock);
}

void safeDeleteFromTableById(ShellThreadTable shellThread, int id){
    pthread_mutex_lock(&shellThreadTableLock);
    startShellThread = removeFromTableById(shellThread, id);
    pthread_mutex_unlock(&shellThreadTableLock);
}

void *threadForking(void *arg){
    char **splitInput = (char **)arg;
    printf("IN THREAD!\n");
    //myShellThread->pid = myShellThread->pid +1;
    //printf("pid = %d", myShellThread->pid);
    struct rusage usage;
    pid_t pid;
    int id;
    char cmdExecFile[32];
    sprintf(cmdExecFile, "/bin/%s", splitInput[0]);
    splitInput[0] = cmdExecFile;
    char *stringcopy = malloc (1 + strlen (cmdExecFile));
    if (stringcopy)
        strcpy (stringcopy, cmdExecFile);
    switch(pid = fork()) {
        case -1:
            perror("Fork failed");
            break;
        case 0:
            execvp(splitInput[0], splitInput);
            break;
        default:
            id = incrementLastThreadID();
            printf("[%d] %d \n", id, pid);
            fflush(stdout);
            safeAddtoTable(startShellThread, splitInput, pid, id, stringcopy);
            wait(NULL);
            getrusage(RUSAGE_SELF, &usage);
            /* info about rusage found in man 2 getrusage page */
            printf(
                  "\nsystem time:\t%ld seconds\t%d microseconds\nuser time:\t%ld seconds\t%d microseconds\nContext switches:\t%ld involuntary\t%ld voluntary\nPage fault:\t%ld\nPage reclaims:\t%ld\n\n",
                  usage.ru_stime.tv_sec, // system seconds
                  usage.ru_stime.tv_usec, // system microseconds
                  usage.ru_utime.tv_sec, // user seconds
                  usage.ru_utime.tv_usec, // user microseconds
                  usage.ru_nivcsw, // involuntary interruption
                  usage.ru_nvcsw, // voluntary interruption
                  usage.ru_majflt, // page faults
                  usage.ru_minflt // page reclaims
                );
            safeDeleteFromTableById(startShellThread, id);
            break;
    }
    //myShellThread->pid--;
    return 0;
}

void test(void *arg){
  ShellThread *myShellThread = (ShellThread*)arg;
  struct rusage usage;
  char cmdExecFile[32];
  pid_t pid;
  sprintf(cmdExecFile, "/bin/%s", myShellThread->userInputTable[0]);
  myShellThread->userInputTable[0] = cmdExecFile;
  switch(pid = fork()) {
      case -1:
          perror("Fork failed");
          break;
      case 0:
          execvp(myShellThread->userInputTable[0], myShellThread->userInputTable);
          break;
      default:
          wait(NULL);
          getrusage(RUSAGE_SELF, &usage);
          /* info about rusage found in man 2 getrusage page */
          printf(
                  "\nsystem time:\t%ld seconds\t%d microseconds\nuser time:\t%ld seconds\t%d microseconds\nContext switches:\t%ld involuntary\t%ld voluntary\nPage fault:\t%ld\nPage reclaims:\t%ld\n\n",
                  usage.ru_stime.tv_sec, // system seconds
                  usage.ru_stime.tv_usec, // system microseconds
                  usage.ru_utime.tv_sec, // user seconds
                  usage.ru_utime.tv_usec, // user microseconds
                  usage.ru_nivcsw, // involuntary interruption
                  usage.ru_nvcsw, // voluntary interruption
                  usage.ru_majflt, // page faults
                  usage.ru_minflt // page reclaims
                );
          break;
  }
}


int main(int argc , char **argv) {
    char userInput[160] = " ";
    char workingDir[160] = ".";
    int threadCount = 1;
    int * threadTable;
    while (strcmp (userInput, "exit") != 0){
        start:
        printf("%s", "Log710A2017%> ");
        fgets(userInput, sizeof userInput, stdin);
        userInput[strcspn(userInput, "\r\n")] = 0;
        //scanf("%[^\n]%*c", userInput);
        fflush(stdin);
        fflush(stdin);
      char ** splitInput;
      char * splitter = strtok (userInput, " ");
      int whiteSpacesCount = 0, i;

      if(strcmp (userInput, "exit") == 0){
          pthread_mutex_lock(&shellThreadTableLock);
          int a = getTableLength(startShellThread);
          pthread_mutex_unlock(&shellThreadTableLock);
          if(a > 0){
              printf("Impossible de quitter, %d threads sont en cours\n", a);
              goto start;
          }
        return 0;
      }

      while (splitter) {
        splitInput = realloc (splitInput, sizeof (char*) * ++whiteSpacesCount);
        if (splitInput == NULL)
          exit (-1); /* memory allocation failed */
        splitInput[whiteSpacesCount-1] = splitter;
        splitter = strtok (NULL, " ");
      }
      splitInput = realloc (splitInput, sizeof (char*) * (whiteSpacesCount+1));
      splitInput[whiteSpacesCount] = 0;
      // Variable initiation

      /**
       * Add /bin/ before command
       * TODO: consider using bash command which
       **/
       //DEBUG PURPOSE
       int count = 0;
       for (i = 0; i < (whiteSpacesCount+1); ++i){
         //printf ("splitInput[%d] = %s\n", i, splitInput[i]);
         count++;
       }
       count--;
       //printf("sizeof of array: %d\n", count);

        if (splitInput[0] == NULL){
            goto start;
        }
      if(strcmp (splitInput[0], "exit") == 0){
          pthread_mutex_lock(&shellThreadTableLock);
          int a = getTableLength(startShellThread);
          pthread_mutex_unlock(&shellThreadTableLock);
          if(a > 0){
              printf("Impossible de quitter, %d threads sont en cours\n", a);
              goto start;
          }
          return 0;
      }
      else if(strcmp (splitInput[0], "cd") == 0){
          chdir(splitInput[1]);
      }
      else if(strcmp (splitInput[0], "aptaches") == 0){
          pthread_mutex_lock(&shellThreadTableLock);
          printTable(startShellThread);
          pthread_mutex_unlock(&shellThreadTableLock);
      }
      else if(strcmp (splitInput[count-1], "&") == 0){
        splitInput[count-1] = NULL;
        pthread_t tid;
//        ShellThread *shellThread1;
//        shellThread1 = malloc(sizeof(ShellThread));
//        (*shellThread1).userInputTable = splitInput;
        pthread_create(&tid, NULL, threadForking, (void*) splitInput);
      }
      else{
        ShellThread *shellThread1;
        shellThread1 = malloc(sizeof(ShellThread));
        (*shellThread1).userInputTable = splitInput;
        test((void*) shellThread1);
      }
    }
    return 0;
}


