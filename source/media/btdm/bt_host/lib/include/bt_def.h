#ifndef _BT_DEF_H
#define _BT_DEF_H



#define uint8_t U8
#define uint16_t U16
#define uint32_t U32
#define uint64_t U64

#define int8_t I8
#define int16_t I16
#define int32_t I32
#define int64_t I64


typedef unsigned short __kernel_sa_family_t;
typedef __kernel_sa_family_t	sa_family_t;

typedef char __s8;
typedef unsigned char __u8;

typedef short __s16;
typedef unsigned short __u16;

typedef int __s32;
typedef unsigned int __u32;
typedef unsigned int uint;



typedef long long __s64;
typedef unsigned long long __u64;

typedef int         ptrdiff_t;
typedef U32			dma_addr_t;

typedef __u16 __le16;
typedef __u16 __be16;
typedef __u32 __le32;
typedef __u32 __be32;
typedef __u64 __le64;
typedef __u64 __be64;

typedef __u16 __sum16;
typedef __u32 __wsum;

typedef unsigned int size_t;
typedef unsigned int sk_buff_data_t;



struct work_struct;
typedef void (*work_func_t)(struct work_struct *work);

struct work_struct {
	//atomic_long_t data;
	//struct list_head entry;
	work_func_t func;
#ifdef CONFIG_LOCKDEP
	//struct lockdep_map lockdep_map;
#endif
};



struct sk_buff_head {
	/* These two members must be first. */
	struct sk_buff	*next;
	struct sk_buff	*prev;
	
	__u32		qlen;
	//spinlock_t	lock;
};

#define cpu_to_le64 __cpu_to_le64
#define le64_to_cpu __le64_to_cpu
#define cpu_to_le32 __cpu_to_le32
#define le32_to_cpu __le32_to_cpu
#define cpu_to_le16 __cpu_to_le16
#define le16_to_cpu __le16_to_cpu
#define cpu_to_be64 __cpu_to_be64
#define be64_to_cpu __be64_to_cpu
#define cpu_to_be32 __cpu_to_be32
#define be32_to_cpu __be32_to_cpu
#define cpu_to_be16 __cpu_to_be16
#define be16_to_cpu __be16_to_cpu
#define cpu_to_le64p __cpu_to_le64p
#define le64_to_cpup __le64_to_cpup
#define cpu_to_le32p __cpu_to_le32p
#define le32_to_cpup __le32_to_cpup
#define cpu_to_le16p __cpu_to_le16p
#define le16_to_cpup __le16_to_cpup
#define cpu_to_be64p __cpu_to_be64p
#define be64_to_cpup __be64_to_cpup
#define cpu_to_be32p __cpu_to_be32p
#define be32_to_cpup __be32_to_cpup
#define cpu_to_be16p __cpu_to_be16p
#define be16_to_cpup __be16_to_cpup
#define cpu_to_le64s __cpu_to_le64s
#define le64_to_cpus __le64_to_cpus
#define cpu_to_le32s __cpu_to_le32s
#define le32_to_cpus __le32_to_cpus
#define cpu_to_le16s __cpu_to_le16s
#define le16_to_cpus __le16_to_cpus
#define cpu_to_be64s __cpu_to_be64s
#define be64_to_cpus __be64_to_cpus
#define cpu_to_be32s __cpu_to_be32s
#define be32_to_cpus __be32_to_cpus
#define cpu_to_be16s __cpu_to_be16s
#define be16_to_cpus __be16_to_cpus

#define __constant_htonl(x) ((__force __be32)___constant_swab32((x)))
#define __constant_ntohl(x) ___constant_swab32((__force __be32)(x))
#define __constant_htons(x) ((__force __be16)___constant_swab16((x)))
#define __constant_ntohs(x) ___constant_swab16((__force __be16)(x))
#define __constant_cpu_to_le64(x) ((__force __le64)(__u64)(x))
#define __constant_le64_to_cpu(x) ((__force __u64)(__le64)(x))
#define __constant_cpu_to_le32(x) ((__force __le32)(__u32)(x))
#define __constant_le32_to_cpu(x) ((__force __u32)(__le32)(x))
#define __constant_cpu_to_le16(x) ((__force __le16)(__u16)(x))
#define __constant_le16_to_cpu(x) ((__force __u16)(__le16)(x))
#define __constant_cpu_to_be64(x) ((__force __be64)___constant_swab64((x)))
#define __constant_be64_to_cpu(x) ___constant_swab64((__force __u64)(__be64)(x))
#define __constant_cpu_to_be32(x) ((__force __be32)___constant_swab32((x)))
#define __constant_be32_to_cpu(x) ___constant_swab32((__force __u32)(__be32)(x))
#define __constant_cpu_to_be16(x) ((__force __be16)___constant_swab16((x)))
#define __constant_be16_to_cpu(x) ___constant_swab16((__force __u16)(__be16)(x))
#define __cpu_to_le64(x) ((__force __le64)(__u64)(x))
#define __le64_to_cpu(x) ((__force __u64)(__le64)(x))
#define __cpu_to_le32(x) ((__force __le32)(__u32)(x))
#define __le32_to_cpu(x) ((__force __u32)(__le32)(x))
#define __cpu_to_le16(x) ((__force __le16)(__u16)(x))
#define __le16_to_cpu(x) ((__force __u16)(__le16)(x))
#define __cpu_to_be64(x) ((__force __be64)__swab64((x)))
#define __be64_to_cpu(x) __swab64((__force __u64)(__be64)(x))
#define __cpu_to_be32(x) ((__force __be32)__swab32((x)))
#define __be32_to_cpu(x) __swab32((__force __u32)(__be32)(x))
#define __cpu_to_be16(x) ((__force __be16)__swab16((x)))
#define __be16_to_cpu(x) __swab16((__force __u16)(__be16)(x))

