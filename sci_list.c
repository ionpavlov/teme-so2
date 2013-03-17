#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/list.h>

#include "sci_list.h"

static struct list_head head;

void sci_info_init(void){
    INIT_LIST_HEAD(&head);
}

struct sci_info *sci_info_alloc(long syscall, long pid) {
    struct sci_info *si;

    si = kmalloc(sizeof(*si), GFP_KERNEL);
    if (si == NULL)
        return NULL;
    si->syscall = syscall;
    si->pid = pid;

    return si;
}

void  sci_info_add(long syscall, long pid)
{
    struct sci_info *si;
    if (sci_info_contains_pid_syscall(pid,syscall)) {
        return;
    }
    si = sci_info_alloc(syscall, pid);
    if (pid == 0)
        sci_info_remove_for_syscall(syscall);
    printk(KERN_ALERT "ADDING ELEMETN\n");
    
    list_add(&si->list, &head);
    
    sci_info_print_list();
    
}
void sci_info_remove_for_syscall(long syscall)
{
    struct list_head *p, *q;
	struct sci_info *si;

	list_for_each_safe(p, q, &head) {
		si = list_entry(p, struct sci_info, list);
        if (syscall == -1 || si->syscall == syscall) {
            list_del(p);
            kfree(si);
        }
	}
}

void sci_info_remove_for_pid(long pid)
{
    struct list_head *p, *q;
	struct sci_info *si;

	list_for_each_safe(p, q, &head) {
		si = list_entry(p, struct sci_info, list);
        if (si->pid == pid) {
            list_del(p);
            kfree(si);
        }
	}
}

void sci_info_remove_for_pid_syscall(long pid, long syscall)
{
    struct list_head *p, *q;
	struct sci_info *si;

	list_for_each_safe(p, q, &head) {
		si = list_entry(p, struct sci_info, list);
       if ((si->pid == pid || si->pid == 0) && si->syscall == syscall){
            list_del(p);
            kfree(si);
        }
	}
}

void sci_info_purge_list(void)
{
	sci_info_remove_for_syscall(-1);
}

int sci_info_contains_pid_syscall(long pid, long syscall){
	struct list_head *p;
	struct sci_info *si;

	list_for_each(p, &head) {
		si = list_entry(p, struct sci_info, list);
		if ((si->pid == pid || si->pid == 0) && si->syscall == syscall)
			return 1;
	}

	return 0;
}
void sci_info_print_list(void)
{
	struct list_head *p;
	struct sci_info *si;

	printk(KERN_ALERT ": [ ");
	list_for_each(p, &head) {
		si = list_entry(p, struct sci_info, list);
		printk("(s = %ld, d = %ld) ", si->syscall, si->pid);
	}
	printk("]\n");
}