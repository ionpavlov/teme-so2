/*
 * User: Alexandru George Burghelea
 * 342C5
 * SO2 2013
 * Tema 1
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/sched.h>
#include "sci_lin.h"

MODULE_DESCRIPTION("System call interceptor");
MODULE_AUTHOR("Alexandru George Burghelea");
MODULE_LICENSE("GPL");

extern void *sys_call_table[];
extern long my_nr_syscalls;

asmlinkage long my_syscall(int cmd, long syscall, long pid)
{
	printk(LOG_LEVEL "THIS IS ME TRING TO INTERCEPT THE CALLS");

	switch (cmd)
	{
		case REQUEST_SYSCALL_INTERCEPT:
			printk(LOG_LEVEL "Intercept request for %ld\n", syscall);
			break;
		case REQUEST_SYSCALL_RELEASE:
			printk(LOG_LEVEL "Release request for %ld\n", syscall);
			break;
		case REQUEST_START_MONITOR:
			printk(LOG_LEVEL "Monitor request for %ld %ld\n", pid, syscall);
			break;
		case REQUEST_STOP_MONITOR:
			printk(LOG_LEVEL "Stop request for %ld %ld\n", pid, syscall);
			break;
		default:
			printk(LOG_LEVEL ">>>> PANICA <<<<\n");

	}

	return 0;
}

static int sci_init(void)
{
	printk(LOG_LEVEL "SCI Loading %ld\n", my_nr_syscalls);
	sys_call_table[MY_SYSCALL_NO] = my_syscall;
	return 0;
}



static void sci_exit(void)
{
	printk(LOG_LEVEL "SCI Unloading\n");
}

module_init(sci_init);
module_exit(sci_exit);