/*
 * WARNING: non atomic version.
 */
static inline void
set_bit(unsigned long nr, volatile void * addr)
{
	int *m = ((int *) addr) + (nr >> 5);

	*m |= 1 << (nr & 31);
}


#define bool    int
#define s8	char
#define __user


typedef struct _GSLIST GList;
typedef struct _GSLIST GSList;
struct _GSLIST {
	GSList *next;
	void *data;
};

/* Error Codes */

#define EPERM           1
#define ENOENT          2
#define ESRCH           3
#define EINTR           4
#define EIO             5
#define ENXIO           6
#define E2BIG           7
#define ENOEXEC         8
#define EBADF           9
#define ECHILD          10
#define EAGAIN          11
#define ENOMEM          12
#define EACCES          13
#define EFAULT          14
#define EBUSY           16
#define EEXIST          17
#define EXDEV           18
#define ENODEV          19
#define ENOTDIR         20
#define EISDIR          21
#define EINVAL          22
#define ENFILE          23
#define EMFILE          24
#define ENOTTY          25
#define EFBIG           27
#define ENOSPC          28
#define ESPIPE          29
#define EROFS           30
#define EMLINK          31
#define EPIPE           32
#define EDOM            33
#define ERANGE          34
#define EDEADLK         36
#define ENAMETOOLONG    38
#define ENOLCK          39
#define ENOSYS          40
#define ENOTEMPTY       41
#define EILSEQ          42

#define	EWOULDBLOCK	EAGAIN	/* Operation would block */
#define	EINPROGRESS	36	/* Operation now in progress */
#define	EALREADY	37	/* Operation already in progress */
#define	ENOTSOCK	38	/* Socket operation on non-socket */
#define	EDESTADDRREQ	39	/* Destination address required */
#define	EMSGSIZE	40	/* Message too long */
#define	EPROTOTYPE	41	/* Protocol wrong type for socket */
#define	ENOPROTOOPT	42	/* Protocol not available */
#define	EPROTONOSUPPORT	43	/* Protocol not supported */
#define	ESOCKTNOSUPPORT	44	/* Socket type not supported */
#define	EOPNOTSUPP	45	/* Op not supported on transport endpoint */
#define	EPFNOSUPPORT	46	/* Protocol family not supported */
#define	EAFNOSUPPORT	47	/* Address family not supported by protocol */
#define	EADDRINUSE	48	/* Address already in use */
#define	EADDRNOTAVAIL	49	/* Cannot assign requested address */
#define	ENETDOWN	50	/* Network is down */
#define	ENETUNREACH	51	/* Network is unreachable */
#define	ENETRESET	52	/* Net dropped connection because of reset */
#define	ECONNABORTED	53	/* Software caused connection abort */
#define	ECONNRESET	54	/* Connection reset by peer */
#define	ENOBUFS		55	/* No buffer space available */
#define	EISCONN		56	/* Transport endpoint is already connected */
#define	ENOTCONN	57	/* Transport endpoint is not connected */
#define	ESHUTDOWN	58	/* No send after transport endpoint shutdown */
#define	ETOOMANYREFS	59	/* Too many references: cannot splice */
#define	ETIMEDOUT	60	/* Connection timed out */
#define	ECONNREFUSED	61	/* Connection refused */
#define	ELOOP		62	/* Too many symbolic links encountered */
#define	ENAMETOOLONG	63	/* File name too long */
#define	EHOSTDOWN	64	/* Host is down */
#define	EHOSTUNREACH	65	/* No route to host */
#define	ENOTEMPTY	66	/* Directory not empty */
#define EPROCLIM        67      /* SUNOS: Too many processes */
#define	EUSERS		68	/* Too many users */
#define	EDQUOT		69	/* Quota exceeded */
#define	ESTALE		70	/* Stale NFS file handle */
#define	EREMOTE		71	/* Object is remote */
#define	ENOSTR		72	/* Device not a stream */
#define	ETIME		73	/* Timer expired */
#define	ENOSR		74	/* Out of streams resources */
#define	ENOMSG		75	/* No message of desired type */
#define	EBADMSG		76	/* Not a data message */
#define	EIDRM		77	/* Identifier removed */
#define	EDEADLK		78	/* Resource deadlock would occur */
#define	ENOLCK		79	/* No record locks available */
#define	ENONET		80	/* Machine is not on the network */
#define ERREMOTE        81      /* SunOS: Too many lvls of remote in path */
#define	ENOLINK		82	/* Link has been severed */
#define	EADV		83	/* Advertise error */
#define	ESRMNT		84	/* Srmount error */
#define	ECOMM		85      /* Communication error on send */
#define	EPROTO		86	/* Protocol error */
#define	EMULTIHOP	87	/* Multihop attempted */
#define	EDOTDOT		88	/* RFS specific error */
#define	EREMCHG		89	/* Remote address changed */
#define	ENOSYS		90	/* Function not implemented */

