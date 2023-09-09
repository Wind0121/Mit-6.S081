#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"

uint64
sys_sysinfo(void)
{
    struct sysinfo info;
    freebyte(&info.freemem);
    procnum(&info.nproc);

    uint64 addr;
    argaddr(0,&addr);

    if (copyout(myproc()->pagetable, addr, (char *)&info, sizeof info) < 0)
        return -1;

    return 0;
}