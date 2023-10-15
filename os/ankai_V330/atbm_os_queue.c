#include "atbm_os.h"
#include "atbm_os_queue.h"

void init_waitqueue_head(wait_queue_head_t *q)
{
	sema_init(&q->sem, 0);
	INIT_LIST_HEAD(&q->task_list);
}

int __wait_event_timeout(wait_queue_head_t *q, int *con, int timeout) /* ms */
{
	down_timeout(&q->sem, timeout);
	if (*con)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void wake_up(wait_queue_head_t *q)
{
	up(&q->sem);
}

static int workqueue_work(void *data)
{
	struct workqueue_struct *wq = (struct workqueue_struct *)data;

	if (wq == ATBM_NULL)
	{
		return -1;
	}

	while (1)
	{
		down(&wq->sem_sync);
		mutex_lock(&wq->mutex_prot);
		if (wq->work_head.next != &wq->work_head)
		{
			struct work_struct *work = wq->work_head.next;
			while (work != &wq->work_head)
			{
				work->func(work);
				work->prev->next = work->next;
				work->next->prev = work->prev;
				work = work->next;
			}
		}
		mutex_unlock(&wq->mutex_prot);
	}

	return 0;
}

struct workqueue_struct *create_singlethread_workqueue(const char *name)
{
	struct workqueue_struct *wq = ATBM_NULL;

	wq = malloc(sizeof(struct workqueue_struct));
	if (wq)
	{
		memset(wq, 0, sizeof(struct workqueue_struct));
		wq->work_head.prev = wq->work_head.next = &wq->work_head;
		mutex_init(&wq->mutex_prot);
		sema_init(&wq->sem_sync, 0);

		wq->thread = kthread_create(workqueue_work, wq, name);
		if (wq->thread == ATBM_NULL)
		{
			atbm_dbg(ATBM_APOLLO_DBG_ERROR, "%s fail\n", __func__);
			mutex_destroy(&wq->mutex_prot);
			sema_destroy(&wq->sem_sync);
			free(wq);
			wq = ATBM_NULL;
		}
	}

	return wq;
}

void destroy_workqueue(struct workqueue_struct *wq)
{
	kthread_stop(wq->thread);
	mutex_destroy(&wq->mutex_prot);
	sema_destroy(&wq->sem_sync);
	free(wq);
}

void INIT_WORK(struct work_struct *work, work_func work_handle)
{
	memset(work, 0, sizeof(struct work_struct));
	work->func = work_handle;
}

int queue_work(struct workqueue_struct *wq, struct work_struct *work)
{
	mutex_lock(&wq->mutex_prot);
	work->prev = wq->work_head.prev;
	wq->work_head.prev->next = work;
	work->next = &wq->work_head;
	wq->work_head.prev = work;
	mutex_unlock(&wq->mutex_prot);
	up(&wq->sem_sync);
}

int queue_delayed_work(struct workqueue_struct *wq, struct delayed_work *dwork, unsigned long delay)
{
	return 0;
}

void flush_workqueue(struct workqueue_struct *wq)
{
}

bool flush_work(struct work_struct *work)
{
	return true;
}

int work_pending(struct work_struct *work)
{
	return 0;
}

bool cancel_work_sync(struct work_struct *work)
{
	return true;
}

bool cancel_delayed_work_sync(struct delayed_work *dwork)
{
	return true;
}

bool cancel_delayed_work(struct delayed_work *work)
{
	return true;
}


