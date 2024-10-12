#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H


#include "c_def.h"
#include "bt_def.h"


#ifndef AF_BLUETOOTH
#define AF_BLUETOOTH	31
#define PF_BLUETOOTH	AF_BLUETOOTH
#endif

/* Bluetooth versions */
#define BLUETOOTH_VER_1_1	1
#define BLUETOOTH_VER_1_2	2
#define BLUETOOTH_VER_2_0	3

/* Reserv for core and drivers use */
//#define BT_SKB_RESERVE	8
#define BT_SKB_RESERVE	16

/*Only one skb buff for bt sending except hci, Optek*/
#define MAX_BT_SEND_SKB	0x2000

#define BTPROTO_L2CAP	0
#define BTPROTO_HCI	1
#define BTPROTO_SCO	2
#define BTPROTO_RFCOMM	3
#define BTPROTO_BNEP	4
#define BTPROTO_CMTP	5
#define BTPROTO_HIDP	6
#define BTPROTO_AVDTP	7

#define SOL_HCI		0
#define SOL_L2CAP	6
#define SOL_SCO		17
#define SOL_RFCOMM	18

#define BT_SECURITY	4
struct bt_security {
	__u8 level;
	__u8 key_size;
};
#define BT_SECURITY_SDP		0
#define BT_SECURITY_LOW		1
#define BT_SECURITY_MEDIUM	2
#define BT_SECURITY_HIGH	3

#define BT_DEFER_SETUP	7

#define BT_FLUSHABLE	8

#define BT_FLUSHABLE_OFF	0
#define BT_FLUSHABLE_ON		1

#define BT_POWER	9
struct bt_power {
	__u8 force_active;
};
#define BT_POWER_FORCE_ACTIVE_OFF 0
#define BT_POWER_FORCE_ACTIVE_ON  1

#define BT_CHANNEL_POLICY	10

/* BR/EDR only (default policy)
 *   AMP controllers cannot be used.
 *   Channel move requests from the remote device are denied.
 *   If the L2CAP channel is currently using AMP, move the channel to BR/EDR.
 */
#define BT_CHANNEL_POLICY_BREDR_ONLY		0

/* BR/EDR Preferred
 *   Allow use of AMP controllers.
 *   If the L2CAP channel is currently on AMP, move it to BR/EDR.
 *   Channel move requests from the remote device are allowed.
 */
#define BT_CHANNEL_POLICY_BREDR_PREFERRED	1

/* AMP Preferred
 *   Allow use of AMP controllers
 *   If the L2CAP channel is currently on BR/EDR and AMP controller
 *     resources are available, initiate a channel move to AMP.
 *   Channel move requests from the remote device are allowed.
 *   If the L2CAP socket has not been connected yet, try to create
 *     and configure the channel directly on an AMP controller rather
 *     than BR/EDR.
 */
#define BT_CHANNEL_POLICY_AMP_PREFERRED		2

#if 0
__printf(1, 2)
int bt_info(const char *fmt, ...);
__printf(1, 2)
int bt_err(const char *fmt, ...);


#define BT_INFO(fmt, ...)	bt_info(fmt "\n", ##__VA_ARGS__)
#define BT_ERR(fmt, ...)	bt_err(fmt "\n", ##__VA_ARGS__)
#define BT_DBG(fmt, ...)	pr_debug(fmt "\n", ##__VA_ARGS__)
#endif

#define BT_INFO			DBG_Printf	//(fmt, ...)	bt_info(fmt "\n", ##__VA_ARGS__)
#define BT_ERR			DBG_Printf	//(fmt, ...)	bt_err(fmt "\n", ##__VA_ARGS__)
#define BT_DBG			DBG_Printf	//(fmt, ...)	pr_debug(fmt "\n", ##__VA_ARGS__)



/* Connection and socket states */
enum {
	BT_CONNECTED = 1, /* Equal to TCP_ESTABLISHED to make net code happy */
	BT_OPEN,
	BT_BOUND,
	BT_LISTEN,
	BT_CONNECT,
	BT_CONNECT2,
	BT_CONFIG,
	BT_DISCONN,
	BT_CLOSED
};

