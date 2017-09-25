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

int main(int argc , char **argv) {
    // Variable initiation
    struct rusage usage;
    pid_t pid;
    char cmdExecFile[32];

    /**
     * Add /bin/ before command
     * TODO: consider using bash command which 
     **/
    sprintf(cmdExecFile, "/bin/%s", argv[1]);
    argv[1] = cmdExecFile;

    switch(pid = fork()) {
        case -1:
            perror("Fork failed");
            break;
        case 0:
            execvp(argv[1], argv+1);
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

    return 0;
}
