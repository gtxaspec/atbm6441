/**************************************************************************************************************
 * altobeam RTOS wifi hmac source code 
 *
 * Copyright (c) 2018, altobeam.inc   All rights reserved.
 *
 *  The source code contains proprietary information of AltoBeam, and shall not be distributed, 
 *  copied, reproduced, or disclosed in whole or in part without prior written permission of AltoBeam.
*****************************************************************************************************************/
#include "apollo.h"
#include "wsm.h"
#include "atbm_ioctl.h"
//#include "atbm_hal.h"
/******** Functions below is Wlan API **********/
//#include "atbm_sysops.h"
#include <stdint.h>
#include "netif/etharp.h"
#include "netif/ethernetif.h"
#include "lwip/tcpip.h"
#include "lwip/netif.h"
#include "lwip/sockets.h"
//#include "capture.h"
#include "lwip/dhcp.h"
//#include "akos_api.h"
//#include "arch_mmc_sd.h"
#include "drv_module.h"
//#include "platform_devices.h"
#include "drv_gpio.h"
//#include "err.h"
//#include "wifi.h"


#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "rtthread.h"
//#include "platform_devices.h"
#include "wlan_mgnt.h"
#include "wlan_dev.h"
#include "wlan_prot.h"


#include "drv_os.h"

#define ATBM_SDIO_BLOCK_SIZE 256

typedef enum _BUS_MODE
{
    USE_ONE_BUS,
    USE_FOUR_BUS,
    USE_EIGHT_BUS
}T_eBUS_MODE;

extern struct atbm_common *g_hw_priv;

extern bool sdio_init_info(int idex, int *bus_width, int *frequency);
#if (PLATFORM==AK_RTOS_300) || (PLATFORM==AK_RTOS_37D) || (PLATFORM==AK_RTOS_330) 
extern err_t etharp_output(struct netif *netif, struct pbuf *q, const ip4_addr_t *ipaddr);
#else
extern err_t etharp_output(struct netif *netif, struct pbuf *q, const ip_addr_t *ipaddr);
#endif

void atbm_akwifi_slect_sdio(int slect_gpio)
{
	/*
	*select sdio mode
	*/
	//wifi_printk(WIFI_ALWAYS, "atbm_akwifi_slect_sdio slect sdio mode,pin(%d)\r\n",slect_gpio);
	gpio_set_pin_as_gpio(slect_gpio);
	gpio_set_pull_up_r(slect_gpio,0);
	gpio_set_pull_down_r(slect_gpio,0);
	gpio_set_pin_dir(slect_gpio,0);
//	gpio_set_pin_level(wifi->gpio_int.nb,0);
	mdelay(200);
}

void atbm_akwifi_reset_sdio(int reset_gpio)
{
	/*
	*reset
	*/
	gpio_set_pin_as_gpio(reset_gpio);
	gpio_set_pull_down_r(reset_gpio,0);
	gpio_set_pin_dir(reset_gpio,1);
	//wifi_printk(WIFI_ALWAYS, "atbm_akwifi_reset_sdio reset  0(%d)\r\n",reset_gpio);
	gpio_set_pin_level(reset_gpio,0);
	mdelay(10);
	gpio_set_pin_level(reset_gpio,1);
	//wifi_printk(WIFI_ALWAYS, "atbm_akwifi_reset_sdio reset  1(%d)\r\n",reset_gpio);
}

