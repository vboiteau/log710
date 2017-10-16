#include <linux/kernel.h>
#include <linux/syscall.h>

asmlinkage long sys_log710a2017as1(void) {
    printk(KERN_ALERT "LOG710 AUTOMNE 2017 Appel Systeme 01!\n");
    return 0;
}
