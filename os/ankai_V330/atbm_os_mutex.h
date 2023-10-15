#ifndef ATBM_OS_MUTEX_H
#define ATBM_OS_MUTEX_H

#include "rtdef.h"

typedef unsigned char spinlock_t;

struct mutex
{
	rt_mutex_t mutex;
};

struct semaphore 
{
	rt_sem_t sem;
};

void spin_lock_init(spinlock_t *lock);
void spin_lock(spinlock_t *lock);
void spin_unlock(spinlock_t *lock);
void spin_lock_bh(spinlock_t *lock);
void spin_unlock_bh(spinlock_t *lock);
void spin_lock_irqsave(spinlock_t *lock, unsigned long flags);
void spin_unlock_irqrestore(spinlock_t *lock, unsigned long flags);
int mutex_init(struct mutex *lock);
int mutex_destroy(struct mutex *lock);
void mutex_lock(struct mutex *lock);
void mutex_unlock(struct mutex *lock);
int sema_init(struct semaphore *sem, int val);
int sema_destroy(struct semaphore *sem);
void down(struct semaphore *sem);
int down_timeout(struct semaphore *sem, int timeout);
void up(struct semaphore *sem);

#endif // #ifndef ATBM_OS_MUTEX_H

