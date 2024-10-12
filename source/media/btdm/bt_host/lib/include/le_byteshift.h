#ifndef _TOOLS_LE_BYTESHIFT_H
#define _TOOLS_LE_BYTESHIFT_H

//#include <linux/types.h>
#include "be_byteshift.h"

static inline __u16 __get_unaligned_le16(const __u8 *p)
{
	return p[0] | p[1] << 8;
}

static inline __u32 __get_unaligned_le32(const __u8 *p)
{
	return p[0] | p[1] << 8 | p[2] << 16 | p[3] << 24;
}

static inline __u64 __get_unaligned_le64(const __u8 *p)
{
	return (__u64)__get_unaligned_le32(p + 4) << 32 |
	       __get_unaligned_le32(p);
}

static inline void __put_unaligned_le16(__u16 val, __u8 *p)
{
	*p++ = (__u8) val;
	*p++ = (__u8) (val >> 8);
}

static inline void __put_unaligned_le32(__u32 val, __u8 *p)
{
	__put_unaligned_le16(val >> 16, p + 2);
	__put_unaligned_le16(val, p);
}

static inline void __put_unaligned_le64(__u64 val, __u8 *p)
{
	__put_unaligned_le32(val >> 32, p + 4);
	__put_unaligned_le32(val, p);
}

static inline __u16 bswap_16( __u16 val)
{
	__u8 *p = (__u8 *) &val;
	return (p[0] << 8 | p[1]);
}

static inline __u32 bswap_32( __u32 val)
{
	__u8 *p = (__u8 *) &val;
	return (p[0] << 24 | p[1] << 16 | p[2] << 8 | p[3]);	
}

static inline __u64 bswap_64( __u64 val)
{
	__u8 *p = (__u8 *) &val;
	return (p[0] << 56 | p[1] << 48 | p[2] << 40 | p[3] << 32 | p[4] << 24 | p[5] << 16 | p[6] << 8 | p[7]);		
}

static inline __u16 get_unaligned_le16(const void *p)
{
	return __get_unaligned_le16((const __u8 *)p);
}

static inline __u32 get_unaligned_le32(const void *p)
{
	return __get_unaligned_le32((const __u8 *)p);
}

static inline __u64 get_unaligned_le64(const void *p)
{
	return __get_unaligned_le64((const __u8 *)p);
}

static inline void put_unaligned_le16(__u16 val, void *p)
{
	__put_unaligned_le16(val, p);
}

static inline void put_unaligned_le32(__u32 val, void *p)
{
	__put_unaligned_le32(val, p);
}

static inline void put_unaligned_le64(__u64 val, void *p)
{
	__put_unaligned_le64(val, p);
}


static inline uint64_t bt_get_le64(const void *ptr)
{
	return __get_unaligned_le64((const uint64_t *) ptr);
}

static inline uint64_t bt_get_be64(const void *ptr)
{
	return __get_unaligned_be64((const uint64_t *) ptr);
}

static inline uint32_t bt_get_le32(const void *ptr)
{
	return __get_unaligned_le32((const uint32_t *) ptr);
}

static inline uint32_t bt_get_be32(const void *ptr)
{
	return __get_unaligned_be32((const uint32_t *) ptr);
}

static inline uint16_t bt_get_le16(const void *ptr)
{
	return __get_unaligned_le16((const uint16_t *) ptr);
}

static inline uint16_t bt_get_be16(const void *ptr)
{
	return __get_unaligned_be16((const uint16_t *) ptr);
}

static inline void bt_put_le64(uint64_t val, const void *ptr)
{
	__put_unaligned_le64(val, (uint64_t *) ptr);
}

static inline void bt_put_be64(uint64_t val, const void *ptr)
{
	__put_unaligned_be64(val, (uint64_t *) ptr);
}

static inline void bt_put_le32(uint32_t val, const void *ptr)
{
	__put_unaligned_le32(val, (uint32_t *) ptr);
}

static inline void bt_put_be32(uint32_t val, const void *ptr)
{
	__put_unaligned_be32(val, (uint32_t *) ptr);
}

static inline void bt_put_le16(uint16_t val, const void *ptr)
{
	__put_unaligned_le16(val, (uint16_t *) ptr);
}

static inline void bt_put_be16(uint16_t val, const void *ptr)
{
	__put_unaligned_be16(val, (uint16_t *) ptr);
}

#if 1

#define __swab16	bswap_16
#define __swab32	bswap_32
#define __swab64	bswap_64

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

#endif


#define ___htonl(x) __cpu_to_be32(x)
#define ___htons(x) __cpu_to_be16(x)
#define ___ntohl(x) __be32_to_cpu(x)
#define ___ntohs(x) __be16_to_cpu(x)


#define htonl(x) ___htonl(x)
#define ntohl(x) ___ntohl(x)
#define htons(x) ___htons(x)
#define ntohs(x) ___ntohs(x)

/*
static inline void ntoh128(const uint128_t *src, uint128_t *dst)
{
	int i;
	
	for (i = 0; i < 16; i++)
		dst->data[15 - i] = src->data[i];
}

static inline void btoh128(const uint128_t *src, uint128_t *dst)
{
	CFasm_memcpy(dst, src, sizeof(uint128_t));
}
*/

#include "bluetooth.h"

static inline uint64_t ntoh64(uint64_t n)
{
	uint64_t h;
	uint64_t tmp = ntohl(n & 0x00000000ffffffff);
	
	h = ntohl(n >> 32);
	h |= tmp << 32;
	
	return h;
}

static inline void ntoh128(const uint128_t *src, uint128_t *dst)
{
	int i;
	
	for (i = 0; i < 16; i++)
		dst->data[15 - i] = src->data[i];
}

static inline void btoh128(const uint128_t *src, uint128_t *dst)
{
	CFasm_memcpy(dst, src, sizeof(uint128_t));
}


#define hton64(x)     ntoh64(x)

#define hton128(x, y) ntoh128(x, y)
#define htob128(x, y) btoh128(x, y)

#endif /* _TOOLS_LE_BYTESHIFT_H */
