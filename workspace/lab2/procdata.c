#include <linux/syscalls.h>
#include <asm/current.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/uidgid.h>


asmlinkage long sys_log710a2017as2(const struct procdata __user *p){
    struct procdata trustedProcData;
    printk(KERN_ALERT "LOG710 L2P3: pid: %ld\n", (long)current->pid);
    printk(KERN_ALERT "LOG710 L2P3: ppid: %d\n", task_ppid_nr(current));
    printk(KERN_ALERT "LOG710 L2P3: uid: %d\n", current->cred->uid);
    printk(KERN_ALERT "LOG710 L2P3: state: %d\n", current->state);
    if(!p){
	return -EFAULT;
    }
    trustedProcData.pid = current->pid;
    //trustedProcData->parent_pid = task_ppid_nr(current);
    trustedProcData.parent_pid = current->parent->pid;
    trustedProcData.uid = __kuid_val(current->cred->uid);
    trustedProcData.state = current->state;
    strcpy(trustedProcData.comm, current->comm);
    printk(KERN_ALERT "LOG710 L2P3: p.pid: %ld\n", (long)trustedProcData.pid);
    printk(KERN_ALERT "LOG710 L2P3: p.ppid: %d\n", trustedProcData.parent_pid);
    printk(KERN_ALERT "LOG710 L2P3: p.uid: %d\n", trustedProcData.uid);
    printk(KERN_ALERT "LOG710 L2P3: p.state: %d\n", trustedProcData.state);
    if(copy_to_user(p, &trustedProcData, sizeof(trustedProcData))){
	return -EFAULT;
    }
    return 0;
}
