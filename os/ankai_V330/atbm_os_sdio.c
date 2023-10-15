#include "atbm_os.h"
#include "atbm_os_sdio.h"
#include <stdint.h>
#include "drv_gpio.h"
#include "drv_module.h"
#include "interrupt.h"
#include "rtdef.h"

//#define AK_SIDO_DEBUG_LEVEL	WIFI_DBG_MSG
//#define AK_SIDO_ERR_LEVEL	WIFI_DBG_ERROR

typedef void (*irq_handle)(struct sdio_func *func);

static struct sdio_func func0;
static struct sdio_func func1;
struct atbm_sdio_cccr cccr;
static struct sdio_func *irq_func = ATBM_NULL;
static struct mutex sdio_lock;
static irq_handle func_irqhanlde = ATBM_NULL;

static int mmc_bus_interface_control(int enable);
static int mmc_sdio_switch_hs(int enable);
static int atbm_sdio_read_cccr(void);

#define ATBM_CALL_IRQHANDLE() 					if(func_irqhanlde&&irq_func) func_irqhanlde(irq_func)
#define ATBM_SET_IRQ(__func,__handle)			irq_func = __func;func_irqhanlde = __handle

#define SDIO_FUNC_SET_EN(__func,__en)			(__func)->en = __en
#define SDIO_FUNC_SET_NUM(__func,__num)			(__func)->num = __num
#define SDIO_FUNC_SET_PRIV(__func,__priv)		(__func)->priv = __priv
#define SDIO_FUNC_SET_BLKSIZE(__func,__blks)	(__func)->blocksize = __blks
#define SDIO_FUNC_GET_EN(__func)				((__func)->en)
#define SDIO_FUNC_GET_NUM(__func)				((__func)->func)
#define SDIO_FUNC_GET_PRIV(__func)				((__func)->priv)
#define SDIO_FUNC_GET_BLKSIZE(__func)			((__func)->blocksize)	

#define SDIO_FUNC0_INT()						SDIO_FUNC_SET_NUM(&func0,0) ;SDIO_FUNC_SET_EN(&func0,1);	\
												SDIO_FUNC_SET_PRIV(&func0,ATBM_NULL);SDIO_FUNC_SET_BLKSIZE(&func0,288)
#define SDIO_FUNC1_INT()						SDIO_FUNC_SET_NUM(&func1,1) ;SDIO_FUNC_SET_EN(&func1,0);	\
												SDIO_FUNC_SET_PRIV(&func1,ATBM_NULL);SDIO_FUNC_SET_BLKSIZE(&func1,288)
#define SDIO_FUNC1()							func1
#define SDIO_FUNC0()							func0
#define SDIO_LOCK_INIT()						mutex_init(&sdio_lock)
#define SDIO_LOCK()								mutex_lock(&sdio_lock)
#define SDIO_UNLOCK()							mutex_unlock(&sdio_lock)
#define __ATBM_SDIO_ALIGNSIZE(___blks,___size)	((___size)+(___blks)-((___size)%(___blks)))
#define ATBM_SDIO_ALIGNSIZE(_blks,_size)		(((_size)%(_blks)==0)?(_size):__ATBM_SDIO_ALIGNSIZE(_blks,_size))
#define ATBM_SDIO_ENABLE_HS(X) mmc_sdio_switch_hs(X)
#define ATBM_SET_BUS() mmc_bus_interface_control()
#define ATBM_READ_CCCR() atbm_sdio_read_cccr()
#define SDIO_FBR_BASE(f)	((f) * 0x100) /* base of function f's FBRs */

#ifdef AK_GPIO
static T_hHisr atbm_sdio_handle;
static void *hisr_stack = ATBM_NULL;
static long isr_gpio = 0;
#define ATBM_HISR_STACK_SIZE	(2048)
#define ATBM_SET_HANLE(__hanle) atbm_sdio_handle = __hanle
#define ATBM_GET_HANEL()		atbm_sdio_handle
#endif

int g_get_sdio_which_mci = 0;

extern int hw_net_cfg_ok;

