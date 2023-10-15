#ifndef ATBM_OS_H
#define ATBM_OS_H

#define false 0
#define true  1

typedef int bool;

#define ATBM_NULL	((void *)0)

#define OS_FAIL     -1
#define OS_SUCCESS  0

#define HZ          (1000)

#define unlikely(a) (a)
#define likely(a) (a)

#define max_t(type, x, y) ({			\
	type __max1 = (x);			\
	type __max2 = (y);			\
	__max1 > __max2 ? __max1: __max2; })

#define atbm_packed __attribute__ ((packed))

#define ZEROSIZE 0

#define __bitwise__

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

#define ether_setup(x)

#define netdev_priv(dev) &dev->priv[0]

#define smp_mb()

#define might_sleep() do{}while(0)

#define pr_err printf

#define  is_broadcast_ether_addr(addr)  (((addr)[0] & (addr)[1] & (addr)[2] & (addr)[3] & (addr)[4] & (addr)[5]) == 0xff)

#define  is_multicast_ether_addr(addr)  (0x01 & (addr)[0])

#define netdev_attach_ops(dev,ops) dev->netdev_ops=ops

#define ASSERT_RTNL()

#define dev_alloc_name(x,y) (1)

#define SET_NETDEV_DEV(x,y)

#define register_netdevice(x) (0)

#define unregister_netdevice(x)

#define IS_ERR(x) (x==0)

#define asynchronize_rcu()

#define printk_once printf

#define in_interrupt() (0)

typedef unsigned int gfp_t;

#define __ALIGN_KERNEL_MASK(x,mask)	(((x) + (mask)) & ~(mask))
#define __ALIGN_KERNEL(x,a)		__ALIGN_KERNEL_MASK(x,(__typeof__(x))(a)-1)
#define ALIGN2(x,a)		__ALIGN_KERNEL((x),(a))

typedef unsigned long long __le64;
typedef unsigned int __le32;
typedef unsigned short __le16;

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef signed char s8;


typedef unsigned int size_t;

#define WARN(condition, format...) ({					\
	int __ret_warn_on = !!(condition);				\
	if (unlikely(__ret_warn_on))					\
		printf(format);					\
	unlikely(__ret_warn_on);					\
})


#define WARN_ON(x)  (x)

#define WARN_ON_ONCE(x) (x)

#define softirq_count()  (1)

#define schedule_timeout_interruptible msleep

/** 16 bits byte swap */
#define swap_byte_16(x) \
((t_u16)((((t_u16)(x) & 0x00ffU) << 8) | \
         (((t_u16)(x) & 0xff00U) >> 8)))

/** 32 bits byte swap */
#define swap_byte_32(x) \
((t_u32)((((t_u32)(x) & 0x000000ffUL) << 24) | \
         (((t_u32)(x) & 0x0000ff00UL) <<  8) | \
         (((t_u32)(x) & 0x00ff0000UL) >>  8) | \
         (((t_u32)(x) & 0xff000000UL) >> 24)))

/** Convert to correct endian format */
#ifdef  BIG_ENDIAN_SUPPORT
/** CPU to little-endian convert for 16-bit */
#define     cpu_to_le16(x)  swap_byte_16(x)
/** CPU to little-endian convert for 32-bit */
#define     cpu_to_le32(x)  swap_byte_32(x)
/** Little-endian to CPU convert for 16-bit */
#define     le16_to_cpu(x)  swap_byte_16(x)
/** Little-endian to CPU convert for 32-bit */
#define     le32_to_cpu(x)  swap_byte_32(x)
#else
/** Do nothing */
#define     cpu_to_le16(x)  (x)
/** Do nothing */
#define     cpu_to_le32(x)  (x)
/** Do nothing */
#define     le16_to_cpu(x)  (x)
/** Do nothing */
#define     le32_to_cpu(x)  (x)

#define   __cpu_to_le16(x)  (x)

#define   __cpu_to_le32(x)  (x)

#define   __le16_to_cpu(x)  (x)

#define   __le32_to_cpu(x)  (x)
#endif

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define typeof(x) __typeof__(x)

#define __must_check

#define __force

#define  EPERM     1  /* Operation not permitted */
#define  ENOENT     2  /* No such file or directory */
#define  ESRCH     3  /* No such process */
#define  EINTR     4  /* Interrupted system call */
#define  EIO     5  /* I/O error */
#define  ENXIO     6  /* No such device or address */
#define  E2BIG     7  /* Arg list too long */
#define  ENOEXEC     8  /* Exec format error */
#define  EBADF     9  /* Bad file number */
#define  ECHILD    10  /* No child processes */
#define  EAGAIN    11  /* Try again */
#define  ENOMEM    12  /* Out of memory */
#define  EACCES    13  /* Permission denied */
#define  EFAULT    14  /* Bad address */
#define  ENOTBLK    15  /* Block device required */
#define  EBUSY    16  /* Device or resource busy */
#define  EEXIST    17  /* File exists */
#define  EXDEV    18  /* Cross-device link */
#define  ENODEV    19  /* No such device */
#define  ENOTDIR    20  /* Not a directory */
#define  EISDIR    21  /* Is a directory */
#define  EINVAL    22  /* Invalid argument */
#define  ENFILE    23  /* File table overflow */
#define  EMFILE    24  /* Too many open files */
#define  ENOTTY    25  /* Not a typewriter */
#define  ETXTBSY    26  /* Text file busy */
#define  EFBIG    27  /* File too large */
#define  ENOSPC    28  /* No space left on device */
#define  ESPIPE    29  /* Illegal seek */
#define  EROFS    30  /* Read-only file system */
#define  EMLINK    31  /* Too many links */
#define  EPIPE    32  /* Broken pipe */
#define  EDOM    33  /* Math argument out of domain of func */
#define  ERANGE    34  /* Math result not representable */
#define  EDEADLK    35  /* Resource deadlock would occur */
#define  ENAMETOOLONG  36  /* File name too long */
#define  ENOLCK    37  /* No record locks available */
#define  ENOSYS    38  /* Function not implemented */
#define  ENOTEMPTY  39  /* Directory not empty */
#define  ELOOP    40  /* Too many symbolic links encountered */
#define  EWOULDBLOCK  EAGAIN  /* Operation would block */
#define  ENOMSG    42  /* No message of desired type */
#define  EIDRM    43  /* Identifier removed */
#define  ECHRNG    44  /* Channel number out of range */
#define  EL2NSYNC  45  /* Level 2 not synchronized */
#define  EL3HLT    46  /* Level 3 halted */
#define  EL3RST    47  /* Level 3 reset */
#define  ELNRNG    48  /* Link number out of range */
#define  EUNATCH    49  /* Protocol driver not attached */
#define  ENOCSI    50  /* No CSI structure available */
#define  EL2HLT    51  /* Level 2 halted */
#define  EBADE    52  /* Invalid exchange */
#define  EBADR    53  /* Invalid request descriptor */
#define  EXFULL    54  /* Exchange full */
#define  ENOANO    55  /* No anode */
#define  EBADRQC    56  /* Invalid request code */
#define  EBADSLT    57  /* Invalid slot */
#define  EBFONT    59  /* Bad font file format */
#define  ENOSTR    60  /* Device not a stream */
#define  ENODATA    61  /* No data available */
#define  ETIME    62  /* Timer expired */
#define  ENOSR    63  /* Out of streams resources */
#define  ENONET    64  /* Machine is not on the network */
#define  ENOPKG    65  /* Package not installed */
#define  EREMOTE    66  /* Object is remote */
#define  ENOLINK    67  /* Link has been severed */
#define  EADV    68  /* Advertise error */
#define  ESRMNT    69  /* Srmount error */
#define  ECOMM    70  /* Communication error on send */
#define  EPROTO    71  /* Protocol error */
#define  EMULTIHOP  72  /* Multihop attempted */
#define  EDOTDOT    73  /* RFS specific error */
#define  EBADMSG    74  /* Not a data message */
#define  EOVERFLOW  75  /* Value too large for defined data type */
#define  ENOTUNIQ  76  /* Name not unique on network */
#define  EBADFD    77  /* File descriptor in bad state */
#define  EREMCHG    78  /* Remote address changed */
#define  ELIBACC    79  /* Can not access a needed shared library */
#define  ELIBBAD    80  /* Accessing a corrupted shared library */
#define  ELIBSCN    81  /* .lib section in a.out corrupted */
#define  ELIBMAX    82  /* Attempting to link in too many shared libraries */
#define  ELIBEXEC  83  /* Cannot exec a shared library directly */
#define  EILSEQ    84  /* Illegal byte sequence */
#define  ERESTART  85  /* Interrupted system call should be restarted */
#define  ESTRPIPE  86  /* Streams pipe error */
#define  EUSERS    87  /* Too many users */
#define  ENOTSOCK  88  /* Socket operation on non-socket */
#define  EDESTADDRREQ  89  /* Destination address required */
#define  EMSGSIZE  90  /* Message too long */
#define  EPROTOTYPE  91  /* Protocol wrong type for socket */
#define  ENOPROTOOPT  92  /* Protocol not available */
#define  EPROTONOSUPPORT  93  /* Protocol not supported */
#define  ESOCKTNOSUPPORT  94  /* Socket type not supported */
#define  EOPNOTSUPP  95  /* Operation not supported on transport endpoint */
#define  EPFNOSUPPORT  96  /* Protocol family not supported */
#define  EAFNOSUPPORT  97  /* Address family not supported by protocol */
#define  EADDRINUSE  98  /* Address already in use */
#define  EADDRNOTAVAIL  99  /* Cannot assign requested address */
#define  ENETDOWN  100  /* Network is down */
#define  ENETUNREACH  101  /* Network is unreachable */
#define  ENETRESET  102  /* Network dropped connection because of reset */
#define  ECONNABORTED  103  /* Software caused connection abort */
#define  ECONNRESET  104  /* Connection reset by peer */
#define  ENOBUFS    105  /* No buffer space available */
#define  EISCONN    106  /* Transport endpoint is already connected */
#define  ENOTCONN  107  /* Transport endpoint is not connected */
//#define  ESHUTDOWN  108  /* Cannot send after transport endpoint shutdown */
#define  ETOOMANYREFS  109  /* Too many references: cannot splice */
#define  ETIMEDOUT  110  /* Connection timed out */
#define  ECONNREFUSED  111  /* Connection refused */
#define  EHOSTDOWN  112  /* Host is down */
#define  EHOSTUNREACH  113  /* No route to host */
#define  EALREADY  114  /* Operation already in progress */
#define  EINPROGRESS  115  /* Operation now in progress */
#define  ESTALE    116  /* Stale NFS file handle */
#define  EUCLEAN    117  /* Structure needs cleaning */
#define  ENOTNAM    118  /* Not a XENIX named type file */
#define  ENAVAIL    119  /* No XENIX semaphores available */
#define  EISNAM    120  /* Is a named type file */
#define  EREMOTEIO  121  /* Remote I/O error */
#define  EDQUOT    122  /* Quota exceeded */