/* The rest have no SunOS equivalent. */
#define	ESTRPIPE	91	/* Streams pipe error */
#define	EOVERFLOW	92	/* Value too large for defined data type */
#define	EBADFD		93	/* File descriptor in bad state */
#define	ECHRNG		94	/* Channel number out of range */
#define	EL2NSYNC	95	/* Level 2 not synchronized */
#define	EL3HLT		96	/* Level 3 halted */
#define	EL3RST		97	/* Level 3 reset */
#define	ELNRNG		98	/* Link number out of range */
#define	EUNATCH		99	/* Protocol driver not attached */
#define	ENOCSI		100	/* No CSI structure available */
#define	EL2HLT		101	/* Level 2 halted */
#define	EBADE		102	/* Invalid exchange */
#define	EBADR		103	/* Invalid request descriptor */
#define	EXFULL		104	/* Exchange full */
#define	ENOANO		105	/* No anode */
#define	EBADRQC		106	/* Invalid request code */
#define	EBADSLT		107	/* Invalid slot */
#define	EDEADLOCK	108	/* File locking deadlock error */
#define	EBFONT		109	/* Bad font file format */
#define	ELIBEXEC	110	/* Cannot exec a shared library directly */
#define	ENODATA		111	/* No data available */
#define	ELIBBAD		112	/* Accessing a corrupted shared library */
#define	ENOPKG		113	/* Package not installed */
#define	ELIBACC		114	/* Can not access a needed shared library */
#define	ENOTUNIQ	115	/* Name not unique on network */
#define	ERESTART	116	/* Interrupted syscall should be restarted */
#define	EUCLEAN		117	/* Structure needs cleaning */
#define	ENOTNAM		118	/* Not a XENIX named type file */
#define	ENAVAIL		119	/* No XENIX semaphores available */
#define	EISNAM		120	/* Is a named type file */
#define	EREMOTEIO	121	/* Remote I/O error */
#define	EILSEQ		122	/* Illegal byte sequence */
#define	ELIBMAX		123	/* Atmpt to link in too many shared libs */
#define	ELIBSCN		124	/* .lib section in a.out corrupted */

#define	ENOMEDIUM	125	/* No medium found */
#define	EMEDIUMTYPE	126	/* Wrong medium type */
#define	ECANCELED	127	/* Operation Cancelled */
#define	ENOKEY		128	/* Required key not available */
#define	EKEYEXPIRED	129	/* Key has expired */
#define	EKEYREVOKED	130	/* Key has been revoked */
#define	EKEYREJECTED	131	/* Key was rejected by service */

/* for robust mutexes */
#define	EOWNERDEAD	132	/* Owner died */
#define	ENOTRECOVERABLE	133	/* State not recoverable */

#define	ERFKILL		134	/* Operation not possible due to RF-kill */

#define EHWPOISON	135	/* Memory page has hardware error */

#define ERESTARTSYS	512
#define ERESTARTNOINTR	513
#define ERESTARTNOHAND	514	/* restart if no handler.. */
#define ENOIOCTLCMD	515	/* No ioctl command */
#define ERESTART_RESTARTBLOCK 516 /* restart by calling sys_restart_syscall */
#define EPROBE_DEFER	517	/* Driver requests probe retry */
#define EOPENSTALE	518	/* open found a stale dentry */

/* Defined for the NFSv3 protocol */
#define EBADHANDLE	521	/* Illegal NFS file handle */
#define ENOTSYNC	522	/* Update synchronization mismatch */
#define EBADCOOKIE	523	/* Cookie is stale */
#define ENOTSUPP	524	/* Operation is not supported */
#define ETOOSMALL	525	/* Buffer or request is too small */
#define ESERVERFAULT	526	/* An untranslatable error occurred */
#define EBADTYPE	527	/* Type not supported by server */
#define EJUKEBOX	528	/* Request initiated, but will not complete before timeout */
#define EIOCBQUEUED	529	/* iocb queued, will get completion event */

/*
 * Support EDEADLOCK for compatibiity with older MS-C versions.
 */
#define EDEADLOCK       EDEADLK

