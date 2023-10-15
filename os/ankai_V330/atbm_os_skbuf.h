#ifndef ATBM_OS_SKBUF_H
#define ATBM_OS_SKBUF_H

#define netdev_tx_t int

#define NETDEV_TX_BUSY (-1)
#define NETDEV_TX_OK   (0)

#define MAX_ADDR_LEN	32

#define IFF_TX_SKB_SHARING	0x10000

#define free_netdev free

#define init_net 0

#define dev_net_set(x,y)

#define __aligned(x)			__attribute__((aligned(x)))


typedef unsigned char *sk_buff_data_t;

struct sk_buff 
{  
	/** next pbuf in singly linked pbuf chain */
	struct sk_buff *next;  
	struct sk_buff *prev;
	/** pointer to the memory in the buffer,start buf */
	unsigned char *Head;

	/** pointer to the valid data in the buffer,start buf */
	unsigned char *data;

	/** pointer to the current data tail in the buffer */
	unsigned char *Tail;

	unsigned char *End;

	/** data length in the buffer */
	unsigned short len;  

	/**Head buffer  len*/
	unsigned short bufferLen;

	unsigned short data_len;

	/** length of atbm_buff ,not include struct atbm_buf,when is_os_buffer =1 reseved*/
	unsigned short totalLen;  
  
	unsigned char if_id;

	unsigned char Type;
	unsigned char pkt_type;
	/*indicate atbm_buff is not malloc by atbm_wifi,just malloc by tcpip */
	unsigned char ref;
	unsigned char priority;

	unsigned char reseved[3];

	unsigned char cb[48] __aligned(8); // tx /rx  frame descriptors   /rate   tx info

	sk_buff_data_t mac_header;
	sk_buff_data_t network_header;
	sk_buff_data_t transport_header;

	struct net_device *dev;
	unsigned short protocol;
};

struct sk_buff_head 
{
	struct sk_buff *next;
	struct sk_buff *prev;
	int qlen;
	spinlock_t lock;
};

#define ATBM_OS_SKB_LEN(skb)      ((skb)->len)
#define ATBM_OS_SKB_DATA(skb)     (((skb)->data))
#define ATBM_OS_SKB_HEAD(skb)     (((skb)->Head))

struct net_device_stats {
	unsigned long	rx_packets;
	unsigned long	tx_packets;
	unsigned long	rx_bytes;
	unsigned long	tx_bytes;
	unsigned long	rx_errors;
	unsigned long	tx_errors;
	unsigned long	rx_dropped;
	unsigned long	tx_dropped;
	unsigned long	multicast;
	unsigned long	collisions;
	unsigned long	rx_length_errors;
	unsigned long	rx_over_errors;
	unsigned long	rx_crc_errors;
	unsigned long	rx_frame_errors;
	unsigned long	rx_fifo_errors;
	unsigned long	rx_missed_errors;
	unsigned long	tx_aborted_errors;
	unsigned long	tx_carrier_errors;
	unsigned long	tx_fifo_errors;
	unsigned long	tx_heartbeat_errors;
	unsigned long	tx_window_errors;
	unsigned long	rx_compressed;
	unsigned long	tx_compressed;
};

struct net_device {
	struct netif    *nif;
	char			name[IFNAMSIZ];
	struct net_device_stats	stats;
	unsigned int		mtu;
	unsigned int		priv_flags;
	unsigned short		needed_headroom;
	unsigned short		needed_tailroom;
	unsigned char		perm_addr[MAX_ADDR_LEN];
	unsigned char		dev_addr[ETH_ALEN];
	unsigned char       reserved[2];
	void (*destructor)(struct net_device *dev);
	struct net_device_ops *netdev_ops;
	unsigned int        priv[ZEROSIZE];
};

struct net_device_ops {
	int			(*ndo_init)(struct net_device *dev);
	void			(*ndo_uninit)(struct net_device *dev);
	int			(*ndo_open)(struct net_device *dev);
	int			(*ndo_stop)(struct net_device *dev);
	netdev_tx_t		(*ndo_start_xmit) (struct sk_buff *skb,
						   struct net_device *dev);
	//u16			(*ndo_select_queue)(struct net_device *dev,
	//					    struct sk_buff *skb);
	void			(*ndo_change_rx_flags)(struct net_device *dev,
						       int flags);
	void			(*ndo_set_rx_mode)(struct net_device *dev);
	void			(*ndo_set_multicast_list)(struct net_device *dev);
	int			(*ndo_set_mac_address)(struct net_device *dev,
						       void *addr);
	int			(*ndo_validate_addr)(struct net_device *dev);
	//int			(*ndo_do_ioctl)(struct net_device *dev,
	//				        struct ifreq *ifr, int cmd);
	//int			(*ndo_set_config)(struct net_device *dev,
	//				          struct ifmap *map);
	int			(*ndo_change_mtu)(struct net_device *dev,
						  int new_mtu);
	//int			(*ndo_neigh_setup)(struct net_device *dev,
	//					   struct neigh_parms *);
	void			(*ndo_tx_timeout) (struct net_device *dev);

	struct net_device_stats* (*ndo_get_stats)(struct net_device *dev);

