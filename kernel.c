#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/wait.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/list.h>

DECLARE_WAIT_QUEUE_HEAD(wq_head);
DEFINE_SPINLOCK(wq_lock);
DEFINE_SPINLOCK(wq2_lock);
static int enter_wait_queue(void)
{
    DEFINE_WAIT(wq_entry);
    prepare_to_wait_exclusive(&wq_head, &wq_entry, TASK_INTERRUPTIBLE);

    schedule();

    spin_lock(&wq2_lock);
    __set_current_state(TASK_RUNNING);
    remove_wait_queue(&wq_head, &wq_entry);
    spin_unlock(&wq2_lock);

    return 0;
}

static int clean_wait_queue(void)
{
    while (waitqueue_active(&wq_head))
    {
        wake_up_nr(&wq_head, 1);
        msleep(300);
    }
    return 0;
}

SYSCALL_DEFINE1(call_wait_queue, int, id)
{
    switch (id)
    {
    case 1:
        enter_wait_queue();
        break;
    case 2:
        clean_wait_queue();
        break;
    }
    return 0;
}