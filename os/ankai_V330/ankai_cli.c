#include <stdlib.h>
#include <string.h>
#include "finsh.h"
#include "apollo.h"
#include "atbm_os.h"
#include "atbm_ioctl.h"
#include "wsm.h"
#include "sockets.h"

#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"

enum{
	RATE_INDEX_B_1M,
	RATE_INDEX_B_2M,
	RATE_INDEX_B_5_5M,
	RATE_INDEX_B_11M,
	RATE_INDEX_PBCC_22M,
	RATE_INDEX_PBCC_33M,
	RATE_INDEX_A_6M,
	RATE_INDEX_A_9M,
	RATE_INDEX_A_12M,
	RATE_INDEX_A_18M,
	RATE_INDEX_A_24M,
	RATE_INDEX_A_36M,
	RATE_INDEX_A_48M,
	RATE_INDEX_A_54M,
	RATE_INDEX_N_6_5M,
	RATE_INDEX_N_13M,
	RATE_INDEX_N_19_5M,
	RATE_INDEX_N_26M,
	RATE_INDEX_N_39M,
	RATE_INDEX_N_52M,
	RATE_INDEX_N_58_5M,
	RATE_INDEX_N_65M,
	RATE_INDEX_N_MCS32_6M,
	RATE_INDEX_MAX
};

typedef struct _wifi_cmd_entry
{
    char cmd_name[32];
    void (*cmd_entry)(int argc, char *argv[]);
}wifi_cmd_entry_t;

struct transform
{
	char *type;
	int type_value;
};

static struct transform ps_trans[4]={
	{"NO_SLEEP",    HAL_NO_SLEEP},
	{"MODEM_SLEEP", HAL_MODEM_SLEEP},
	{"LIGHT_SLEEP", HAL_LIGHT_SLEEP},
	{"DEEP_SLEEP",  HAL_DEEP_SLEEP}
};

extern struct atbm_common *g_hw_priv;

int powersave_transform(char *ps_type)
{
	if (NULL != ps_type)
	{
		int i = 0;
		for (i=0; i<4; i++)
		{
			if (!strcmp(ps_trans[i].type, ps_type))
			{
				return ps_trans[i].type_value;
			}
		}
	}

	printf("Line:%d fail\n", __LINE__);
	return -1;
}

int atbm_wifi_ps_cmd(int argc, char *argv[])
{
	u8 ps_level;
	u8 ps_mode;
	struct wsm_powersave_mode_req ps;

	printf("argc:%d argv[0]:%s argv[1]:%s\n", argc, argv[0], argv[1]);
	if (2 == argc)
	{
		printf("set_ps variables:\n"
	       "  ps_level (power save level, refer: 0/1/2\n"
	       "  ps_type (power save mode, refer: NO_SLEEP/MODEM_SLEEP/LIGHT_SLEEP/DEEP_SLEEP)\n");
		return 0;
	}
	if (argc < 6 || argc%2)
	{
		printf("Invalid SET_POWERSAVE command: needs at least 4 arguments (ps_level、ps_type set together), and arguments must be even number\n");
		goto err;
	}
	else
	{
		int arg_cnt = 2;
		int level_set = 0;
		int type_set = 0;
		int ret = 0;

		while (arg_cnt < argc)
		{
			if (!strcmp(argv[arg_cnt], "ps_level"))
			{
				arg_cnt++;
				int level = atoi(argv[arg_cnt]);
				if (level > 2 || level < 0)
				{
					printf("Line:%d invalid ps_level %d\n", __LINE__, level);
					printf("Line:%d no ps_level values configured.\n", __LINE__);
					goto err;
				}
				else
				{
					ps_level = level;
					level_set = 1;
				}
			}
			else if (!strcmp(argv[arg_cnt], "ps_type"))
			{
				arg_cnt++;
				int ps_type = 0;
				ps_type = powersave_transform(argv[arg_cnt]);
				if (-1 == ps_type)
				{
					printf("Line:%d invalid ps_type %s\n", __LINE__, argv[arg_cnt]);
					printf("Line:%d no ps_type values configured.\n", __LINE__);
					goto err;
				}
				else
				{
					ps_mode = ps_type;
					type_set = 1;
				}
			}
			else
			{
				printf("Line:%d invalid command %s\n", __LINE__, argv[arg_cnt]);
				goto err;
			}

			arg_cnt++;
		}

		if (!level_set || !type_set)
		{
			printf("Line:%d not set ps_level or ps_type\n", __LINE__);
			printf("Line:%d no values configured.\n", __LINE__);
			goto err;
		}

		ps.powersave_mode = ps_mode;
		ps.powerave_level = ps_level;
		ret = atbm_wsm_ps(g_hw_priv, (unsigned int)&ps);
		if (ret)
		{
			goto err;
		}
	}

	return 0;
err:
	return -1;
}