	//void			(*ndo_vlan_rx_register)(struct net_device *dev,
	//					        struct vlan_group *grp);
	void			(*ndo_vlan_rx_add_vid)(struct net_device *dev,
						       unsigned short vid);
	void			(*ndo_vlan_rx_kill_vid)(struct net_device *dev,
						        unsigned short vid);
#ifdef CONFIG_NET_POLL_CONTROLLER
	void                    (*ndo_poll_controller)(struct net_device *dev);
#endif
	int			(*ndo_set_vf_mac)(struct net_device *dev,
						  int queue, u8 *mac);
	int			(*ndo_set_vf_vlan)(struct net_device *dev,
						   int queue, u16 vlan, u8 qos);
	int			(*ndo_set_vf_tx_rate)(struct net_device *dev,
						      int vf, int rate);
/*
 * The struct ifla_vf_info was added via b280da8d54b8d82b52f368a8703b7ada6c1744d5
 * on the v2.6.34-rc1~233^2~338 release
 */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,34))
	//int			(*ndo_get_vf_config)(struct net_device *dev,
	//					     int vf,
	//					     struct ifla_vf_info *ivf);
#endif
#if defined(CONFIG_FCOE) || defined(CONFIG_FCOE_MODULE)
	int			(*ndo_fcoe_enable)(struct net_device *dev);
	int			(*ndo_fcoe_disable)(struct net_device *dev);
	//int			(*ndo_fcoe_ddp_setup)(struct net_device *dev,
	//					      u16 xid,
	//					      struct scatterlist *sgl,
	//					      unsigned int sgc);
	int			(*ndo_fcoe_ddp_done)(struct net_device *dev,
						     u16 xid);
#define NETDEV_FCOE_WWNN 0
#define NETDEV_FCOE_WWPN 1
	int			(*ndo_fcoe_get_wwn)(struct net_device *dev,
						    u64 *wwn, int type);
#endif
};

struct sk_buff *dev_alloc_skb(unsigned int length);
unsigned char *skb_put(struct sk_buff *skb, unsigned int len);
unsigned int dev_kfree_skb(struct sk_buff *skb);
void kfree_skb(struct sk_buff *skb);
void skb_set_mac_header(struct sk_buff *skb, const int offset);
void skb_set_network_header(struct sk_buff *skb, const int offset);
unsigned char *skb_network_header(const struct sk_buff *skb);

void skb_set_transport_header(struct sk_buff *skb, const int offset);
unsigned char *skb_transport_header(const struct sk_buff *skb);

unsigned char *skb_push(struct sk_buff *skb, unsigned int len);
unsigned char *skb_pull(struct sk_buff *skb, unsigned int len);
unsigned int skb_headroom(const struct sk_buff *skb);
unsigned int skb_tailroom(const struct sk_buff *skb);
unsigned int skb_reserve(struct sk_buff *skb, unsigned int len);
struct sk_buff *__dev_alloc_skb(unsigned int length, int _gfp_mask);
void skb_set_tail_pointer(struct sk_buff *skb, const unsigned int offset);
void skb_trim(struct sk_buff *skb, unsigned int len);
void __skb_queue_tail(struct sk_buff_head *list, struct sk_buff *newsk);
void skb_queue_tail(struct sk_buff_head *list, struct sk_buff *newsk);
struct sk_buff *skb_peek(struct sk_buff_head *list_);
struct sk_buff *__skb_dequeue(struct sk_buff_head *list);
struct sk_buff *skb_dequeue(struct sk_buff_head *list);
void __skb_queue_purge(struct sk_buff_head *list);
void skb_queue_purge(struct sk_buff_head *list);
struct sk_buff *alloc_skb(unsigned int size, unsigned int priority);
int skb_queue_empty(const struct sk_buff_head *list);
unsigned int skb_queue_len(const struct sk_buff_head *list_);
void __skb_queue_splice(const struct sk_buff_head *list, struct sk_buff *prev, struct sk_buff *next);
void skb_queue_splice_tail_init(struct sk_buff_head *list, struct sk_buff_head *head);
void __skb_queue_after(struct sk_buff_head *list, struct sk_buff *prev, struct sk_buff *newsk);
void __skb_queue_head(struct sk_buff_head *list, struct sk_buff *newsk);
void __skb_queue_head_init(struct sk_buff_head *list);
void skb_queue_head_init(struct sk_buff_head *list);
void skb_queue_splice(const struct sk_buff_head *list, struct sk_buff_head *head);
void skb_queue_splice_init(struct sk_buff_head *list, struct sk_buff_head *head);
unsigned char *skb_end_pointer(const struct sk_buff *skb);
struct sk_buff *skb_copy(const struct sk_buff *skb, unsigned int gfp_mask);
void dev_kfree_skb_any(struct sk_buff *skb);
void dev_kfree_skb_irq(struct sk_buff *skb);																												
struct sk_buff *skb_clone(struct sk_buff *skb, unsigned int gfp_mask);
int skb_cloned(const struct sk_buff *skb);
int skb_shared(const struct sk_buff *skb);
unsigned int skb_headlen(const struct sk_buff *skb);
int skb_is_nonlinear(const struct sk_buff *skb);
int atbm_skb_linearize(const struct sk_buff *skb);
int pskb_expand_head(struct sk_buff *skb, int nhead, int ntail,
		     gfp_t gfp_mask);
unsigned short skb_get_queue_mapping(struct sk_buff *skb);
int skb_padto(struct sk_buff *skb, unsigned int len);
int pskb_may_pull(struct sk_buff *skb, unsigned int len);
int netif_rx(struct sk_buff *skb);


#endif // #ifndef ATBM_OS_SKBUF_H