/* If unused will be removed by compiler */
static inline const char *state_to_string(int state)
{
	switch (state) {
	case BT_CONNECTED:
		return "BT_CONNECTED";
	case BT_OPEN:
		return "BT_OPEN";
	case BT_BOUND:
		return "BT_BOUND";
	case BT_LISTEN:
		return "BT_LISTEN";
	case BT_CONNECT:
		return "BT_CONNECT";
	case BT_CONNECT2:
		return "BT_CONNECT2";
	case BT_CONFIG:
		return "BT_CONFIG";
	case BT_DISCONN:
		return "BT_DISCONN";
	case BT_CLOSED:
		return "BT_CLOSED";
	}

	return "invalid state";
}

/* BD Address */
typedef struct {
	__u8 b[6];
} __attribute__((packed)) bdaddr_t;

/* BD Address type */
#define BDADDR_BREDR		0x00
#define BDADDR_LE_PUBLIC	0x01
#define BDADDR_LE_RANDOM	0x02

static inline bool bdaddr_type_is_valid(__u8 type)
{
	switch (type) {
	case BDADDR_BREDR:
	case BDADDR_LE_PUBLIC:
	case BDADDR_LE_RANDOM:
		return true;
	}

	return false;
}

static inline bool bdaddr_type_is_le(__u8 type)
{
	switch (type) {
	case BDADDR_LE_PUBLIC:
	case BDADDR_LE_RANDOM:
		return true;
	}

	return false;
}

#define BDADDR_ANY   (&(bdaddr_t) {{0, 0, 0, 0, 0, 0} })
#define BDADDR_FLASH_INIT   (&(bdaddr_t) {{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF} })

#define BDADDR_LOCAL (&(bdaddr_t) {{0, 0, 0, 0xff, 0xff, 0xff} })

/* Copy, swap, convert BD Address */
static inline int bacmp(const bdaddr_t *ba1, const bdaddr_t *ba2)
{
	return memcmp(ba1, ba2, sizeof(bdaddr_t));
}
static inline void bacpy(bdaddr_t *dst, const bdaddr_t *src)
{
	CFasm_memcpy(dst, src, sizeof(bdaddr_t));
}

void baswap(bdaddr_t *dst, bdaddr_t *src);

/* Common socket structures and functions */

#define bt_sk(__sk) ((struct bt_sock *) __sk)

struct bt_sock {
	struct sock sk;
	bdaddr_t    src;
	bdaddr_t    dst;
	struct list_head accept_q;
	struct sock *parent;
	unsigned long flags;
};

enum {
	BT_SK_DEFER_SETUP,
	BT_SK_SUSPEND,
};

struct bt_sock_list {
	struct hlist_head head;
	rwlock_t          lock;
#ifdef CONFIG_PROC_FS
        int (* custom_seq_show)(struct seq_file *, void *);
#endif
};

int  bt_sock_register(int proto, const struct net_proto_family *ops);
void bt_sock_unregister(int proto);
void bt_sock_link(struct bt_sock_list *l, struct sock *s);
void bt_sock_unlink(struct bt_sock_list *l, struct sock *s);
int  bt_sock_recvmsg(struct kiocb *iocb, struct socket *sock,
				struct msghdr *msg, size_t len, int flags);
int  bt_sock_stream_recvmsg(struct kiocb *iocb, struct socket *sock,
			struct msghdr *msg, size_t len, int flags);
//uint bt_sock_poll(struct file *file, struct socket *sock, poll_table *wait);
int  bt_sock_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg);
int  bt_sock_wait_state(struct sock *sk, int state, unsigned long timeo);

void bt_accept_enqueue(struct sock *parent, struct sock *sk);
void bt_accept_unlink(struct sock *sk);
struct sock *bt_accept_dequeue(struct sock *parent, struct socket *newsock);

/* Skb helpers */
struct l2cap_ctrl {
	unsigned int	sframe:1,
			poll:1,
			final:1,
			fcs:1,
			sar:2,
			super:2;
	__u16		reqseq;
	__u16		txseq;
	__u8		retries;
};