char ConvertHexChar(char ch)
{
	if(ch >= '0' && ch <= '9')
		return ch - '0';
	if(ch >= 'a' && ch <= 'f')
		return ch - 'a' + 10;
	if(ch >= 'A' && ch < 'F')
		return ch - 'A' + 10;
	return -1;
}

static int str2mac(char *macstr, char *mac_save)
{
	char i = 0;
	char tmp = 0;

	for (i=0; i<5; i++)
	{
		tmp = ConvertHexChar(*macstr++);
		if (tmp < 0)
		{
			return -1;
		}
		mac_save[i] = tmp*16;
		tmp = ConvertHexChar(*macstr++);
		if (tmp < 0)
		{
			return -1;
		}
		mac_save[i] += tmp;
		macstr++;
	}
	tmp = ConvertHexChar(*macstr++);
	if (tmp < 0)
	{
		return -1;
	}
	mac_save[5] = tmp*16;
	tmp = ConvertHexChar(*macstr);
	if (tmp < 0)
	{
		return -1;
	}
	mac_save[5] += tmp;
	return 0;
}

int atbm_wifi_connect_cmd(int argc, char *argv[])
{
	char *ssid = ATBM_NULL;
	u8 key_mgmt;
	u8 key_id;
	char *key = ATBM_NULL;
	char *bssid_ptr = ATBM_NULL;
	char bssid[6];
	char bssid_set = 0;
	struct wsm_join join;

	if (2 == argc)
	{
		printf("set_network variables:\n"
	       "  ssid (network name, SSID)\n"
	       "  key (passphrase or password)\n"
	       "  key_mgmt (key management protocol, refer: NONE/WEP/WEP_SHARED/WPA/WPA2)\n"
	       "  key_id (key identity)\n"
	       "  bssid (ap mac, refer: b2:c5:66:74:a1:ee)\n");
		return 0;
	}
	if (argc < 4)
	{
		printf("Invalid SET_NETWORK command: needs at least 2 argument\n");
		goto err;
	}
	else
	{
		int arg_cnt = 2;
		while (arg_cnt < argc)
		{
			if (!strcmp(argv[arg_cnt], "ssid"))
			{
				arg_cnt++;
				ssid = argv[arg_cnt];
			}
			else if (!strcmp(argv[arg_cnt], "key_mgmt"))
			{
				arg_cnt++;
				key_mgmt = atoi(argv[arg_cnt]);
			}
			else if(!strcmp(argv[arg_cnt], "key_id"))
			{
				arg_cnt++;
				key_id = atoi(argv[arg_cnt]);
			}else if(!strcmp(argv[arg_cnt], "key"))
			{
				arg_cnt++;
				key = argv[arg_cnt];
			}
			else if (!strcmp(argv[arg_cnt], "bssid"))
			{
				arg_cnt++;
				bssid_ptr = argv[arg_cnt];
				memset(bssid, 0, sizeof(bssid));
				if (strlen(bssid_ptr) != 17)
				{
					goto err;
				}
				if (str2mac(bssid_ptr, bssid) == 0)
				{
					bssid_set = 1;
				}
				else
				{
					goto err;
				}
			}

			arg_cnt++;
		}
	}

	memset(&join, 0, sizeof(join));
	strcpy(join.ssid, ssid);
	join.ssidLength = strlen(ssid);
	strcpy(join.key, key);
	join.keyLength = strlen(key);
	join.keyId = key_id;
	join.keyMgmt = key_mgmt;
	if (bssid_set)
	{
		memcpy(join.bssid, bssid, 6);
	}

	return atbm_wsm_connect(g_hw_priv, (unsigned int)&join);
err:
	return -1;
}

