#include "atbm_os.h"
#include "atbm_os_mem.h"

void *krealloc(const void *p, size_t new_size, gfp_t flags)
{
	void *ret;

	if (unlikely(!new_size)) {
		kfree(p);
		return ATBM_NULL;
	}

	ret = malloc(new_size);
	if (ret && p != ret)
		kfree(p);

	return ret;
}

void *kzalloc(size_t size, gfp_t flags)
{
	void *ret;

	ret = malloc(size);
	if (ret)
		memset(ret, 0, size);

	return ret;
}


void *kcalloc(size_t n, size_t size, gfp_t flags)
{
	void *ret;

	ret = malloc(size * n);
	if (ret)
		memset(ret, 0, size);

	return ret;
}

unsigned long copy_from_user(void *to, const void *from, unsigned long n)
{
	memcpy(to, from, n);
	return 0;
}

unsigned long copy_to_user(void *to, const void *from, unsigned long n)
{
	memcpy(to, from, n);
	return 0;
}