extern bool sdio_init_info(int idex, int *bus_width, int *frequency);
extern bool sdio_write_byte(unsigned char func, unsigned long addr, unsigned char wdata);
extern bool sdio_read_byte(unsigned char func, unsigned long addr,  unsigned char *rdata);
extern bool sdio_enable_func2(unsigned char func);
extern void sdio_enable_irq(INT_VECTOR v, int enable);
extern bool sdio_read_multi(unsigned char func, unsigned long src, unsigned long count, unsigned char opcode, unsigned char rdata[]);
extern bool sdio_write_multi(unsigned char func, unsigned long dest, unsigned long count, unsigned char opcode, unsigned char wdata[]);
extern void sdio_set_mci_num(int v);

/**                                             
* @brief atbm_get_sdio_which_mci  function
* @author charlie
* @date 2019-12-19
* @param [in] sdio_irq_vector
* @return int
* @retval 0 operation failed
* @retval 1 operation successful 
*/ 
int atbm_get_sdio_which_mci(void)
{
	int i = 0;
	unsigned char sdio_irq_vector = 0;
	int bus_width = 0;
	int frequency = 0;
	T_eCARD_INTERFACE cif;
	
	for(i = 0; i < 3; i++)
	{
		if(sdio_init_info(i, &bus_width, &frequency))
		{
			break;
		}
	}
	
	if(i == 0)
	{
		cif = INTERFACE_SDMMC4;
		sdio_irq_vector =INT_VECTOR_MCI0;
	}
	else if(i == 1)
	{
		cif = INTERFACE_SDIO;
		sdio_irq_vector =INT_VECTOR_MCI1;
	}
#if defined(CONFIG_MACH_AK37D)	
	else if(i == 2)
	{
		cif = INTERFACE_SDIO2;
		sdio_irq_vector =INT_VECTOR_MCI2;
	}
#endif	
	else
	{
		//wifi_printk(WIFI_ERROR, "find sdio index in dtb fail\n");
		return -1;
	}
	//wifi_printk(WIFI_ALWAYS, "fixed sdio interface =%d,sdio_irq_vector=%d\n", cif, sdio_irq_vector);
	g_get_sdio_which_mci = sdio_irq_vector;
	return 0;
}

int mmc_io_rw_direct(unsigned int write, unsigned fn, unsigned addr, unsigned char in, unsigned char *out)
{
	int ret;

	if(write)
	{
		ret = sdio_write_byte(fn, addr, in);
	}
	else
	{
		ret = sdio_read_byte(fn, addr, out);
	}
	if(ret == 0)
	{
		return 0;
	}

	return -1;
}

int mmc_bus_interface_control(int enable)
{
	int ret;
	unsigned char data;

	ret = mmc_io_rw_direct(0, 0, SDIO_CCCR_IF, 0, &data);
	if (ret != 0)
	{
		goto out;
	}

	data |= 0X20;
	ret = mmc_io_rw_direct(1, 0, SDIO_CCCR_IF, data, &data);
	if (ret != 0)
	{
		goto out;
	}
out:
	if (ret != 0)
	{
		//wifi_printk(WIFI_ALWAYS,"sdio_read_cccr err\n");
	}
	return ret;
}

int mmc_sdio_switch_hs(int enable)
{
	int ret;
	unsigned char speed;

	ret = mmc_io_rw_direct(0, 0, SDIO_CCCR_SPEED, 0, &speed);
	if (ret != 0)
	{
		//wifi_printk(WIFI_SDIO, "mmc_sdio_switch_hs err:%d\n", ret);
		return ret;
	}

	if (enable)
	{
		speed |= SDIO_SPEED_EHS;
	}
	else
	{
		speed &= ~SDIO_SPEED_EHS;
	}

	ret = mmc_io_rw_direct(1, 0, SDIO_CCCR_SPEED, speed, NULL);
	if (ret != 0)
	{
		//wifi_printk(WIFI_SDIO, "mmc_sdio_switch_hs err1:%d\n", ret);
	}
	else
	{
		//wifi_printk(WIFI_SDIO, "mmc_sdio_switch_hs ok\n");
	}

	return ret;
}

