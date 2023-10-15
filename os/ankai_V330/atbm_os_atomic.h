#ifndef ATBM_OS_ATOMIC_H
#define ATBM_OS_ATOMIC_H

typedef struct _atomic_t
{
	int val;
} atomic_t;

void atomic_set(atomic_t *at, int val);
int atomic_read(atomic_t *at);
int atomic_add_return(int i, atomic_t *at);
int atomic_sub_return(int i, atomic_t *at);
int atomic_xchg(atomic_t *at, int val);
void set_bit(unsigned long nr, unsigned long *addr);
void clear_bit(unsigned long nr, unsigned long *addr);
int test_bit(unsigned long nr, unsigned long *addr);
int test_and_set_bit(unsigned long nr, unsigned long *addr);
int test_and_clear_bit(unsigned long nr, unsigned long *addr);

#endif // #ifndef ATBM_OS_ATOMIC_H

