#include <sys/syscall.h>
#include <stdio.h>
#include "procdata.h"

#define sys_log710a2017as2	378

struct procdata  getprocdata (void){
	struct procdata p;
	long toReturn = syscall(sys_log710a2017as2, &p);
	printf("code de retour: %d\n", toReturn);
	return p;
};
int main (){
	struct procdata p = getprocdata();
	printf("p.pid: %ld\n", p.pid);
        printf("p.ppid: %ld\n", p.parent_pid);
        printf("p.uid: %ld\n", p.uid);
        printf("p.state: %ld\n", p.state);
        printf("p.comm: %s\n", p.comm);
	return 0;
}