int get_status_send(int argc, char *argv[])
{
	int ret = 0;
	u32 ipaddr;
	u32 ipmask;
	u32 gwaddr;
	struct	wsm_sdio_getconfig_cnf status_info;

	ret = atbm_wsm_get_status(g_hw_priv,(unsigned int)&status_info);
	if (ret)
	{
		goto err;
	}
	if (status_info.WifiMode || status_info.bconnect)
	{
		ipaddr = ntohl(status_info.con_event.ipaddr);
		ipmask = ntohl(status_info.con_event.ipmask);
		gwaddr = ntohl(status_info.con_event.gwaddr);
		printf("ipaddr:%u:%u:%u:%u ipmask:%u:%u:%u:%u gwaddr:%u:%u:%u:%u\n",
			(ipaddr&(0xff<<24)) >> 24,(ipaddr&(0xff<<16)) >> 16,(ipaddr&(0xff<<8)) >> 8,(ipaddr&0xff),
			(ipmask&(0xff<<24)) >> 24,(ipmask&(0xff<<16)) >> 16,(ipmask&(0xff<<8)) >> 8,(ipmask&0xff),
			(gwaddr&(0xff<<24)) >> 24,(gwaddr&(0xff<<16)) >> 16,(gwaddr&(0xff<<8)) >> 8,(gwaddr&0xff));
		printf("connected bssid:"MACSTR" ssid:%s\n", MAC2STR(status_info.con_event.bssid), status_info.con_event.ssid);
	}
	return 0;
err:
	return -1;
}

int list_filters_cmd(int argc, char *argv[])
{
	int ret = 0;
	int i = 0;
	struct wsm_tcp_filter_req req;

	ret = atbm_wsm_tcp_filter(g_hw_priv, (unsigned int)&req, 0);
	if (!ret)
	{
		printf("<TCP FILTER CONFIGURED>\n");
		printf("SRC PORT:");
		for (i=0; i<req.src_cnt; i++)
		{
			printf("%d ", ntohs(req.src_tcpport[i]));
		}
		printf("\n");
		printf("DES PORT:");
		for (i=0; i<req.des_cnt; i++)
		{
			printf("%d ", ntohs(req.des_tcpport[i]));
		}
		printf("\nICMP: %s\n", (req.Flags&HI_SDIO_FILTER_F_ICMP)?"SET":"NOT SET");
	}
	else
	{
		goto err;
	}

	return 0;
err:
	return -1;
}

static struct wsm_tcp_filter_req tcp_filter_set; 

int set_filter_cmd(int argc, char *argv[])
{
	int ret = 0;
	int ports[4];
	int port_cnt;
	int mode;
	int enable;

	if (2 == argc)
	{
		printf("set_filter variables:\n"
	       "  SRC (source tcp port, for client)\n"
	       "  DES (destination tcp port, for server)\n"
	       "  ICMP (configure ICMP filter, 1 or 0, default not set)\n"
	       "  example: set_filter SRC 1201 1316\n");
		return 0;
	}
	else
	{
		if (argc < 4 || argc > 7)
		{
			printf("Invalid SET_FILTER command: needs at least 2 argument, max 5 arguments\n");
			goto err;
		}
		else
		{
			int i = 0;
			int j = 0;
			int tcp_port = 0;

			if (!strcmp(argv[2], "SRC"))
			{
				memset(tcp_filter_set.src_tcpport, 0, sizeof(tcp_filter_set.src_tcpport));
				tcp_filter_set.src_cnt = 0;

				for (i=3; i<argc; i++)
				{
					tcp_port = atoi(argv[i]);
					for (j=0; j<tcp_filter_set.src_cnt; j++)
					{
						if (htons(tcp_port) == tcp_filter_set.src_tcpport[j])
						{
							printf("Line:%d the same tcp_port %d. please reset tcp ports\n", __LINE__, tcp_port);
							memset(tcp_filter_set.src_tcpport, 0, sizeof(tcp_filter_set.src_tcpport));
							tcp_filter_set.src_cnt = 0;
							goto err;
						}
					}
					tcp_filter_set.src_tcpport[tcp_filter_set.src_cnt] = htons(tcp_port);
					tcp_filter_set.src_cnt++;
				}
			}
			else if (!strcmp(argv[2], "DES"))
			{
				memset(tcp_filter_set.des_tcpport, 0, sizeof(tcp_filter_set.des_tcpport));
				tcp_filter_set.des_cnt = 0;

				for (i=3; i<argc; i++)
				{
					tcp_port = atoi(argv[i]);
					for (j=0; j<tcp_filter_set.des_cnt; j++)
					{
						if (htons(tcp_port) == tcp_filter_set.des_tcpport[j])
						{
							printf("Line:%d the same tcp_port %d. please reset tcp ports\n", __LINE__, tcp_port);
							memset(tcp_filter_set.des_tcpport, 0, sizeof(tcp_filter_set.des_tcpport));
							tcp_filter_set.des_cnt = 0;
							goto err;
						}
					}
					tcp_filter_set.des_tcpport[tcp_filter_set.des_cnt] = htons(tcp_port);
					tcp_filter_set.des_cnt++;
				}
			}
			else if (!strcmp(argv[2], "ICMP"))
			{
				tcp_filter_set.Flags = 0;
				tcp_filter_set.Flags = atoi(argv[3])?HI_SDIO_FILTER_F_ICMP:0;
			}
			else
			{
				printf("Line:%d invalid command %s\n", __LINE__, argv[0]);
				goto err;
			}
		}
	}

	return 0;
err:
	return -1;
}