#define  ENOMEDIUM  123  /* No medium found */
#define  EMEDIUMTYPE  124  /* Wrong medium type */


#define ENSROK    0 /* DNS server returned answer with no data */
#define ENSRNODATA  160 /* DNS server returned answer with no data */
#define ENSRFORMERR 161 /* DNS server claims query was misformatted */
#define ENSRSERVFAIL 162  /* DNS server returned general failure */
#define ENSRNOTFOUND 163  /* Domain name not found */
#define ENSRNOTIMP  164 /* DNS server does not implement requested operation */
#define ENSRREFUSED 165 /* DNS server refused query */
#define ENSRBADQUERY 166  /* Misformatted DNS query */
#define ENSRBADNAME 167 /* Misformatted domain name */
#define ENSRBADFAMILY 168 /* Unsupported address family */
#define ENSRBADRESP 169 /* Misformatted DNS reply */
#define ENSRCONNREFUSED 170 /* Could not contact DNS servers */
#define ENSRTIMEOUT 171 /* Timeout while contacting DNS servers */
#define ENSROF    172 /* End of file */
#define ENSRFILE  173 /* Error reading file */
#define ENSRNOMEM 174 /* Out of memory */
#define ENSRDESTRUCTION 175 /* Application terminated lookup */
#define ENSRQUERYDOMAINTOOLONG  176 /* Domain name is too long */
#define ENSRCNAMELOOP 177 /* Domain name is too long */
#define ENOTSUPP      178 /*no support */

#define LINUX_VERSION_CODE (3*100+3)
#define KERNEL_VERSION(x,y,z) (x*100+y)

#define module_init(x)
#define module_exit(x)

#define MODULE_DESCRIPTION(x)
#define MODULE_LICENSE(x)
#define MODULE_ALIAS(x)

#define module_param(x,y,z)
#define MODULE_PARM_DESC(x,y)

#define MODULE_AUTHOR(x)



#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))

#define BITS_PER_BYTE		8
#define BITS_TO_LONGS(nr)	DIV_ROUND_UP(nr, BITS_PER_BYTE * sizeof(long))

#define __rcu

#define rcu_access_pointer(a)   (a)
#define rcu_assign_pointer(a,b) (a=b)
#define rcu_dereference(a)      (a)
#define rcu_dereference_check(p, c) rcu_dereference(p)

#define rcu_dereference_protected(p,c) (p)

#define RCU_INIT_POINTER(p, v) (p = v)

#define call_rcu(x,y)

#define synchronize_rcu()

#define init_completion(x)

#define __acquires(x)

#define eth_type_trans(x,y) (0)

#define  local_bh_disable()

#define local_bh_enable()

#define ETH_ALEN    (6)
#define ETH_HLEN	(14)		/* Total octets in header.	 */

struct rcu_head 
{
	struct rcu_head *next;
	void (*func)(struct rcu_head *head);
};

#define lockdep_assert_held(l)			do { } while (0)


#define lockdep_is_held(l) (1)


#include <stdio.h>
#include "stddef.h"
#include "list.h"
#include "lwip/sockets.h"
#include "rtdef.h"

#include "atbm_os_mem.h"
#include "atbm_os_atomic.h"
#include "atbm_os_timer.h"
#include "atbm_os_mutex.h"
#include "atbm_os_queue.h"
#include "atbm_os_skbuf.h"
#include "atbm_os_thread.h"
#include "atbm_os_sdio.h"
#include "atbm_os_misc.h"

int apollo_sdio_module_init(void);
void  apollo_sdio_module_exit(void);



#define BIT(nr)			(1UL << (nr))

#define KERN_DEBUG
#define KERN_ERR
#define KERN_ALERT
#define KERN_INFO
#define KERN_WARNING

#define printk printf

#define IEEE80211_FCTL_VERS		0x0003
#define IEEE80211_FCTL_FTYPE		0x000c
#define IEEE80211_FCTL_STYPE		0x00f0
#define IEEE80211_FCTL_TODS		0x0100
#define IEEE80211_FCTL_FROMDS		0x0200
#define IEEE80211_FCTL_MOREFRAGS	0x0400
#define IEEE80211_FCTL_RETRY		0x0800
#define IEEE80211_FCTL_PM		0x1000
#define IEEE80211_FCTL_MOREDATA		0x2000
#define IEEE80211_FCTL_PROTECTED	0x4000
#define IEEE80211_FCTL_ORDER		0x8000

#define IEEE80211_SCTL_FRAG		0x000F
#define IEEE80211_SCTL_SEQ		0xFFF0

#define IEEE80211_FTYPE_MGMT		0x0000
#define IEEE80211_FTYPE_CTL		0x0004
#define IEEE80211_FTYPE_DATA		0x0008

