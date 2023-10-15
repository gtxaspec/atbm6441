#include "atbm_os.h"
#include "atbm_os_mutex.h"

extern void store_all_int(void);
extern void restore_all_int(void);

void spin_lock_init(spinlock_t *lock)
{
	*lock = 0;
}

void spin_lock(spinlock_t *lock)
{
}

void spin_unlock(spinlock_t *lock)
{
}

void spin_lock_bh(spinlock_t *lock)
{
}

void spin_unlock_bh(spinlock_t *lock)
{
}

void spin_lock_irqsave(spinlock_t *lock, unsigned long flags)
{
	store_all_int();
}

void spin_unlock_irqrestore(spinlock_t *lock, unsigned long flags)
{
	restore_all_int();
}

int mutex_init(struct mutex *lock)
{
	lock->mutex = rt_mutex_create("AtbmMutex", RT_IPC_FLAG_PRIO);
	if(lock->mutex)
	{
		return OS_SUCCESS;
	}
	else
	{
		//atbm_dbg(ATBM_APOLLO_DBG_ERROR, "%s fail\n", __func__);
		printf("%s fail\n", __func__);
		return OS_FAIL;
	}
}

int mutex_destroy(struct mutex *lock)
{
	rt_mutex_delete(lock->mutex);
    return 0;
}

void mutex_lock(struct mutex *lock)
{
	rt_err_t ret = -1;

	ret = rt_mutex_take(lock->mutex, RT_WAITING_FOREVER);
	if (ret != RT_EOK)
	{
		printf("%s fail\n", __func__);
	}
}

void mutex_unlock(struct mutex *lock)
{
	rt_err_t ret = -1;

	ret = rt_mutex_release(lock->mutex);
	if (ret != RT_EOK)
	{
		printf("%s fail\n", __func__);
	}
}

int sema_init(struct semaphore *sem, int val)
{
	sem->sem = rt_sem_create("AtbmSem", val, RT_IPC_FLAG_PRIO);
	if (sem->sem)
	{
		return OS_SUCCESS;
	}
	else
	{
		atbm_dbg(ATBM_APOLLO_DBG_ERROR, "%s fail\n", __func__);
		return OS_FAIL;
	}
}

int sema_destroy(struct semaphore *sem)
{
	rt_sem_delete(sem->sem);
	return 0;
}

void down(struct semaphore *sem)
{
	rt_sem_take(sem->sem, RT_WAITING_FOREVER);
}

int down_timeout(struct semaphore *sem, int timeout)
{
	int ret = 0;

	ret = rt_sem_take(sem->sem, RT_WAITING_FOREVER);
	if(ret == RT_EOK)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

void up(struct semaphore *sem)
{
	rt_sem_release(sem->sem);
}

