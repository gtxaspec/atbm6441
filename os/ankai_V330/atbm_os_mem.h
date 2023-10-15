#ifndef ATBM_OS_MEM_H
#define ATBM_OS_MEM_H

#define GFP_ATOMIC	1
#define GFP_KERNEL	2
#define GFP_DMA		4


#define kmalloc(x,y) malloc(x)
#define vmalloc malloc
#define kfree free
#define vfree free

void *krealloc(const void *p, size_t new_size, gfp_t flags);
void *kzalloc(size_t size, gfp_t flags);
void *kcalloc(size_t n, size_t size, gfp_t flags);

unsigned long copy_from_user(void *to, const void *from, unsigned long n);
unsigned long copy_to_user(void *to, const void *from, unsigned long n);

#endif // #ifndef ATBM_OS_MEM_H

