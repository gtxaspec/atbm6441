#include "atbm_os_atomic.h"

static unsigned int __atbm_local_irq_save(void)
{
	store_all_int();
	return 0;
}

static void __atbm_local_irq_restore(unsigned int cpu_sr)
{
	cpu_sr = cpu_sr;
	restore_all_int();
}

unsigned int atbm_local_irq_save(void)
{
	return __atbm_local_irq_save();
}

void atbm_local_irq_restore(unsigned int cpu_sr)
{
	cpu_sr = cpu_sr;
	__atbm_local_irq_restore(cpu_sr);
}

void atomic_set(atomic_t *at, int val)
{
	unsigned int cpu_sr = __atbm_local_irq_save();
	at->val = val;
	__atbm_local_irq_restore(cpu_sr);
}

int atomic_read(atomic_t *at)
{
	int val = 0;
	unsigned int cpu_sr = __atbm_local_irq_save();
	val = at->val;
	__atbm_local_irq_restore(cpu_sr);
	return val;
}

int atomic_add_return(int i, atomic_t *at)
{
	unsigned int cpu_sr = __atbm_local_irq_save();
	at->val += i;
	__atbm_local_irq_restore(cpu_sr);
	return  at->val;
}

int atomic_sub_return(int i, atomic_t *at)
{
	unsigned int cpu_sr = __atbm_local_irq_save();
	at->val -= i;
	__atbm_local_irq_restore(cpu_sr);
	return  at->val;
}

int atomic_xchg(atomic_t *at, int val)
{
	int tmp = 0;
	unsigned int cpu_sr = __atbm_local_irq_save();
	tmp = at->val;
	at->val = val;
	__atbm_local_irq_restore(cpu_sr);
	return tmp;
}


void set_bit(unsigned long nr, unsigned long *addr)
{
	unsigned long mask;
	unsigned int cpu_sr = __atbm_local_irq_save();

	addr += nr >> 5;
	mask = 1<<(nr & 0x1f);
	*addr |= mask;

	__atbm_local_irq_restore(cpu_sr);
}
 
void clear_bit(unsigned long nr, unsigned long *addr)
{
	unsigned long mask;
	unsigned int cpu_sr = __atbm_local_irq_save();

	addr += nr >> 5;
	mask = 1<<(nr & 0x1f);
	*addr &= ~mask;

	__atbm_local_irq_restore(cpu_sr);
}

int test_bit(unsigned long nr, unsigned long *addr)
{
	return (1 & (((const int *)addr)[nr >> 5] >> (nr & 31))) != 0;
}

int test_and_set_bit(unsigned long nr, unsigned long *addr)
{
	unsigned long mask;
	int retval;
	unsigned int cpu_sr = __atbm_local_irq_save();

	addr += nr >> 5;
	mask = 1<<(nr & 0x1f);

	retval = *addr & mask;

	*addr |= mask;

	__atbm_local_irq_restore(cpu_sr);

	return retval;
}

int test_and_clear_bit(unsigned long nr, unsigned long *addr)
{
	unsigned long mask;
	int retval;
	unsigned int cpu_sr = __atbm_local_irq_save();

	addr += nr >> 5;
	mask = 1<<(nr & 0x1f);

	retval = *addr & mask;

	*addr &= ~mask;

	__atbm_local_irq_restore(cpu_sr);

	return retval;
}

