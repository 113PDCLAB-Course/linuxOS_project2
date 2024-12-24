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

// https://davidleitw.github.io/posts/linux_wake_queue1/ 好用的網站

DECLARE_WAIT_QUEUE_HEAD(wq_head);
static bool condition = 0;

static int enter_wait_queue(void)
{
    wait_event_interruptible(wq_head, condition);
    return 0;
}

static int clean_wait_queue(void)
{
    condition = true;
    while (waitqueue_active(&wq_head))
    {
        wake_up_nr(&wq_head, 1);
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