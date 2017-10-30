#include <sys/syscall.h>
#include "procdata.h"
#include <stdio.h>
#include <current.h>
#include <asm-generic/current.h>
#include <asm-generic/uaccess.h>
#include <linux/sched.h>

#define sys_log710a2017as2 CHOSENNUMBER

long callAS2 (void) {
    return (long) syscall(sys_log710a2017as2);
};

int main() {
    struct procdata currentProcData;
    struct procdata* procDataToPass;
    currentProcData->pid = getpid();
    currentProcData->parent_pid = getppid();
    currentProcData->uid = getuid();
    struct task_struct taskStruct = get_current();
    currentProcData->state = taskStruct->state;
    currentProcData->comm = taskStruct->comm;
    if (copy_to_user(procDataToPass, currentProcData, sizeof(procdata))) {
        return -EFAULT;
    }
    printf("Lecode de retour du deuxième apple system est : %d\n", callAS2());
    return 0;
}