# define __force




//for BT HCI

//from net/sock.h
struct sock {
		unsigned long 		sk_flags;
		int sk_state;
};

//from linux/types.h
struct list_head {
	struct list_head *next, *prev;
};

struct hlist_head {
	struct hlist_node *first;
};

struct hlist_node {
	struct hlist_node *next, **pprev;
};

//
typedef struct {
	volatile unsigned int lock;
} arch_rwlock_t;

//from linux/rwlock_types.h
typedef struct {
	arch_rwlock_t raw_lock;
#ifdef CONFIG_GENERIC_LOCKBREAK
	unsigned int break_lock;
#endif
#ifdef CONFIG_DEBUG_SPINLOCK
	unsigned int magic, owner_cpu;
	void *owner;
#endif
#ifdef CONFIG_DEBUG_LOCK_ALLOC
	struct lockdep_map dep_map;
#endif
} rwlock_t;


struct net_proto_family {
	int		family;
	//int		(*create)(struct net *net, struct socket *sock,
	//	int protocol, int kern);
	//struct module	*owner;
};


struct kiocb {
	//atomic_t		ki_users;

	//struct file		*ki_filp;
	//struct kioctx		*ki_ctx;	/* NULL for sync ops */
	//kiocb_cancel_fn		*ki_cancel;
	//void			(*ki_dtor)(struct kiocb *);

	//union {
	//	void __user		*user;
	//	struct task_struct	*tsk;
	//} ki_obj;

	//__u64			ki_user_data;	/* user's data for completion */
	//loff_t			ki_pos;

	//void			*private;
	/* State that we remember to be able to restart/retry  */
	unsigned short		ki_opcode;
	//size_t			ki_nbytes; 	/* copy of iocb->aio_nbytes */
	//char 			__user *ki_buf;	/* remaining iocb->aio_buf */
	//size_t			ki_left; 	/* remaining bytes */
	//struct iovec		ki_inline_vec;	/* inline vector */
 	//struct iovec		*ki_iovec;
 	//unsigned long		ki_nr_segs;
 	//unsigned long		ki_cur_seg;

	//struct list_head	ki_list;	/* the aio core uses this
	//					 * for cancellation */

	/*
	 * If the aio_resfd field of the userspace iocb is not zero,
	 * this is the underlying eventfd context to deliver events to.
	 */
	//struct eventfd_ctx	*ki_eventfd;
};


struct msghdr {
	void	*	msg_name;	/* Socket name			*/
	int		msg_namelen;	/* Length of name		*/
	//struct iovec *	msg_iov;	/* Data blocks			*/
	//__kernel_size_t	msg_iovlen;	/* Number of blocks		*/
	//void 	*	msg_control;	/* Per protocol magic (eg BSD file descriptor passing) */
	//__kernel_size_t	msg_controllen;	/* Length of cmsg list */
	//unsigned int	msg_flags;
};

struct socket {
	//socket_state		state;
	
	//kmemcheck_bitfield_begin(type);
	short			type;
	//kmemcheck_bitfield_end(type);
	
	unsigned long		flags;
	
	//struct socket_wq __rcu	*wq;
	
	//struct file		*file;
	struct sock		*sk;
	//const struct proto_ops	*ops;
};

typedef struct poll_table_struct {
	//poll_queue_proc _qproc;
	unsigned long _key;
} poll_table;


enum {
	GFP_KERNEL,
		GFP_ATOMIC,
		__GFP_HIGHMEM,
		__GFP_HIGH

};

#define  _gfp_t int;

struct sk_buff {
	/* These two members must be first. */
	struct sk_buff		*next;
	struct sk_buff		*prev;

	//struct net_device	*dev;
	void *dev;
	
	unsigned int		len,data_len;



	sk_buff_data_t		tail;
	sk_buff_data_t		end;
	unsigned char		*head,
		*data;


	/*
	 * This is the control buffer. It is free to use for every
	 * layer. Please put your private variables there. If you
	 * want to keep them across layers you have to do a skb_clone()
	 * first. This is owned by whoever has the skb queued ATM.
	 */
	char			cb[48];

	__u32			priority;

	sk_buff_data_t		transport_header;


#if 0
	ktime_t			tstamp;

	struct sock		*sk;
	struct net_device	*dev;

	/*
	 * This is the control buffer. It is free to use for every
	 * layer. Please put your private variables there. If you
	 * want to keep them across layers you have to do a skb_clone()
	 * first. This is owned by whoever has the skb queued ATM.
	 */
	char			cb[48] __aligned(8);

	unsigned long		_skb_refdst;
#ifdef CONFIG_XFRM
	struct	sec_path	*sp;
#endif
	unsigned int		len,
				data_len;
	__u16			mac_len,
				hdr_len;
	union {
		__wsum		csum;
		struct {
			__u16	csum_start;
			__u16	csum_offset;
		};
	};
	__u32			priority;
	kmemcheck_bitfield_begin(flags1);
	__u8			local_df:1,
				cloned:1,
				ip_summed:2,
				nohdr:1,
				nfctinfo:3;
	__u8			pkt_type:3,
				fclone:2,
				ipvs_property:1,
				peeked:1,
				nf_trace:1;
	kmemcheck_bitfield_end(flags1);
	__be16			protocol;