static int atbm_sdio_read_cccr(void)
{
	int ret;
	int cccr_vsn;
	unsigned char data;

	memset(&cccr, 0, sizeof(struct atbm_sdio_cccr));

	ret = mmc_io_rw_direct(0, 0, SDIO_CCCR_CCCR, 0, &data);
	if (ret != 0)
	{
		goto out;
	}

	cccr_vsn = data & 0x0f;
	//wifi_printk(WIFI_SDIO,"SDIO_CCCR_CCCR:%x\n", cccr_vsn);

	if (cccr_vsn > SDIO_CCCR_REV_1_20)
	{
		//wifi_printk(WIFI_SDIO, "unrecognised CCCR structure version %d\n", cccr_vsn);
		return -1;
	}

	cccr.sdio_vsn = (data & 0xf0) >> 4;

	ret = mmc_io_rw_direct(0, 0, SDIO_CCCR_CAPS, 0, &data);
	if (ret != 0)
	{
		goto out;
	}

	//wifi_printk(WIFI_SDIO, "SDIO_CCCR_CAPS:%x\n", data);
	if (data & SDIO_CCCR_CAP_SMB)
	{
		cccr.multi_block = 1;
	}
	if (data & SDIO_CCCR_CAP_LSC)
	{
		cccr.low_speed = 1;
	}
	if (data & SDIO_CCCR_CAP_4BLS)
	{
		cccr.wide_bus = 1;
	}

	if (cccr_vsn >= SDIO_CCCR_REV_1_10)
	{
		ret = mmc_io_rw_direct(0, 0, SDIO_CCCR_POWER, 0, &data);
		//wifi_printk(WIFI_SDIO, "SDIO_CCCR_POWER:%x\n", data);
		if (ret != 0)
		{
			goto out;
		}
		if (data & SDIO_POWER_SMPC)
		{
			cccr.high_power = 1;
		}
	}

	if (cccr_vsn >= SDIO_CCCR_REV_1_20)
	{
		ret = mmc_io_rw_direct(0, 0, SDIO_CCCR_SPEED, 0, &data);
		//wifi_printk(WIFI_SDIO, "SDIO_CCCR_SPEED:%x\n", data);
		if (ret != 0)
		{
			goto out;
		}
		if (data & SDIO_SPEED_SHS)
		{
			cccr.high_speed = 1;
		}
	}

	ret = mmc_io_rw_direct(0, 0, 7, 0, &data);
	if (ret != 0)
	{
		goto out;
	}

	data |= 0X20;
	ret = mmc_io_rw_direct(1, 0, 7, data, &data);
	if (ret != 0)
	{
		goto out;
	}
	
out:
	if (ret != 0)
	{
		//wifi_printk(WIFI_SDIO, "sdio_read_cccr err\n");
	}
	return ret;
}

int __atbm_sdio_enable_func(int num)
{
	int ret;
	unsigned char reg;

	//wifi_printk(WIFI_SDIO, "enable func for sdio\n");
	ret = mmc_io_rw_direct(0, 0, SDIO_CCCR_IOEx, 0, &reg);
	if (ret != 0)
	{
		goto err;
	}

	reg |= 1 << num;
	ret = mmc_io_rw_direct(1, 0, SDIO_CCCR_IOEx, reg, NULL);
	if (ret  != 0)
	{
		goto err;
	}

	while (1) 
	{
		ret = mmc_io_rw_direct(0, 0, SDIO_CCCR_IOEx, 0, &reg);
		if (ret != 0)
		{
			goto err;
		}
		if (reg & (1 << num))
		{
			break;
		}
	}

	//wifi_printk(WIFI_SDIO, "SDIO: Enabled func %d\n", reg);
	return 0;

err:
	//wifi_printk(WIFI_SDIO, "SDIO: Failed to enable device\n");
	return ret;
}

void atbm_sdio_isr_enable(unsigned char enable);

static void atbm_sdio_gpio_handle(unsigned long *pin, unsigned long polarity)
{
	//	atbm_sdio_isr_enable(0);
	AK_Activate_HISR(ATBM_GET_HANEL());
}