struct hci_dev;

typedef void (*hci_req_complete_t)(struct hci_dev *hdev, u8 status);

struct hci_req_ctrl {
	bool			start;
	u8			event;
	hci_req_complete_t	complete;
};

typedef struct {
	uint8_t data[16];
} uint128_t;

struct bt_skb_cb {
	__u8 pkt_type;
	__u8 incoming;
	__u16 expect;
	__u8 force_active;
	struct l2cap_ctrl control;
	struct hci_req_ctrl req;
};
#define bt_cb(skb) ((struct bt_skb_cb *)((skb)->cb))

#define HCI_SEND_MAX_BYTE	(HCI_MAX_ACL_SIZE+HCI_ACL_HDR_SIZE)
//extern U8 hci_send_buf[];
extern U8 bt_hci_rcv_frame[];

extern struct sk_buff hci_skb[];
extern struct sk_buff hci_rx_skb_s;


#include "debug.h"
#if 0
static inline struct sk_buff *bt_hci_skb_alloc(unsigned int len, gfp_t how)
{
	struct sk_buff *skb;
	//DBG_Assert (len <= HCI_SEND_MAX_BYTE);
		
	memset((void *)&hci_skb,0,sizeof(hci_skb));
	skb = &hci_skb;
		
	skb->head = hci_send_buf;
	skb->data = hci_send_buf;
	skb_reset_tail_pointer(skb);
	//skb->end = skb->tail + size;
	skb->len = 0;
	
	skb_reserve(skb, BT_SKB_RESERVE);


	bt_cb(skb)->incoming  = 0;
	
	return skb;
#if 0
	skb = alloc_skb(len + BT_SKB_RESERVE, how);
	if (skb) {
		skb_reserve(skb, BT_SKB_RESERVE);
		bt_cb(skb)->incoming  = 0;
	}
	return skb;
#endif
}
#endif

struct hci_dev *hci_dev_get(int index);

static inline void *bt_hci_rx_skb_set_buf(struct sk_buff *skb, u8 *buf)
{
	skb->head = buf;
	skb->data = buf;
	skb_reset_tail_pointer(skb);
}

static inline struct sk_buff *bt_hci_rx_skb_alloc(unsigned int len, gfp_t how)
{
	struct sk_buff *skb;
	//DBG_Assert (len <= HCI_SEND_MAX_BYTE);
	
	memset((void *)&hci_rx_skb_s,0,sizeof(hci_rx_skb_s));
	skb = &hci_rx_skb_s;
	
	skb->head = bt_hci_rcv_frame;
	skb->data = bt_hci_rcv_frame;
	skb_reset_tail_pointer(skb);
	//skb->end = skb->tail + size;
	skb->len = 0;

	skb->dev = hci_dev_get(0);
	
	//skb_reserve(skb, BT_SKB_RESERVE);
	
	
	bt_cb(skb)->incoming  = 0;
	
	return skb;
#if 0
	skb = alloc_skb(len + BT_SKB_RESERVE, how);
	if (skb) {
		skb_reserve(skb, BT_SKB_RESERVE);
		bt_cb(skb)->incoming  = 0;
	}
	return skb;
#endif
}

struct sk_buff *bt_hci_skb_alloc(unsigned int len, gfp_t how);
struct sk_buff *bt_send_skb_alloc(unsigned int len, gfp_t how);


static inline struct sk_buff *bt_skb_alloc(unsigned int len, gfp_t how)
{

	struct sk_buff *skb;
	skb = bt_send_skb_alloc(len, GFP_ATOMIC);
	return skb;

#if 0
	skb = alloc_skb(len + BT_SKB_RESERVE, how);
	if (skb) {
		skb_reserve(skb, BT_SKB_RESERVE);
		bt_cb(skb)->incoming  = 0;
	}
	return skb;
#endif
}





