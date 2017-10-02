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

struct ShellThread {
   char ** userInputTable;
   int pid;
};

void *threadForking(void *arg){
  struct ShellThread *myShellThread = (struct ShellThread*)arg;
  printf("IN THREAD!\n");
  //myShellThread->pid = myShellThread->pid +1;
  //printf("pid = %d", myShellThread->pid);
  struct rusage usage;
  pid_t pid;
  char cmdExecFile[32];
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
          printf("[%d] %d", myShellThread->pid, pid);
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
  //myShellThread->pid--;
  return 0;
}

void test(void *arg){
  struct ShellThread *myShellThread = (struct ShellThread*)arg;
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
      printf("%s", "Log710A2017%> ");
      scanf(" %[^\n]s", userInput);
      char ** splitInput;
      char * splitter = strtok (userInput, " ");
      int whiteSpacesCount = 0, i;

      if(strcmp (userInput, "exit") == 0){
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
         printf ("splitInput[%d] = %s\n", i, splitInput[i]);
         count++;
       }
       count--;
       printf("sizeof of array: %d\n", count);

      if(strcmp (splitInput[0], "exit") == 0){
          return 0;
      }
      else if(strcmp (splitInput[0], "cd") == 0){
          chdir(splitInput[1]);
      }
      else if(strcmp (splitInput[0], "aptaches") == 0){
          printf("Nombre de thread en cours : %d\n", threadCount);
      }
      else if(strcmp (splitInput[count-1], "&") == 0){
        splitInput[count-1] = NULL;
        pthread_t tid;
        printf("Before Thread...\n");
        struct ShellThread *shellThread1;
        shellThread1 = malloc(sizeof(struct ShellThread));
        (*shellThread1).userInputTable = splitInput;
        (*shellThread1).pid = threadCount;
        pthread_create(&tid, NULL, threadForking, (void*) shellThread1);
        printf("current threadcount : %d\n", threadCount);
        //threadTable[threadCount-1] = threadCount;
        //threadCount++;
        printf("After Thread...\n");
      }
      else{
        struct ShellThread *shellThread1;
        shellThread1 = malloc(sizeof(struct ShellThread));
        (*shellThread1).userInputTable = splitInput;
        test((void*) shellThread1);
      }
    }
    return 0;
}
