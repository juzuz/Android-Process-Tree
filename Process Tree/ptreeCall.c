#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/list.h>
#include<linux/syscalls.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/unistd.h>
MODULE_LICENSE("Dual BSD/GPL");
// MODULE_LICENSE("GPL");



#define  __NR_ptreecall 356


struct prinfo {
pid_t parent_pid;
/* process id of parent, set 0 if it has no parent*/
pid_t pid;
/* process id */
pid_t first_child_pid;
/* pid of youngest child, set 0 if it has no child */
pid_t next_sibling_pid; /* pid of older sibling, set 0 if it has no sibling*/
long state;
/* current state of process */
long uid;
/* user id of process owner */
char comm[64];
/* name of program executed */
};


//github.com/torvalds/linux/blob/master/include/linux/sched.h     line 632 def of task_struct
//https://docs.huihoo.com/doxygen/linux/kernel/3.7/structlist__head.html
//https://github.com/torvalds/linux/blob/master/include/linux/list.h  def of list
void copyInfo(struct prinfo *p,struct task_struct *t)
{
    p->parent_pid = t ->parent->pid;
    p ->pid = t->pid;
    
 
    if (list_empty(&(t->children)))
        p->first_child_pid = 0;
    else
        p->first_child_pid = list_entry((&t->children)-> next,struct task_struct,sibling)->pid;

    if (list_empty(&(t->sibling)))
        p->next_sibling_pid = 0;
    else
    {
        pid_t sib_pid = list_entry(&t->sibling,struct task_struct,sibling)->pid;
        if(sib_pid == p->parent_pid) 
            p->next_sibling_pid = 0;
        else 
            p->next_sibling_pid = sib_pid;
    }
    p->state = t ->state;
    p->uid = t ->cred -> uid;
    //l-1711 sched.h
    get_task_comm(p->comm,t);  
    
}

void dfs(struct prinfo *buf, int *nr, struct task_struct *t)
{
    struct task_struct *tmp;
 //cursor
    struct list_head *lh;

    copyInfo(&buf[*nr],t);
    *nr += 1;

    
    
    list_for_each(lh,&t->children)
    {
        tmp = list_entry(lh,struct task_struct,sibling);
        dfs(buf,nr,tmp);
    }

    
}

static int (*oldcall)(void);

static int sys_ptree(struct prinfo *buf,int *nr)
{
    struct prinfo *tbuf;
    int *tnr;

    tbuf = kmalloc_array(1000,sizeof(*buf),GFP_KERNEL);
    tnr = kmalloc(sizeof(int),GFP_KERNEL);

    if(tbuf== NULL || tnr == NULL)
    {
            printk("Allocation failed");
            return -EADDRNOTAVAIL;
    }  
    *tnr = 0;

    read_lock(&tasklist_lock);
        dfs(tbuf,tnr,&init_task);
    read_unlock(&tasklist_lock);


    if (copy_to_user(buf, tbuf, 1000 * sizeof(*tbuf))) {
		printk("Buffer memory copy failed!\n");
		return -EFAULT;
	}

    if (copy_to_user(nr, tnr, sizeof(int))) {
		printk("NR copy failed!\n");
		return -EFAULT;
	}

    kfree(tbuf);  
    kfree(tnr);

    return *nr;
   // printk("this is my system second call!\n the uid = %ld\n str: %s\n",n,str);
    //return n;
}

static int addsyscall_init(void)
{
    long *syscall = (long*)0xc000d8c4;
    oldcall = (int(*)(void))(syscall[__NR_ptreecall]);
    syscall[__NR_ptreecall] = (unsigned long) sys_ptree;
    printk(KERN_INFO "module load!\n");
    return 0;
}

static void addsyscall_exit(void)
{
    long *syscall = (long*)0xc000d8c4;
    syscall[__NR_ptreecall] = (unsigned long)oldcall;
    printk(KERN_INFO "module exit!\n");
}
module_init(addsyscall_init);
module_exit(addsyscall_exit);