#ifdef AK_GPIO
int atbm_akwifi_setup_sdio(void)
{
	int fd;
	int ret= 0;
	uint8_t width = USE_FOUR_BUS; //USE_ONE_BUS;
	T_WIFI_INFO  *wifi =  ATBM_NULL; //(T_WIFI_INFO *)wifi_dev.dev_data;
	fd = dev_open(DEV_WIFI);
    if(fd < 0)
    {
        //wifi_printk(WIFI_DBG_ERROR, "open wifi faile\r\n");
        return -1;
    }
	dev_read(fd,  &wifi, sizeof(unsigned int *));
	
	gpio_share_pin_set( ePIN_AS_SDIO , wifi->sdio_share_pin);
	//atbm_akwifi_slect_sdio(wifi->gpio_int.nb);
	atbm_akwifi_reset_sdio(wifi->gpio_reset.nb);
	if(1 == wifi->bus_mode)
	{
		width = USE_ONE_BUS;
		
		//wifi_printk(WIFI_DBG_ERROR, "atbm_akwifi_init reset  USE_ONE_BUS\r\n");
	}else if(4 == wifi->bus_mode)
	{
		width = USE_FOUR_BUS;
		
		//wifi_printk(WIFI_DBG_ERROR, "atbm_akwifi_init reset  USE_FOUR_BUS\r\n");
	}	
	dev_close(fd);
	
	ret = sdio_initial(3, width , ATBM_SDIO_BLOCK_SIZE);//(3, width , 512)
	if(ret == false){
		//wifi_printk(WIFI_DBG_ERROR, "sdio_initial (%d) err\r\n",ret);
		return -1;
	}
//	sdio_set_clock(wifi->clock, get_asic_freq(), 0); // SD_POWER_SAVE_ENABLE
	
	//wifi_printk(WIFI_ALWAYS, "atbm_akwifi_setup_sdio success\n");
	return 0;
}
#else
extern bool sdio_initial();
int atbm_akwifi_setup_sdio(void)
{
	int ret;
	int i=0,n=0;
	unsigned char bus_mode = 0;
	int bus_width = 0, frequency = 0;
	T_eCARD_INTERFACE cif;
	n = 3;		
	for(i = 0; i < n; i++)
	{
		if(sdio_init_info(i, &bus_width, &frequency))
		{
			break;
		}
	}
	if(i > n){
		//wifi_printk(WIFI_DBG_ERROR,"[%s]-line:%d,i=%d,n=%d\n",__FUNCTION__, __LINE__,i,n);
		return -1;
	}
	if(bus_width == 1) {
		bus_mode = USE_ONE_BUS;
	}
	else if(bus_width == 4) {
		bus_mode = USE_FOUR_BUS;
	}
	else {
		bus_mode = USE_FOUR_BUS;
	}
	
	if(i == 0){
		cif = INTERFACE_SDMMC4;
	}else if(i == 1){
		cif = INTERFACE_SDIO;
	}
	else if(i == 2){
		cif = INTERFACE_SDIO2;
	}
	else{
		//wifi_printk(WIFI_DBG_ERROR,"find sdio index in dtb fail\n");
		return -1;
	}
	printf("fixed sdio bus mode =%d,interface =%d\n",bus_mode,cif);
	ret = sdio_initial();//(3, width , 512)
	if(ret == 0){
		return -1;
	}	
	return 0;
}
#endif

extern int ver_register(lib_info_t *lib);
#define ATBM6441_WIFI_DRV_LIB_VER    "libdrv_wifi_atbm6441 V1.1.10"

#define TX_RX_DEBUG         0

#define SCAN_BUF_SIZE       (4096)
#define AK_FALSE            	0
#define AK_TRUE             	1

static char wifi_init_state = 0; 
static struct rt_wlan_device * s_wlan_dev = NULL;

struct lwip_prot_des
{
    struct rt_wlan_prot prot;
    struct eth_device eth;
    rt_int8_t connected_flag;
    struct rt_timer timer;
//    struct rt_work work;
};

struct eth_device *eth = NULL;

static const char *ver_info[] = {
    ATBM6441_WIFI_DRV_LIB_VER,
	    __DATE__
};


static lib_info_t lib_info = {
    .ver_info = (char **)&ver_info,
    .exp      = NULL,
};

static int __wifi_init_check(void)
{
    if(wifi_init_state != 0) {
		return 1;
    }
    drv_printk(C3, M_DRVSYS, "wifi not init\n");
    return 0;
}

#if TX_RX_DEBUG
static void __print_data(char *data, int len)
{
    int i = 0;
    for(i = 0; i < len; i++){
        drv_printk(C3, M_DRVSYS, "%02x ", data[i]);
    }
    drv_printk(C3, M_DRVSYS, "\n");
}
#endif

char *rtt_atbm6441_wifi_version(void)
{
	return ATBM6441_WIFI_DRV_LIB_VER;
}

int wifi_init_use_time = 0;
static int sta_conn_flag = AK_TRUE;
int wifi_hw_inited = 0;

void atbm_set_wlan_device(struct rt_wlan_device *wlan)
{
    struct lwip_prot_des *lwip_prot = wlan->prot;
    eth = &lwip_prot->eth;
}

