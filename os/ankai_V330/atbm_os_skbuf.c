#include "apollo.h"
#include "atbm_os_skbuf.h"

#include "netif/etharp.h"
#include "netif/ethernetif.h"
#include "lwip/tcpip.h"
#include "lwip/netif.h"
#include "lwip/sockets.h"

#define ATBM_MEM_ALIGNMENT        (32)
#define ATBM_MEM_ALIGN(addr)      ((void *)(((unsigned int)(addr) + ATBM_MEM_ALIGNMENT - 1) & ~(ATBM_MEM_ALIGNMENT - 1)))
#define ATBM_MEM_ALIGN_SIZE(size) (((size) + ATBM_MEM_ALIGNMENT - 1) & ~(ATBM_MEM_ALIGNMENT - 1))

#define ATBM_HWBUF_EXTERN_HEADROM_LEN 	(64)  
#define ATBM_HWBUF_EXTERN_TAILROM_LEN 	(64)  

int atbm_skb_cnt = 0;
int atbm_skb_free = 0;

int lwip_queue_enable = 0;
int lwip_enable = 0;

extern struct eth_device *eth;
extern struct atbm_common *g_hw_priv;

extern void atbm6441_wifi_input(void *buf,int len);

static struct sk_buff * __atbm_pbuf_alloc(unsigned int length)
{
	struct sk_buff *p;
	unsigned int size;

	atbm_skb_cnt++;

	size = ATBM_MEM_ALIGN_SIZE(sizeof(struct sk_buff)  + ATBM_HWBUF_EXTERN_HEADROM_LEN + ATBM_HWBUF_EXTERN_TAILROM_LEN+ length);
	p = (struct sk_buff *)atbm_kzalloc(size, GFP_KERNEL);
	if (p == ATBM_NULL) 
	{
		//ATBM_WARN("__atbm_pbuf_alloc fail \n");
		return ATBM_NULL;
	}

	/* Set up internal structure of the atbm_buff. */
	ATBM_OS_SKB_DATA(p) = ATBM_MEM_ALIGN((void *)((unsigned char *)p + sizeof(struct sk_buff) + ATBM_HWBUF_EXTERN_HEADROM_LEN));
	ATBM_OS_SKB_HEAD(p) = ATBM_MEM_ALIGN((unsigned char *)(p+1));

	p->ref = 1;
	p->totalLen = ATBM_MEM_ALIGN_SIZE(ATBM_HWBUF_EXTERN_HEADROM_LEN + ATBM_HWBUF_EXTERN_TAILROM_LEN + length);
	p->bufferLen = ATBM_MEM_ALIGN_SIZE(ATBM_HWBUF_EXTERN_HEADROM_LEN + ATBM_HWBUF_EXTERN_TAILROM_LEN + length);

	/*add for wifi*/
	ATBM_OS_SKB_LEN(p) = 0;
	p->data_len = 0;
	p->Tail = ATBM_OS_SKB_DATA(p);
	p->End = p->Tail + size;
  
  return p;
}

static void __atbm_pbuf_free(struct sk_buff *p)
{
	atbm_skb_free++;
	//ATBM_ASSERT(p->ref == 1);
	p->ref--;
	atbm_kfree(p);
}

static void atbm_skb_reinit(struct sk_buff *skb)
{
	ATBM_OS_SKB_DATA(skb) = ATBM_MEM_ALIGN((void *)((unsigned char *)skb + sizeof(struct sk_buff) + ATBM_HWBUF_EXTERN_HEADROM_LEN));
	ATBM_OS_SKB_HEAD(skb) = (unsigned char *)(skb+1);
	skb->bufferLen = skb->totalLen;

	/*add for wifi*/
	ATBM_OS_SKB_LEN(skb) = 0;
	skb->Tail = ATBM_OS_SKB_DATA(skb);
}

struct sk_buff *dev_alloc_skb(unsigned int length)
{
	struct sk_buff *skb;

	skb =  __atbm_pbuf_alloc(length);
	if(skb == ATBM_NULL)
	{
		//ATBM_BUG_ON(1);
		return ATBM_NULL;
	}