int enable_filter_cmd(int argc, char *argv[])
{
	tcp_filter_set.Flags |= HI_SDIO_FILTER_F_ARP|HI_SDIO_FILTER_F_DNS;
	return atbm_wsm_tcp_filter(g_hw_priv, (unsigned int)&tcp_filter_set, 1);
}

int scan_cmd(int argc, char *argv[])
{
	return atbm_wsm_set_scan(g_hw_priv);
}

int scan_state_cmd(int argc, char *argv[])
{
	int ret = 0;
	char scan_state = 0;

	ret = atbm_wsm_get_scan_state(g_hw_priv, (unsigned int)&scan_state);
	if (ret == -1)
	{
		goto err;
	}
	else if (!ret)
	{
		printf("scan successed.\n");
	}
	else
	{
		printf("scanning.\n");
	}

	return 0;
err:
	return -1;
}

int scan_results_cmd(int argc, char *argv[])
{
	int ret = 0;
	struct wsm_scan_result_req scan_info;
	int start = 1;

	memset(&scan_info, 0, sizeof(scan_info));
	scan_info.remain = 1;

	while (scan_info.remain)
	{
		ret = atbm_wsm_get_scan_info(g_hw_priv, (unsigned int)&scan_info);
		if (!ret)
		{
			int i = 0;
			printf("\nget scan cnt: %d (%s)\n", scan_info.scan_cnt, scan_info.remain?"REMAIN":"OVER");
			if (start)
			{
				printf("bssid\t/\tlevel\t/\tchannel\t/\tssid\n");
				start = 0;
			}
			for (i=0; i<scan_info.scan_cnt; i++)
			{
				printf("\t"MACSTR"\t%d\t%d\t%s\n", MAC2STR(scan_info.scan_info[i].bssid),scan_info.scan_info[i].rssi, 
					scan_info.scan_info[i].channel, scan_info.scan_info[i].ssid);
			}
		}
		else
		{
			break;
		}
	}

	return ret;
}

int set_wifi_mode_cmd(int argc, char *argv[])
{
	int ret = 0;
	char is_ap = 0;

	if (argc < 3)
	{
		printf("Invalid SET_WIFI_MODE command: needs at least 1 argument\n");
		goto err;
	}

	if (!strcmp(argv[2], "AP"))
	{
		is_ap = 1;
	}
	else if (!strcmp(argv[2], "STA"))
	{
		is_ap = 0;
	}
	else
	{
		printf("Line:%d invalid wifi mode %s\n", __LINE__, argv[2]);
		goto err;
	}
	
	ret = atbm_wsm_set_wifi_mode(g_hw_priv, is_ap);
	if (ret)
	{
		goto err;
	}

	return 0;
err:
	return -1;
}