/* management */
#define IEEE80211_STYPE_ASSOC_REQ	0x0000
#define IEEE80211_STYPE_ASSOC_RESP	0x0010
#define IEEE80211_STYPE_REASSOC_REQ	0x0020
#define IEEE80211_STYPE_REASSOC_RESP	0x0030
#define IEEE80211_STYPE_PROBE_REQ	0x0040
#define IEEE80211_STYPE_PROBE_RESP	0x0050
#define IEEE80211_STYPE_BEACON		0x0080
#define IEEE80211_STYPE_ATIM		0x0090
#define IEEE80211_STYPE_DISASSOC	0x00A0
#define IEEE80211_STYPE_AUTH		0x00B0
#define IEEE80211_STYPE_DEAUTH		0x00C0
#define IEEE80211_STYPE_ACTION		0x00D0

/* control */
#define IEEE80211_STYPE_BACK_REQ	0x0080
#define IEEE80211_STYPE_BACK		0x0090
#define IEEE80211_STYPE_PSPOLL		0x00A0
#define IEEE80211_STYPE_RTS		0x00B0
#define IEEE80211_STYPE_CTS		0x00C0
#define IEEE80211_STYPE_ACK		0x00D0
#define IEEE80211_STYPE_CFEND		0x00E0
#define IEEE80211_STYPE_CFENDACK	0x00F0

/* data */
#define IEEE80211_STYPE_DATA			0x0000
#define IEEE80211_STYPE_DATA_CFACK		0x0010
#define IEEE80211_STYPE_DATA_CFPOLL		0x0020
#define IEEE80211_STYPE_DATA_CFACKPOLL		0x0030
#define IEEE80211_STYPE_NULLFUNC		0x0040
#define IEEE80211_STYPE_CFACK			0x0050
#define IEEE80211_STYPE_CFPOLL			0x0060
#define IEEE80211_STYPE_CFACKPOLL		0x0070
#define IEEE80211_STYPE_QOS_DATA		0x0080
#define IEEE80211_STYPE_QOS_DATA_CFACK		0x0090
#define IEEE80211_STYPE_QOS_DATA_CFPOLL		0x00A0
#define IEEE80211_STYPE_QOS_DATA_CFACKPOLL	0x00B0
#define IEEE80211_STYPE_QOS_NULLFUNC		0x00C0
#define IEEE80211_STYPE_QOS_CFACK		0x00D0
#define IEEE80211_STYPE_QOS_CFPOLL		0x00E0
#define IEEE80211_STYPE_QOS_CFACKPOLL		0x00F0

/* miscellaneous IEEE 802.11 constants */
#define IEEE80211_MAX_FRAG_THRESHOLD	2352
#define IEEE80211_MAX_RTS_THRESHOLD	2353
#define IEEE80211_MAX_AID		2007
#define IEEE80211_MAX_TIM_LEN		251
/* Maximum size for the MA-UNITDATA primitive, 802.11 standard section
   6.2.1.1.2.

   802.11e clarifies the figure in section 7.1.2. The frame body is
   up to 2304 octets long (maximum MSDU size) plus any crypt overhead. */
#define IEEE80211_MAX_DATA_LEN		2304
/* 30 byte 4 addr hdr, 2 byte QoS, 2304 byte MSDU, 12 byte crypt, 4 byte FCS */
#define IEEE80211_MAX_FRAME_LEN		2352

#define IEEE80211_MAX_SSID_LEN		32

#define IEEE80211_MAX_MESH_ID_LEN	32

#define IEEE80211_QOS_CTL_LEN		2
/* 1d tag mask */
#define IEEE80211_QOS_CTL_TAG1D_MASK		0x0007
/* TID mask */
#define IEEE80211_QOS_CTL_TID_MASK		0x000f
/* EOSP */
#define IEEE80211_QOS_CTL_EOSP			0x0010
/* ACK policy */
#define IEEE80211_QOS_CTL_ACK_POLICY_NORMAL	0x0000
#define IEEE80211_QOS_CTL_ACK_POLICY_NOACK	0x0020
#define IEEE80211_QOS_CTL_ACK_POLICY_NO_EXPL	0x0040
#define IEEE80211_QOS_CTL_ACK_POLICY_BLOCKACK	0x0060
#define IEEE80211_QOS_CTL_ACK_POLICY_MASK	0x0060
/* A-MSDU 802.11n */
#define IEEE80211_QOS_CTL_A_MSDU_PRESENT	0x0080
/* Mesh Control 802.11s */
#define IEEE80211_QOS_CTL_MESH_CONTROL_PRESENT  0x0100

/* U-APSD queue for WMM IEs sent by AP */
#define IEEE80211_WMM_IE_AP_QOSINFO_UAPSD	(1<<7)
#define IEEE80211_WMM_IE_AP_QOSINFO_PARAM_SET_CNT_MASK	0x0f

/* U-APSD queues for WMM IEs sent by STA */
#define IEEE80211_WMM_IE_STA_QOSINFO_AC_VO	(1<<0)
#define IEEE80211_WMM_IE_STA_QOSINFO_AC_VI	(1<<1)
#define IEEE80211_WMM_IE_STA_QOSINFO_AC_BK	(1<<2)
#define IEEE80211_WMM_IE_STA_QOSINFO_AC_BE	(1<<3)
#define IEEE80211_WMM_IE_STA_QOSINFO_AC_MASK	0x0f

/* U-APSD max SP length for WMM IEs sent by STA */
#define IEEE80211_WMM_IE_STA_QOSINFO_SP_ALL	0x00
#define IEEE80211_WMM_IE_STA_QOSINFO_SP_2	0x01
#define IEEE80211_WMM_IE_STA_QOSINFO_SP_4	0x02
#define IEEE80211_WMM_IE_STA_QOSINFO_SP_6	0x03
#define IEEE80211_WMM_IE_STA_QOSINFO_SP_MASK	0x03
#define IEEE80211_WMM_IE_STA_QOSINFO_SP_SHIFT	5


/* 802.11n HT capabilities masks (for cap_info) */
#define IEEE80211_HT_CAP_LDPC_CODING		0x0001
#define IEEE80211_HT_CAP_SUP_WIDTH_20_40	0x0002
#define IEEE80211_HT_CAP_SM_PS			0x000C
#define		IEEE80211_HT_CAP_SM_PS_SHIFT	2
#define IEEE80211_HT_CAP_GRN_FLD		0x0010
#define IEEE80211_HT_CAP_SGI_20			0x0020
#define IEEE80211_HT_CAP_SGI_40			0x0040
#define IEEE80211_HT_CAP_TX_STBC		0x0080
#define IEEE80211_HT_CAP_RX_STBC		0x0300
#define		IEEE80211_HT_CAP_RX_STBC_SHIFT	8
#define IEEE80211_HT_CAP_DELAY_BA		0x0400
#define IEEE80211_HT_CAP_MAX_AMSDU		0x0800
#define IEEE80211_HT_CAP_DSSSCCK40		0x1000
#define IEEE80211_HT_CAP_RESERVED		0x2000
#define IEEE80211_HT_CAP_40MHZ_INTOLERANT	0x4000
#define IEEE80211_HT_CAP_LSIG_TXOP_PROT		0x8000

/* 802.11n HT extended capabilities masks (for extended_ht_cap_info) */
#define IEEE80211_HT_EXT_CAP_PCO		0x0001
#define IEEE80211_HT_EXT_CAP_PCO_TIME		0x0006
#define		IEEE80211_HT_EXT_CAP_PCO_TIME_SHIFT	1
#define IEEE80211_HT_EXT_CAP_MCS_FB		0x0300
#define		IEEE80211_HT_EXT_CAP_MCS_FB_SHIFT	8
#define IEEE80211_HT_EXT_CAP_HTC_SUP		0x0400
#define IEEE80211_HT_EXT_CAP_RD_RESPONDER	0x0800

