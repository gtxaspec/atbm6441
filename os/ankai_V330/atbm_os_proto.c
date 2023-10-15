#include "apollo.h"
#include "atbm_os_proto.h"
#include "txrx.h"

int ieee80211_has_tods(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_TODS)) != 0;
}

int ieee80211_has_fromds(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FROMDS)) != 0;
}

int ieee80211_has_a4(__le16 fc)
{
	__le16 tmp = cpu_to_le16(IEEE80211_FCTL_TODS | IEEE80211_FCTL_FROMDS);
	return (fc & tmp) == tmp;
}

int ieee80211_has_morefrags(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_MOREFRAGS)) != 0;
}

int ieee80211_has_retry(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_RETRY)) != 0;
}

int ieee80211_has_pm(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_PM)) != 0;
}

int ieee80211_has_moredata(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_MOREDATA)) != 0;
}

int ieee80211_has_protected(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_PROTECTED)) != 0;
}

int ieee80211_has_order(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_ORDER)) != 0;
}

int ieee80211_is_mgmt(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_MGMT);
}

int ieee80211_is_ctl(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_CTL);
}

int ieee80211_is_data(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_DATA);
}

int ieee80211_is_data_qos(__le16 fc)
{
	/*
	 * mask with QOS_DATA rather than IEEE80211_FCTL_STYPE as we just need
	 * to check the one bit
	 */
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_STYPE_QOS_DATA)) ==
	       cpu_to_le16(IEEE80211_FTYPE_DATA | IEEE80211_STYPE_QOS_DATA);
}

int ieee80211_is_data_present(__le16 fc)
{
	/*
	 * mask with 0x40 and test that that bit is clear to only return true
	 * for the data-containing substypes.
	 */
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | 0x40)) ==
	       cpu_to_le16(IEEE80211_FTYPE_DATA);
}

int ieee80211_is_assoc_req(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_ASSOC_REQ);
}

int ieee80211_is_assoc_resp(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_ASSOC_RESP);
}

int ieee80211_is_reassoc_req(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_REASSOC_REQ);
}

int ieee80211_is_reassoc_resp(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_REASSOC_RESP);
}

int ieee80211_is_probe_req(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_PROBE_REQ);
}

int ieee80211_is_probe_resp(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_PROBE_RESP);
}

int ieee80211_is_beacon(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_BEACON);
}

int ieee80211_is_atim(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_ATIM);
}

int ieee80211_is_disassoc(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_DISASSOC);
}

int ieee80211_is_auth(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_AUTH);
}

int ieee80211_is_deauth(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_DEAUTH);
}

int ieee80211_is_action(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_ACTION);
}

int ieee80211_is_back_req(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_CTL | IEEE80211_STYPE_BACK_REQ);
}

int ieee80211_is_back(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_CTL | IEEE80211_STYPE_BACK);
}

int ieee80211_is_pspoll(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_CTL | IEEE80211_STYPE_PSPOLL);
}

int ieee80211_is_rts(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_CTL | IEEE80211_STYPE_RTS);
}

int ieee80211_is_cts(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_CTL | IEEE80211_STYPE_CTS);
}

int ieee80211_is_ack(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_CTL | IEEE80211_STYPE_ACK);
}

int ieee80211_is_cfend(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_CTL | IEEE80211_STYPE_CFEND);
}

int ieee80211_is_cfendack(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_CTL | IEEE80211_STYPE_CFENDACK);
}

int ieee80211_is_nullfunc(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_DATA | IEEE80211_STYPE_NULLFUNC);
}