int atbm_sdio_register_driver(struct sdio_driver *drv)
{
	ATBM_SDIO_ENABLE_HS(1);
	ATBM_READ_CCCR();
	SDIO_FUNC0_INT();
	SDIO_FUNC1_INT();
	SDIO_LOCK_INIT();
	return drv->probe(&SDIO_FUNC1(), drv->id_table);
}

void atbm_sdio_unregister_driver(struct sdio_driver *drv)
{
	drv->remove(&SDIO_FUNC1());
}

int sdio_memcpy_fromio(struct sdio_func *func, void *dst, unsigned int addr, int count)
{
	bool ret = true;
	//printf("%s\n", __func__);
	#if 0
	if (hw_net_cfg_ok){
		 struct rt_thread *thread;
		 thread = rt_thread_self();
		 printf("f1 addr:%x  th:%x\n", addr, thread);
	}
	#endif
	if(func->en == 0)
	{
		//wifi_printk(AK_SIDO_DEBUG_LEVEL,"%s:%d func not en \n",__func__,func->func);
		return -1;
	}
	if((((unsigned int)dst)&3)||(count&3))
	{
		//wifi_printk(WIFI_ALWAYS,"Read Data & Len must be 4 align,dst=0x%08x,count=%d",(unsigned int)dst, count);
	}

#if (ATBM_WIFI_PLATFORM == PLATFORM_ANKAI_RTOS)
	atbm_local_irq_save();
#endif
	ret=sdio_read_multi(func->num,addr,count,1,(unsigned char*)dst);
#if (ATBM_WIFI_PLATFORM == PLATFORM_ANKAI_RTOS)
	atbm_local_irq_restore(0);
#endif
	if(ret==false)
	{
		printf(" __atbm_sdio_memcpy_fromio ret =%d\n",ret);
	}
	else{
		//printf("sdio_memcpy_fromio ok\n");
	}
	//printf("f2\n");
	return !ret;
}

int sdio_memcpy_toio(struct sdio_func *func, unsigned int addr, void *dst, int count)
{
	bool ret;
	//printf("%s\n", __func__);
	#if 0
	if (hw_net_cfg_ok){
		struct rt_thread *thread;
		 thread = rt_thread_self();
		printf("t1 addr:%x  th:%x\n", addr, thread);
	}
	#endif
	if(func->en == 0)
	{
		//wifi_printk(AK_SIDO_DEBUG_LEVEL,"%s:%d func not en \n",__func__,func->func);
		return -1;
	}
	if((((unsigned int)dst)&3)||(count&3))
	{
		//wifi_printk(WIFI_ALWAYS,"Write Data & Len must be 4 align,dst=0x%08x,count=%d",(unsigned int )dst,count);
	}

	ret=sdio_write_multi(func->num,addr,count,1,(unsigned char*)dst);
	if(ret==false)
	{
		printf(" __atbm_sdio_memcpy_toio ret =%d\n",ret);
	}
	else{
		//printf("sdio_memcpy_toio ok\n");
	}
	//printf("t2\n");
	return !ret;
}

unsigned char sdio_f0_readb(struct sdio_func *func, unsigned int addr, int *err_ret)
{
	unsigned char data = 0;
	bool ret = true;

	ret = sdio_read_byte(0,addr,&data);
	*err_ret = 0;
	if(ret == false)
	{
		*err_ret = -1;
		printf("%s err\n", __func__);
	}

	return data;
}

void sdio_f0_writeb(struct sdio_func *func, unsigned char b, unsigned int addr, int *err_ret)
{
	bool ret = true;

	ret = sdio_write_byte(0,addr,b);
	*err_ret = 0;
	if(ret == false)
	{
		*err_ret = -1;
		printf("%s err\n", __func__);
	}

	return;
}

u8 sdio_readb(struct sdio_func *func, unsigned int addr, int *err_ret)
{
	unsigned char data = 0;
	bool ret = true;

	ret = sdio_read_byte(func->num, addr, &data);
	*err_ret = 0;
	if(ret == false)
	{
		*err_ret = -1;
		printf("%s err\n", __func__);
	}

	return data;
}

void sdio_writeb(struct sdio_func *func, unsigned char b, unsigned int addr, int *err_ret)
{
	bool ret = true;

	ret = sdio_write_byte(func->num, addr, b);
	*err_ret = 0;
	if(ret == false)
	{
		*err_ret = -1;
		printf("%s err\n", __func__);
	}

	return;
}