/* 802.11n HT capability AMPDU settings (for ampdu_params_info) */
#define IEEE80211_HT_AMPDU_PARM_FACTOR		0x03
#define IEEE80211_HT_AMPDU_PARM_DENSITY		0x1C
#define		IEEE80211_HT_AMPDU_PARM_DENSITY_SHIFT	2


/*
 * Maximum length of AMPDU that the STA can receive.
 * Length = 2 ^ (13 + max_ampdu_length_exp) - 1 (octets)
 */
enum ieee80211_max_ampdu_length_exp {
	IEEE80211_HT_MAX_AMPDU_8K = 0,
	IEEE80211_HT_MAX_AMPDU_16K = 1,
	IEEE80211_HT_MAX_AMPDU_32K = 2,
	IEEE80211_HT_MAX_AMPDU_64K = 3
};

#define IEEE80211_HT_MAX_AMPDU_FACTOR 13

/* Minimum MPDU start spacing */
enum ieee80211_min_mpdu_spacing {
	IEEE80211_HT_MPDU_DENSITY_NONE = 0,	/* No restriction */
	IEEE80211_HT_MPDU_DENSITY_0_25 = 1,	/* 1/4 usec */
	IEEE80211_HT_MPDU_DENSITY_0_5 = 2,	/* 1/2 usec */
	IEEE80211_HT_MPDU_DENSITY_1 = 3,	/* 1 usec */
	IEEE80211_HT_MPDU_DENSITY_2 = 4,	/* 2 usec */
	IEEE80211_HT_MPDU_DENSITY_4 = 5,	/* 4 usec */
	IEEE80211_HT_MPDU_DENSITY_8 = 6,	/* 8 usec */
	IEEE80211_HT_MPDU_DENSITY_16 = 7	/* 16 usec */
};

/* 802.11n HT capability MSC set */
#define IEEE80211_HT_MCS_RX_HIGHEST_MASK	0x3ff
#define IEEE80211_HT_MCS_TX_DEFINED		0x01
#define IEEE80211_HT_MCS_TX_RX_DIFF		0x02
/* value 0 == 1 stream etc */
#define IEEE80211_HT_MCS_TX_MAX_STREAMS_MASK	0x0C
#define IEEE80211_HT_MCS_TX_MAX_STREAMS_SHIFT	2
#define		IEEE80211_HT_MCS_TX_MAX_STREAMS	4
#define IEEE80211_HT_MCS_TX_UNEQUAL_MODULATION	0x10

/*
 * 802.11n D5.0 20.3.5 / 20.6 says:
 * - indices 0 to 7 and 32 are single spatial stream
 * - 8 to 31 are multiple spatial streams using equal modulation
 *   [8..15 for two streams, 16..23 for three and 24..31 for four]
 * - remainder are multiple spatial streams using unequal modulation
 */
#define IEEE80211_HT_MCS_UNEQUAL_MODULATION_START 33
#define IEEE80211_HT_MCS_UNEQUAL_MODULATION_START_BYTE \
	(IEEE80211_HT_MCS_UNEQUAL_MODULATION_START / 8)

/* for ht_param */
#define IEEE80211_HT_PARAM_CHA_SEC_OFFSET		0x03
#define		IEEE80211_HT_PARAM_CHA_SEC_NONE		0x00
#define		IEEE80211_HT_PARAM_CHA_SEC_ABOVE	0x01
#define		IEEE80211_HT_PARAM_CHA_SEC_BELOW	0x03
#define IEEE80211_HT_PARAM_CHAN_WIDTH_ANY		0x04
#define IEEE80211_HT_PARAM_RIFS_MODE			0x08
#define IEEE80211_HT_PARAM_SPSMP_SUPPORT		0x10
#define IEEE80211_HT_PARAM_SERV_INTERVAL_GRAN		0xE0

/* for operation_mode */
#define IEEE80211_HT_OP_MODE_PROTECTION			0x0003
#define		IEEE80211_HT_OP_MODE_PROTECTION_NONE		0
#define		IEEE80211_HT_OP_MODE_PROTECTION_NONMEMBER	1
#define		IEEE80211_HT_OP_MODE_PROTECTION_20MHZ		2
#define		IEEE80211_HT_OP_MODE_PROTECTION_NONHT_MIXED	3
#define IEEE80211_HT_OP_MODE_NON_GF_STA_PRSNT		0x0004
#define IEEE80211_HT_OP_MODE_NON_HT_STA_PRSNT		0x0010

/* for stbc_param */
#define IEEE80211_HT_STBC_PARAM_DUAL_BEACON		0x0040
#define IEEE80211_HT_STBC_PARAM_DUAL_CTS_PROT		0x0080
#define IEEE80211_HT_STBC_PARAM_STBC_BEACON		0x0100
#define IEEE80211_HT_STBC_PARAM_LSIG_TXOP_FULLPROT	0x0200
#define IEEE80211_HT_STBC_PARAM_PCO_ACTIVE		0x0400
#define IEEE80211_HT_STBC_PARAM_PCO_PHASE		0x0800


/* block-ack parameters */
#define IEEE80211_ADDBA_PARAM_POLICY_MASK 0x0002
#define IEEE80211_ADDBA_PARAM_TID_MASK 0x003C
#define IEEE80211_ADDBA_PARAM_BUF_SIZE_MASK 0xFFC0
#define IEEE80211_DELBA_PARAM_TID_MASK 0xF000
#define IEEE80211_DELBA_PARAM_INITIATOR_MASK 0x0800

/*
 * A-PMDU buffer sizes
 * According to IEEE802.11n spec size varies from 8K to 64K (in powers of 2)
 */
#define IEEE80211_MIN_AMPDU_BUF 0x8
#define IEEE80211_MAX_AMPDU_BUF 0x40


/* Spatial Multiplexing Power Save Modes (for capability) */
#define WLAN_HT_CAP_SM_PS_STATIC	0
#define WLAN_HT_CAP_SM_PS_DYNAMIC	1
#define WLAN_HT_CAP_SM_PS_INVALID	2
#define WLAN_HT_CAP_SM_PS_DISABLED	3

/* for SM power control field lower two bits */
#define WLAN_HT_SMPS_CONTROL_DISABLED	0
#define WLAN_HT_SMPS_CONTROL_STATIC	1
#define WLAN_HT_SMPS_CONTROL_DYNAMIC	3

/* Authentication algorithms */
#define WLAN_AUTH_OPEN 0
#define WLAN_AUTH_SHARED_KEY 1
#define WLAN_AUTH_FT 2
#define WLAN_AUTH_SAE 3
#define WLAN_AUTH_LEAP 128

#define WLAN_AUTH_CHALLENGE_LEN 128

#define WLAN_CAPABILITY_ESS		(1<<0)
#define WLAN_CAPABILITY_IBSS		(1<<1)


enum ieee80211_timeout_interval_type {
	WLAN_TIMEOUT_REASSOC_DEADLINE = 1 /* 802.11r */,
	WLAN_TIMEOUT_KEY_LIFETIME = 2 /* 802.11r */,
	WLAN_TIMEOUT_ASSOC_COMEBACK = 3 /* 802.11w */,
};

/* BACK action code */
enum ieee80211_back_actioncode {
	WLAN_ACTION_ADDBA_REQ = 0,
	WLAN_ACTION_ADDBA_RESP = 1,
	WLAN_ACTION_DELBA = 2,
};

/* BACK (block-ack) parties */
enum ieee80211_back_parties {
	WLAN_BACK_RECIPIENT = 0,
	WLAN_BACK_INITIATOR = 1,
};

/* SA Query action */
enum ieee80211_sa_query_action {
	WLAN_ACTION_SA_QUERY_REQUEST = 0,
	WLAN_ACTION_SA_QUERY_RESPONSE = 1,
};


