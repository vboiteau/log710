#include <sys/types.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
/**
 * stackoverflow visited 18 sept. 2017 
 * https://stackoverflow.com/questions/17432502/how-can-i-measure-cpu-time-and-wall-clock-time-on-both-linux-windows
 **/
double get_wall_time() {
    struct timeval time;
    if(gettimeofday(&time, NULL)) {
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

double get_cpu_time() {
    return (double) clock() / CLOCKS_PER_SEC;
}
/* End of stackoverflow ref */

double getSeconds(double time) {
    return floor(time);
}

double getMicroseconds(double time) {
    return fmod(round((time * 1000000)), 1000000);
}

int main(int argc , const char **argv) {
    pid_t pid;
    char cmd[256];
    double startWall = get_wall_time();
    double startCPU = get_cpu_time();
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
            double endWall = get_wall_time();
            double endCPU = get_cpu_time();
            double durationWall = endWall - startWall;
            double durationCPU = endCPU - startCPU;
            printf("\nwall time:\t%.0f seconds\t%.0f microseconds\ncpu time:\t%.0f seconds\t%.0f microseconds\n\n", getSeconds(durationWall), getMicroseconds(durationWall), getSeconds(durationCPU), getMicroseconds(durationCPU));
            break;

    }
    return 0;
}
