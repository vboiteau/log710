#ifndef PROCDATA
#define PROCDATA

#include <sys/types.h>
#include <unistd.h>

struct procdata{
    long state;
    pid_t pid;
    pid_t parent_pid;
    uid_t uid;
    char comm[16];
};

#endif