void sdio_claim_host(struct sdio_func *func)
{
	func = func;
	SDIO_LOCK();
	if (hw_net_cfg_ok){
		//struct rt_thread *thread;
		//thread = rt_thread_self();
		//printf("claim %x\n", thread);
		//printf("c\n");
	}
}

void sdio_release_host(struct sdio_func *func)
{
	func = func;
	if (hw_net_cfg_ok){
		//struct rt_thread *thread;
		//thread = rt_thread_self();
		//printf("release %x\n", thread);
		//printf("r\n");
	}
	SDIO_UNLOCK();
}

int atbm_sdio_enable_func(struct sdio_func *func)
{
	//wifi_printk(AK_SIDO_DEBUG_LEVEL,"atbm_sdio_enable_func(%d)\n",func->func);
	SDIO_FUNC_SET_EN(func, 1);
	//charlie fixed for rtt sdio
	sdio_enable_func2(func->num);
	return 0;
}

int atbm_sdio_disable_func(struct sdio_func *func)
{
	SDIO_FUNC_SET_EN(func, 0);
	return 0;
}

#ifdef AK_GPIO
void atbm_sdio_isr_enable(unsigned char enable)
{
	gpio_int_control(isr_gpio, enable);
}

void atbm_sdio_hisr_handle(void)
{
	ATBM_CALL_IRQHANDLE();
}
static void atbm_sdio_init_irq_gpio(int gpio_isr)
{
#define ATBM_AK_GPIO_INPUT			(0)
#define ATBM_AK_GPIO_INTR_LOW		(0)
#define ATBM_AK_INT_EN				(1)
	gpio_set_pin_as_gpio(gpio_isr);
	gpio_set_pin_dir(gpio_isr, ATBM_AK_GPIO_INPUT);
	gpio_set_int_p(gpio_isr, ATBM_AK_GPIO_INTR_LOW);
	gpio_int_control(gpio_isr, ATBM_AK_INT_EN);
	isr_gpio = gpio_isr;
}

static int atbm_sdio_setup_gpio_irq(int gpio_isr)
{
	T_hHisr atbm_hanlde = 0;

	//wifi_printk(AK_SIDO_DEBUG_LEVEL, "atbm_sdio_setup_gpio_irq(%d)\n", gpio_isr);
	atbm_sdio_init_irq_gpio(gpio_isr);

	hisr_stack = atbm_kmalloc(ATBM_HISR_STACK_SIZE, GFP_KERNEL);
	if(hisr_stack==NULL)
	{
		//wifi_printk(AK_SIDO_DEBUG_LEVEL, "atbm_sdio_hisr_handle,stack err\n");
		return -1;
	}

	atbm_hanlde = AK_Create_HISR(atbm_sdio_hisr_handle,"atbm", 0, hisr_stack, ATBM_HISR_STACK_SIZE);
	if(AK_IS_INVALIDHANDLE(atbm_hanlde))
	{
		//wifi_printk(AK_SIDO_DEBUG_LEVEL, "atbm_sdio_hisr_handle,atbm_hanlde err\n");
		return -1;
	}

	ATBM_SET_HANLE(atbm_hanlde);

//	gpio_register_int_callback(gpio_isr,0,0,atbm_sdio_gpio_handle);
	gpio_intr_enable(gpio_isr, 1, 1, atbm_sdio_gpio_handle);
	gpio_int_control(gpio_isr, 1);

	return 0;
}

void atbm_sdio_gpioirq_en(struct sdio_func *func, unsigned char en)
{
	ATBM_BUG_ON(func == NULL);
	atbm_sdio_isr_enable(en);
}