int  atbm_wifi_hw_init(void)
{
	if(!wifi_hw_inited)
	{
		printf("[Wifi] Enter %s \n", __func__);
		apollo_sdio_module_init();
		wifi_hw_inited = 1;
	}
	return 0;
}

int  atbm_wifi_hw_deinit(void)
{
	if(wifi_hw_inited)
	{
		printf("[Wifi] Enter %s \n", __func__);
		apollo_sdio_module_exit();
	}
	return 0;
}

rt_err_t atbm6441_init(struct rt_wlan_device *wlan)
{
	int ret = 0;
	int t1,t2;
	unsigned char mac[6] = {0,0,0,0,0,0};

	consolse_use_buffer(0);
	drv_printk(C3, M_DRVSYS, "atbm6441wifi lib version:%s\n", (char *)rtt_atbm6441_wifi_version());
    if(__wifi_init_check()) {
        drv_printk(C2, M_DRVSYS, " wifi has inited \r\n");
        return 0;
    }
	
	t1 = get_tick_count();
	wifi_init_use_time = t1;
	ret = atbm_akwifi_setup_sdio();
	if(ret < 0 ){
		drv_printk(C2, M_DRVSYS, "atbm_akwifi_setup_sdio error\n");
		return -1;
	}

	atbm_set_wlan_device(wlan);
	atbm_wifi_hw_init(); 
	drv_printk(C3, M_DRVSYS, "[%s]-line:%d wifi init done!\n",__FUNCTION__, __LINE__);

	memcpy(mac, g_hw_priv->mac_addr, ETH_ALEN);
	drv_printk(C3, M_DRVSYS, "\n mac addr=%x:%x:%x:%x:%x:%x\n\n"
		, mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);

    wifi_init_state = 1;
	t2 = get_tick_count();
	drv_printk(C3, M_DRVSYS, "fixed wifi init time1 is:%ldms\n", t2-t1);

	return ret;
}

rt_err_t atbm6441_deinit(struct rt_wlan_device *wlan)
{
	int ret = 0;
	consolse_use_buffer(0);
	drv_printk(C3, M_DRVSYS, "atbm6441wifi lib version:%s\n", (char *)rtt_atbm6441_wifi_version());
    if(!__wifi_init_check()) {
        drv_printk(C2, M_DRVSYS, " wifi has not inited \r\n");
        return 0;
    }

	atbm_wifi_hw_deinit(); 
	drv_printk(C3, M_DRVSYS, "[%s]-line:%d wifi deinit done!\n",__FUNCTION__, __LINE__);
    wifi_init_state = 0;
	return ret;
}

rt_err_t atbm6441_set_mode(struct rt_wlan_device *wlan, rt_wlan_mode_t mode)
{
	int ret = 0;

    if(!__wifi_init_check())
            return -1;

    if(mode == RT_WLAN_AP) {
        ret = atbm_wsm_set_wifi_mode(g_hw_priv, 1);
    } else if ( mode == RT_WLAN_STATION){
        ret = atbm_wsm_set_wifi_mode(g_hw_priv, 0);
    }
    drv_printk(C3, M_DRVSYS, "atbm6441_set_mode %d \n", mode);

	if (ret != 0) {
		drv_printk(C2, M_DRVSYS, "set mode fail\n");
	}

	return ret;
}

rt_err_t atbm6441_scan(struct rt_wlan_device *wlan, struct rt_scan_info *scan_info)
{
	#if 0
	int ret = 0;

	drv_printk(C3, M_DRVSYS, "atbm6441_scan\n");
	ret = atbm_wsm_set_scan(g_hw_priv);
	if (ret != 0) {
		drv_printk(C2, M_DRVSYS, "scan fail\n");
	}
    #endif
    return 0;
}