/* cipher suite selectors */
#define WLAN_CIPHER_SUITE_USE_GROUP	0x000FAC00
#define WLAN_CIPHER_SUITE_WEP40		0x000FAC01
#define WLAN_CIPHER_SUITE_TKIP		0x000FAC02
/* reserved: 				0x000FAC03 */
#define WLAN_CIPHER_SUITE_CCMP		0x000FAC04
#define WLAN_CIPHER_SUITE_WEP104	0x000FAC05
#define WLAN_CIPHER_SUITE_AES_CMAC	0x000FAC06

#define WLAN_CIPHER_SUITE_SMS4		0x00147201

/* AKM suite selectors */
#define WLAN_AKM_SUITE_8021X		0x000FAC01
#define WLAN_AKM_SUITE_PSK		0x000FAC02
#define WLAN_AKM_SUITE_SAE			0x000FAC08
#define WLAN_AKM_SUITE_FT_OVER_SAE	0x000FAC09

#define WLAN_MAX_KEY_LEN		32

#define WLAN_PMKID_LEN			16

#define WLAN_OUI_WFA			0x506f9a
#define WLAN_OUI_TYPE_WFA_P2P		9

/*
 * WMM/802.11e Tspec Element
 */
#define IEEE80211_WMM_IE_TSPEC_TID_MASK		0x0F
#define IEEE80211_WMM_IE_TSPEC_TID_SHIFT	1

enum ieee80211_tspec_status_code {
	IEEE80211_TSPEC_STATUS_ADMISS_ACCEPTED = 0,
	IEEE80211_TSPEC_STATUS_ADDTS_INVAL_PARAMS = 0x1,
};


/*
 * A mesh STA sets the ESS and IBSS capability bits to zero.
 * however, this holds true for p2p probe responses (in the p2p_find
 * phase) as well.
 */
#define WLAN_CAPABILITY_IS_STA_BSS(cap)	\
	(!((cap) & (WLAN_CAPABILITY_ESS | WLAN_CAPABILITY_IBSS)))

#define WLAN_CAPABILITY_CF_POLLABLE	(1<<2)
#define WLAN_CAPABILITY_CF_POLL_REQUEST	(1<<3)
#define WLAN_CAPABILITY_PRIVACY		(1<<4)
#define WLAN_CAPABILITY_SHORT_PREAMBLE	(1<<5)
#define WLAN_CAPABILITY_PBCC		(1<<6)
#define WLAN_CAPABILITY_CHANNEL_AGILITY	(1<<7)

/* 802.11h */
#define WLAN_CAPABILITY_SPECTRUM_MGMT	(1<<8)
#define WLAN_CAPABILITY_QOS		(1<<9)
#define WLAN_CAPABILITY_SHORT_SLOT_TIME	(1<<10)
#define WLAN_CAPABILITY_DSSS_OFDM	(1<<13)
/* measurement */
#define IEEE80211_SPCT_MSR_RPRT_MODE_LATE	(1<<0)
#define IEEE80211_SPCT_MSR_RPRT_MODE_INCAPABLE	(1<<1)
#define IEEE80211_SPCT_MSR_RPRT_MODE_REFUSED	(1<<2)

#define IEEE80211_SPCT_MSR_RPRT_TYPE_BASIC	0
#define IEEE80211_SPCT_MSR_RPRT_TYPE_CCA	1
#define IEEE80211_SPCT_MSR_RPRT_TYPE_RPI	2


/* 802.11g ERP information element */
#define WLAN_ERP_NON_ERP_PRESENT (1<<0)
#define WLAN_ERP_USE_PROTECTION (1<<1)
#define WLAN_ERP_BARKER_PREAMBLE (1<<2)


#define IEEE80211_HT_CTL_LEN		4

#define IEEE80211_HT_MCS_MASK_LEN		(10)


/* Reason codes */
enum ieee80211_reasoncode {
	WLAN_REASON_UNSPECIFIED = 1,
	WLAN_REASON_PREV_AUTH_NOT_VALID = 2,
	WLAN_REASON_DEAUTH_LEAVING = 3,
	WLAN_REASON_DISASSOC_DUE_TO_INACTIVITY = 4,
	WLAN_REASON_DISASSOC_AP_BUSY = 5,
	WLAN_REASON_CLASS2_FRAME_FROM_NONAUTH_STA = 6,
	WLAN_REASON_CLASS3_FRAME_FROM_NONASSOC_STA = 7,
	WLAN_REASON_DISASSOC_STA_HAS_LEFT = 8,
	WLAN_REASON_STA_REQ_ASSOC_WITHOUT_AUTH = 9,
	/* 802.11h */
	WLAN_REASON_DISASSOC_BAD_POWER = 10,
	WLAN_REASON_DISASSOC_BAD_SUPP_CHAN = 11,
	/* 802.11i */
	WLAN_REASON_INVALID_IE = 13,
	WLAN_REASON_MIC_FAILURE = 14,
	WLAN_REASON_4WAY_HANDSHAKE_TIMEOUT = 15,
	WLAN_REASON_GROUP_KEY_HANDSHAKE_TIMEOUT = 16,
	WLAN_REASON_IE_DIFFERENT = 17,
	WLAN_REASON_INVALID_GROUP_CIPHER = 18,
	WLAN_REASON_INVALID_PAIRWISE_CIPHER = 19,
	WLAN_REASON_INVALID_AKMP = 20,
	WLAN_REASON_UNSUPP_RSN_VERSION = 21,
	WLAN_REASON_INVALID_RSN_IE_CAP = 22,
	WLAN_REASON_IEEE8021X_FAILED = 23,
	WLAN_REASON_CIPHER_SUITE_REJECTED = 24,
	/* 802.11e */
	WLAN_REASON_DISASSOC_UNSPECIFIED_QOS = 32,
	WLAN_REASON_DISASSOC_QAP_NO_BANDWIDTH = 33,
	WLAN_REASON_DISASSOC_LOW_ACK = 34,
	WLAN_REASON_DISASSOC_QAP_EXCEED_TXOP = 35,
	WLAN_REASON_QSTA_LEAVE_QBSS = 36,
	WLAN_REASON_QSTA_NOT_USE = 37,
	WLAN_REASON_QSTA_REQUIRE_SETUP = 38,
	WLAN_REASON_QSTA_TIMEOUT = 39,
	WLAN_REASON_QSTA_CIPHER_NOT_SUPP = 45,
	/* 802.11s */
	WLAN_REASON_MESH_PEER_CANCELED = 52,
	WLAN_REASON_MESH_MAX_PEERS = 53,
	WLAN_REASON_MESH_CONFIG = 54,
	WLAN_REASON_MESH_CLOSE = 55,
	WLAN_REASON_MESH_MAX_RETRIES = 56,
	WLAN_REASON_MESH_CONFIRM_TIMEOUT = 57,
	WLAN_REASON_MESH_INVALID_GTK = 58,
	WLAN_REASON_MESH_INCONSISTENT_PARAM = 59,
	WLAN_REASON_MESH_INVALID_SECURITY = 60,
	WLAN_REASON_MESH_PATH_ERROR = 61,
	WLAN_REASON_MESH_PATH_NOFORWARD = 62,
	WLAN_REASON_MESH_PATH_DEST_UNREACHABLE = 63,
	WLAN_REASON_MAC_EXISTS_IN_MBSS = 64,
	WLAN_REASON_MESH_CHAN_REGULATORY = 65,
	WLAN_REASON_MESH_CHAN = 66,
};