int sdio_claim_irq(struct sdio_func *func, void (*irq_handle)(struct sdio_func *func))
{
	bool ret = true;
	unsigned char reg = 0;
	int fd = 0;
	T_WIFI_INFO *wifi = ATBM_NULL; //(T_WIFI_INFO *)wifi_dev.dev_data;
	
	fd = dev_open(DEV_WIFI);
    if(fd < 0)
    {
		//wifi_printk(AK_SIDO_DEBUG_LEVEL,"open wifi faile\r\n");
		return -1;
    }
	dev_read(fd, &wifi, sizeof(unsigned int *));

	ATBM_SET_IRQ(func, irq_handle);
	ret = sdio_read_byte(0, SDIO_CCCR_IENx, &reg);
	if(ret == false)
	{
		//wifi_printk(AK_SIDO_DEBUG_LEVEL, "read ATBM_SDIO_CCCR_IENx err\n");
		goto __fail;
	}

	/* Master interrupt enable ... */
	reg |= BIT(0);

	/* ... for our function */
	reg |= BIT(1);
	
	ret = sdio_write_byte(0, SDIO_CCCR_IENx, reg);
	if(ret == false)
	{
		//wifi_printk(AK_SIDO_DEBUG_LEVEL, "write ATBM_SDIO_CCCR_IENx err\n");
		goto __fail;
	}

	ret = atbm_sdio_setup_gpio_irq(wifi->gpio_int.nb);
	if(ret == 0){
		dev_close(fd);
		return 0;
	}

__fail:	
	dev_close(fd);
	//wifi_printk(AK_SIDO_DEBUG_LEVEL, "atbm_sdio_claim_irq fail \n");
	return -1;
}

int sdio_release_irq(struct sdio_func *func)
{
	ATBM_SET_IRQ(ATBM_NULL, ATBM_NULL);
	AK_Delete_HISR(ATBM_GET_HANEL());
	atbm_kfree(hisr_stack);
	return 0;
}
#else //AK_SDIO

#define SDIO_MESSAGE 1
static bool atbm_sdio_interrupt_int_callback(void)
{
	ATBM_CALL_IRQHANDLE();
	return true;
}

static bool sdio_interrupt_int_callback(unsigned long gpionum, unsigned char level)
{
	sdio_enable_irq(g_get_sdio_which_mci, 0);
	DrvModule_Send_Message(DRV_MODULE_MCI2, SDIO_MESSAGE, NULL);
	return true;
}

static int atbm_sdio_data1_irq(void)
{
    //create drv module sdio task
    if(!DrvModule_Create_Task(DRV_MODULE_MCI2))
    {
       DrvModule_UnProtect(DRV_MODULE_MCI2); 
       return -1;
    }

	//wifi_printk(WIFI_ALWAYS,"atbm_sdio_data1_irq check .....[1]\n");
	DrvModule_Protect(DRV_MODULE_MCI2); 
    //set massage and handler
    DrvModule_Map_Message(DRV_MODULE_MCI2, SDIO_MESSAGE, atbm_sdio_interrupt_int_callback);
	int_register_irq(g_get_sdio_which_mci, sdio_interrupt_int_callback);
	sdio_enable_irq(g_get_sdio_which_mci, 1);
	
	//wifi_printk(WIFI_ALWAYS,"\nWifilib =============0x2010,8000:0x%x\n", *(volatile int *)(0x20108000));
	//wifi_printk(WIFI_ALWAYS,"\nWifilib =============0x2010,8038:0x%x\n", *(volatile int *)(0x20108038));
	//wifi_printk(WIFI_ALWAYS,"\nWifilib =============0x0800,0024:0x%x\n", *(volatile int *)(0x08000024));
	DrvModule_UnProtect(DRV_MODULE_MCI2); 
	return 0;
}

void sdio_irq_handle_callback(struct rt_sdio_function *func)
{
	//printf("sdio_irq_handle_callback\n");
	sdio_enable_irq(g_get_sdio_which_mci, 0);
	atbm_sdio_interrupt_int_callback();
}

