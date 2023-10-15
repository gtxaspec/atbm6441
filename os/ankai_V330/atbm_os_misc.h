#ifndef ATBM_OS_MISC_H
#define ATBM_OS_MISC_H

#define DUMP_PREFIX_NONE 0

void register_reboot_notifier(void *arg);
void unregister_reboot_notifier(void *arg);
void wait_for_completion(void *arg);
void destroy_rcu_head_on_stack(void *arg);
void rtnl_lock(void);
void rtnl_unlock(void);
void rcu_read_lock(void);
void rcu_read_unlock(void);
void print_hex_dump_bytes(const char *prefix_str, int prefix_type, const void *buf, size_t len);
int atbm_module_attribute_init(void);
void atbm_module_attribute_exit(void);
unsigned int crc32_be(unsigned int crc, unsigned char const *p, size_t len);

#endif // #ifndef ATBM_OS_MISC_H