/* Status codes */
enum ieee80211_statuscode {
	WLAN_STATUS_SUCCESS = 0,
	WLAN_STATUS_UNSPECIFIED_FAILURE = 1,
	WLAN_STATUS_CAPS_UNSUPPORTED = 10,
	WLAN_STATUS_REASSOC_NO_ASSOC = 11,
	WLAN_STATUS_ASSOC_DENIED_UNSPEC = 12,
	WLAN_STATUS_NOT_SUPPORTED_AUTH_ALG = 13,
	WLAN_STATUS_UNKNOWN_AUTH_TRANSACTION = 14,
	WLAN_STATUS_CHALLENGE_FAIL = 15,
	WLAN_STATUS_AUTH_TIMEOUT = 16,
	WLAN_STATUS_AP_UNABLE_TO_HANDLE_NEW_STA = 17,
	WLAN_STATUS_ASSOC_DENIED_RATES = 18,
	/* 802.11b */
	WLAN_STATUS_ASSOC_DENIED_NOSHORTPREAMBLE = 19,
	WLAN_STATUS_ASSOC_DENIED_NOPBCC = 20,
	WLAN_STATUS_ASSOC_DENIED_NOAGILITY = 21,
	/* 802.11h */
	WLAN_STATUS_ASSOC_DENIED_NOSPECTRUM = 22,
	WLAN_STATUS_ASSOC_REJECTED_BAD_POWER = 23,
	WLAN_STATUS_ASSOC_REJECTED_BAD_SUPP_CHAN = 24,
	/* 802.11g */
	WLAN_STATUS_ASSOC_DENIED_NOSHORTTIME = 25,
	WLAN_STATUS_ASSOC_DENIED_NODSSSOFDM = 26,
	/* 802.11w */
	WLAN_STATUS_ASSOC_REJECTED_TEMPORARILY = 30,
	WLAN_STATUS_ROBUST_MGMT_FRAME_POLICY_VIOLATION = 31,
	/* 802.11i */
	WLAN_STATUS_INVALID_IE = 40,
	WLAN_STATUS_INVALID_GROUP_CIPHER = 41,
	WLAN_STATUS_INVALID_PAIRWISE_CIPHER = 42,
	WLAN_STATUS_INVALID_AKMP = 43,
	WLAN_STATUS_UNSUPP_RSN_VERSION = 44,
	WLAN_STATUS_INVALID_RSN_IE_CAP = 45,
	WLAN_STATUS_CIPHER_SUITE_REJECTED = 46,
	/* 802.11e */
	WLAN_STATUS_UNSPECIFIED_QOS = 32,
	WLAN_STATUS_ASSOC_DENIED_NOBANDWIDTH = 33,
	WLAN_STATUS_ASSOC_DENIED_LOWACK = 34,
	WLAN_STATUS_ASSOC_DENIED_UNSUPP_QOS = 35,
	WLAN_STATUS_REQUEST_DECLINED = 37,
	WLAN_STATUS_INVALID_QOS_PARAM = 38,
	WLAN_STATUS_CHANGE_TSPEC = 39,
	WLAN_STATUS_WAIT_TS_DELAY = 47,
	WLAN_STATUS_NO_DIRECT_LINK = 48,
	WLAN_STATUS_STA_NOT_PRESENT = 49,
	WLAN_STATUS_STA_NOT_QSTA = 50,
	/* 802.11s */
	WLAN_STATUS_ANTI_CLOG_REQUIRED = 76,
	WLAN_STATUS_FCG_NOT_SUPP = 78,
	WLAN_STATUS_STA_NO_TBTT = 78,
};

#define IEEE80211_HT_CAP_DELAY_BA		0x0400

#define ETH_P_LOOP	0x0060		/* Ethernet Loopback packet	*/
#define ETH_P_PUP	0x0200		/* Xerox PUP packet		*/
#define ETH_P_PUPAT	0x0201		/* Xerox PUP Addr Trans packet	*/
#define ETH_P_IP	0x0800		/* Internet Protocol packet	*/
#define ETH_P_X25	0x0805		/* CCITT X.25			*/
#define ETH_P_ARP	0x0806		/* Address Resolution packet	*/
#define	ETH_P_BPQ	0x08FF		/* G8BPQ AX.25 Ethernet Packet	[ NOT AN OFFICIALLY REGISTERED ID ] */
#define ETH_P_IEEEPUP	0x0a00		/* Xerox IEEE802.3 PUP packet */
#define ETH_P_IEEEPUPAT	0x0a01		/* Xerox IEEE802.3 PUP Addr Trans packet */
#define ETH_P_DEC       0x6000          /* DEC Assigned proto           */
#define ETH_P_DNA_DL    0x6001          /* DEC DNA Dump/Load            */
#define ETH_P_DNA_RC    0x6002          /* DEC DNA Remote Console       */
#define ETH_P_DNA_RT    0x6003          /* DEC DNA Routing              */
#define ETH_P_LAT       0x6004          /* DEC LAT                      */
#define ETH_P_DIAG      0x6005          /* DEC Diagnostics              */
#define ETH_P_CUST      0x6006          /* DEC Customer use             */
#define ETH_P_SCA       0x6007          /* DEC Systems Comms Arch       */
#define ETH_P_TEB	0x6558		/* Trans Ether Bridging		*/
#define ETH_P_RARP      0x8035		/* Reverse Addr Res packet	*/
#define ETH_P_ATALK	0x809B		/* Appletalk DDP		*/
#define ETH_P_AARP	0x80F3		/* Appletalk AARP		*/
#define ETH_P_8021Q	0x8100          /* 802.1Q VLAN Extended Header  */
#define ETH_P_IPX	0x8137		/* IPX over DIX			*/
#define ETH_P_IPV6	0x86DD		/* IPv6 over bluebook		*/
#define ETH_P_PAUSE	0x8808		/* IEEE Pause frames. See 802.3 31B */
#define ETH_P_SLOW	0x8809		/* Slow Protocol. See 802.3ad 43B */
#define ETH_P_WCCP	0x883E		/* Web-cache coordination protocol
				 * defined in draft-wilson-wrec-wccp-v2-00.txt */
#define ETH_P_PPP_DISC	0x8863		/* PPPoE discovery messages     */
#define ETH_P_PPP_SES	0x8864		/* PPPoE session messages	*/
#define ETH_P_MPLS_UC	0x8847		/* MPLS Unicast traffic		*/
#define ETH_P_MPLS_MC	0x8848		/* MPLS Multicast traffic	*/
#define ETH_P_ATMMPOA	0x884c		/* MultiProtocol Over ATM	*/
#define ETH_P_LINK_CTL	0x886c		/* HPNA, wlan link local tunnel */
#define ETH_P_ATMFATE	0x8884		/* Frame-based ATM Transport
					 * over Ethernet
					 */
#define ETH_P_PAE	0x888E		/* Port Access Entity (IEEE 802.1X) */
#define ETH_P_AOE	0x88A2		/* ATA over Ethernet		*/
#define ETH_P_TIPC	0x88CA		/* TIPC 			*/
#define ETH_P_1588	0x88F7		/* IEEE 1588 Timesync */
#define ETH_P_FCOE	0x8906		/* Fibre Channel over Ethernet  */
#define ETH_P_FIP	0x8914		/* FCoE Initialization Protocol */
#define ETH_P_EDSA	0xDADA		/* Ethertype DSA [ NOT AN OFFICIALLY REGISTERED ID ] */
/*
 *	Non DIX types. Won't clash for 1500 types.
 */
#define ETH_P_802_3	0x0001		/* Dummy type for 802.3 frames  */
#define ETH_P_AX25	0x0002		/* Dummy protocol id for AX.25  */
#define ETH_P_ALL	0x0003		/* Every packet (be careful!!!) */
#define ETH_P_802_2	0x0004		/* 802.2 frames 		*/
#define ETH_P_SNAP	0x0005		/* Internal only		*/
#define ETH_P_DDCMP     0x0006          /* DEC DDCMP: Internal only     */
#define ETH_P_WAN_PPP   0x0007          /* Dummy type for WAN PPP frames*/
#define ETH_P_PPP_MP    0x0008          /* Dummy type for PPP MP frames */
#define ETH_P_LOCALTALK 0x0009		/* Localtalk pseudo type 	*/
#define ETH_P_CAN	0x000C		/* Controller Area Network      */
#define ETH_P_PPPTALK	0x0010		/* Dummy type for Atalk over PPP*/
#define ETH_P_TR_802_2	0x0011		/* 802.2 frames 		*/
#define ETH_P_MOBITEX	0x0015		/* Mobitex (kaz@cafe.net)	*/
#define ETH_P_CONTROL	0x0016		/* Card specific control frames */
#define ETH_P_IRDA	0x0017		/* Linux-IrDA			*/
#define ETH_P_ECONET	0x0018		/* Acorn Econet			*/
#define ETH_P_HDLC	0x0019		/* HDLC frames			*/
#define ETH_P_ARCNET	0x001A		/* 1A for ArcNet :-)            */
#define ETH_P_DSA	0x001B		/* Distributed Switch Arch.	*/
#define ETH_P_TRAILER	0x001C		/* Trailer switch tagging	*/
#define ETH_P_PHONET	0x00F5		/* Nokia Phonet frames          */
#define ETH_P_IEEE802154 0x00F6		/* IEEE802.15.4 frame		*/
#define ETH_P_CAIF	0x00F7		/* ST-Ericsson CAIF protocol	*/