int sdio_claim_irq(struct sdio_func *func, void (*irq_handle)(struct sdio_func *func))
{
	bool ret = true;
	unsigned char reg = 0;

	ATBM_SET_IRQ(func,irq_handle);
	ret = mmc_io_rw_direct(0, 0, SDIO_CCCR_IENx, 0, &reg);
	if(ret == false)
	{
		printf("read ATBM_SDIO_CCCR_IENx err\n");
		goto __fail;
	}

	/* Master interrupt enable ... */
	reg |= BIT(0);
	/* ... for our function */
	reg |= BIT(func->num);
	
	//wifi_printk(WIFI_SDIO, "write ATBM_SDIO_CCCR_IENx %x\n", reg);
	ret = mmc_io_rw_direct(1, 0, SDIO_CCCR_IENx, reg, ATBM_NULL);
	if(ret == false)
	{
		printf("write ATBM_SDIO_CCCR_IENx err\n");
		goto __fail;
	}

	ret = mmc_io_rw_direct(0, 0, SDIO_CCCR_IENx, 0, &reg);	
	//wifi_printk(WIFI_SDIO, "atbm_sdio_claim_irq  %x\n",reg);
	ret = atbm_get_sdio_which_mci();
	if(ret != 0)
	{
		printf("sdio get irq vector fail\n");
	}

	/*Init sdio Interrupt,after the sdio Initial*/
#if 0 // charlie add for using rtt sdio intr
	sdio_register_irq_handler(g_get_sdio_which_mci/*,func->num,sdio_irq_handle_callback*/);
#else
	sdio_set_mci_num(g_get_sdio_which_mci);
	ret = atbm_sdio_data1_irq();
#endif
	if(ret != 0)
	{
		printf("sdio register irq err1\n");
	}

	return 0;
__fail:	
	return -1;
}

int sdio_release_irq(struct sdio_func *func)
{
	int ret;
	unsigned char reg;

	ret = mmc_io_rw_direct(0, 0, SDIO_CCCR_IENx, 0, &reg);
	if (ret)
	{
		return ret;
	}

	reg &= ~(1 << func->num);
	/* Disable master interrupt with the last function interrupt */
	if (!(reg & 0xFE))
	{
		reg = 0;
	}

	ret = mmc_io_rw_direct(1, 0, SDIO_CCCR_IENx, reg, ATBM_NULL);
	if (ret)
	{
		return ret;
	}
	
	ATBM_SET_IRQ(ATBM_NULL,ATBM_NULL);
	
	sdio_enable_irq(g_get_sdio_which_mci, 0);
	return 0;
}
#endif

int __atbm_sdio_set_blocksize(struct sdio_func *func, int blksz)
{
	int ret = 0;

	//wifi_printk(WIFI_SDIO, "atbm_sdio_set_blocksize func->func =%d :%d\n", func->func, blksz);

	ret = mmc_io_rw_direct(1, 0, SDIO_FBR_BASE(func->num) + SDIO_FBR_BLKSIZE,
		blksz & 0xff, ATBM_NULL);
	if (ret)
	{
		goto end;
	}

	ret = mmc_io_rw_direct(1, 0, SDIO_FBR_BASE(func->num) + SDIO_FBR_BLKSIZE + 1,
		(blksz >> 8) & 0xff, ATBM_NULL);
	if (ret)
	{
		goto end;
	}

end:
	return ret;
}

int sdio_set_block_size_(struct sdio_func *func, unsigned blksz)
{
	int ret;
	ret=sdio_set_block_len(func->num, blksz);
	SDIO_FUNC_SET_BLKSIZE(func, blksz);
	//wifi_printk(AK_SIDO_DEBUG_LEVEL,"atbm_sdio_set_blocksize(%d)(%d) ret =%d\n",blksz, func->blocksize, ret);
	return 0;
}

unsigned int sdio_align_size(struct sdio_func *func, unsigned int sz)
{
	unsigned int block_size = SDIO_FUNC_GET_BLKSIZE(func);
	unsigned int alignsize = sz;

	if((block_size == 0)||(sz == 0))
	{
		//wifi_printk(AK_SIDO_DEBUG_LEVEL,"atbm_sdio_alignsize err\n");
		return alignsize;
	}

	alignsize = ATBM_SDIO_ALIGNSIZE(block_size, sz);
	return alignsize;
}

void atbm_sdio_set_drvdata(struct sdio_func *func, void *priv)
{
	SDIO_FUNC_SET_PRIV(func, priv);
}

void *atbm_sdio_get_drvdata(struct sdio_func *func)
{
	return SDIO_FUNC_GET_PRIV(func);
}


