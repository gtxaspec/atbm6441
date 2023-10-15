#ifndef ATBM_OS_SDIO_H
#define ATBM_OS_SDIO_H

#define  SDIO_CCCR_IENx		0x04
#define  SDIO_CCCR_IOEx		0x02
#define  SDIO_CCCR_ABORT		0x06	/* function abort/card reset */
#define  SDIO_CCCR_IF		0x07	/* bus interface controls */
#define  SDIO_CCCR_SPEED		0x13

#define  SDIO_SPEED_SHS		0x01	/* Supports High-Speed mode */

#define  SDIO_SPEED_BSS_SHIFT	1
#define  SDIO_SPEED_SDR25	(1<<SDIO_SPEED_BSS_SHIFT)
#define  SDIO_SPEED_EHS		SDIO_SPEED_SDR25	/* Enable High-Speed */
#define  SDIO_BUS_WIDTH_4BIT	0x02
#define  SDIO_FBR_BLKSIZE   0x10

#define  SDIO_CCCR_POWER		0x12

#define  SDIO_POWER_SMPC	0x01	/* Supports Master Power Control */
#define  SDIO_POWER_EMPC	0x02	/* Enable Master Power Control */

#define  SDIO_CCCR_CAPS 0x08

#define  SDIO_CCCR_CAP_SDC	0x01	/* can do CMD52 while data transfer */
#define  SDIO_CCCR_CAP_SMB	0x02	/* can do multi-block xfers (CMD53) */
#define  SDIO_CCCR_CAP_SRW	0x04	/* supports read-wait protocol */
#define  SDIO_CCCR_CAP_SBS	0x08	/* supports suspend/resume */
#define  SDIO_CCCR_CAP_S4MI	0x10	/* interrupt during 4-bit CMD53 */
#define  SDIO_CCCR_CAP_E4MI	0x20	/* enable ints during 4-bit CMD53 */
#define  SDIO_CCCR_CAP_LSC	0x40	/* low speed card */
#define  SDIO_CCCR_CAP_4BLS	0x80	/* 4 bit low speed card */

#define  SDIO_CCCR_CCCR  0x00
#define  SDIO_CCCR_REV_1_00	0	/* CCCR/FBR Version 1.00 */
#define  SDIO_CCCR_REV_1_10	1	/* CCCR/FBR Version 1.10 */
#define  SDIO_CCCR_REV_1_20	2	/* CCCR/FBR Version 1.20 */
	
#define  SDIO_SDIO_REV_1_00	0	/* SDIO Spec Version 1.00 */
#define  SDIO_SDIO_REV_1_10	1	/* SDIO Spec Version 1.10 */
#define  SDIO_SDIO_REV_1_20	2	/* SDIO Spec Version 1.20 */
#define  SDIO_SDIO_REV_2_00	3	/* SDIO Spec Version 2.00 */

#define SDIO_ANY_ID (~0)

#define SDIO_DEVICE(vend,dev) \
	.class = SDIO_ANY_ID, \
	.vendor = (vend), .device = (dev)

#define NOTIFY_DONE 0


#define __init

typedef enum
{
    INTERFACE_NOT_SD,
    INTERFACE_SDMMC4,
    INTERFACE_SDMMC8,
    INTERFACE_SDIO,
    INTERFACE_SDIO2
}T_eCARD_INTERFACE;

struct sdio_device_id {
	unsigned char class;
	unsigned short vendor;
	unsigned short device;
};

struct device {
	int id;
};

struct dev_pm_ops {
	int (*suspend)(struct device *dev);
	int (*resume)(struct device *dev);
};

struct sdio_func
{
	unsigned int num;
	unsigned int en;
	unsigned int blocksize;
	void *priv;
	struct device dev;
};

struct device_driver {
	const struct dev_pm_ops *pm;
};

struct sdio_driver 
{
	char *name;
	const struct sdio_device_id *id_table;
	int (*probe)(struct sdio_func *, const struct sdio_device_id *);
	void (*remove)(struct sdio_func *);
	struct device_driver drv;
};

struct atbm_sdio_cccr
{
	unsigned int sdio_vsn;
	unsigned int sd_vsn;
	unsigned int multi_block:1,
					low_speed:1,
					wide_bus:1,
					high_power:1,
					high_speed:1,
					disable_cd:1;
};

static struct notifier_block{
	int (*notifier_call)(struct notifier_block *nb, unsigned long action, void *unused);
	int priority;
};

int atbm_sdio_register_driver(struct sdio_driver *drv);
void atbm_sdio_unregister_driver(struct sdio_driver *drv);
int sdio_memcpy_fromio(struct sdio_func *func, void *dst, unsigned int addr, int count);
int sdio_memcpy_toio(struct sdio_func *func, unsigned int addr, void *dst, int count);
unsigned char sdio_f0_readb(struct sdio_func *func, unsigned int addr, int *err_ret);
void sdio_f0_writeb(struct sdio_func *func, unsigned char b, unsigned int addr, int *err_ret);
unsigned char sdio_readb(struct sdio_func *func, unsigned int addr, int *err_ret);
void sdio_writeb(struct sdio_func *func, unsigned char b, unsigned int addr, int *err_ret);
void sdio_claim_host(struct sdio_func *func);
void sdio_release_host(struct sdio_func *func);
int atbm_sdio_enable_func(struct sdio_func *func);
int atbm_sdio_disable_func(struct sdio_func *func);
int sdio_claim_irq(struct sdio_func *func, void (*irq_handle)(struct sdio_func *func));
int sdio_release_irq(struct sdio_func *func);
int sdio_set_block_size_(struct sdio_func *func, unsigned blksz);
unsigned int sdio_align_size(struct sdio_func *func, unsigned int sz);
void atbm_sdio_set_drvdata(struct sdio_func *func, void *priv);
void *atbm_sdio_get_drvdata(struct sdio_func *func);

#endif // #ifndef ATBM_OS_SDIO_H