#define ATBM_WPA_REPLAY_COUNTER_LEN 8
#define ATBM_WPA_NONCE_LEN 32
#define ATBM_WPA_KEY_RSC_LEN 8


enum nl80211_iftype 
{
	NL80211_IFTYPE_UNSPECIFIED,
	NL80211_IFTYPE_ADHOC,
	NL80211_IFTYPE_STATION,
	NL80211_IFTYPE_AP,
	NL80211_IFTYPE_AP_VLAN,
	NL80211_IFTYPE_WDS,
	NL80211_IFTYPE_MONITOR,
	NL80211_IFTYPE_MESH_POINT,
	NL80211_IFTYPE_P2P_CLIENT,
	NL80211_IFTYPE_P2P_GO,

	/* keep last */
	NUM_NL80211_IFTYPES,
	NL80211_IFTYPE_MAX = NUM_NL80211_IFTYPES - 1
};

/* Action category code */
enum ieee80211_category 
{
	WLAN_CATEGORY_SPECTRUM_MGMT = 0,
	WLAN_CATEGORY_QOS = 1,
	WLAN_CATEGORY_DLS = 2,
	WLAN_CATEGORY_BACK = 3,
	WLAN_CATEGORY_PUBLIC = 4,
	WLAN_CATEGORY_HT = 7,
	WLAN_CATEGORY_SA_QUERY = 8,
	WLAN_CATEGORY_PROTECTED_DUAL_OF_ACTION = 9,
	WLAN_CATEGORY_TDLS = 12,
	WLAN_CATEGORY_MESH_ACTION = 13,
	WLAN_CATEGORY_MULTIHOP_ACTION = 14,
	WLAN_CATEGORY_SELF_PROTECTED = 15,
	WLAN_CATEGORY_WMM = 17,
	WLAN_CATEGORY_VENDOR_SPECIFIC_PROTECTED = 126,
	WLAN_CATEGORY_VENDOR_SPECIFIC = 127,
};

/* SPECTRUM_MGMT action code */
enum ieee80211_spectrum_mgmt_actioncode 
{
	WLAN_ACTION_SPCT_MSR_REQ = 0,	 
	WLAN_ACTION_SPCT_MSR_RPRT = 1,  
	WLAN_ACTION_SPCT_TPC_REQ = 2,	 
	WLAN_ACTION_SPCT_TPC_RPRT = 3,  
	WLAN_ACTION_SPCT_CHL_SWITCH = 4,
};

/* HT action codes */
enum ieee80211_ht_actioncode 
{
	WLAN_HT_ACTION_NOTIFY_CHANWIDTH = 0, 
	WLAN_HT_ACTION_SMPS = 1,             
	WLAN_HT_ACTION_PSMP = 2,             
	WLAN_HT_ACTION_PCO_PHASE = 3,        
	WLAN_HT_ACTION_CSI = 4,              
	WLAN_HT_ACTION_NONCOMPRESSED_BF = 5, 
	WLAN_HT_ACTION_COMPRESSED_BF = 6,    
	WLAN_HT_ACTION_ASEL_IDX_FEEDBACK = 7,
};

/* Self Protected Action codes */
enum ieee80211_self_protected_actioncode 
{
	WLAN_SP_RESERVED = 0,
	WLAN_SP_MESH_PEERING_OPEN = 1,
	WLAN_SP_MESH_PEERING_CONFIRM = 2,
	WLAN_SP_MESH_PEERING_CLOSE = 3,
	WLAN_SP_MGK_INFORM = 4,
	WLAN_SP_MGK_ACK = 5,
};

/* Security key length */
enum ieee80211_key_len 
{
	WLAN_KEY_LEN_WEP40 = 5,
	WLAN_KEY_LEN_WEP104 = 13,
	WLAN_KEY_LEN_CCMP = 16,
	WLAN_KEY_LEN_TKIP = 32,
	WLAN_KEY_LEN_AES_CMAC = 16,
	WLAN_KEY_LEN_SMS4 = 32,
};

/* Public action codes */
enum ieee80211_pub_actioncode 
{
	WLAN_PUB_ACTION_TDLS_DISCOVER_RES = 14,
};

enum nl80211_band 
{
	NL80211_BAND_2GHZ,
	NL80211_BAND_5GHZ,
};

enum ieee80211_band
{
	IEEE80211_BAND_2GHZ = NL80211_BAND_2GHZ,
	IEEE80211_BAND_5GHZ = NL80211_BAND_5GHZ,
	/* keep last */
	IEEE80211_NUM_BANDS
};

#define ieee80211_hr_chan_to_freq(chan) ieee80211_dsss_chan_to_freq(chan)
#define ieee80211_freq_to_hr_chan(freq) ieee80211_freq_to_dsss_chan(freq)

#define ieee80211_erp_chan_to_freq(chan) ieee80211_hr_chan_to_freq(chan)
#define ieee80211_freq_to_erp_chan(freq) ieee80211_freq_to_hr_chan(freq)

#define IEEE80211_MIN_ACTION_SIZE offsetof(struct ieee80211_mgmt, u.action.u)

#define WLAN_SA_QUERY_TR_ID_LEN         (2)

#define LINK_ID_UNMAPPED		(15)
#define MAX_STA_IN_AP_MODE	    (4)
#define RX_SKB__RX_PACKAGE      (2)

struct ieee80211_hdr 
{
	unsigned short frame_control;
	unsigned short duration_id;
	unsigned char addr1[6];
	unsigned char addr2[6];
	unsigned char addr3[6];
	unsigned short seq_ctrl;
	unsigned char addr4[6];
}atbm_packed;

struct ieee80211_hdr_3addr 
{
	unsigned short frame_control;
	unsigned short duration_id;
	unsigned char addr1[6];
	unsigned char addr2[6];
	unsigned char addr3[6];
	unsigned short seq_ctrl;
}atbm_packed;

struct ieee80211_qos_hdr 
{
	unsigned short frame_control;
	unsigned short duration_id;
	unsigned char addr1[6];
	unsigned char addr2[6];
	unsigned char addr3[6];
	unsigned short seq_ctrl;
	unsigned short qos_ctrl;
}atbm_packed;

struct ieee80211_msrment_ie 
{
	unsigned char token;
	unsigned char mode;
	unsigned char type;
	unsigned char request[ZEROSIZE];
}  atbm_packed ;

struct ieee80211_channel_sw_ie 
{
	unsigned char mode;
	unsigned char new_ch_num;
	unsigned char count;
} atbm_packed;

struct ieee80211_ext_chansw_ie
{
	unsigned char mode;
	unsigned char new_operaring_class;
	unsigned char new_ch_num;
	unsigned char count;
}atbm_packed;

struct ieee80211_sec_chan_offs_ie 
{
	unsigned char sec_chan_offs;
}atbm_packed;

struct ieee80211_channel_sw_packed_ie
{
	struct ieee80211_channel_sw_ie *chan_sw_ie;
	struct ieee80211_ext_chansw_ie *ex_chan_sw_ie;
	struct ieee80211_sec_chan_offs_ie *sec_chan_offs_ie;
}atbm_packed;