rt_err_t atbm6441_join(struct rt_wlan_device *wlan, struct rt_sta_info *sta_info)
{
	int ret = 0;
	struct wsm_join join_req;

    if(!__wifi_init_check())
        return -1;
    
    drv_printk(C3, M_DRVSYS, "atbm6441_join...\n");
    //TODO: set encryption with sta_info->security
//    WLAN_AUTH_MODE authMode = WLAN_WPA_AUTH_PSK;
//    WLAN_ENCRYPTION encryption = WLAN_ENCRYPT_AES;
	memset(&join_req, 0, sizeof(join_req));
	if (sta_info->ssid.len > 0) {
		memcpy(join_req.ssid, sta_info->ssid.val, sta_info->ssid.len+1);
		join_req.ssidLength = sta_info->ssid.len;
	}
	if (sta_info->key.len > 0) {
		memcpy(join_req.key, sta_info->key.val, sta_info->key.len);
		join_req.keyLength = sta_info->key.len;
		join_req.keyMgmt = KEY_MGMT_WPA2;
	}

	ret = atbm_wsm_connect(g_hw_priv, (unsigned int)&join_req);
	if(ret != 0) {
       drv_printk(C2, M_DRVSYS, "wifi connect fail\n");
    }

	return ret;
}

rt_err_t atbm6441_softap(struct rt_wlan_device *wlan, struct rt_ap_info *ap_info)
{
	drv_printk(C3, M_DRVSYS, " %s not support \n", __FUNCTION__);
    return 0;
}

extern int wifi_disconnect_cmd(int argc, char *argv[]);

rt_err_t atbm6441_disconnect(struct rt_wlan_device *wlan)
{
	return wifi_disconnect_cmd(0, NULL);
}

rt_err_t atbm6441_ap_stop(struct rt_wlan_device *wlan)
{
	drv_printk(C3, M_DRVSYS, " %s not support \n", __FUNCTION__);
	return 0;
}

rt_err_t atbm6441_ap_deauth(struct rt_wlan_device *wlan, rt_uint8_t mac[])
{
    drv_printk(C3, M_DRVSYS, " %s not support \n", __FUNCTION__);
    return -1;
}

rt_err_t atbm6441_scan_stop(struct rt_wlan_device *wlan)
{
    drv_printk(C3, M_DRVSYS, " %s not support \n", __FUNCTION__);
    return -1;
}

rt_err_t atbm6441_get_rssi(struct rt_wlan_device *wlan)
{
	int ret = 0;
	struct wsm_rssi_req rssi_req;

    if(!__wifi_init_check())
        return -1;

    drv_printk(C3, M_DRVSYS, "get wifi rssi signal \n");
	memset(&rssi_req, 0, sizeof(rssi_req));

	ret = atbm_wsm_get_rssi(g_hw_priv, (unsigned int)&rssi_req);
	if (ret != 0) {
		drv_printk(C2, M_DRVSYS, "get rssi fail\n");
	}
	else {
		drv_printk(C3, M_DRVSYS, " wifi rssi: %d \n", rssi_req.rssi);
	}

    return ret;
}

rt_err_t atbm6441_cfg_promisc(struct rt_wlan_device *wlan, rt_bool_t start)
{
    drv_printk(C3, M_DRVSYS, " %s not support \n", __FUNCTION__);
    return -1;
}

rt_err_t atbm6441_cfg_filter(struct rt_wlan_device *wlan, struct rt_wlan_filter *filter)
{
    drv_printk(C3, M_DRVSYS, " %s not support \n", __FUNCTION__);
    return -1;
}

rt_err_t atbm6441_set_channel(struct rt_wlan_device *wlan, int channel)
{
	int ret = 0;

    if(!__wifi_init_check())
        return -1;

    drv_printk(C3, M_DRVSYS, "set channel... \n");
	ret = atbm_wsm_set_wifi_channel(g_hw_priv, channel);
	if (ret != 0) {
		drv_printk(C2, M_DRVSYS, "set channel fail\n");
	}

    return ret;
}

int atbm6441_get_channel(struct rt_wlan_device *wlan)
{
    drv_printk(C3, M_DRVSYS, " %s not support \n", __FUNCTION__);
    return -1;
}

rt_err_t atbm6441_set_country(struct rt_wlan_device *wlan, rt_country_code_t country_code)
{
    drv_printk(C3, M_DRVSYS, " %s not support \n", __FUNCTION__);
    return -1;
}

rt_country_code_t atbm6441_get_country(struct rt_wlan_device *wlan)
{
    drv_printk(C3, M_DRVSYS, " %s not support \n", __FUNCTION__);
    return -1;
}