int set_ap_cfg_cmd( int argc, char *argv[])
{
	struct wsm_ap_cfg_req ap_cfg_req;
	char *ssid = ATBM_NULL;
	char *key = ATBM_NULL;
	u8 key_mgmt = 0;
	u8 key_id = 0;

	if (2 == argc)
	{
		printf("set_ap_cfg variables:\n"
		   "  ssid (network name, SSID)\n"
		   "  key (passphrase or password)\n"
		   "  key_mgmt (key management protocol, refer: NONE/WEP/WPA/WPA2)\n"
		   "  key_id (key identity)\n");
		return 0;
	}
	if (argc < 4)
	{
		printf("Invalid SET_AP_CFG command: needs at least 2 argument\n");
		goto err;
	}
	else
	{
		int arg_cnt = 2;
		while (arg_cnt < argc)
		{
			if (!strcmp(argv[arg_cnt], "ssid"))
			{
				arg_cnt++;
				ssid = argv[arg_cnt];
			}
			else if (!strcmp(argv[arg_cnt], "key_mgmt"))
			{
				arg_cnt++;
				key_mgmt = atoi(argv[arg_cnt]);
			}
			else if(!strcmp(argv[arg_cnt], "key_id"))
			{
				arg_cnt++;
				key_id = atoi(argv[arg_cnt]);
			}else if(!strcmp(argv[arg_cnt], "key"))
			{
				arg_cnt++;
				key = argv[arg_cnt];
			}

			arg_cnt++;
		}
	}

	memset(&ap_cfg_req, 0, sizeof(ap_cfg_req));
	strcpy(ap_cfg_req.join_parameter.ssid, ssid);
	ap_cfg_req.join_parameter.ssidLength = strlen(ssid);
	strcpy(ap_cfg_req.join_parameter.key, key);
	ap_cfg_req.join_parameter.keyLength = strlen(key);
	ap_cfg_req.join_parameter.keyMgmt = key_mgmt;
	ap_cfg_req.join_parameter.keyId = key_id;
	atbm_wsm_set_ap_cfg(g_hw_priv, (unsigned int)&ap_cfg_req);
	return 0;
err:
	return -1;
}

int switch_channel_cmd(int argc, char *argv[])
{
	int ret = 0;

	if (argc < 3)
	{
		printf("Invalid SWITCH_CHANNEL command: needs at least 1 argument\n");
		goto err;
	}
	else
	{
		char channel_id = atoi(argv[2]);
		if (channel_id <= 0 || channel_id > 14)
		{
			printf("Line:%d invalid channel_id %s\n", __LINE__, argv[2]);
			printf("Line:%d no channel_id values configured.\n", __LINE__);
			goto err;
		}

		return atbm_wsm_set_wifi_channel(g_hw_priv, channel_id);
	}

	return 0;
err:
	return -1;
}

int set_country_cmd(int argc, char *argv[])
{
	int ret = 0;

	if (argc < 3)
	{
		printf("Invalid SET_COUNTRY command: needs at least 1 argument\n");
		goto err;
	}
	else
	{
		char country_id = atoi(argv[2]);
		ret = atbm_wsm_set_country(g_hw_priv, country_id);
		if (ret)
		{
			goto err;
		}
	}

	return 0;
err:
	return -1;
}

int get_country_cmd(int argc, char *argv[])
{
	int ret = 0;
	char country_id = 0;
	
	ret = atbm_wsm_get_country(g_hw_priv, (unsigned int)&country_id);
	if (ret)
	{
		goto err;
	}

	if (country_id >= COUNTRY_MAX)
	{
		printf("Line:%d get invalid country\n", __LINE__);
		goto err;
	}

	printf("country: %d\n", country_id);

	return 0;
err:
	return -1;
}

int get_stalist_cmd(int argc, char *argv[])
{
	int ret = 0;
	int i = 0;
	struct wsm_sta_list_req stalist_req;

	memset(&stalist_req, 0, sizeof(stalist_req));
	ret = atbm_wsm_get_sta_list(g_hw_priv, (unsigned int)&stalist_req);
	if (ret)
	{
		goto err;
	}

	printf("STA List:\n");
	for (i=0; i<stalist_req.sta_cnt; i++)
	{
		printf(""MACSTR"\n", MAC2STR(stalist_req.info[i].macAddr));
	}
	printf("\n");

	return 0;
err:
	return -1;
}

int smart_cfg_start_cmd(int argc, char *argv[])
{
	return atbm_wsm_smartcfg_start(g_hw_priv);
}

int smart_cfg_stop_cmd(int argc, char *argv[])
{
	return atbm_wsm_smartcfg_stop(g_hw_priv);
}

int ap_touch_start_cmd(int argc, char *argv[])
{
	int ret = 0;
	u8 is_no_notify;

	if (argc < 3)
	{
		printf("Invalid AP_TOUCH_START command: needs at least 1 argument\n");
		goto err;
	}
	else
	{
		if (!strcmp(argv[2], "AP_NOTIFY"))
		{
			is_no_notify = 0;
		}
		else
		{
			is_no_notify = 1;
		}

		ret = atbm_wsm_ap_touch_start(g_hw_priv, is_no_notify);
		if (ret)
		{
			goto err;
		}
	}

	return 0;
err:
	return -1;
}

int ap_touch_stop_cmd(int argc, char *argv[])
{
	return atbm_wsm_ap_touch_stop(g_hw_priv);
}

