#ifndef ATBM_IOCTL_H
#define ATBM_IOCTL_H

#include "atbm_ioctl_ext.h"

enum{
	KEY_MGMT_NONE,
	KEY_MGMT_WEP,
	KEY_MGMT_WEP_SHARED,
	KEY_MGMT_WPA,
	KEY_MGMT_WPA2,
	KEY_MGMT_MAX
};

void atbm_ioctl_connect_async(struct HostConnectEvent *event, char is_connected);
void atbm_ioctl_driver_async(int insmod);
void atbm_ioctl_wakeup_async(int reason);
void atbm_ioctl_disconn_async(int reason);
void atbm_ioctl_conn_err_async(int reason);

int atbm_wsm_ps(struct atbm_common *hw_priv, unsigned int data);
int atbm_wsm_connect(struct atbm_common *hw_priv, unsigned int data);
int atbm_wsm_get_status(struct atbm_common *hw_priv, unsigned int buff);
int atbm_wsm_tcp_filter(struct atbm_common *hw_priv, unsigned int data, int set_flag);
int atbm_wsm_set_scan(struct atbm_common *hw_priv);
int atbm_wsm_get_scan_info(struct atbm_common *hw_priv, unsigned int data);
int atbm_wsm_set_wifi_mode(struct atbm_common *hw_priv, char is_ap);
int atbm_wsm_set_ap_cfg(struct atbm_common *hw_priv, unsigned int data);
int atbm_wsm_set_wifi_channel(struct atbm_common *hw_priv, char channel);
int atbm_wsm_set_country(struct atbm_common *hw_priv, char country);
int atbm_wsm_get_country(struct atbm_common *hw_priv, unsigned int data);
int atbm_wsm_get_sta_list(struct atbm_common *hw_priv, unsigned int data);
int atbm_wsm_smartcfg_start(struct atbm_common *hw_priv);
int atbm_wsm_smartcfg_stop(struct atbm_common *hw_priv);
int atbm_wsm_ap_touch_start(struct atbm_common *hw_priv, char data);
int atbm_wsm_ap_touch_stop(struct atbm_common *hw_priv);
int atbm_wsm_etf_start_tx(struct atbm_common *hw_priv, unsigned int data);
int atbm_wsm_etf_single_tone(struct atbm_common *hw_priv, unsigned int data);
int atbm_wsm_etf_stop_tx(struct atbm_common *hw_priv);
int atbm_wsm_etf_start_rx(struct atbm_common *hw_priv, unsigned int data);
int atbm_wsm_etf_start_rx_no_drop(struct atbm_common *hw_priv, unsigned int data);
int atbm_wsm_etf_stop_rx(struct atbm_common *hw_priv);
int atbm_wsm_etf_reset_rx(struct atbm_common *hw_priv);
int atbm_wsm_set_adaptive(struct atbm_common *hw_priv, unsigned int data);
int atbm_wsm_get_version(struct atbm_common *hw_priv, unsigned int data);
int atbm_wsm_get_sdk_version(struct atbm_common *hw_priv, unsigned int data);
int atbm_wsm_update_fw(struct atbm_common *hw_priv, unsigned int data);
int atbm_wsm_set_listen_itvl(struct atbm_common *hw_priv, char interval);
int atbm_wsm_send_at_cmd(struct atbm_common *hw_priv, unsigned int data);
int atbm_wsm_set_dbg_print(struct atbm_common *hw_priv, char enable);
int atbm_wsm_fw_sleep(struct atbm_common *hw_priv);
int atbm_wsm_get_rate(struct atbm_common *hw_priv, unsigned int buff);
int atbm_wsm_send_ipc_data(struct atbm_common *hw_priv, unsigned int data);
int atbm_wsm_get_rssi(struct atbm_common *hw_priv, unsigned int buff);
int atbm_wsm_get_time(struct atbm_common *hw_priv, unsigned int buff);
int atbm_wsm_clear_wifi_config(struct atbm_common *hw_priv);
int atbm_wsm_get_etf_rx_info(struct atbm_common *hw_priv, unsigned int buff);

int atbm_ioctl_add(void);
void atbm_ioctl_free(void);

#endif /* ATBM_IOCTL_H */


