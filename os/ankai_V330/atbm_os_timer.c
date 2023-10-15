#include "atbm_os.h"
#include "atbm_os_timer.h"

#define ATBM_TIME_HASH_MAX  (8)
#define ATBM_AK_TIME_ALIG   (5)
static struct atbm_timer_list *atbm_timer_hash[ATBM_TIME_HASH_MAX];
static struct mutex hash_lock;
static signed long cur_timeout_id = -1;
static unsigned int set_start_timer = 0;
static int hash_lock_init = 0;

#define ATBM_AK_TIMEOUT_ALIG(_time)			((_time)+ATBM_AK_TIME_ALIG-((_time)%ATBM_AK_TIME_ALIG))
#define ATBM_AK_TIMEOUT_VAL(__val)			(((__val)%ATBM_AK_TIME_ALIG)== 0?(__val):ATBM_AK_TIMEOUT_ALIG(__val))
#define ATBM_AK_TIMER_NO_LOOP				(0)
#define TIMER_HASH(__index)					((__index)&(ATBM_TIME_HASH_MAX-1))

#define HASH_LOCK()							mutex_lock(&hash_lock)
#define HASH_UNLOCK()						mutex_unlock(&hash_lock)
#define HASH_LOCK_INIT()					mutex_init(&hash_lock)
#define TIMER_HASH_GET(__index)				(atbm_timer_hash[__index])
#define TIMER_HASH_SET(__index,__timer)		atbm_timer_hash[__index] = __timer;

#define ATBM_SET_TIMER_IN_START(__start)	(*((volatile unsigned int *)(&set_start_timer)) = (unsigned int)(__start))
#define ATBM_GET_TIMER_IN_START()			*((volatile unsigned int *)(&set_start_timer))
#define ATBM_SET_CUR_ID(__id)				(*((volatile unsigned int *)(&cur_timeout_id)) = (unsigned int)(__id))
#define ATBM_GET_CUR_ID()					*((volatile unsigned int *)(&cur_timeout_id))

static void __timer_hash_add(struct atbm_timer_list *timer)
{
	unsigned int hash_index = TIMER_HASH(timer->TimerHander);
	timer->hnext = TIMER_HASH_GET(hash_index);
	TIMER_HASH_SET(hash_index,timer);
}

static struct atbm_timer_list *__timer_hash_get(unsigned int timer_handle)
{
	unsigned int hash_index = TIMER_HASH(timer_handle);
	struct atbm_timer_list *timer = TIMER_HASH_GET(hash_index);

	while(timer)
	{
		if(timer->TimerHander == timer_handle)
		{
			break;
		}
		timer = timer->hnext;
	}

	return timer;
}

static void __timer_hash_del_bytimer(struct atbm_timer_list *timer)
{
	unsigned int hash_index = TIMER_HASH(timer->TimerHander);
	struct atbm_timer_list *hash_pTimer = TIMER_HASH_GET(hash_index);
	
	if(hash_pTimer == ATBM_NULL)
	{
		return;
	}

	if(hash_pTimer == timer)
	{
		TIMER_HASH_SET(hash_index,timer->hnext);
		return;
	}

	while((hash_pTimer->hnext)&&(hash_pTimer->hnext != timer))
	{
		hash_pTimer = hash_pTimer->hnext;
	}

	if(hash_pTimer->hnext)
	{
		hash_pTimer->hnext = timer->hnext;
	}

	timer->hnext = ATBM_NULL;
}

static void __timer_hash_del_byhandle(unsigned int timer_handle)
{
	unsigned int hash_index = TIMER_HASH(timer_handle);
	struct atbm_timer_list *timer = __timer_hash_get(timer_handle);
	if(timer == ATBM_NULL)
	{	
		return;
	}

	__timer_hash_del_bytimer(timer);
}

static void  __timer_out_func(signed long timer_id, unsigned long delay)
{	
	struct atbm_timer_list *timer;

	HASH_LOCK();
	timer = __timer_hash_get(timer_id);
	HASH_UNLOCK();

	if(timer == ATBM_NULL)
	{
		if((ATBM_GET_TIMER_IN_START())&&(ATBM_GET_CUR_ID() == -1))
		{
			ATBM_SET_CUR_ID(timer_id);
		}
		return;
	}

	HASH_LOCK();
	__timer_hash_del_bytimer(timer);
	HASH_UNLOCK();
	
	if(timer->function)
	{
		timer->function(timer->data);
	}
}

static int _cancel_timer(struct atbm_timer_list *timer)
{	
	if (timer->bTimerStart == 0) 
	{
		return -1;
	}

	HASH_LOCK();
	timer->bTimerStart  = 0;
	__timer_hash_del_bytimer(timer);
	HASH_UNLOCK();

	drv_vtimer_stop(timer->TimerHander);
	return 0;
}

void atbm_init_timer(struct atbm_timer_list *timer)
{
	if (!hash_lock_init)
	{
		HASH_LOCK_INIT();
		hash_lock_init = 1;
	}
	timer->function = ATBM_NULL;
    timer->data = 0;
	timer->bTimerStart  = 0;
	timer->hnext = ATBM_NULL;
}

int atbm_mod_timer(struct atbm_timer_list *timer, unsigned long expires)
{
	unsigned int init_ms = 0;
	unsigned long time_after = expires>jiffies?(expires-jiffies):0;
	
	_cancel_timer(timer);

	if(time_after == 0)
	{
		if(timer->function)
		{
			timer->function(timer->data);
		}
		return 0;
	}

	init_ms = ATBM_AK_TIMEOUT_VAL(time_after * 1000 /* us turn */);
	ATBM_SET_TIMER_IN_START(1);
	timer->TimerHander = drv_vtimer_start(init_ms,ATBM_AK_TIMER_NO_LOOP, __timer_out_func);
	if (timer->TimerHander < 0) 
	{
		return -1;
	}

	HASH_LOCK();
	timer->bTimerStart = 1;
	__timer_hash_add(timer);
	HASH_UNLOCK();
	
	if(ATBM_GET_CUR_ID() != -1)
	{
		__timer_out_func(ATBM_GET_CUR_ID(),0);
		ATBM_SET_CUR_ID(-1);
	}	
	ATBM_SET_TIMER_IN_START(0);

	return 0;
}

int atbm_del_timer(struct atbm_timer_list *timer)
{
	_cancel_timer(timer);
	memset(timer, 0, sizeof(struct atbm_timer_list));
	return 0;
}

int atbm_del_timer_sync(struct atbm_timer_list *timer)
{
	return atbm_del_timer(timer);
}

int atbm_timer_pending(struct atbm_timer_list *timer)
{
	return (timer->bTimerStart == 1);
}

void mdelay(unsigned int uiMiliSecond)
{
    drv_os_mdelay(uiMiliSecond);
}

unsigned int atbm_GetOsTimeMs()
{
	unsigned int time = get_tick_count();
	return time;
}