rt_err_t atbm6441_set_mac(struct rt_wlan_device *wlan, rt_uint8_t *mac)
{
    drv_printk(C3, M_DRVSYS, " %s not support \n", __FUNCTION__);
    return -1;
}

rt_err_t atbm6441_get_mac(struct rt_wlan_device *wlan, rt_uint8_t *mac)
{
	drv_printk(C3, M_DRVSYS, " atbm6441 get mac [%s]-line:%d\n",__FUNCTION__, __LINE__);
	memcpy(mac, g_hw_priv->mac_addr, ETH_ALEN);
	return 0;
}

void atbm6441_wifi_input(void *buf,int len)
{
    if(s_wlan_dev && s_wlan_dev->flags != 0)
    {
        if(s_wlan_dev->prot != NULL) {
            
            //debug
            #if TX_RX_DEBUG
            drv_printk(C3, M_DRVSYS, "rx:");
            char *data = (char *)buf;
            __print_data(&data[0], len);
            #endif
            
            rt_wlan_dev_report_data(s_wlan_dev, buf, len);
        } else {
            drv_printk(C2, M_DRVSYS, "wlan_dev->prot == NULL !");
        }
    }
    else
       drv_printk(C3, M_DRVSYS, "wifi devcie not register !");
}

int atbm6441_recv(struct rt_wlan_device *wlan, void *buff, int len)
{
    drv_printk(C3, M_DRVSYS, " %s not support \n", __FUNCTION__);
    return -1;
}

extern void atbm_wifi_tx_pkt_dir(void *buff, int len);
//extern void atbm_wifi_tx_pkt(struct sk_buff *skb);

int atbm6441_send(struct rt_wlan_device *wlan, void *buff, int len)
{
	//drv_printk(C3, M_DRVSYS, "atbm6441_send: %d\n", len); 
    if(s_wlan_dev && (s_wlan_dev->flags != 0) )
    {
        //debug 
        #if 0//TX_RX_DEBUG
        drv_printk(C3, M_DRVSYS, "tx:");
        char *tmp = (char *)buff;
        __print_data((char *)&tmp[0], len);
        #endif
        atbm_wifi_tx_pkt_dir(buff, len);    
    }
    return 0;
}

rt_err_t atbm6441_set_powersave(struct rt_wlan_device *wlan, int level)
{
	int ret = 0;
	struct wsm_powersave_mode_req ps;
	u8 ps_mode = HAL_LIGHT_SLEEP;

	switch (level)
	{
		case 1:
			ps_mode = HAL_MODEM_SLEEP;
			break;
		case 2:
			ps_mode = HAL_LIGHT_SLEEP;
			break;
		case 3:
			ps_mode = HAL_DEEP_SLEEP;
			break;
		default:
			drv_printk(C2, M_DRVSYS, "invalide powersave level: %d\n", level);
	}

	memset(&ps, 0, sizeof(ps));
	ps.powerave_level = 1;
	ps.powersave_mode = ps_mode;
	ret = atbm_wsm_ps(g_hw_priv, (unsigned int)&ps);
	if (ret)
	{
		drv_printk(C2, M_DRVSYS, "wifi set powersave fail\n");
	}
	return ret;
}

int atbm6441_get_powersave(struct rt_wlan_device *wlan)
{
    drv_printk(C3, M_DRVSYS, " %s not support \n", __FUNCTION__);
    return -1;
}

rt_err_t atbm6441_suspend(struct rt_wlan_device *wlan)
{
	int ret = 0;

	ret= atbm_wsm_fw_sleep(g_hw_priv);
	if (ret)
	{
		drv_printk(C2, M_DRVSYS, "wifi suspend fail\n");
	}
	return ret;
}

rt_err_t atbm6441_set_pm_switch(struct rt_wlan_device *wlan, int pm_on)
{
	int ret = 0;
	struct wsm_powersave_mode_req ps;
	u8 ps_mode = pm_on==1?HAL_LIGHT_SLEEP:HAL_NO_SLEEP;

	memset(&ps, 0, sizeof(ps));
	ps.powerave_level = 1;
	ps.powersave_mode = ps_mode;
	ret = atbm_wsm_ps(g_hw_priv, (unsigned int)&ps);
	if (ret)
	{
		drv_printk(C2, M_DRVSYS, "wifi switch pm fail\n");
	}
	return ret;
}