static inline struct sk_buff *bt_skb_send_alloc(struct sock *sk,
					unsigned long len, int nb, int *err)
{
	struct sk_buff *skb;

#if 0
	skb = sock_alloc_send_skb(sk, len + BT_SKB_RESERVE, nb, err);
	if (skb) {
		skb_reserve(skb, BT_SKB_RESERVE);
		bt_cb(skb)->incoming  = 0;
	}

	if (!skb && *err)
		return NULL;

	*err = sock_error(sk);
	if (*err)
		goto out;

	if (sk->sk_shutdown) {
		*err = -ECONNRESET;
		goto out;
	}

	return skb;

out:
	kfree_skb(skb);
	return NULL;
#endif
	DBG_Assert(FALSE);
	return NULL;

}

/**
 *	skb_queue_empty - check if a queue is empty
 *	@list: queue head
 *
 *	Returns true if the queue is empty, false otherwise.
 */
static inline int skb_queue_empty(const struct sk_buff_head *list)
{
	return list->next == (struct sk_buff *)list;
}

/**
 *	skb_queue_is_last - check if skb is the last entry in the queue
 *	@list: queue head
 *	@skb: buffer
 *
 *	Returns true if @skb is the last buffer on the list.
 */
static inline bool skb_queue_is_last(const struct sk_buff_head *list,
				     const struct sk_buff *skb)
{
	return skb->next == (struct sk_buff *)list;
}

/**
 *	skb_queue_is_first - check if skb is the first entry in the queue
 *	@list: queue head
 *	@skb: buffer
 *
 *	Returns true if @skb is the first buffer on the list.
 */
static inline bool skb_queue_is_first(const struct sk_buff_head *list,
				      const struct sk_buff *skb)
{
	return skb->prev == (struct sk_buff *)list;
}

/**
 *	skb_queue_next - return the next packet in the queue
 *	@list: queue head
 *	@skb: current buffer
 *
 *	Return the next packet in @list after @skb.  It is only valid to
 *	call this if skb_queue_is_last() evaluates to false.
 */
static inline struct sk_buff *skb_queue_next(const struct sk_buff_head *list,
					     const struct sk_buff *skb)
{
	/* This BUG_ON may seem severe, but if we just return then we
	 * are going to dereference garbage.
	 */
	BUG_ON(skb_queue_is_last(list, skb));
	return skb->next;
}

/**
 *	skb_queue_prev - return the prev packet in the queue
 *	@list: queue head
 *	@skb: current buffer
 *
 *	Return the prev packet in @list before @skb.  It is only valid to
 *	call this if skb_queue_is_first() evaluates to false.
 */
static inline struct sk_buff *skb_queue_prev(const struct sk_buff_head *list,
					     const struct sk_buff *skb)
{
	/* This BUG_ON may seem severe, but if we just return then we
	 * are going to dereference garbage.
	 */
	DBG_Assert(skb_queue_is_first(list, skb));
	return skb->prev;
}

/**
 *	skb_peek - peek at the head of an &sk_buff_head
 *	@list_: list to peek at
 *
 *	Peek an &sk_buff. Unlike most other operations you _MUST_
 *	be careful with this one. A peek leaves the buffer on the
 *	list and someone else may run off with it. You must hold
 *	the appropriate locks or have a private queue to do this.
 *
 *	Returns %NULL for an empty list or a pointer to the head element.
 *	The reference count is not incremented and the reference is therefore
 *	volatile. Use with caution.
 */
static inline struct sk_buff *skb_peek(const struct sk_buff_head *list_)
{
	struct sk_buff *skb = list_->next;

	if (skb == (struct sk_buff *)list_)
		skb = NULL;
	return skb;
}

/*
 * remove sk_buff from list. _Must_ be called atomically, and with
 * the list known..
 */
static inline void skb_unlink(struct sk_buff *skb, struct sk_buff_head *list)
{
	struct sk_buff *next, *prev;

	list->qlen--;
	next	   = skb->next;
	prev	   = skb->prev;
	skb->next  = skb->prev = NULL;
	next->prev = prev;
	prev->next = next;
}

static inline void skb_queue_splice(const struct sk_buff_head *list,
									  struct sk_buff *prev,
									  struct sk_buff *next)
{
	struct sk_buff *first = list->next;
	struct sk_buff *last = list->prev;
	
