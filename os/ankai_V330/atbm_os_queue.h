#ifndef ATBM_OS_QUEUE_H
#define ATBM_OS_QUEUE_H

#include "atbm_os_mutex.h"
#include "atbm_os_timer.h"


struct __wait_queue_head 
{
	struct semaphore sem;
	struct list_head task_list;
};

typedef struct __wait_queue_head wait_queue_head_t;

#define wait_event_timeout(q, con, tmo) __wait_event_timeout(&q, &con, tmo)

typedef void (*work_func)(struct work_struct *work);

#define alloc_workqueue(x,y,z) create_singlethread_workqueue(x)

struct work_struct 
{
	struct work_struct *prev;
	struct work_struct *next;
	work_func func;
};

struct delayed_work
{
	struct work_struct work;
	struct atbm_timer_list timer;
};

struct workqueue_struct
{
	struct work_struct work_head;
	void *thread;
	struct mutex mutex_prot;
	struct semaphore sem_sync;
};


#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

#define list_entry(ptr, type, member) \
	container_of(ptr, type, member) 


#define list_first_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)

#define list_last_entry(ptr, type, member) \
	list_entry((ptr)->prev, type, member)

#define list_first_entry_or_null(ptr, type, member) \
	(!list_empty(ptr) ? list_first_entry(ptr, type, member) : NULL)

#define list_next_entry(pos, member) \
	list_entry((pos)->member.next, __typeof__(*(pos)), member)

#define list_prev_entry(pos, member) \
	list_entry((pos)->member.prev, __typeof__(*(pos)), member)

#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_prev(pos, head) \
	for (pos = (head)->prev; pos != (head); pos = pos->prev)

#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

#define list_for_each_prev_safe(pos, n, head) \
	for (pos = (head)->prev, n = pos->prev; \
	     pos != (head); \
	     pos = n, n = pos->prev)

#define list_for_each_entry(pos, head, member) \
	for (pos = list_first_entry(head, __typeof__(*pos), member); \
	     &pos->member != (head);					\
	     pos = list_next_entry(pos, member))

#define list_for_each_entry_reverse(pos, head, member) \
	for (pos = list_last_entry(head, __typeof__(*pos), member); \
	     &pos->member != (head); \
	     pos = list_prev_entry(pos, member))

#define list_prepare_entry(pos, head, member) \
	((pos) ? : list_entry(head, __typeof__(*pos), member))

#define list_for_each_entry_continue(pos, head, member) \
	for (pos = list_next_entry(pos, member); \
	     &pos->member != (head); \
	     pos = list_next_entry(pos, member))

#define list_for_each_entry_continue_reverse(pos, head, member) \
	for (pos = list_prev_entry(pos, member); \
	     &pos->member != (head); \
	     pos = list_prev_entry(pos, member))

#define list_for_each_entry_from(pos, head, member) \
	for (; &pos->member != (head); \
	     pos = list_next_entry(pos, member))

#define list_for_each_entry_safe(pos, n, head, member) \
	for (pos = list_first_entry(head, __typeof__(*pos), member), \
			n = list_next_entry(pos, member); \
	     &pos->member != (head); \
	     pos = n, n = list_next_entry(n, member))

#define list_for_each_entry_safe_continue(pos, n, head, member) \
	for (pos = list_next_entry(pos, member), \
		n = list_next_entry(pos, member); \
		 &pos->member != (head); \
		 pos = n, n = list_next_entry(n, member))

#define list_for_each_entry_safe_from(pos, n, head, member) \
	for (n = list_next_entry(pos, member); \
		 &pos->member != (head); \
		 pos = n, n = list_next_entry(n, member))

#define list_for_each_entry_safe_reverse(pos, n, head, member) \
	for (pos = list_last_entry(head, __typeof__(*pos), member), \
		n = list_prev_entry(pos, member); \
		 &pos->member != (head); \
		 pos = n, n = list_prev_entry(n, member))

#define list_safe_reset_next(pos, n, member)				\
	n = list_next_entry(pos, member)

void init_waitqueue_head(wait_queue_head_t *q);
int __wait_event_timeout(wait_queue_head_t *q, int *con, int timeout); /* ms */
void wake_up(wait_queue_head_t *q);
struct workqueue_struct *create_singlethread_workqueue(const char *name);
void destroy_workqueue(struct workqueue_struct *wq);
void INIT_WORK(struct work_struct *work, work_func work_handle);
int queue_work(struct workqueue_struct *wq, struct work_struct *work);
int queue_delayed_work(struct workqueue_struct *wq, struct delayed_work *work, unsigned long delay);
void flush_workqueue(struct workqueue_struct *wq);
bool flush_work(struct work_struct *work);
int work_pending(struct work_struct *work);
bool cancel_work_sync(struct work_struct *work);
bool cancel_delayed_work_sync(struct delayed_work *dwork);
bool cancel_delayed_work(struct delayed_work *work);


#endif // #ifndef ATBM_OS_QUEUE_H

