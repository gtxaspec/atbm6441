#ifndef ATBM_OS_TIMER_H
#define ATBM_OS_TIMER_H

#include <stdbool.h>

#define jiffies get_tick_count()

#define msecs_to_jiffies(x) (x)
#define usecs_to_jiffies(x) (x)

#define msleep mdelay

#define time_after(a,b) ((long)(b) - (long)(a) < 0)

typedef void (*TIMER_CALLBACK)(unsigned long data);

struct atbm_timer_list
{
    unsigned int TimerId;
    TIMER_CALLBACK function;
    unsigned long data;
	char *pTimerName;
	unsigned int TimerHander;
	int bTimerStart;
	struct atbm_timer_list *hnext;
};

void atbm_init_timer(struct atbm_timer_list *timer);
int atbm_mod_timer(struct atbm_timer_list *timer, unsigned long expires);
int atbm_del_timer(struct atbm_timer_list *timer);
int atbm_del_timer_sync(struct atbm_timer_list *timer);
int atbm_timer_pending(struct atbm_timer_list *timer);
void mdelay(unsigned int uiMiliSecond);

#endif // #ifndef ATBM_OS_TIMER_H