int ieee80211_is_qos_nullfunc(__le16 fc)
{
	return (fc & cpu_to_le16(IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) ==
	       cpu_to_le16(IEEE80211_FTYPE_DATA | IEEE80211_STYPE_QOS_NULLFUNC);
}

int ieee80211_is_first_frag(__le16 seq_ctrl)
{
	return (seq_ctrl & cpu_to_le16(IEEE80211_SCTL_FRAG)) == 0;
}

unsigned char *ieee80211_get_qos_ctl(struct ieee80211_hdr *hdr)
{
	if (ieee80211_has_a4(hdr->frame_control))
		return (unsigned char *)hdr + 30;
	else
		return (unsigned char *)hdr + 24;
}

unsigned char *ieee80211_get_SA(struct ieee80211_hdr *hdr)
{
	if (ieee80211_has_a4(hdr->frame_control))
		return hdr->addr4;
	if (ieee80211_has_fromds(hdr->frame_control))
		return hdr->addr3;
	return hdr->addr2;
}

unsigned char *ieee80211_get_DA(struct ieee80211_hdr *hdr)
{
	if (ieee80211_has_tods(hdr->frame_control))
		return hdr->addr3;
	else
		return hdr->addr1;
}

bool ieee80211_is_robust_mgmt_frame(struct ieee80211_hdr *hdr)
{
	if (ieee80211_is_disassoc(hdr->frame_control) ||
	    ieee80211_is_deauth(hdr->frame_control))
		return true;

	if (ieee80211_is_action(hdr->frame_control)) {
		unsigned char *category;

		/*
		 * Action frames, excluding Public Action frames, are Robust
		 * Management Frames. However, if we are looking at a Protected
		 * frame, skip the check since the data may be encrypted and
		 * the frame has already been found to be a Robust Management
		 * Frame (by the other end).
		 */
		if (ieee80211_has_protected(hdr->frame_control))
			return true;
		category = ((unsigned char *) hdr) + 24;
		return *category != WLAN_CATEGORY_PUBLIC &&
			*category != WLAN_CATEGORY_HT &&
			*category != WLAN_CATEGORY_SELF_PROTECTED &&
			*category != WLAN_CATEGORY_VENDOR_SPECIFIC;
	}

	return false;
}

int ieee80211_fhss_chan_to_freq(int channel)
{
	if ((channel > 1) && (channel < 96))
		return channel + 2400;
	else
		return -1;
}

int ieee80211_freq_to_fhss_chan(int freq)
{
	if ((freq > 2401) && (freq < 2496))
		return freq - 2400;
	else
		return -1;
}

int ieee80211_dsss_chan_to_freq(int channel)
{
	if ((channel > 0) && (channel < 14))
		return 2407 + (channel * 5);
	else if (channel == 14)
		return 2484;
	else
		return -1;
}

int ieee80211_freq_to_dsss_chan(int freq)
{
	if ((freq >= 2410) && (freq < 2475))
		return (freq - 2405) / 5;
	else if ((freq >= 2482) && (freq < 2487))
		return 14;
	else
		return -1;
}

int ieee80211_ofdm_chan_to_freq(int s_freq, int channel)
{
	if ((channel > 0) && (channel <= 200) &&
	    (s_freq >= 4000))
		return s_freq + (channel * 5);
	else
		return -1;
}

int ieee80211_freq_to_ofdm_chan(int s_freq, int freq)
{
	if ((freq > (s_freq + 2)) && (freq <= (s_freq + 1202)) &&
	    (s_freq >= 4000))
		return (freq + 2 - s_freq) / 5;
	else
		return -1;
}

unsigned long ieee80211_tu_to_usec(unsigned long tu)
{
	return 1024 * tu;
}

bool ieee80211_is_public_action(struct ieee80211_hdr *hdr,
					      size_t len)
{
	struct ieee80211_mgmt *mgmt = (void *)hdr;

	if (len < IEEE80211_MIN_ACTION_SIZE)
		return false;
	if (!ieee80211_is_action(hdr->frame_control))
		return false;
	return mgmt->u.action.category == WLAN_CATEGORY_PUBLIC;
}

unsigned int ieee80211_hdrlen(__le16 fc)
{
	unsigned int hdrlen = 24;

	if (ieee80211_is_data(fc)) {
		if (ieee80211_has_a4(fc))
			hdrlen = 30;
		if (ieee80211_is_data_qos(fc)) {
			hdrlen += IEEE80211_QOS_CTL_LEN;
			if (ieee80211_has_order(fc))
				hdrlen += IEEE80211_HT_CTL_LEN;
		}
		goto out;
	}

	if (ieee80211_is_ctl(fc)) {
		/*
		 * ACK and CTS are 10 bytes, all others 16. To see how
		 * to get this condition consider
		 *   subtype mask:   0b0000000011110000 (0x00F0)
		 *   ACK subtype:    0b0000000011010000 (0x00D0)
		 *   CTS subtype:    0b0000000011000000 (0x00C0)
		 *   bits that matter:         ^^^      (0x00E0)
		 *   value of those: 0b0000000011000000 (0x00C0)
		 */
		if ((fc & cpu_to_le16(0x00E0)) == cpu_to_le16(0x00C0))
			hdrlen = 10;
		else
			hdrlen = 16;
	}
out:
	return hdrlen;
}

unsigned int ieee80211_get_hdrlen_from_skb(const struct sk_buff *skb)
{
	const struct ieee80211_hdr *hdr =
			(const struct ieee80211_hdr *)ATBM_OS_SKB_DATA(skb);
	unsigned int hdrlen;

	if (unlikely(ATBM_OS_SKB_LEN(skb) < 10))
		return 0;
	hdrlen = ieee80211_hdrlen(hdr->frame_control);
	if (unlikely(hdrlen > ATBM_OS_SKB_LEN(skb)))
		return 0;
	return hdrlen;
}

bool ieee80211_frame_allowed(struct sk_buff *skb, unsigned short fc)
{
	return true;
}

void ieee80211_amsdu_to_8023s(struct sk_buff *skb, struct sk_buff_head *list,
			      const unsigned char *addr, enum nl80211_iftype iftype,
			      bool has_80211_header)
{
	struct sk_buff *frame = ATBM_NULL;
	unsigned short ethertype;
	unsigned char *payload;
	const struct ieee8023_hdr *eth;
	int remaining, err;
	unsigned char dst[ETH_ALEN], src[ETH_ALEN];

	if (has_80211_header) {
		err = ieee80211_data_to_8023(skb, addr, iftype);
		if (err)
			goto out;

		/* skip the wrapping header */
		eth = (struct ieee8023_hdr *)skb_pull(skb, sizeof(struct ieee8023_hdr));
		if (!eth)
			goto out;
	} else {
		eth = (struct ieee8023_hdr *) ATBM_OS_SKB_DATA(skb);
	}

	while (skb != frame) {
		unsigned char padding;
		unsigned short len = eth->h_proto;
		unsigned int subframe_len = sizeof(struct ieee8023_hdr) + ntohs(len);

		remaining = ATBM_OS_SKB_LEN(skb);
		memcpy(dst, eth->h_dest, ETH_ALEN);
		memcpy(src, eth->h_source, ETH_ALEN);

		padding = (4 - subframe_len) & 0x3;
		/* the last MSDU has no padding */
		if (subframe_len > remaining)
			goto purge;

		skb_pull(skb, sizeof(struct ieee8023_hdr));
		/* reuse skb for the last subframe */
		if (remaining <= subframe_len + padding)
			frame = skb;
		else {
			unsigned int hlen = 32;
			/*
			 * Allocate and reserve two bytes more for payload
			 * alignment since sizeof(struct ethhdr) is 14.
			 */
			frame = dev_alloc_skb(hlen + subframe_len + 2);
			if (!frame)
				goto purge;

			skb_reserve(frame, hlen + sizeof(struct ieee8023_hdr) + 2);
			memcpy(skb_put(frame, ntohs(len)), ATBM_OS_SKB_DATA(skb),ntohs(len));

			eth = (struct ieee8023_hdr *)skb_pull(skb, ntohs(len) +
							padding);
			if (!eth) {
				dev_kfree_skb(frame);
				goto purge;
			}
		}

		//skb_reset_network_header(frame);
		//frame->dev = skb->dev;
		//frame->priority = skb->priority;

		payload = ATBM_OS_SKB_DATA(frame);
		ethertype = (payload[6] << 8) | payload[7];

		if (likely((compare_ether_addr(payload, rfc1042_header) == 0 &&
			    ethertype != ETH_P_AARP && ethertype != ETH_P_IPX) ||
			   compare_ether_addr(payload,bridge_tunnel_header) == 0)) {
			/* remove RFC1042 or Bridge-Tunnel
			 * encapsulation and replace EtherType */
			skb_pull(frame, 6);
			memcpy(skb_push(frame, ETH_ALEN), src, ETH_ALEN);
			memcpy(skb_push(frame, ETH_ALEN), dst, ETH_ALEN);
		} else {
			memcpy(skb_push(frame, sizeof(unsigned short)), &len,
				sizeof(unsigned short));
			memcpy(skb_push(frame, ETH_ALEN), src, ETH_ALEN);
			memcpy(skb_push(frame, ETH_ALEN), dst, ETH_ALEN);
		}
		skb_queue_tail(list, frame);
	}

	return;

 purge:
	skb_queue_purge(list);
 out:
	dev_kfree_skb(skb);
}

int ieee80211_forward_skb(struct atbm_vif *priv,struct sk_buff *skb)
{
	struct ieee8023_hdr *hdr = (struct ieee8023_hdr *) ATBM_OS_SKB_DATA(skb);
	struct sk_buff *txskb = ATBM_NULL;
	int ret = true;
	struct sta_info *sta_priv = ATBM_NULL;
	
	if((priv->type == NL80211_IFTYPE_AP) || (priv->type == NL80211_IFTYPE_P2P_GO)){
		 if (is_multicast_ether_addr(hdr->h_dest)) {
				txskb = dev_alloc_skb(ATBM_OS_SKB_LEN(skb));
                if (txskb) {
					  txskb->priority = 0;
                      memcpy(ATBM_OS_SKB_DATA(txskb), ATBM_OS_SKB_DATA(skb), ATBM_OS_SKB_LEN(skb));
					  skb_put(txskb,ATBM_OS_SKB_LEN(skb));
					  atbm_tx(priv, txskb);
                }
				ret = true;
        }
        else if(memcmp(hdr->h_dest,priv->addr,6)){
                sta_priv = sta_info_get(priv,hdr->h_dest);
                if (sta_priv) {					
					txskb = dev_alloc_skb(ATBM_OS_SKB_LEN(skb));
	                if (txskb) {
						  txskb->priority = 0;
	                      memcpy(ATBM_OS_SKB_DATA(txskb), ATBM_OS_SKB_DATA(skb), ATBM_OS_SKB_LEN(skb));
						  skb_put(txskb,ATBM_OS_SKB_LEN(skb));
						  atbm_tx(priv, txskb);
	                }
					ret = false;
					dev_kfree_skb(skb);
                }
        }
	}
	

	return ret;
}

void ieee80211_deliver_skb(struct atbm_vif *priv, struct sk_buff *skb, unsigned short *need_free)
{
		if(ieee80211_forward_skb(priv,skb)){	
			//tcp_opt->net_rx(priv->ndev,skb);/////////////////////////////////////////////
		}
		*need_free=0;
}

#if 0
int ieee80211_rx_h_amsdu(struct atbm_vif *priv,struct sk_buff *skb)
{
	struct ieee80211_hdr *hdr;
	unsigned short fc;
	unsigned short need_free = 1;
	struct sk_buff_head frame_list;
	struct ieee80211_rx_status *status = IEEE80211_SKB_RXCB(skb);

	if (!(status->flag & RX_FLAG_AMSDU)){
		return 0;
	}

	hdr = (struct ieee80211_hdr *) ATBM_OS_SKB_DATA(skb);
	fc = hdr->frame_control;

	__skb_queue_head_init(&frame_list);

	ieee80211_amsdu_to_8023s(skb, &frame_list, priv->addr, priv->type, true);

	while (!skb_queue_empty(&frame_list)) {
		skb = skb_dequeue(&frame_list);

		if (!ieee80211_frame_allowed(skb, fc)) {
			dev_kfree_skb(skb);
			continue;
		}
		ieee80211_deliver_skb(priv,skb,&need_free);
		if(need_free){
			dev_kfree_skb(skb);
			need_free = 1;
		}
	}

	return 1;
}
#endif

int ieee80211_data_to_8023(struct sk_buff *skb, const unsigned char *addr, enum nl80211_iftype iftype)
{
	struct ieee80211_hdr *hdr = (struct ieee80211_hdr *) ATBM_OS_SKB_DATA(skb);
	unsigned short hdrlen, ethertype;
	unsigned char *payload;
	unsigned char dst[ETH_ALEN];
	unsigned char src[ETH_ALEN];

	if (unlikely(!ieee80211_is_data_present(hdr->frame_control)))
		return -1;

	hdrlen = ieee80211_hdrlen(hdr->frame_control);

	/* convert IEEE 802.11 header + possible LLC headers into Ethernet
	 * header
	 * IEEE 802.11 address fields:
	 * ToDS FromDS Addr1 Addr2 Addr3 Addr4
	 *   0     0   DA    SA    BSSID n/a
	 *   0     1   DA    BSSID SA    n/a
	 *   1     0   BSSID SA    DA    n/a
	 *   1     1   RA    TA    DA    SA
	 */
	memcpy(dst, ieee80211_get_DA(hdr), ETH_ALEN);
	memcpy(src, ieee80211_get_SA(hdr), ETH_ALEN);

	switch (hdr->frame_control &
		cpu_to_le16(IEEE80211_FCTL_TODS | IEEE80211_FCTL_FROMDS)) {
	case cpu_to_le16(IEEE80211_FCTL_TODS):
		if (unlikely(iftype != NL80211_IFTYPE_AP &&
			     iftype != NL80211_IFTYPE_P2P_GO))
			return -1;
		break;
	case cpu_to_le16(IEEE80211_FCTL_TODS | IEEE80211_FCTL_FROMDS):
		//if (unlikely(iftype != NL80211_IFTYPE_WDS &&
		//	     iftype != ATBM_NL80211_IFTYPE_STATION))
			return -1;
		break;
	case cpu_to_le16(IEEE80211_FCTL_FROMDS):
		if ((iftype != NL80211_IFTYPE_STATION &&
		     iftype != NL80211_IFTYPE_P2P_CLIENT ) ||
		    (is_multicast_ether_addr(dst) &&
		     !compare_ether_addr(src, addr)))
			return -1;
		break;
	case cpu_to_le16(0):
		if (iftype != NL80211_IFTYPE_ADHOC &&
		    iftype != NL80211_IFTYPE_STATION)
				return -1;
		break;
	}

	payload = ATBM_OS_SKB_DATA(skb) + hdrlen;
	ethertype = (payload[6] << 8) | payload[7];
	if (likely((compare_ether_addr(payload, rfc1042_header) == 0 &&
		    ethertype != ETH_P_AARP && ethertype != ETH_P_IPX) ||
		   compare_ether_addr(payload, bridge_tunnel_header) == 0)) {
		/* remove RFC1042 or Bridge-Tunnel encapsulation and
		 * replace EtherType */
		skb_pull(skb, hdrlen + 6);
		memcpy(skb_push(skb, ETH_ALEN), src, ETH_ALEN);
		memcpy(skb_push(skb, ETH_ALEN), dst, ETH_ALEN);
	} else {
		struct ieee8023_hdr *ehdr;
//		atbm_uint16 len;

		skb_pull(skb, hdrlen);
//		len = atbm_htons(ATBM_OS_SKB_LEN(skb));
		ehdr = (struct ieee8023_hdr *) skb_push(skb, sizeof(struct ieee8023_hdr));
		memcpy(ehdr->h_dest, dst, ETH_ALEN);
		memcpy(ehdr->h_source, src, ETH_ALEN);
		ehdr->h_proto = ethertype;
	}
	return 0;
}

int ieee80211_data_from_8023(struct sk_buff *skb, const unsigned char *addr,
			     enum nl80211_iftype iftype, unsigned char *bssid, bool qos)
{
	struct ieee80211_qos_hdr *  hdr;
	struct ieee8023_hdr *ethdr = (struct ieee8023_hdr *)ATBM_OS_SKB_DATA(skb);
	//struct atbmwifi_ieee80211_tx_info * tx_info = IEEE80211_SKB_TXCB(skb);
	unsigned short hdrlen, ethertype;
	unsigned char * llchdr=ATBM_NULL;
	unsigned short fc;
	const unsigned char *encaps_data;
	//int iv=0;
	int encaps_len, skip_header_bytes;
	//int link_id;
	
	if (unlikely(ATBM_OS_SKB_LEN(skb) < ETH_HLEN))
		return -EINVAL;
	
	//nh_pos = skb_network_header(skb) - skb->abuf;
	//h_pos = skb_transport_header(skb) - skb->abuf;

	/* convert Ethernet header to proper 802.11 header (based on
	 * operation mode) */
	ethertype = ntohs(ethdr->h_proto);
	
	
	fc = cpu_to_le16(IEEE80211_FTYPE_DATA | IEEE80211_STYPE_DATA);


	//wifi_printk(WIFI_TX,"%s: ethertype(%x) \n",__FUNCTION__,ethertype);
	skip_header_bytes = ETH_HLEN;
	if (ethertype == ETH_P_AARP || ethertype == ETH_P_IPX) {
		encaps_data = bridge_tunnel_header;
		encaps_len = sizeof(bridge_tunnel_header);
		skip_header_bytes -= 2;
	} else if (ethertype > 0x600) {
		encaps_data = rfc1042_header;
		encaps_len = sizeof(rfc1042_header);
		skip_header_bytes -= 2;
	} else {
		encaps_data = ATBM_NULL;
		encaps_len = 0;
	}
	skb_pull(skb, skip_header_bytes);
	
	if (encaps_data) {
		llchdr = skb_push(skb, encaps_len);
	}

	hdrlen = 24;
	if (qos)
		hdrlen += 2;
	
	//if(tx_info->sta_priv)
	//	link_id = tx_info->sta_priv->link_id;
	//else 
	//	link_id = 0;
		
	//iv= atbmwifi_tx_h_crypt_early(priv,ethdr,skb,link_id);
	
	//hdrlen += iv;
		//hdr->frame_control |= atbm_cpu_to_le16(IEEE80211_FCTL_PROTECTED);

	hdr= (struct ieee80211_qos_hdr *)skb_push(skb, hdrlen);

	//if(iv){
		//hdrlen += iv;
	//	hdr->frame_control |= atbm_cpu_to_le16(IEEE80211_FCTL_PROTECTED);
	//}

	fc = cpu_to_le16(IEEE80211_FTYPE_DATA | IEEE80211_STYPE_DATA);


	switch (iftype) {
	case NL80211_IFTYPE_AP:
	case NL80211_IFTYPE_P2P_GO:
		fc |= cpu_to_le16(IEEE80211_FCTL_FROMDS);
		/* DA BSSID SA */
		memcpy(hdr->addr1, ethdr->h_dest, ETH_ALEN);
		memcpy(hdr->addr2, addr, ETH_ALEN);
		memcpy(hdr->addr3, ethdr->h_source, ETH_ALEN);
		break;
	case NL80211_IFTYPE_STATION:
	case NL80211_IFTYPE_P2P_CLIENT:
		fc |= cpu_to_le16(IEEE80211_FCTL_TODS);
		/* BSSID SA DA */
		memcpy(hdr->addr1, bssid, ETH_ALEN);
		memcpy(hdr->addr2, ethdr->h_source, ETH_ALEN);
		memcpy(hdr->addr3, ethdr->h_dest, ETH_ALEN);
		break;
#ifdef CONFIG_WIFI_IBSS
	case NL80211_IFTYPE_ADHOC:
		/* DA SA BSSID */
		memcpy(hdr->addr1, ethdr->h_dest, ETH_ALEN);
		memcpy(hdr->addr2, ethdr->h_source, ETH_ALEN);
		memcpy(hdr->addr3, bssid, ETH_ALEN);
		break;
#endif /*CONFIG_WIFI_IBSS*/
	default:
		return -EOPNOTSUPP;
	}

	if (qos) {
		unsigned char ack_policy=0, tid;
		fc |= cpu_to_le16(IEEE80211_STYPE_QOS_DATA);
		/* use the data classifier to determine what 802.1d tag the
		 * data frame has */
		//skb->priority = atbm_cfg80211_classify8021d(skb);
		tid = skb->priority & IEEE80211_QOS_CTL_TAG1D_MASK;
		
		/* qos header is 2 bytes */		
		hdr->qos_ctrl = 	ack_policy | tid;	
	}

	hdr->frame_control = fc;
	hdr->duration_id = 0;
	hdr->seq_ctrl = 0;
	//nh_pos -= skip_header_bytes;
	//h_pos -= skip_header_bytes;
	if (encaps_data) {		
		memcpy(llchdr, encaps_data, encaps_len);
	}
	//tx_info->hdr = (struct atbmwifi_ieee80211_hdr *)hdr;
	//tx_info->hdrlen = hdrlen;
	
	return 0;
}

#define	WME_OUI_BYTES		0x00, 0x50, 0xf2

void ieee80211_bss_info_change_notify(struct atbm_vif *priv, unsigned int changed)
{
}

int ieee80211_frequency_to_channel(int freq)
{
	/* see 802.11 17.3.8.3.2 and Annex J */
	if (freq == 2484){
		return 14;
	}else if (freq < 2484){
		return (freq - 2407) / 5;
	}else if (freq >= 4910 && freq <= 4980){
		return (freq - 4000) / 5;
	}else{
		return (freq - 5000) / 5;
	}
}

int ieee80211_channel_to_frequency(int chan, enum ieee80211_band band)
{
	/* see 802.11 17.3.8.3.2 and Annex J
	 * there are overlapping channel numbers in 5GHz and 2GHz bands */
	if (band == IEEE80211_BAND_5GHZ) {
		if (chan >= 182 && chan <= 196){
			return 4000 + chan * 5;
		}else{
			return 5000 + chan * 5;
		}
	} else { /* ATBM_IEEE80211_BAND_2GHZ */
		if (chan == 14){
			return 2484;
		}else if (chan < 14){
			return 2407 + chan * 5;
		}else{
			return 0; /* not supported */
		}
	}
}
#if 0
int rx_filter_retry(struct atbm_vif *priv, unsigned char link_id,struct sk_buff *skb)
{
	struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)ATBM_OS_SKB_DATA(skb);
	//struct ieee80211_rx_status *status = IEEE80211_SKB_RXCB(skb);

	if(is_multicast_ether_addr(hdr->addr1)){
		return 0;
	}		
	if (!ieee80211_is_data_present(hdr->frame_control)){
		return 0;
	}
	if(link_id == LINK_ID_UNMAPPED)
		link_id = MAX_STA_IN_AP_MODE+1;
	if(link_id>MAX_STA_IN_AP_MODE+1){
		//wifi_printk(WIFI_DBG_ERROR, "[rx_retry] link_id fail(%d)\n",link_id);
		return -1;
	}

	ieee80211_parse_qos(priv,skb);
	return 0;
}