rt_err_t atbm6441_sleep(struct rt_wlan_device *wlan, rt_wlan_sleep_cfg_t *sleep_cfg)
{
	int ret = 0;

	if (sleep_cfg == NULL)
	{
		drv_printk(C2, M_DRVSYS, " %s invalid param \n", __FUNCTION__);
	}
	if (sleep_cfg->dim > 20)
	{
		drv_printk(C2, M_DRVSYS, " %s invalid param dim\n", __FUNCTION__);
	}

	ret = atbm_wsm_set_listen_itvl(g_hw_priv, sleep_cfg->dim);
    if (ret)
	{
		drv_printk(C2, M_DRVSYS, "wifi sleep dim fail\n");
	}
	return ret;
}

rt_err_t atbm6441_wow_enable(struct rt_wlan_device *wlan)
{
	drv_printk(C3, M_DRVSYS, " %s all support \n", __FUNCTION__);
	return 0;
}

rt_err_t atbm6441_set_wow_event(struct rt_wlan_device *wlan, unsigned int ul_event)
{
	int ret = 0;
	int wow_event = 0;
	drv_printk(C2, M_DRVSYS, "wow event: %x\n", ul_event);
	switch (ul_event)
	{
		case RTOS_WOW_EVENT_DISASSOC:
		case RTOS_WOW_EVENT_TCP_UDP_KEEP_ALIVE:
			drv_printk(C2, M_DRVSYS, "default support wow event !\n");
			return 0;
		case RTOS_WOW_EVENT_NETPATTERN_TCP:
			wow_event = WAKEUP_TCP_NETPATTERN;
			break;
		case RTOS_WOW_EVENT_NETPATTERN_UDP:
			wow_event = WAKEUP_UDP_NETPATTERN;
			break;
		case RTOS_WOW_EVENT_SSID_WAKEUP:
			wow_event = WAKEUP_WK_SSID;
			break;
		case RTOS_WOW_EVENT_MAGIC_PACKET:
			wow_event = WAKEUP_MAGIC_PKT;
			break;
		case RTOS_WOW_EVENT_ALL_CLEAR:
			break;
		default:
			drv_printk(C2, M_DRVSYS, "unsupported wow event: %x\n", ul_event);
			return -1;
	}

	ret = atbm_wsm_set_wakeup_event(g_hw_priv, wow_event);
	if (ret)
	{
		drv_printk(C2, M_DRVSYS, "wifi set wakeup event fail\n");
	}
	return ret;
}

rt_err_t atbm6441_add_netpattern(struct rt_wlan_device *wlan, rtos_wlan_netpattern_info *add_netpattern_info)
{
	int ret = 0;
	struct wsm_netpattern_info netpattern;

	if (add_netpattern_info == NULL)
	{
		drv_printk(C2, M_DRVSYS, " %s invalid param \n", __FUNCTION__);
	}

	netpattern.netpattern_index = add_netpattern_info->netpattern_index;
	netpattern.protocol = add_netpattern_info->protocol;
	netpattern.src_ipaddr = add_netpattern_info->src_ipaddr;
	netpattern.src_port = add_netpattern_info->src_port;
	if (add_netpattern_info->netpattern_len > 63)
	{
		add_netpattern_info->netpattern_len = 63;
	}
	rt_memcpy(netpattern.netpattern_data, add_netpattern_info->netpattern_data, add_netpattern_info->netpattern_len);
	netpattern.netpattern_data[add_netpattern_info->netpattern_len] = '\0';
	netpattern.netpattern_len = add_netpattern_info->netpattern_len;

	ret = atbm_wsm_add_netpattern_info(g_hw_priv, (unsigned int)&netpattern);
	if (ret)
	{
		drv_printk(C2, M_DRVSYS, "wifi add netpattern fail\n");
	}
	return ret;
}

rt_err_t atbm6441_del_netpattern(struct rt_wlan_device *wlan, unsigned int ul_netpattern_index)
{
	int ret = 0;
	struct wsm_netpattern_info netpattern;

	netpattern.netpattern_index = ul_netpattern_index;

	ret = atbm_wsm_del_netpattern_info(g_hw_priv, (unsigned int)&netpattern);
	if (ret)
	{
		drv_printk(C2, M_DRVSYS, "wifi del netpattern fail\n");
	}
	return ret;
}

