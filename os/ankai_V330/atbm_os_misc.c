#include "atbm_os.h"
#include "atbm_os_misc.h"

void register_reboot_notifier(void *arg)
{
}

void unregister_reboot_notifier(void *arg)
{
}

void wait_for_completion(void *arg)
{
}

void destroy_rcu_head_on_stack(void *arg)
{
}

void rtnl_lock(void)
{
}

void rtnl_unlock(void)
{
}

void rcu_read_lock(void)
{
}

void rcu_read_unlock(void)
{
}

void print_hex_dump_bytes(const char *prefix_str, int prefix_type, const void *buf, size_t len)
{
	int i = 0;
	printk("%s\n", prefix_str);
	for (i=0; i<len; i++)
	{
		printk("%x\t", ((char *)buf)[i]);
	}
	printk("\n");
}

int atbm_module_attribute_init(void)
{
}

void atbm_module_attribute_exit(void)
{
}

#define CRCPOLY_BE 0x04c11db7

unsigned int crc32_be(unsigned int crc, unsigned char const *p, size_t len)
{
	int i;
	while (len--) {
		crc ^= *p++ << 24;
		for (i = 0; i < 8; i++)
			crc =
			    (crc << 1) ^ ((crc & 0x80000000) ? CRCPOLY_BE :
					  0);
	}
	return crc;
}