	void			(*destructor)(struct sk_buff *skb);
#if defined(CONFIG_NF_CONNTRACK) || defined(CONFIG_NF_CONNTRACK_MODULE)
	struct nf_conntrack	*nfct;
#endif
#ifdef NET_SKBUFF_NF_DEFRAG_NEEDED
	struct sk_buff		*nfct_reasm;
#endif
#ifdef CONFIG_BRIDGE_NETFILTER
	struct nf_bridge_info	*nf_bridge;
#endif

	int			skb_iif;

	__u32			rxhash;

	__be16			vlan_proto;
	__u16			vlan_tci;

#ifdef CONFIG_NET_SCHED
	__u16			tc_index;	/* traffic control index */
#ifdef CONFIG_NET_CLS_ACT
	__u16			tc_verd;	/* traffic control verdict */
#endif
#endif

	__u16			queue_mapping;
	kmemcheck_bitfield_begin(flags2);
#ifdef CONFIG_IPV6_NDISC_NODETYPE
	__u8			ndisc_nodetype:2;
#endif
	__u8			pfmemalloc:1;
	__u8			ooo_okay:1;
	__u8			l4_rxhash:1;
	__u8			wifi_acked_valid:1;
	__u8			wifi_acked:1;
	__u8			no_fcs:1;
	__u8			head_frag:1;
	/* Encapsulation protocol and NIC drivers should use
	 * this flag to indicate to each other if the skb contains
	 * encapsulated packet or not and maybe use the inner packet
	 * headers if needed
	 */
	__u8			encapsulation:1;
	/* 7/9 bit hole (depending on ndisc_nodetype presence) */
	kmemcheck_bitfield_end(flags2);

#ifdef CONFIG_NET_DMA
	dma_cookie_t		dma_cookie;
#endif
#ifdef CONFIG_NETWORK_SECMARK
	__u32			secmark;
#endif
	union {
		__u32		mark;
		__u32		dropcount;
		__u32		reserved_tailroom;
	};

	sk_buff_data_t		inner_transport_header;
	sk_buff_data_t		inner_network_header;
	sk_buff_data_t		inner_mac_header;
	sk_buff_data_t		transport_header;
	sk_buff_data_t		network_header;
	sk_buff_data_t		mac_header;
	/* These elements must be at the end, see alloc_skb() for details.  */
	sk_buff_data_t		tail;
	sk_buff_data_t		end;
	unsigned char		*head,
				*data;
	unsigned int		truesize;
	atomic_t		users;
#endif
};

//from skbuff.h
static inline unsigned char *skb_tail_pointer(const struct sk_buff *skb)
{
	return skb->tail;
}

static inline void skb_reset_tail_pointer(struct sk_buff *skb)
{
	skb->tail = skb->data;
}

static inline void skb_set_tail_pointer(struct sk_buff *skb, const int offset)
{
	skb->tail = skb->data + offset;
}

static inline unsigned char *skb_put(struct sk_buff *skb, unsigned int len)
{
	unsigned char *tmp = skb_tail_pointer(skb);
	//SKB_LINEAR_ASSERT(skb);
	skb->tail += len;
	skb->len  += len;
	return tmp;
}

static inline unsigned char *skb_push(struct sk_buff *skb, unsigned int len)
{
	skb->data -= len;
	skb->len  += len;
	return skb->data;
}

static inline unsigned char *skb_pull(struct sk_buff *skb, unsigned int len)
{
	skb->len -= len;
	//BUG_ON(skb->len < skb->data_len);
	return skb->data += len;
}

static inline void skb_reserve(struct sk_buff *skb, int len)
{
	skb->data += len;
	skb->tail += len;
}


static inline void skb_queue_head_init(struct sk_buff_head *list)
{
	list->prev = list->next = (struct sk_buff *)list;
	list->qlen = 0;
}


typedef unsigned gfp_t;

//from linux/mutex.h
typedef struct {
	int counter;
} atomic_t;


struct kref {
	atomic_t refcount;
};



struct mutex {
	/* 1: unlocked, 0: locked, negative: locked, possible waiters */
	atomic_t		count;
#if 0
	spinlock_t		wait_lock;
	struct list_head	wait_list;
#if defined(CONFIG_DEBUG_MUTEXES) || defined(CONFIG_SMP)
	struct task_struct	*owner;
#endif
#ifdef CONFIG_MUTEX_SPIN_ON_OWNER
	void			*spin_mlock;	/* Spinner MCS lock */
#endif
#ifdef CONFIG_DEBUG_MUTEXES
	const char 		*name;
	void			*magic;
#endif
#ifdef CONFIG_DEBUG_LOCK_ALLOC
	struct lockdep_map	dep_map;
#endif
#endif
};