	return skb;
}

unsigned char *skb_put(struct sk_buff *skb, unsigned int len)
{
	unsigned char *tmp = skb->Tail;
	skb->Tail += len;
	ATBM_OS_SKB_LEN(skb)  += len;
	return tmp;
}

unsigned int dev_kfree_skb(struct sk_buff *skb)
{
#if defined (WLAN_ZERO_COPY1)
	atbm_dev_free_os_skb(skb);
#endif //#if defined (WLAN_ZERO_COPY1)
	__atbm_pbuf_free(skb);
	return 1;
}

void kfree_skb(struct sk_buff *skb)
{
	dev_kfree_skb(skb);
}

void skb_set_mac_header(struct sk_buff *skb, const int offset)
{
	skb->mac_header = ATBM_OS_SKB_DATA(skb) + offset; 
}

void skb_set_network_header(struct sk_buff *skb, const int offset)
{
	skb->network_header = ATBM_OS_SKB_DATA(skb) + offset;
}

unsigned char *skb_network_header(const struct sk_buff *skb)
{
	return skb->network_header;
}

void skb_set_transport_header(struct sk_buff *skb, const int offset)
{
	skb->transport_header = ATBM_OS_SKB_DATA(skb) + offset;
}

unsigned char *skb_transport_header(const struct sk_buff *skb)
{
	return skb->transport_header;
}

unsigned char *skb_push(struct sk_buff *skb, unsigned int len)
{
 	ATBM_OS_SKB_DATA(skb) -= len;
 	ATBM_OS_SKB_LEN(skb)  += len;
	return ATBM_OS_SKB_DATA(skb);
}

unsigned char *skb_pull(struct sk_buff *skb, unsigned int len)
{
	ATBM_OS_SKB_LEN(skb) -= len;
	return ATBM_OS_SKB_DATA(skb) += len;
}

unsigned int skb_headroom(const struct sk_buff *skb)
{
	return ATBM_OS_SKB_DATA(skb) - (unsigned char *)ATBM_OS_SKB_HEAD(skb);
}

unsigned int skb_tailroom(const struct sk_buff *skb)
{
	return ((unsigned char *)ATBM_OS_SKB_HEAD(skb)+ skb->bufferLen) - (unsigned char *)skb->Tail;
}

unsigned int skb_reserve(struct sk_buff *skb, unsigned int len)
{
	ATBM_OS_SKB_DATA(skb) += len;
	skb->Tail = ATBM_OS_SKB_DATA(skb);
	return 0;
}

struct sk_buff *__dev_alloc_skb(unsigned int length, int _gfp_mask)
{
	return dev_alloc_skb(length);
}		

void skb_set_tail_pointer(struct sk_buff *skb, const unsigned int offset)
{
	skb->Tail = ATBM_OS_SKB_DATA(skb) + offset;
}

void skb_trim(struct sk_buff *skb, unsigned int len)
{
	ATBM_OS_SKB_LEN(skb) = len;
	atbm_skb_set_tail_pointer(skb, len);
}

static void __skb_insert(struct sk_buff *newsk, struct sk_buff *prev, 
	struct sk_buff *next, struct sk_buff_head *list)
{
	newsk->next = next;
	newsk->prev = prev;
	next->prev  = prev->next = newsk;
	list->qlen++;
}

static void __skb_queue_before(struct sk_buff_head *list,
				      struct sk_buff *next, struct sk_buff *newsk)
{
	__skb_insert(newsk, next->prev, next, list);
}

void __skb_queue_tail(struct sk_buff_head *list, struct sk_buff *newsk)
{
	__skb_queue_before(list, (struct sk_buff *)list, newsk);
}

void skb_queue_tail(struct sk_buff_head *list, struct sk_buff *newsk)
{
	
	unsigned long flags;

	spin_lock_irqsave(&list->lock, flags);
	__atbm_skb_queue_tail(list, newsk);
	spin_unlock_irqrestore(&list->lock, flags);
}