	first->prev = prev;
	prev->next = first;
	
	last->next = next;
	next->prev = last;
}

/**
 *	skb_queue_splice_tail - join two skb lists, each list being a queue
 *	@list: the new list to add
 *	@head: the place to add it in the first list
 */
static inline void skb_queue_splice_tail(const struct sk_buff_head *list,
					 struct sk_buff_head *head)
{
	if (!skb_queue_empty(list)) {
		skb_queue_splice(list, head->prev, (struct sk_buff *) head);
		head->qlen += list->qlen;
	}
}

/**
 *	skb_queue_splice_tail_init - join two skb lists and reinitialise the emptied list
 *	@list: the new list to add
 *	@head: the place to add it in the first list
 *
 *	Each of the lists is a queue.
 *	The list at @list is reinitialised
 */
static inline void skb_queue_splice_tail_init(struct sk_buff_head *list,
					      struct sk_buff_head *head)
{
	if (!skb_queue_empty(list)) {
		skb_queue_splice(list, head->prev, (struct sk_buff *) head);
		head->qlen += list->qlen;
		skb_queue_head_init(list);
	}
}




/**
 *	__skb_dequeue - remove from the head of the queue
 *	@list: list to dequeue from
 *
 *	Remove the head of the list. This function does not take any locks
 *	so must be used with appropriate locks held only. The head item is
 *	returned or %NULL if the list is empty.
 */
static inline struct sk_buff *skb_dequeue(struct sk_buff_head *list)
{
	struct sk_buff *skb = skb_peek(list);
	if (skb)
		skb_unlink(skb, list);
	return skb;
}
/**
 *	skb_peek_tail - peek at the tail of an &sk_buff_head
 *	@list_: list to peek at
 *
 *	Peek an &sk_buff. Unlike most other operations you _MUST_
 *	be careful with this one. A peek leaves the buffer on the
 *	list and someone else may run off with it. You must hold
 *	the appropriate locks or have a private queue to do this.
 *
 *	Returns %NULL for an empty list or a pointer to the tail element.
 *	The reference count is not incremented and the reference is therefore
 *	volatile. Use with caution.
 */
static inline struct sk_buff *skb_peek_tail(const struct sk_buff_head *list_)
{
	struct sk_buff *skb = list_->prev;

	if (skb == (struct sk_buff *)list_)
		skb = NULL;
	return skb;

}
/**
 *	__skb_dequeue_tail - remove from the tail of the queue
 *	@list: list to dequeue from
 *
 *	Remove the tail of the list. This function does not take any locks
 *	so must be used with appropriate locks held only. The tail item is
 *	returned or %NULL if the list is empty.
 */
static inline struct sk_buff *skb_dequeue_tail(struct sk_buff_head *list)
{
	struct sk_buff *skb = skb_peek_tail(list);
	if (skb)
		__skb_unlink(skb, list);
	return skb;
}

/**
 *	skb_clone	-	duplicate an sk_buff
 *	@skb: buffer to clone
 *	@gfp_mask: allocation priority
 *
 *	Duplicate an &sk_buff. The new one is not owned by a socket. Both
 *	copies share the same packet data but not structure. The new
 *	buffer has a reference count of 1. If the allocation fails the
 *	function returns %NULL otherwise the new buffer is returned.
 *
 *	If this function is called from an interrupt gfp_mask() must be
 *	%GFP_ATOMIC.
 */

static struct sk_buff *skb_clone(struct sk_buff *skb, int gfp_mask)
{
	return skb;

#if 0
	struct sk_buff *n;

	if (skb_orphan_frags(skb, gfp_mask))
		return NULL;