struct device {

struct mutex		mutex;

#if 0
	struct device		*parent;

	struct device_private	*p;

	struct kobject kobj;
	const char		*init_name; /* initial name of the device */
	const struct device_type *type;

	struct mutex		mutex;	/* mutex to synchronize calls to
					 * its driver.
					 */

	struct bus_type	*bus;		/* type of bus device is on */
	struct device_driver *driver;	/* which driver has allocated this
					   device */
	void		*platform_data;	/* Platform specific data, device
					   core doesn't touch it */
	struct dev_pm_info	power;
	struct dev_pm_domain	*pm_domain;

#ifdef CONFIG_PINCTRL
	struct dev_pin_info	*pins;
#endif

#ifdef CONFIG_NUMA
	int		numa_node;	/* NUMA node this device is close to */
#endif
	u64		*dma_mask;	/* dma mask (if dma'able device) */
	u64		coherent_dma_mask;/* Like dma_mask, but for
					     alloc_coherent mappings as
					     not all hardware supports
					     64 bit addresses for consistent
					     allocations such descriptors. */

	struct device_dma_parameters *dma_parms;

	struct list_head	dma_pools;	/* dma pools (if dma'ble) */

	struct dma_coherent_mem	*dma_mem; /* internal for coherent mem
					     override */
#ifdef CONFIG_CMA
	struct cma *cma_area;		/* contiguous memory area for dma
					   allocations */
#endif
	/* arch specific additions */
	struct dev_archdata	archdata;

	struct device_node	*of_node; /* associated device tree node */
	struct acpi_dev_node	acpi_node; /* associated ACPI device node */

	dev_t			devt;	/* dev_t, creates the sysfs "dev" */
	u32			id;	/* device instance */

	spinlock_t		devres_lock;
	struct list_head	devres_head;

	struct klist_node	knode_class;
	struct class		*class;
	const struct attribute_group **groups;	/* optional groups */

	void	(*release)(struct device *dev);
	struct iommu_group	*iommu_group;
#endif

};



/*
 * WARNING: non atomic version.
 */
static __inline__ void
clear_bit(unsigned long nr, volatile void * addr)
{
	int *m = ((int *) addr) + (nr >> 5);

	*m &= ~(1 << (nr & 31));
}

/*
 * WARNING: non atomic version.
 */
static __inline__ void
change_bit(unsigned long nr, volatile void * addr)
{
	int *m = ((int *) addr) + (nr >> 5);

	*m ^= 1 << (nr & 31);
}

static inline int
test_bit(int nr, const volatile void * addr)
{
	return (1UL & (((const int *) addr)[nr >> 5] >> (nr & 31))) != 0UL;
}
/*
*
* WARNING: non atomic version.
*/
static inline int
test_and_clear_bit(unsigned long nr, volatile void * addr)
{
	unsigned long mask = 1 << (nr & 0x1f);
	int *m = ((int *) addr) + (nr >> 5);
	int old = *m;
	
	*m = old & ~mask;
	return (old & mask) != 0;
}

static __inline__ int
test_and_change_bit(unsigned long nr, volatile void * addr)
{
	unsigned long mask = 1 << (nr & 0x1f);
	int *m = ((int *) addr) + (nr >> 5);
	int old = *m;
	
	*m = old ^ mask;
	return (old & mask) != 0;
}

/*
 * WARNING: non atomic version.
 */
static inline int
test_and_set_bit(unsigned long nr, volatile void * addr)
{
	unsigned long mask = 1 << (nr & 0x1f);
	int *m = ((int *) addr) + (nr >> 5);
	int old = *m;

	*m = old | mask;
	return (old & mask) != 0;
}



#define HZ		100

#define PAGE_SIZE 0x400

#define ATOMIC_INIT(i)	{ (i) }

#define __init

typedef unsigned int           __kernel_size_t;
typedef int                    __kernel_ssize_t;

typedef __kernel_ssize_t	ssize_t;



/*
 * ..and if you can't take the strict
 * types, you can specify one yourself.
 *
 * Or not use min/max/clamp at all, of course.
 */
#define min_t(type, x, y) ({			\
	type __min1 = (x);			\
	type __min2 = (y);			\
	__min1 < __min2 ? __min1: __min2; })

#define max_t(type, x, y) ({			\
	type __max1 = (x);			\
	type __max2 = (y);			\
	__max1 > __max2 ? __max1: __max2; })

#define BITS_PER_LONG 32
#define BIT(nr)			(1UL << (nr))
#define BIT_MASK(nr)		(1UL << ((nr) % BITS_PER_LONG))
#define BIT_WORD(nr)		((nr) / BITS_PER_LONG)
#define BITS_PER_BYTE		8

