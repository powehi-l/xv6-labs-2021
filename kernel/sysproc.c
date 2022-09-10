#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  backtrace();
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64 
sys_sigalarm(void)
{
	int ticks;
	if(argint(0, &ticks) < 0)
		return -1;
	uint64 handler;
	if(argaddr(1, &handler) < 0)
		return -1;
	struct proc* p = myproc();
	p->alarm_interval = ticks;
	p->handler = handler;
	p->ticks_pass = 0;
//	printf("current ticks: %d handler: %d pass: %d", ticks, handler, p->ticks_pass);
	return 0;
}

uint64 
sys_sigreturn(void)
{
  struct proc *p = myproc();
        p->trapframe->ra = p->alarm_ra;
        p->trapframe->sp = p->alarm_sp;
        p->trapframe->gp = p->alarm_gp;
        p->trapframe->tp = p->alarm_tp;

        p->trapframe->t0 = p->alarm_t0;
        p->trapframe->t1 = p->alarm_t1;
        p->trapframe->t2 = p->alarm_t2;
        p->trapframe->t3 = p->alarm_t3;
        p->trapframe->t4 = p->alarm_t4;
        p->trapframe->t5 = p->alarm_t5;
        p->trapframe->t6 = p->alarm_t6;

		    p->trapframe->s0 = p->alarm_s0;
        p->trapframe->s1 = p->alarm_s1;
        p->trapframe->s2 = p->alarm_s2;
        p->trapframe->s3 = p->alarm_s3;
        p->trapframe->s4 = p->alarm_s4;
        p->trapframe->s5 = p->alarm_s5;
        p->trapframe->s6 = p->alarm_s6;
        p->trapframe->s7 = p->alarm_s7;
        p->trapframe->s9 = p->alarm_s9;
        p->trapframe->s10 = p->alarm_s10;
        p->trapframe->s11 = p->alarm_s11;

        p->trapframe->a0 = p->alarm_a0;
        p->trapframe->a1 = p->alarm_a1;
        p->trapframe->a2 = p->alarm_a2;
        p->trapframe->a3 = p->alarm_a3;
        p->trapframe->a4 = p->alarm_a4;
        p->trapframe->a5 = p->alarm_a5;
        p->trapframe->a6 = p->alarm_a6;
        p->trapframe->a7 = p->alarm_a7;
		    p->trapframe->epc = p->alarm_epc;

        p->trapframe->epc = p->alarm_epc;
        p->in_handler = 0;
	return 0;
}