int etf_start_tx_cmd(int argc, char *argv[])
{
	struct wsm_etf_req etf_req;
	u32 rate;
	u32 len;
	u8 channel;
	u8 is40M;
	u8 greenfield;

	if (argc < 12)
	{
		printf("etf_start_rx variables:\n"
			"  rate(10,20,55,110,60,90,120,180,240,360,480, 540, 65, 130, 195, 260, 390, 520, 585, 650)\n"
		   "  channel(0~14)\n"
		   "  len(>=0)n"
		   "  40m (0-20M 1-40M)\n"
		   "  greenfield(0-1)\n");
		return 0;
	}
	else
	{
		int arg_cnt = 2;
		while (arg_cnt < argc)
		{
			if (!strcmp(argv[arg_cnt], "rate"))
			{
				arg_cnt++;
				rate = atoi(argv[arg_cnt]);
			}
			else if (!strcmp(argv[arg_cnt], "channel"))
			{
				arg_cnt++;
				channel = atoi(argv[arg_cnt]);
			}
			else if (!strcmp(argv[arg_cnt], "len"))
			{
				arg_cnt++;
				len = atoi(argv[arg_cnt]);
			}
			else if(!strcmp(argv[arg_cnt], "40m"))
			{
				arg_cnt++;
				is40M = atoi(argv[arg_cnt]);
			}else if(!strcmp(argv[arg_cnt], "greenfield"))
			{
				arg_cnt++;
				greenfield = atoi(argv[arg_cnt]);
			}

			arg_cnt++;
		}
	}
	printf("Current channel:%d  rate:%d  len:%d  is40M:%d  greenfield:%d\n", channel, rate,
		len, is40M, greenfield);

	memset(&etf_req, 0, sizeof(etf_req));
	etf_req.channel = channel;
	etf_req.is40M = is40M;
	etf_req.rate = rate;
	etf_req.len = len;
	etf_req.greenfield = greenfield;
	return  atbm_wsm_etf_start_tx(g_hw_priv, (unsigned int)&etf_req);
}

int etf_stop_tx_cmd(int argc, char *argv[])
{
	return atbm_wsm_etf_stop_tx(g_hw_priv);
}

int etf_singletone_cmd(int argc, char *argv[])
{
	int ret = 0;
	struct wsm_etf_req etf_req;


	if (argc < 3)
	{
		printf("Invalid etf_singletone_cmd command: needs at least 1 argument\n");
		return -1;
	}

	etf_req.channel = atoi(argv[2]);
	return atbm_wsm_etf_single_tone(g_hw_priv, (unsigned int)&etf_req);
}

int etf_start_rx_cmd(int argc, char *argv[])
{
	struct wsm_etf_req etf_req;
	u8 channel;
	u8 is40M;

	if (argc < 6)
	{
		printf("etf_start_rx variables:\n"
		   "  channel()0~14\n"
		   "  40m (0-20M 1-40M)\n");
		return 0;
	}
	else
	{
		int arg_cnt = 2;
		while (arg_cnt < argc)
		{
			if (!strcmp(argv[arg_cnt], "channel"))
			{
				arg_cnt++;
				channel = atoi(argv[arg_cnt]);
			}
			else if(!strcmp(argv[arg_cnt], "40m"))
			{
				arg_cnt++;
				is40M = atoi(argv[arg_cnt]);
			}
			arg_cnt++;
		}
	}
	printf("Current channel:%d is40M:%d  \n", channel,
		is40M);

	memset(&etf_req, 0, sizeof(etf_req));
	etf_req.channel = channel;
	etf_req.is40M = is40M;
	return	atbm_wsm_etf_start_rx(g_hw_priv, (unsigned int)&etf_req);

}

int etf_stop_rx_cmd(int argc, char *argv[])
{
	return atbm_wsm_etf_stop_rx(g_hw_priv);
}

int etf_reset_rx_cmd(int argc, char *argv[])
{
	return atbm_wsm_etf_reset_rx(g_hw_priv);
}

int adaptive_cmd(int argc, char *argv[])
{
	int ret = 0;
	struct wsm_adaptive_req adaptive_req;
	int adaptive_flag = 0;
	if (argc < 3)
	{
		printf("Invalid ADAPTIVE command: needs at least 1 argument\n");
		goto err;
	}
	else
	{
		if (!strcmp(argv[2], "ON"))
		{
			adaptive_flag = 1;
		}
		else if (!strcmp(argv[2], "OFF"))
		{
			adaptive_flag = 0;
		}
		else
		{
			printf("Line:%d invalid adaptive flag %s\n", __LINE__, argv[2]);
			goto err;
		}

		adaptive_req.adaptive_flag = adaptive_flag;

		ret = atbm_wsm_set_adaptive(g_hw_priv, (unsigned int)&adaptive_req);
		if (ret)
		{
			goto err;
		}
	}

	return 0;
err:
	return -1;
}