#define atomic_read(v)		(*(volatile int *)&(v)->counter)
#define atomic_set(v, i)	(((v)->counter) = i)
//#define atomic_dec(v)		(((v)->counter)--)
//#define atomic_inc(v)		(((v)->counter)++)
//#define atomic_dec_and_test(v) (--((v)->counter) == 0)


#define __exit
#define __init

#ifndef max
#define max(a,b)	(a>b? a : b)
#endif

#ifndef min
#define min(a,b)	(a<b? a : b)
#endif

static inline void *  ERR_PTR(long error)
{
	return (void *) error;
}

static inline long  PTR_ERR(const void *ptr)
{
	return (long) ptr;
}

static inline long  IS_ERR(const void *ptr)
{
	DBG_Assert(FALSE);

	return 0;	//IS_ERR_VALUE((unsigned long)ptr);
}


enum sock_type {
	SOCK_DGRAM	= 1,
	SOCK_STREAM	= 2,
	SOCK_RAW	= 3,
	SOCK_RDM	= 4,
	SOCK_SEQPACKET	= 5,
	SOCK_DCCP	= 6,
	SOCK_PACKET	= 10,
};

#define SOCK_MAX (SOCK_PACKET + 1)
/* Mask which covers at least up to SOCK_MASK-1.  The
 *  * remaining bits are used as flags. */
#define SOCK_TYPE_MASK 0xf

#if 0

#define get_unaligned_le(ptr) (typeof(*(ptr)))({			\
	sizeof(*(ptr)) == 1 ? *(ptr) :					\
	(sizeof(*(ptr)) == 2 ? get_unaligned_le16((ptr)) :		\
	(sizeof(*(ptr)) == 4 ? get_unaligned_le32((ptr)) :		\
	(sizeof(*(ptr)) == 8 ? get_unaligned_le64((ptr)) :	\
	__bad_unaligned_access_size())));			\
	})



#define put_unaligned_le(val, ptr) ({					\
	void *__gu_p = (ptr);						\
	switch (sizeof(*(ptr))) {					\
	case 1:								\
	*(u8 *)__gu_p = (__force u8)(val);			\
	break;							\
	case 2:								\
	put_unaligned_le16((__force u16)(val), __gu_p);		\
	break;							\
	case 4:								\
	put_unaligned_le32((__force u32)(val), __gu_p);		\
	break;							\
	case 8:								\
	put_unaligned_le64((__force u64)(val), __gu_p);		\
	break;							\
	default:							\
	__bad_unaligned_access_size();				\
	break;							\
	}								\
(void)0; })
#endif

#define put_unaligned_le(val, ptr) ({					\
	void *__gu_p = (ptr);						\
	switch (sizeof(*(ptr))) {					\
	case 1:								\
	*(u8 *)__gu_p = (__force u8)(val);			\
	break;							\
	case 2:								\
	put_unaligned_le16((__force u16)(val), __gu_p);		\
	break;							\
	case 4:								\
	put_unaligned_le32((__force u32)(val), __gu_p);		\
	break;							\
	case 8:								\
	put_unaligned_le64((__force u64)(val), __gu_p);		\
	break;							\
	}								\
	(void)0; })


#define put_unaligned put_unaligned_le



#define BUG_ON
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define write_unlock
#define write_lock

#define kfree_skb
#define mutex_lock
#define mutex_unlock
#define read_lock
#define read_unlock

#define lock_sock
#define release_sock

//#define skb_put
//#define skb_push
//#define skb_pull
/*
static inline unsigned char *skb_put(struct sk_buff *skb, unsigned int len)
{
	unsigned char *tmp = skb->data;;
	skb->data += len;
	skb->len  += len;
	return tmp;
}
*/


#define kfree
#define proto_unregister
#define proto_register

#define sco_conn_lock
#define sco_conn_unlock
#define __net_timestamp

#define sock_put
#define sock_hold

//#define list_del
//#define list_add

#define sock_register

#define __skb_queue_purge
#define __skb_queue_tail
#define __skb_queue_purge
#define __skb_queue_head_init
#define skb_orphan
//#define skb_queue_head_init
#define skb_trim
//#define atomic_inc
#define copy_to_user	CFasm_memcpy
#define copy_from_user CFasm_memcpy

#define synchronize_rcu
#define skb_queue_purge
//#define list_del_rcu
//#define list_add_rcu


//hci
#define hci_req_lock
#define hci_req_unlock
#define read_lock
#define read_unlock
#define mutex_lock
#define mutex_unlock

#define enable_hs 0x0				//not support 3.0+HS
//#define enable_hs 0x1
#define disable_ertm 0x1


//#define BT_INI_INQUIRY			//at power on, enter to inquiry
//#define BT_INT_CONNECT			//at power on, try to connect to other bt device

//#define BT_W_EEPROM

#define MAX_MULTILINK		5

#define BT_INQUIRY_NO_DISPLAY

extern u16 top_sco_handle;

#define BT_TX_NO_FLUSH

//from atomic.h
extern u8 g_relink_state;