static void __skb_unlink(struct sk_buff *skb, struct sk_buff_head *list)
{
	struct sk_buff *next, *prev;

	list->qlen--;
	next	   = skb->next;
	prev	   = skb->prev;
	skb->next  = skb->prev = ATBM_NULL;
	next->prev = prev;
	prev->next = next;
}

struct sk_buff *skb_peek(struct sk_buff_head *list_)
{
	struct sk_buff *list = ((struct sk_buff *)list_)->next;
	if (list == (struct sk_buff *)list_)
	{
		list = ATBM_NULL;
	}
	return list;
}

struct sk_buff *__skb_dequeue(struct sk_buff_head *list)
{
	struct sk_buff *skb = atbm_skb_peek(list);
	if (skb)
	{
		__skb_unlink(skb, list);
	}
	return skb;
}

struct sk_buff *skb_dequeue(struct sk_buff_head *list)
{
	unsigned long flags;
	struct sk_buff *result;

	spin_lock_irqsave(&list->lock, flags);
	result = __skb_dequeue(list);
	spin_unlock_irqrestore(&list->lock, flags);
	return result;
}

void __skb_queue_purge(struct sk_buff_head *list)
{
	struct sk_buff *skb;

	while ((skb = __skb_dequeue(list)) != ATBM_NULL)
	{
		dev_kfree_skb(skb);
	}
}

void skb_queue_purge(struct sk_buff_head *list)
{
	__skb_queue_purge(list);
}

struct sk_buff *alloc_skb(unsigned int size, unsigned int priority)
{
	return dev_alloc_skb(size);
}

int skb_queue_empty(const struct sk_buff_head *list)
{
	return list->next == (struct sk_buff *)list;
}

unsigned int skb_queue_len(const struct sk_buff_head *list_)
{
	return list_->qlen;
}

void __skb_queue_splice(const struct sk_buff_head *list, struct sk_buff *prev, struct sk_buff *next)
{
	struct sk_buff *first = list->next;
	struct sk_buff *last = list->prev;

	first->prev = prev;
	prev->next = first;

	last->next = next;
	next->prev = last;
}

void skb_queue_splice_tail_init(struct sk_buff_head *list, struct sk_buff_head *head)
{
	if (!skb_queue_empty(list)) 
	{
		__skb_queue_splice(list, head->prev, (struct sk_buff *)head);
		head->qlen += list->qlen;
		__skb_queue_head_init(list);
	}
}

void __skb_queue_after(struct sk_buff_head *list, struct sk_buff *prev, struct sk_buff *newsk)
{
	__skb_insert(newsk, prev, prev->next, list);
}

void __skb_queue_head(struct sk_buff_head *list, struct sk_buff *newsk)
{
	__skb_queue_after(list, (struct sk_buff *)list, newsk);
}

void __skb_queue_head_init(struct sk_buff_head *list)
{
	list->prev = list->next = (struct sk_buff *)list;
	list->qlen = 0;
}

void skb_queue_head_init(struct sk_buff_head *list)
{
	spin_lock_init(&list->lock);
	__skb_queue_head_init(list);
}

void skb_queue_splice(const struct sk_buff_head *list, struct sk_buff_head *head)
{
	if (!skb_queue_empty(list)) 
	{
		__skb_queue_splice(list, (struct sk_buff *) head, head->next);
		head->qlen += list->qlen;
	}
}

void skb_queue_splice_init(struct sk_buff_head *list, struct sk_buff_head *head)
{
	if (!skb_queue_empty(list)) 
	{
		__skb_queue_splice(list, (struct sk_buff *) head, head->next);
		head->qlen += list->qlen;
		__skb_queue_head_init(list);
	}
}

unsigned char *skb_end_pointer(const struct sk_buff *skb)
{
	return skb->End;
}

struct sk_buff *skb_copy(const struct sk_buff *skb, unsigned int gfp_mask)
{
	int headerlen = skb_headroom(skb);
	unsigned int size = (skb_end_pointer(skb) - skb->Head) + skb->len;
	struct sk_buff *n = alloc_skb(size, gfp_mask);