int ieee80211_rx(struct atbm_vif *priv,struct sk_buff *skb)
{
	struct ieee80211_rx_status *hdr = (struct ieee80211_rx_status *)IEEE80211_SKB_RXCB(skb);
	
	if(rx_filter_retry(priv,hdr->link_id,skb) < 0){		
		//wifi_printk(WIFI_RX, "[rx_retry] filter\n");
		return -1;
	}

#ifdef ATBM_RX_STATUS_USE_QUEUE
	skb->Type = RX_SKB__RX_PACKAGE;
#endif
#if 0//(ATBM_RX_TASK_QUEUE==0)
	skb_queue_tail(&priv->rx_task_skb_list,skb);
	/*Do rx task schedule,change task excute text*/
	
	///TODO atbm_net_RxTask
	queue_work(priv->hw_priv, priv->rx_task_work);

#else //ATBM_RX_TASK_QUEUE
	if(__ieee80211_rx(priv, skb)){
		//iot_printf("atbm_dev_kfree_skb %x\n",skb);
		dev_kfree_skb(skb);
	}
#endif //ATBM_RX_TASK_QUEUE
	return 0;
} 

int __ieee80211_rx(struct atbm_vif *priv,struct sk_buff *skb)
{
	struct ieee80211_hdr *hdr=ATBM_NULL;
	unsigned short fc;
	unsigned char ret=0;
	unsigned short needfree = 0;
	struct ieee80211_rx_status *hwhdr = (struct ieee80211_rx_status *)IEEE80211_SKB_RXCB(skb);
	hdr = (struct ieee80211_hdr *)ATBM_OS_SKB_DATA(skb);
	fc = hdr->frame_control;

	if(ieee80211_is_ctl(fc)){
		needfree = 1;
	}
	else if(ieee80211_is_nullfunc(fc)){
		needfree = 1;
	}else if(ieee80211_is_qos_nullfunc(fc)){
		needfree = 1;
	}
	else if (ieee80211_is_data_present(fc)) {
		needfree = 1;
		//amsdu
		ieee80211_parse_qos(priv,skb);
		ret = ieee80211_rx_h_amsdu(priv,skb);
		if(ret){
			needfree = 0;
			goto __free_return;
		}
		ret = ieee80211_data_to_8023(skb, priv->addr,priv->type);
		if(ret){
			//needfree = 1;
			//wifi_printk(WIFI_RX,"atbmwifi_ieee80211_data_to_8023 err\n");
			goto __free_return;
		}
		//update rssi
		//priv->bss.rssi = hwhdr->signal;
		ieee80211_deliver_skb(priv,skb,&needfree);
	}
	else {
		needfree = 1;
	}
	
__free_return:
	return needfree;
}
#endif