enum {
	RELINK_IDLE = 0,
		RELINK_START,
		RELINK_REQUEST_CANCEL
};

#define atomic_read(v)	(*(volatile int *)&(v)->counter)

#define atomic_set(v, i) (((v)->counter) = (i))

/**
 * atomic_sub_return - subtract integer from atomic variable
 * @i: integer value to subtract
 * @v: pointer of type atomic_t
 *
 * Atomically subtracts @i from @v and returns the result
 */
static inline int atomic_sub_return(int i, atomic_t *v)
{
	unsigned long flags;
	int temp;

	//raw_local_irq_save(flags); /* Don't trace it in an irqsoff handler */
	temp = v->counter;
	temp -= i;
	v->counter = temp;
	//raw_local_irq_restore(flags);

	return temp;
}

/**
 * atomic_add_return - add integer to atomic variable
 * @i: integer value to add
 * @v: pointer of type atomic_t
 *
 * Atomically adds @i to @v and returns the result
 */
static inline int atomic_add_return(int i, atomic_t *v)
{
	unsigned long flags;
	int temp;

	//raw_local_irq_save(flags); /* Don't trace it in an irqsoff handler */
	temp = v->counter;
	temp += i;
	v->counter = temp;
	//raw_local_irq_restore(flags);

	return temp;
}

static inline int atomic_add_negative(int i, atomic_t *v)
{
	return atomic_add_return(i, v) < 0;
}

static inline void atomic_add(int i, atomic_t *v)
{
	atomic_add_return(i, v);
}

static inline void atomic_sub(int i, atomic_t *v)
{
	atomic_sub_return(i, v);
}

static inline void atomic_inc(atomic_t *v)
{
	atomic_add_return(1, v);
}

static inline void atomic_dec(atomic_t *v)
{
	atomic_sub_return(1, v);
}

#define atomic_dec_return(v)		atomic_sub_return(1, (v))
#define atomic_inc_return(v)		atomic_add_return(1, (v))

#define atomic_sub_and_test(i, v)	(atomic_sub_return((i), (v)) == 0)
#define atomic_dec_and_test(v)		(atomic_dec_return(v) == 0)
#define atomic_inc_and_test(v)		(atomic_inc_return(v) == 0)


//from kref.h
/**
 * kref_init - initialize object.
 * @kref: object in question.
 */
static inline void kref_init(struct kref *kref)
{
	atomic_set(&kref->refcount, 1);
}

/**
 * kref_get - increment refcount for object.
 * @kref: object.
 */
static inline void kref_get(struct kref *kref)
{
	/* If refcount was 0 before incrementing then we have a race
	 * condition when this kref is freeing by some other thread right now.
	 * In this case one should use kref_get_unless_zero()
	 */
	atomic_inc_return(&kref->refcount);
	//atomic_inc(&kref->refcount);
}

/**
 * kref_sub - subtract a number of refcounts for object.
 * @kref: object.
 * @count: Number of recounts to subtract.
 * @release: pointer to the function that will clean up the object when the
 *	     last reference to the object is released.
 *	     This pointer is required, and it is not acceptable to pass kfree
 *	     in as this function.  If the caller does pass kfree to this
 *	     function, you will be publicly mocked mercilessly by the kref
 *	     maintainer, and anyone else who happens to notice it.  You have
 *	     been warned.
 *
 * Subtract @count from the refcount, and if 0, call release().
 * Return 1 if the object was removed, otherwise return 0.  Beware, if this
 * function returns 0, you still can not count on the kref from remaining in
 * memory.  Only use the return value if you want to see if the kref is now
 * gone, not present.
 */
static inline int kref_sub(struct kref *kref, unsigned int count,
	     void (*release)(struct kref *kref))
{

	if (atomic_sub_and_test((int) count, &kref->refcount)) {
		release(kref);
		return 1;
	}
	return 0;
}

/**
 * kref_put - decrement refcount for object.
 * @kref: object.
 * @release: pointer to the function that will clean up the object when the
 *	     last reference to the object is released.
 *	     This pointer is required, and it is not acceptable to pass kfree
 *	     in as this function.  If the caller does pass kfree to this
 *	     function, you will be publicly mocked mercilessly by the kref
 *	     maintainer, and anyone else who happens to notice it.  You have
 *	     been warned.
 *
 * Decrement the refcount, and if 0, call release().
 * Return 1 if the object was removed, otherwise return 0.  Beware, if this
 * function returns 0, you still can not count on the kref from remaining in
 * memory.  Only use the return value if you want to see if the kref is now
 * gone, not present.
 */
static inline int kref_put(struct kref *kref, void (*release)(struct kref *kref))
{
	return kref_sub(kref, 1, release);
}


extern u8 avrcp_req;
#define ACRCP_REQ_MASK				0x01
#define AVDTP_REQ_MASK				0x02

#define ACRCP_AVDTP_REQ_BOTH		0x03
#define ACRCP_AVDTP_REQ_NONE		0x00


#endif /* BT_DEF_H */