int get_ver_cmd(int argc, char *argv[])
{
	int ret = 0;
	struct wsm_version_req req;

	ret = atbm_wsm_get_version(g_hw_priv, (unsigned int)&req);
	if (ret)
	{
		goto err;
	}

	printf("[HW FW Version]\n %s\n", req.hw_ver);
	return 0;
err:
	return -1;
}

int get_sdk_ver_cmd(int argc, char *argv[])
{
	int ret = 0;
	struct wsm_version_req req;

	ret = atbm_wsm_get_sdk_version(g_hw_priv, (unsigned int)&req);
	if (ret)
	{
		goto err;
	}

	printf("[SDK Version]\n %s\n", req.sdk_ver);
	return 0;
err:
	return -1;
}

int listen_itvl_cmd(int argc, char *argv[])
{
	int ret = 0;


	if (argc < 3)
	{
		printf("Invalid LISTEN_ITVL command: needs at least 1 argument\n");
		goto err;
	}
	else
	{
		char val = atoi(argv[2]);
		if (val <= 0 || val > 20)
		{
			printf("Line:%d invalid interval %s\n", __LINE__, argv[2]);
			goto err;
		}

		ret = atbm_wsm_set_listen_itvl(g_hw_priv, val);
		if (ret)
		{
			goto err;
		}
	}

	return 0;
err:
	return -1;
}

int fw_print_cmd(int argc, char *argv[])
{
	int ret = 0;

	if (argc < 3)
	{
		printf("Invalid FW_PRINT command: needs at least 1 argument\n");
		goto err;
	}
	else
	{
		char val = atoi(argv[2]);
		ret = atbm_wsm_set_dbg_print(g_hw_priv, val);
		if (ret)
		{
			goto err;
		}
	}

	return 0;
err:
	return -1;
}

int fw_at_cmd(int argc, char *argv[])
{
	struct wsm_at_cmd_req at_cmd;

	if (argc != 3)
	{
		printf("Line:%d FW_CMD invalid argument number\n", __LINE__);
		return -1;
	}

	strcpy(at_cmd.cmd, argv[2]);

	return atbm_wsm_send_at_cmd(g_hw_priv, (unsigned int)&at_cmd);
}

int fw_sleep_cmd(int argc, char *argv[])
{
	return atbm_wsm_fw_sleep(g_hw_priv);
}

int get_rssi_cmd(int argc, char *argv[])
{
	int ret = 0;
	struct wsm_rssi_req req;

	memset(&req, 0, sizeof(req));
	ret = atbm_wsm_get_rssi(g_hw_priv, (unsigned int)&req);
	if (ret)
	{
		goto err;
	}
	
	printf("rssi: %d\n", req.rssi);
	return 0;
err:
	return -1;
}

int get_tx_rate_cmd(int argc, char *argv[])
{
	int ret = 0;
	struct wsm_get_rate_req req;

	memset(&req, 0, sizeof(req));
	ret = atbm_wsm_get_rate(g_hw_priv, (unsigned int)&req);
	if (ret)
	{
		goto err;
	}

	if (req.rate >= RATE_INDEX_MAX)
	{
		goto err;
	}

	printf("rate_index: %d\n", req.rate);
	return 0;
err:
	return -1;
}

int wifi_disconnect_cmd(int argc, char *argv[])
{
	struct wsm_at_cmd_req at_cmd;
	char *disconn_AT[32];

	strcpy(at_cmd.cmd, "AT+WIFI_DISCONNECT");

	return atbm_wsm_send_at_cmd(g_hw_priv, (unsigned int)&at_cmd);
}

int auto_reconnect_cmd(int argc, char *argv[])
{
	int ret = 0;
	struct wsm_auto_reconnect_req auto_reconn;

	if (argc != 3)
	{
		printf("Line:%d AUTO_RECONNECT invalid argument number\n", __LINE__);
		return -1;
	}

	auto_reconn.auto_enable = atoi(argv[2]);

	return atbm_wsm_auto_reconnect(g_hw_priv, (unsigned int)&auto_reconn);
}

