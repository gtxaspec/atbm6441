#ifndef ATBM_OS_THREAD_H
#define ATBM_OS_THREAD_H

#include "rtdef.h"

#define WIFI_TASK_TICK               (5)
#define WIFI_TASK_NUM                (8)
#define WIFI_TASK_NAME_LEN           (16)
#define WIFI_THREAD_PRIO_START 		 (8)
#define BH_TASK_PRIO                 (WIFI_THREAD_PRIO_START)	 /** High **/
#define ELOOP_TASK_PRIO              (WIFI_THREAD_PRIO_START + 1)
#define WORK_TASK_PRIO               (WIFI_THREAD_PRIO_START + 2)
#define HIF_TASK_PRIO                (WIFI_THREAD_PRIO_START + 3)
#define MAIL_BOX_BH_PRIO             (WIFI_THREAD_PRIO_START - 1)

struct task_struct
{
	struct rt_thread *thread;
	struct semaphore *sem_task;
};

struct sched_param { 
	int sched_priority;
};

struct completion {
	int *a;
};

#define current 0
#define SCHED_FIFO 0

#define sched_setscheduler(x,y,z)

struct task_struct *kthread_create(int (*threadfn)(void *data), void *data, const char *name);
int kthread_stop(struct task_struct *thread);
void process_wait_timeout(struct semaphore *wait_sem, int timeout);
int wake_up_process(struct task_struct *thread);

#endif // #ifndef ATBM_OS_THREAD_H