rt_err_t atbm6441_set_conn_params(struct rt_wlan_device *wlan, rtos_conn_params_stru *conn_params_info)
{
	int ret = 0;
	struct wsm_conn_param_info conn_param;

	if (conn_params_info == NULL)
	{
		drv_printk(C2, M_DRVSYS, " %s invalid param \n", __FUNCTION__);
	}

	conn_param.sess_id = conn_params_info->sess_id;
	conn_param.protocol = conn_params_info->protocol;
	conn_param.dst_ip = conn_params_info->dst_ip;
	conn_param.dst_port = conn_params_info->dst_port;
	conn_param.interval_time = conn_params_info->interval_timer;
	if (conn_params_info->payload_len > 63)
	{
		conn_params_info->payload_len = 63;
	}
	rt_memcpy(conn_param.tcp_payload, conn_params_info->tcp_payload, conn_params_info->payload_len);
	conn_param.tcp_payload[conn_params_info->payload_len]= '\0';

	ret = atbm_wsm_add_conn_param_info(g_hw_priv, (unsigned int)&conn_param);
	if (ret)
	{
		drv_printk(C2, M_DRVSYS, "wifi add conn params fail\n");
	}
	return ret;
}

rt_err_t atbm6441_del_conn_params(struct rt_wlan_device *wlan, unsigned int ul_sess_id)
{
	int ret = 0;
	struct wsm_conn_param_info conn_param;

	conn_param.sess_id = ul_sess_id;

	ret = atbm_wsm_add_conn_param_info(g_hw_priv, (unsigned int)&conn_param);
	if (ret)
	{
		drv_printk(C2, M_DRVSYS, "wifi del conn params fail\n");
	}
	return ret;
}

rt_err_t atbm6441_set_wakeup_ssid(struct rt_wlan_device *wlan, rtos_wlan_wakeup_ssid_info *set_wakeup_info)
{
	int ret = 0;	
	struct wsm_wakeup_ssid_info ssid_info;

	
	strcpy(ssid_info.ssid, set_wakeup_info->ssid);
	ssid_info.ssid_len = strlen(set_wakeup_info->ssid);

	ret = atbm_wsm_set_wk_ssid(g_hw_priv, (unsigned int)&ssid_info);
	if (ret)
	{
		drv_printk(C2, M_DRVSYS, "wifi set wakeup ssid fail\n");
	}
	return ret;
}

extern int sdio_wakeup_reason;

rt_err_t atbm6441_get_wakeup_reason(struct rt_wlan_device *wlan, unsigned int *pul_wakeup_reason)
{
	int ret = 0;
	int reason = RTOS_WOW_EVENT_HOST_WAKEUP;

	switch(sdio_wakeup_reason)
	{
		case WAKEUP_CONNECT:
			reason = RTOS_WOW_EVENT_DISASSOC;
			break;
		case WAKEUP_KEEPALIVE:
			reason = RTOS_WOW_EVENT_TCP_UDP_KEEP_ALIVE;
			break;
		case WAKEUP_TCP_NETPATTERN:
			reason = RTOS_WOW_EVENT_NETPATTERN_TCP;
			break;
		case WAKEUP_UDP_NETPATTERN:
			reason = RTOS_WOW_EVENT_NETPATTERN_UDP;
			break;
		case WAKEUP_WK_SSID:
			reason = RTOS_WOW_EVENT_SSID_WAKEUP;
			break;
		case WAKEUP_MAGIC_PKT:
			reason = RTOS_WOW_EVENT_MAGIC_PACKET;
			break;
		default:
			ret = -1;
			break;
	}

	if (!ret)
	{
		*pul_wakeup_reason = reason;
	}
	return ret;
}

rt_err_t atbm6441_clear_wakeup_ssid(struct rt_wlan_device *wlan, char *ssid)
{
	int ret = 0;
	struct wsm_wakeup_ssid_info ssid_info;

	strcpy(ssid_info.ssid, ssid);
	ssid_info.ssid_len = strlen(ssid);

	ret = atbm_wsm_clear_wk_ssid(g_hw_priv, (unsigned int)&ssid_info);
	if (ret)
	{
		drv_printk(C2, M_DRVSYS, "wifi clear wakeup ssid fail\n");
	}
	return ret;
}