	if (!n)
	{
		return NULL;
	}

	/* Set the data pointer */
	skb_reserve(n, headerlen);
	/* Set the tail pointer and length */
	skb_put(n, skb->len);

	memcpy(n->Head, skb->Head, headerlen + skb->len);

	return n;
}

void dev_kfree_skb_any(struct sk_buff *skb)
{
	dev_kfree_skb(skb);
}

void dev_kfree_skb_irq(struct sk_buff *skb)	
{
	dev_kfree_skb(skb);
}
																													
struct sk_buff *skb_clone(struct sk_buff *skb, unsigned int gfp_mask)
{
	return ATBM_NULL;
}

int skb_cloned(const struct sk_buff *skb)
{
	return 0;
}

int skb_shared(const struct sk_buff *skb)
{
	return 0;
}

unsigned int skb_headlen(const struct sk_buff *skb)
{
	return skb->len - skb->data_len;
}

int skb_is_nonlinear(const struct sk_buff *skb)
{
	return skb->data_len;
}

int atbm_skb_linearize(const struct sk_buff *skb)
{
	return 0;
}

int pskb_expand_head(struct sk_buff *skb, int nhead, int ntail,
		     gfp_t gfp_mask)
{
	return -1;
}

unsigned short skb_get_queue_mapping(struct sk_buff *skb)
{
	return 0;
}

int skb_padto(struct sk_buff *skb, unsigned int len)
{
	unsigned int size = skb->len;
	if (likely(size >= len))
		return 0;
	return 0;
}

int pskb_may_pull(struct sk_buff *skb, unsigned int len)
{
	if (likely(len <= skb_headlen(skb)))
		return 1;
	if (unlikely(len > skb->len))
		return 0;
	return 1;
}


#define NETDEV_ALIGN		32


struct net_device *alloc_netdev_mqs(int sizeof_priv, const char *name,
		void (*setup)(struct net_device *),
		unsigned int txqs, unsigned int rxqs)
{
	struct net_device *dev;
	size_t alloc_size;
	struct net_device *p;

	BUG_ON(strlen(name) >= sizeof(dev->name));

	alloc_size = sizeof(struct net_device);
	if (sizeof_priv) {
		/* ensure 32-byte alignment of private area */
		alloc_size = ALIGN2(alloc_size, NETDEV_ALIGN);
		alloc_size += sizeof_priv;
	}
	/* ensure 32-byte alignment of whole construct */
	alloc_size += NETDEV_ALIGN - 1;

	p = kzalloc(alloc_size, GFP_KERNEL);
	if (!p) {
		printk(KERN_ERR "alloc_netdev: Unable to allocate device.\n");
		return NULL;
	}

	dev = p;

	if(eth) {
		dev->nif = eth->netif;
	}

	setup(dev);

	strcpy(dev->name, name);

	return dev;
}

void netif_carrier_on(struct net_device *dev)
{
	struct atbm_vif *priv = netdev_priv(dev);

	lwip_queue_enable = 1;
	lwip_enable = 1;

	atomic_set(&priv->enabled, 1);
	if(priv->type == NL80211_IFTYPE_STATION){
		netif_set_up(dev->nif);
		netif_set_link_up(dev->nif);
	}else{
		netif_set_up(dev->nif);
		netif_set_link_up(dev->nif);
	}
}

void netif_carrier_off(struct net_device *dev)
{
	if(lwip_queue_enable || lwip_enable){
		netif_set_down(dev->nif);
		lwip_queue_enable = 0;
		lwip_enable = 0;
	}
}

void netif_tx_wake_all_queues(struct net_device *dev)
{
	if(!lwip_queue_enable && lwip_enable){
		lwip_queue_enable = 1;
	}
}

void netif_tx_start_all_queues(struct net_device *dev)
{
	if(!lwip_queue_enable && lwip_enable){
		lwip_queue_enable = 1;
	}
}

void netif_tx_stop_all_queues(struct net_device *dev)
{
	if(lwip_queue_enable && lwip_enable){
		lwip_queue_enable = 0;
	}
}

