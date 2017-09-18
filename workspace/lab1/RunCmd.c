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

int main(int argc , const char **argv) {
    struct rusage usage;
    pid_t pid;
    char cmd[256];
    switch(pid = fork()) {
        case -1:
            perror("Fork failed");
            break;
        case 0:
            for (int i = 1; i < argc; i++) {
                strcat(cmd, argv[i]);
                strcat(cmd, " ");
            }

            char* passedCmd[] = {"sh", "-c", cmd, NULL};

            execvp(passedCmd[0], passedCmd);
            break;
        default:
            wait(NULL);
            getrusage(RUSAGE_SELF, &usage);
            printf("\nwall time:\t%ld seconds\t%d microseconds\ncpu time:\t%ld seconds\t%d microseconds\nContext switches:\t%ld involuntary\t%ld voluntary\nPage fault:\t%ld\nPage reclaims:\t%ld\n\n", usage.ru_stime.tv_sec, usage.ru_stime.tv_usec, usage.ru_utime.tv_sec, usage.ru_utime.tv_usec, usage.ru_nivcsw, usage.ru_nvcsw, usage.ru_majflt, usage.ru_minflt);
            break;

    }
    return 0;
}