rt_err_t atbm6441_get_lose_connid(struct rt_wlan_device *wlan)
{
	int ret = 0;
	struct wsm_conn_lose_info req;

	ret = atbm_wsm_conn_lose_map(g_hw_priv, (unsigned int)&req);
	if (ret)
	{
		drv_printk(C2, M_DRVSYS, "wifi get keepalive lose conn fail\n");
		return 0xff;
	}
	else {
		return req.lose_map;
	}
}

rt_err_t atbm6441_set_keepalive_switch(struct rt_wlan_device *wlan, rtos_wlan_keepalive_switch_info *keepalive_switch_info)
{
	int ret = 0;
	struct wsm_conn_switch_info conn_switch_info;

	conn_switch_info.sess_id = keepalive_switch_info->keepalive_num;
	conn_switch_info.start_enable = keepalive_switch_info->keepalive_switch;

	ret = atbm_wsm_conn_switch(g_hw_priv, (unsigned int)&conn_switch_info);
	if (ret)
	{
		drv_printk(C2, M_DRVSYS, "wifi keepalive switch fail\n");
	}
	return ret;
}

const struct rt_wlan_dev_ops wlan_ops = {
    .wlan_init = atbm6441_init,
    .wlan_deinit = atbm6441_deinit,
    .wlan_mode = atbm6441_set_mode,
    .wlan_scan = atbm6441_scan,
    .wlan_join = atbm6441_join,
    .wlan_softap = atbm6441_softap,
    .wlan_disconnect = atbm6441_disconnect,
    .wlan_ap_stop = atbm6441_ap_stop,
    .wlan_ap_deauth = atbm6441_ap_deauth,
    .wlan_scan_stop = atbm6441_scan_stop,
    .wlan_get_rssi = atbm6441_get_rssi,
    .wlan_cfg_promisc = atbm6441_cfg_promisc,
    .wlan_cfg_filter = atbm6441_cfg_filter,
    .wlan_set_channel = atbm6441_set_channel,
    .wlan_get_channel = atbm6441_get_channel,
    .wlan_set_country = atbm6441_set_country,
    .wlan_get_country = atbm6441_get_country,
    .wlan_set_mac = atbm6441_set_mac,
    .wlan_get_mac = atbm6441_get_mac,
    .wlan_recv = atbm6441_recv,
    .wlan_send = atbm6441_send,
    .wlan_set_powersave = atbm6441_set_powersave,
    .wlan_get_powersave = atbm6441_get_powersave,
    .wlan_set_sleep = atbm6441_sleep,
    .wlan_wow_enable = atbm6441_wow_enable,
    .wlan_set_wow_event = atbm6441_set_wow_event,
    .wlan_add_netpattern = atbm6441_add_netpattern,
    .wlan_del_netpattern = atbm6441_del_netpattern,
    .wlan_set_wakeup_ssid = atbm6441_set_wakeup_ssid,
    .wlan_get_wakeup_reason = atbm6441_get_wakeup_reason,
    .wlan_clear_wakeup_ssid = atbm6441_clear_wakeup_ssid,
    .wlan_suspend = atbm6441_suspend,
    .wlan_set_pm_switch = atbm6441_set_pm_switch,
    .wlan_set_conn_params = atbm6441_set_conn_params,
    .wlan_del_conn_params = atbm6441_del_conn_params,
    .wlan_get_lose_connid = atbm6441_get_lose_connid,
    .wlan_set_keepalive_switch = atbm6441_set_keepalive_switch,
};

int rt_wifi_device_reg(void)
{

    drv_printk(C3, M_DRVSYS, "F:%s L:%d run \n", __FUNCTION__, __LINE__);
	ver_register(&lib_info);
    if(s_wlan_dev == NULL)
    {
        s_wlan_dev = rt_malloc(sizeof(struct rt_wlan_device));
        if(NULL == s_wlan_dev)
    	{
    		drv_printk(C2, M_DRVSYS, "wifi devcie malloc fail!");
    		return -1;
    	}
    }
		    
    rt_wlan_dev_register(s_wlan_dev, "wifi", &wlan_ops, RT_DEVICE_FLAG_RDWR, NULL);

    return 0;
}

INIT_COMPONENT_EXPORT(rt_wifi_device_reg);