	n = skb + 1;
	if (skb->fclone == SKB_FCLONE_ORIG &&
	    n->fclone == SKB_FCLONE_UNAVAILABLE) {
		atomic_t *fclone_ref = (atomic_t *) (n + 1);
		n->fclone = SKB_FCLONE_CLONE;
		atomic_inc(fclone_ref);
	} else {
		if (skb_pfmemalloc(skb))
			gfp_mask |= __GFP_MEMALLOC;

		n = kmem_cache_alloc(skbuff_head_cache, gfp_mask);
		if (!n)
			return NULL;

		kmemcheck_annotate_bitfield(n, flags1);
		kmemcheck_annotate_bitfield(n, flags2);
		n->fclone = SKB_FCLONE_UNAVAILABLE;
	}

	return __skb_clone(n, skb);
#endif
}

/*
 *	Insert an sk_buff on a list.
 *
 *	The "__skb_xxxx()" functions are the non-atomic ones that
 *	can only be called with interrupts disabled.
 */
static inline void skb_insert(struct sk_buff *newsk,
				struct sk_buff *prev, struct sk_buff *next,
				struct sk_buff_head *list)
{
	newsk->next = next;
	newsk->prev = prev;
	next->prev  = prev->next = newsk;
	list->qlen++;
}

static inline void skb_queue_before(struct sk_buff_head *list,
									  struct sk_buff *next,
									  struct sk_buff *newsk)
{
	skb_insert(newsk, next->prev, next, list);
}

/**
 *	__skb_queue_after - queue a buffer at the list head
 *	@list: list to use
 *	@prev: place after this buffer
 *	@newsk: buffer to queue
 *
 *	Queue a buffer int the middle of a list. This function takes no locks
 *	and you must therefore hold required locks before calling it.
 *
 *	A buffer cannot be placed on two lists at the same time.
 */
static inline void skb_queue_after(struct sk_buff_head *list,
				     struct sk_buff *prev,
				     struct sk_buff *newsk)
{
	skb_insert(newsk, prev, prev->next, list);
}

/**
 *	__skb_queue_head - queue a buffer at the list head
 *	@list: list to use
 *	@newsk: buffer to queue
 *
 *	Queue a buffer at the start of a list. This function takes no locks
 *	and you must therefore hold required locks before calling it.
 *
 *	A buffer cannot be placed on two lists at the same time.
 */
static inline void skb_queue_head(struct sk_buff_head *list,
				    struct sk_buff *newsk)
{
	skb_queue_after(list, (struct sk_buff *)list, newsk);
}

/**
 *	__skb_queue_tail - queue a buffer at the list tail
 *	@list: list to use
 *	@newsk: buffer to queue
 *
 *	Queue a buffer at the end of a list. This function takes no locks
 *	and you must therefore hold required locks before calling it.
 *
 *	A buffer cannot be placed on two lists at the same time.
 */
static inline void skb_queue_tail(struct sk_buff_head *list,
				   struct sk_buff *newsk)
{
	skb_queue_before(list, (struct sk_buff *)list, newsk);
}

/**
 *	skb_queue_len	- get queue length
 *	@list_: list to measure
 *
 *	Return the length of an &sk_buff queue.
 */
static inline __u32 skb_queue_len(const struct sk_buff_head *list_)
{
	return list_->qlen;
}


static inline unsigned int skb_headlen(const struct sk_buff *skb)
{
	return skb->len - skb->data_len;
}


static inline void skb_reset_transport_header(struct sk_buff *skb)
{
	skb->transport_header = skb->data - skb->head;
}

static inline unsigned char *skb_transport_header(const struct sk_buff *skb)
{
	return skb->head + skb->transport_header;
}

int bt_to_errno(__u16 code);

extern int hci_sock_init(void);
extern void hci_sock_cleanup(void);

extern int bt_sysfs_init(void);
extern void bt_sysfs_cleanup(void);

//extern int  bt_procfs_init(struct net *net, const char *name,
//			   struct bt_sock_list* sk_list,
//			   int (* seq_show)(struct seq_file *, void *));
//extern void bt_procfs_cleanup(struct net *net, const char *name);

//extern struct dentry *bt_debugfs;

int l2cap_init(void);
void l2cap_exit(void);

int sco_init(void);
void sco_exit(void);

void bt_sock_reclassify_lock(struct sock *sk, int proto);

#endif /* __BLUETOOTH_H */
