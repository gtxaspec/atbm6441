#include "atbm_os.h"
#include "atbm_os_thread.h"

typedef int (*os_pthread)(void *argument); 

typedef struct os_thread_def  {
	const char name[WIFI_TASK_NAME_LEN];
	os_pthread entry;
	unsigned int stack_size;
	unsigned int tick;
	unsigned char priority;
} AtbmThreadDef_t;

AtbmThreadDef_t ATBMTHREAD[WIFI_TASK_NUM] = {0};
static unsigned int TaskNum = 0;

struct task_struct *kthread_create(int (*threadfn)(void *data), void *data, const char *name)
{
	int prio = WORK_TASK_PRIO;
	struct task_struct *task = ATBM_NULL;

	task = malloc(sizeof(struct task_struct));
	if (task == ATBM_NULL)
	{
		return task;
	}

	if (memcmp(name, "sdio_rx", 7) || memcmp(name, "sdio_tx", 7))
	{
		prio = BH_TASK_PRIO;
	}

	memcpy(&ATBMTHREAD[TaskNum].name, name, WIFI_TASK_NAME_LEN);
	ATBMTHREAD[TaskNum].stack_size= 8192;
	ATBMTHREAD[TaskNum].priority = prio;
	ATBMTHREAD[TaskNum].tick = WIFI_TASK_TICK;
	ATBMTHREAD[TaskNum].entry = threadfn;

	task->thread = rt_thread_create(ATBMTHREAD[TaskNum].name, ATBMTHREAD[TaskNum].entry, data, ATBMTHREAD[TaskNum].stack_size, ATBMTHREAD[TaskNum].priority, ATBMTHREAD[TaskNum].tick);
	if (task->thread != RT_NULL)
	{
		//wifi_printk(WIFI_ALWAYS,"thread start\n");
		rt_thread_startup(task->thread);
	}
	else
	{
		atbm_dbg(ATBM_APOLLO_DBG_ERROR, "%s fail\n", __func__);
		free(task);
		return ATBM_NULL;
	}
	TaskNum++;
	return task;
}

int kthread_stop(struct task_struct *thread)
{
	unsigned int result;

	result = rt_thread_delete(thread->thread);
	if (result == RT_EOK)
	{
		return OS_SUCCESS;
	}
	else
	{
		return OS_FAIL;
	}
}

void process_wait_timeout(struct semaphore *wait_sem, int timeout)
{
	down_timeout(wait_sem, timeout);
}

int wake_up_process(struct task_struct *thread)
{
	up(thread->sem_task);
	return 0;
}


