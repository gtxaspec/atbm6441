#ifndef ATBM_OS_PROTO_H
#define ATBM_OS_PROTO_H

#define compare_ether_addr(a,b) (!((((unsigned char *)(a))[0] == ((unsigned char *)(b))[0]) && \
								(((unsigned char *)(a))[1] == ((unsigned char *)(b))[1]) && \
								(((unsigned char *)(a))[2] == ((unsigned char *)(b))[2]) && \
								(((unsigned char *)(a))[3] == ((unsigned char *)(b))[3]) && \
								(((unsigned char *)(a))[4] == ((unsigned char *)(b))[4]) && \
								(((unsigned char *)(a))[5] == ((unsigned char *)(b))[5])))

int ieee80211_has_tods(__le16 fc);
int ieee80211_has_fromds(__le16 fc);
int ieee80211_has_a4(__le16 fc);
int ieee80211_has_morefrags(__le16 fc);
int ieee80211_has_retry(__le16 fc);
int ieee80211_has_pm(__le16 fc);
int ieee80211_has_moredata(__le16 fc);
int ieee80211_has_protected(__le16 fc);
int ieee80211_has_order(__le16 fc);
int ieee80211_is_mgmt(__le16 fc);
int ieee80211_is_ctl(__le16 fc);
int ieee80211_is_data(__le16 fc);
int ieee80211_is_data_qos(__le16 fc);
int ieee80211_is_data_present(__le16 fc);
int ieee80211_is_assoc_req(__le16 fc);
int ieee80211_is_assoc_resp(__le16 fc);
int ieee80211_is_reassoc_req(__le16 fc);
int ieee80211_is_reassoc_resp(__le16 fc);
int ieee80211_is_probe_req(__le16 fc);
int ieee80211_is_probe_resp(__le16 fc);
int ieee80211_is_beacon(__le16 fc);
int ieee80211_is_atim(__le16 fc);
int ieee80211_is_disassoc(__le16 fc);
int ieee80211_is_auth(__le16 fc);
int ieee80211_is_deauth(__le16 fc);
int ieee80211_is_action(__le16 fc);
int ieee80211_is_back_req(__le16 fc);
int ieee80211_is_back(__le16 fc);
int ieee80211_is_pspoll(__le16 fc);
int ieee80211_is_rts(__le16 fc);
int ieee80211_is_cts(__le16 fc);
int ieee80211_is_ack(__le16 fc);
int ieee80211_is_cfend(__le16 fc);
int ieee80211_is_cfendack(__le16 fc);
int ieee80211_is_nullfunc(__le16 fc);
int ieee80211_is_qos_nullfunc(__le16 fc);
int ieee80211_is_first_frag(__le16 seq_ctrl);
unsigned char *ieee80211_get_qos_ctl(struct ieee80211_hdr *hdr);
unsigned char *ieee80211_get_SA(struct ieee80211_hdr *hdr);
unsigned char *ieee80211_get_DA(struct ieee80211_hdr *hdr);
bool ieee80211_is_robust_mgmt_frame(struct ieee80211_hdr *hdr);
int ieee80211_fhss_chan_to_freq(int channel);
int ieee80211_freq_to_fhss_chan(int freq);
int ieee80211_dsss_chan_to_freq(int channel);
int ieee80211_freq_to_dsss_chan(int freq);
int ieee80211_ofdm_chan_to_freq(int s_freq, int channel);
int ieee80211_freq_to_ofdm_chan(int s_freq, int freq);
unsigned long ieee80211_tu_to_usec(unsigned long tu);
bool ieee80211_is_public_action(struct ieee80211_hdr *hdr, size_t len);
unsigned int ieee80211_hdrlen(__le16 fc);
unsigned int ieee80211_get_hdrlen_from_skb(const struct sk_buff *skb);
bool ieee80211_frame_allowed(struct sk_buff *skb, unsigned short fc);
void ieee80211_amsdu_to_8023s(struct sk_buff *skb, struct sk_buff_head *list,
			      const unsigned char *addr, enum nl80211_iftype iftype,
			      bool has_80211_header);
int ieee80211_forward_skb(struct atbm_vif *priv,struct sk_buff *skb);
void ieee80211_deliver_skb(struct atbm_vif *priv, struct sk_buff *skb, unsigned short *need_free);
int ieee80211_rx_h_amsdu(struct atbm_vif *priv,struct sk_buff *skb);
int ieee80211_data_to_8023(struct sk_buff *skb, const unsigned char *addr, enum nl80211_iftype iftype);
int ieee80211_data_from_8023(struct sk_buff *skb, const unsigned char *addr,
			     enum nl80211_iftype iftype, unsigned char *bssid, bool qos);
void ieee80211_bss_info_change_notify(struct atbm_vif *priv, unsigned int changed);
int ieee80211_frequency_to_channel(int freq);
int ieee80211_channel_to_frequency(int chan, enum ieee80211_band band);
int rx_filter_retry(struct atbm_vif *priv, unsigned char link_id,struct sk_buff *skb);
int ieee80211_rx(struct atbm_vif *priv,struct sk_buff *skb);
int __ieee80211_rx(struct atbm_vif *priv,struct sk_buff *skb);

#endif // #ifndef ATBM_OS_PROTO_H