int netif_rx(struct sk_buff *skb)
{
	atbm6441_wifi_input(skb->data, skb->len);
	dev_kfree_skb(skb);
	return 0;
}

void atbm_wifi_tx_pkt(struct sk_buff *skb)
{
	struct atbm_vif *priv = g_hw_priv->vif_list[0];

	if(!lwip_queue_enable){
		printf("lwip queue not enable\n");
		return;
	}
	if (priv == ATBM_NULL){
		return;
	}
	if (priv->ndev && priv->ndev->netdev_ops){
		priv->ndev->netdev_ops->ndo_start_xmit(skb, priv->ndev);
	}
}

void atbm_wifi_tx_pkt_dir(void *buff, int len)
{
	struct atbm_vif *priv = g_hw_priv->vif_list[0];
	struct sk_buff *skb = ATBM_NULL;
	char *tmp = ATBM_NULL;
	struct sta_info *sta = ATBM_NULL;

	if (priv == ATBM_NULL){
		return;
	}

	skb = dev_alloc_skb(len);
    if (!skb) {
        printf("<ERROR> tx_pkt alloc skb \n");
        return;
    }

    tmp = skb_put(skb, len);
    memcpy(tmp, buff, len);

	if (priv->type == NL80211_IFTYPE_STATION){
		if(!priv->connect_success){	
			dev_kfree_skb(skb);
			printf("atbmwifi_tx_start not connect_ok \n");
			return;
		}
	}
	else if(priv->type == NL80211_IFTYPE_AP){
		if(!is_multicast_ether_addr(ATBM_OS_SKB_DATA(skb))){
			sta = sta_info_get(priv, ATBM_OS_SKB_DATA(skb));
			if(!sta){
				dev_kfree_skb(skb);
				printf("atbmwifi_tx_start sta not found \n");
				return;
			}
		}
	}

	if (priv->ndev && priv->ndev->netdev_ops){
	    priv->ndev->netdev_ops->ndo_start_xmit(skb, priv->ndev);
    } else {
        BUG_ON(1);
        dev_kfree_skb(skb);
    }
}

#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"

void atbm_akwifi_set_netaddr(struct net_device *ndev)
{
	struct atbm_vif *priv = netdev_priv(ndev);
	struct atbm_common *hw_priv = priv->hw_priv;

	ndev->nif->hwaddr_len = ETH_ALEN;
	memcpy(ndev->nif->hwaddr, hw_priv->mac_addr, ndev->nif->hwaddr_len);
	printf("atbm_akwifi_set_netaddr:"MACSTR"\n",MAC2STR(ndev->nif->hwaddr));
}

int atbm_config_ak_net_mac(struct net_device *dev)
{
	dev->nif = eth->netif;
	atbm_akwifi_set_netaddr(dev);
}

void atbm_set_ak_ip_info(struct net_device *dev, unsigned int *ipaddr, unsigned int *ipmask, unsigned int *gwaddr)
{
	if(eth){
		netif_clear_flags(eth->netif, NETIF_FLAG_DHCP);
		printf("ip[%u:%u:%u:%u]\n", ((unsigned char *)(ipaddr))[0], ((unsigned char *)(ipaddr))[1], ((unsigned char *)(ipaddr))[2], ((unsigned char *)(ipaddr))[3]);
		printf("mask[%u:%u:%u:%u]\n", ((unsigned char *)(ipmask))[0], ((unsigned char *)(ipmask))[1], ((unsigned char *)(ipmask))[2], ((unsigned char *)(ipmask))[3]);
		printf("gw[%u:%u:%u:%u]\n", ((unsigned char *)(gwaddr))[0], ((unsigned char *)(gwaddr))[1], ((unsigned char *)(gwaddr))[2], ((unsigned char *)(gwaddr))[3]);
		netifapi_netif_set_addr(eth->netif, (ip_addr_t *)ipaddr, (ip_addr_t *)ipmask, (ip_addr_t *)gwaddr);
	}
}