int send_customer_cmd(int argc, char *argv[])
{
	int ret = 0;
	struct wsm_customer_cmd_req customer_cmd;

	if (argc < 3)
	{
		printf("Line:%d CUSTOMER_CMD invalid argument number\n", __LINE__);
		return -1;
	}

	memset(&customer_cmd, 0, sizeof(customer_cmd));
	customer_cmd.cmd_id = atoi(argv[2]);
	strcpy(customer_cmd.cmd_buff, argv[3]);

	return atbm_wsm_send_customer_cmd(g_hw_priv, (unsigned int)&customer_cmd);
}

int rm_wifi_mod_cmd(int argc, char *argv[])
{
	atbm_wifi_hw_deinit();
	return 0;
}

int install_wifi_mod_cmd(int argc, char *argv[])
{
	atbm_wifi_hw_init();
	return 0;
}

static int atbm_help_cmd(int argc, char *argv[]);

static int cmd_entry_check(wifi_cmd_entry_t * cmd_table, int argc, char **args)
{
    int i,ret;

    for(i = 0; cmd_table[i].cmd_entry != NULL; i++)
    {
        if (strcmp(cmd_table[i].cmd_name, args[1]) == 0)
        {
        	cmd_table[i].cmd_entry(argc, args);
        	return 0;
        }
    }

    return -1;
}

static void wifi_test_help()
{
	rt_kprintf("wifi_test_help\n");
}

/* wifi test command entry table, we can add new cmd hear */
static wifi_cmd_entry_t s_test_cmd_table[] = 
{
	{"set_ps",       atbm_wifi_ps_cmd},
	{"conn",       atbm_wifi_connect_cmd},
	{"get_status", get_status_send},
	{"list_filter", list_filters_cmd},
	{"set_filter", set_filter_cmd},
	{"enable_filter", enable_filter_cmd},
	{"scan", scan_cmd},
	{"scan_state", scan_state_cmd},
	{"scan_results", scan_results_cmd},
	{"set_wifi_mode", set_wifi_mode_cmd},
	{"set_ap_cfg", set_ap_cfg_cmd},
	{"switch_channel", switch_channel_cmd},
	{"set_country", set_country_cmd},
	{"get_country", get_country_cmd},
	{"get_stalist", get_stalist_cmd},
	{"smart_cfg_start", smart_cfg_start_cmd},
	{"smart_cfg_stop", smart_cfg_stop_cmd},
	{"ap_touch_start", ap_touch_start_cmd},
	{"ap_touch_stop", ap_touch_stop_cmd},
	{"etf_start_tx", etf_start_tx_cmd},
	{"etf_stop_tx",	etf_stop_tx_cmd},
	{"etf_singletone",	etf_singletone_cmd},
	{"etf_start_rx",	etf_start_rx_cmd},
	{"etf_stop_rx",	etf_stop_rx_cmd},
	{"etf_reset_rx", etf_reset_rx_cmd},
	{"set_adaptive", adaptive_cmd},
	{"get_ver", get_ver_cmd},
	{"get_sdk_ver", get_sdk_ver_cmd},
	{"listen_itvl", listen_itvl_cmd},
	{"fw_print", fw_print_cmd},
	{"fw_at", fw_at_cmd},
	{"rm_wifi_mod", rm_wifi_mod_cmd},
	{"rssi", get_rssi_cmd},
	{"tx_rate", get_tx_rate_cmd},
	{"disconn", wifi_disconnect_cmd},
	{"auto_reconnect", auto_reconnect_cmd},
	{"install_wifi_mod", install_wifi_mod_cmd},
	{"fw_sleep", fw_sleep_cmd},
	{"customer_cmd", send_customer_cmd},
	{"help", atbm_help_cmd},
    {0,0,0,0}
};

static int atbm_help_cmd(int argc, char *argv[])
{
	int cnt = sizeof(s_test_cmd_table)/sizeof(wifi_cmd_entry_t);
	int i = 0;

	for (i=0;i<cnt;i++){
		rt_kprintf("%s\n", s_test_cmd_table[i].cmd_name);
	}
	return 0;
}

extern int wifi_hw_inited;

static void atbm_wifi_test(int argc, char **args)
{
	int i = 0;
	if(!wifi_hw_inited){
		rt_kprintf("wifi not inited!\n");
		return;
	}
    if(cmd_entry_check(s_test_cmd_table, argc, args) != 0)
    {       
		wifi_test_help();
    }
	
}

MSH_CMD_EXPORT(atbm_wifi_test, altobeam wifi txrx test);