struct ieee80211_mgmt 
{
	unsigned short frame_control;
	unsigned short duration;
	unsigned char da[6];
	unsigned char sa[6];
	unsigned char bssid[6];
	unsigned short seq_ctrl;
	union {
		struct {
			unsigned short auth_alg;
			unsigned short auth_transaction;
			unsigned short status_code;
			/* possibly followed by Challenge text */
			unsigned char variable[ZEROSIZE];
		}   atbm_packed auth;
		struct {
			unsigned short reason_code;
		}   atbm_packed deauth;
		struct {
			unsigned short capab_info;
			unsigned short listen_interval;
			/* followed by SSID and Supported rates */
			unsigned char variable[ZEROSIZE];
		}   atbm_packed assoc_req;
		struct {
			unsigned short capab_info;
			unsigned short status_code;
			unsigned short aid;
			/* followed by Supported rates */
			unsigned char variable[ZEROSIZE];
		}  atbm_packed  assoc_resp, reassoc_resp;
		struct {
			unsigned short capab_info;
			unsigned short listen_interval;
			unsigned char current_ap[6];
			/* followed by SSID and Supported rates */
			unsigned char variable[ZEROSIZE];
		}   atbm_packed reassoc_req;
		struct {
			unsigned short reason_code;
		}  atbm_packed  disassoc;
		struct {
			unsigned long long timestamp;
			unsigned short beacon_int;
			unsigned short capab_info;
			/* followed by some of SSID, Supported rates,
			 * FH Params, DS Params, CF Params, IBSS Params, TIM */
			unsigned char variable[ZEROSIZE];
		}  atbm_packed  beacon ;
		struct {
			/* only variable items: SSID, Supported rates */
			unsigned char variable[ZEROSIZE];
		}  atbm_packed  probe_req;
		struct {
			unsigned long long timestamp;
			unsigned short beacon_int;
			unsigned short capab_info;
			/* followed by some of SSID, Supported rates,
			 * FH Params, DS Params, CF Params, IBSS Params */
			unsigned char variable[ZEROSIZE];
		}  atbm_packed  probe_resp;
		struct {
			unsigned char category;
			union {
				struct {
					unsigned char action_code;
					unsigned char dialog_token;
					unsigned char status_code;
					unsigned char variable[ZEROSIZE];
				}   atbm_packed wme_action;
				struct{
					unsigned char action_code;
					unsigned char element_id;
					unsigned char length;
					struct ieee80211_channel_sw_ie sw_elem;
				}atbm_packed  chan_switch;				
				struct{
					unsigned char action_code;
					struct ieee80211_ext_chansw_ie ext_sw_elem;
					unsigned char variable[ZEROSIZE];
				}atbm_packed ext_chan_switch;
				struct{
					unsigned char action_code;
					unsigned char chan_width;
				}atbm_packed notify_chan_width;
				struct{
					unsigned char action_code;
					unsigned char dialog_token;
					unsigned char element_id;
					unsigned char length;
					struct ieee80211_msrment_ie msr_elem;
				}   atbm_packed measurement;
				struct{
					unsigned char action_code;
					unsigned char dialog_token;
					unsigned short capab;
					unsigned short timeout;
					unsigned short start_seq_num;
				}   atbm_packed addba_req;
				struct{
					unsigned char action_code;
					unsigned char dialog_token;
					unsigned short status;
					unsigned short capab;
					unsigned short timeout;
				}   atbm_packed addba_resp;
				struct{
					unsigned char action_code;
					unsigned short params;
					unsigned short reason_code;
				}  atbm_packed  delba;
				struct {
					unsigned char action_code;
					unsigned char variable[ZEROSIZE];
				}   atbm_packed self_prot;
				struct {
					unsigned char action;
					unsigned char trans_id[WLAN_SA_QUERY_TR_ID_LEN];
				}   atbm_packed sa_query;
				struct {
					unsigned char action;
					unsigned char smps_control;
				}   atbm_packed ht_smps;
			}atbm_packed u;
		}  atbm_packed action;
	} atbm_packed u;
}atbm_packed ;

struct ieee8023_hdr 
{
	unsigned char	h_dest[ETH_ALEN];	/* destination eth addr	*/
	unsigned char	h_source[ETH_ALEN];	/* source ether addr	*/
	unsigned short	h_proto;		/* packet type ID field	*/
}atbm_packed;

#define ethhdr ieee8023_hdr

struct ieee80211_mcs_info {
	u8 rx_mask[IEEE80211_HT_MCS_MASK_LEN];
	__le16 rx_highest;
	u8 tx_params;
	u8 reserved[3];
}atbm_packed;


struct ieee80211_ht_cap {
	__le16 cap_info;
	u8 ampdu_params_info;

	/* 16 bytes MCS information */
	struct ieee80211_mcs_info mcs;

	__le16 extended_ht_cap_info;
	__le32 tx_BF_cap_info;
	u8 antenna_selection_info;
}atbm_packed;

struct ieee80211_sta_ht_cap 
{
	u16 cap; /* use IEEE80211_HT_CAP_ */
	bool ht_supported;
	u8 ampdu_factor;
	u8 ampdu_density;
	struct ieee80211_mcs_info mcs;
}atbm_packed;

struct ieee80211_mmie {
	u8 element_id;
	u8 length;
	__le16 key_id;
	u8 sequence_number[6];
	u8 mic[8];
}atbm_packed;

struct ieee80211_pspoll {
	__le16 frame_control;
	__le16 aid;
	u8 bssid[6];
	u8 ta[6];
}atbm_packed;

struct ieee80211_tim_ie {
	u8 dtim_count;
	u8 dtim_period;
	u8 bitmap_ctrl;
	/* variable size: 1 - 251 bytes */
	u8 virtual_map[1];
}atbm_packed;

struct ieee80211_meshconf_ie {
	u8 meshconf_psel;
	u8 meshconf_pmetric;
	u8 meshconf_congest;
	u8 meshconf_synch;
	u8 meshconf_auth;
	u8 meshconf_form;
	u8 meshconf_cap;
}atbm_packed;

struct ieee80211_rann_ie {
	u8 rann_flags;
	u8 rann_hopcount;
	u8 rann_ttl;
	u8 rann_addr[6];
	u32 rann_seq;
	u32 rann_interval;
	u32 rann_metric;
}atbm_packed;

struct ieee80211_ht_info {
	u8 control_chan;
	u8 ht_param;
	__le16 operation_mode;
	__le16 stbc_param;
	u8 basic_set[16];
}atbm_packed;

static const unsigned char rfc1042_header[] = { 0xaa, 0xaa, 0x03, 0x00, 0x00, 0x00 };

static const unsigned char bridge_tunnel_header[] = { 0xaa, 0xaa, 0x03, 0x00, 0x00, 0xf8 };

#define BUILD_BUG_ON(x) \
	if (x){printf("%s %d err\n", __func__, __LINE__); while(1);}

#define BUG_ON(x) \
	if (x){printf("[BUG]%s %d err\n", __func__, __LINE__); while(1);}


#include "atbm_mac80211.h"
#include "debug.h"

typedef signed int ssize_t;

struct net_device *alloc_netdev_mqs(int sizeof_priv, const char *name,
		void (*setup)(struct net_device *),
		unsigned int txqs, unsigned int rxqs);
void netif_carrier_off(struct net_device *dev);
void netif_carrier_on(struct net_device *dev);
void netif_tx_start_all_queues(struct net_device *dev);
void netif_tx_stop_all_queues(struct net_device *dev);

int atbm_config_ak_net_mac(struct net_device *dev);
void atbm_set_ak_ip_info(struct net_device *dev, unsigned int *ipaddr, unsigned int *ipmask, unsigned int *gwaddr);

#define _IOW(x,y,z)
#define _IOR(x,y,z)
#define _IO(x)
#endif // #ifndef ATBM_OS_H

