#ifndef _ATT_H_
#define _ATT_H_

#include "le_byteshift.h"
#include "uuid.h"

/* Attribute Protocol Opcodes */
#define ATT_OP_ERROR			0x01
#define ATT_OP_MTU_REQ			0x02
#define ATT_OP_MTU_RESP			0x03
#define ATT_OP_FIND_INFO_REQ		0x04
#define ATT_OP_FIND_INFO_RESP		0x05
#define ATT_OP_FIND_BY_TYPE_REQ		0x06
#define ATT_OP_FIND_BY_TYPE_RESP	0x07
#define ATT_OP_READ_BY_TYPE_REQ		0x08
#define ATT_OP_READ_BY_TYPE_RESP	0x09
#define ATT_OP_READ_REQ			0x0A
#define ATT_OP_READ_RESP		0x0B
#define ATT_OP_READ_BLOB_REQ		0x0C
#define ATT_OP_READ_BLOB_RESP		0x0D
#define ATT_OP_READ_MULTI_REQ		0x0E
#define ATT_OP_READ_MULTI_RESP		0x0F
#define ATT_OP_READ_BY_GROUP_REQ	0x10
#define ATT_OP_READ_BY_GROUP_RESP	0x11
#define ATT_OP_WRITE_REQ		0x12
#define ATT_OP_WRITE_RESP		0x13
#define ATT_OP_WRITE_CMD		0x52
#define ATT_OP_PREP_WRITE_REQ		0x16
#define ATT_OP_PREP_WRITE_RESP		0x17
#define ATT_OP_EXEC_WRITE_REQ		0x18
#define ATT_OP_EXEC_WRITE_RESP		0x19
#define ATT_OP_HANDLE_NOTIFY		0x1B
#define ATT_OP_HANDLE_IND		0x1D
#define ATT_OP_HANDLE_CNF		0x1E
#define ATT_OP_SIGNED_WRITE_CMD		0xD2

/* Error codes for Error response PDU */
#define ATT_ECODE_INVALID_HANDLE		0x01
#define ATT_ECODE_READ_NOT_PERM			0x02
#define ATT_ECODE_WRITE_NOT_PERM		0x03
#define ATT_ECODE_INVALID_PDU			0x04
#define ATT_ECODE_AUTHENTICATION		0x05
#define ATT_ECODE_REQ_NOT_SUPP			0x06
#define ATT_ECODE_INVALID_OFFSET		0x07
#define ATT_ECODE_AUTHORIZATION			0x08
#define ATT_ECODE_PREP_QUEUE_FULL		0x09
#define ATT_ECODE_ATTR_NOT_FOUND		0x0A
#define ATT_ECODE_ATTR_NOT_LONG			0x0B
#define ATT_ECODE_INSUFF_ENCR_KEY_SIZE		0x0C
#define ATT_ECODE_INVAL_ATTR_VALUE_LEN		0x0D
#define ATT_ECODE_UNLIKELY			0x0E
#define ATT_ECODE_INSUFF_ENC			0x0F
#define ATT_ECODE_UNSUPP_GRP_TYPE		0x10
#define ATT_ECODE_INSUFF_RESOURCES		0x11
/* Application error */
#define ATT_ECODE_IO				0x80
#define ATT_ECODE_TIMEOUT			0x81
#define ATT_ECODE_ABORTED			0x82

/* Characteristic Property bit field */
#define ATT_CHAR_PROPER_BROADCAST		0x01
#define ATT_CHAR_PROPER_READ			0x02
#define ATT_CHAR_PROPER_WRITE_WITHOUT_RESP	0x04
#define ATT_CHAR_PROPER_WRITE			0x08
#define ATT_CHAR_PROPER_NOTIFY			0x10
#define ATT_CHAR_PROPER_INDICATE		0x20
#define ATT_CHAR_PROPER_AUTH			0x40
#define ATT_CHAR_PROPER_EXT_PROPER		0x80

#define ATT_MAX_VALUE_LEN			512
#define ATT_DEFAULT_L2CAP_MTU			48
#define ATT_DEFAULT_LE_MTU			23

#define ATT_CID					4
#define ATT_PSM					31

/* Flags for Execute Write Request Operation */
#define ATT_CANCEL_ALL_PREP_WRITES              0x00
#define ATT_WRITE_ALL_PREP_WRITES               0x01

/* Find Information Response Formats */
#define ATT_FIND_INFO_RESP_FMT_16BIT		0x01
#define ATT_FIND_INFO_RESP_FMT_128BIT		0x02

#define ATT_BT_UUID_SIZE BT_UUID16


/*********************************************************************
 * Error Response: Error Code
 */

/** @defgroup ATT_ERR_CODE_DEFINES ATT Error Codes
 * @{
 */

#define ATT_ERR_INVALID_HANDLE           0x01 //!< Attribute handle value given was not valid on this attribute server
#define ATT_ERR_READ_NOT_PERMITTED       0x02 //!< Attribute cannot be read
#define ATT_ERR_WRITE_NOT_PERMITTED      0x03 //!< Attribute cannot be written
#define ATT_ERR_INVALID_PDU              0x04 //!< The attribute PDU was invalid
#define ATT_ERR_INSUFFICIENT_AUTHEN      0x05 //!< The attribute requires authentication before it can be read or written
#define ATT_ERR_UNSUPPORTED_REQ          0x06 //!< Attribute server doesn't support the request received from the attribute client
#define ATT_ERR_INVALID_OFFSET           0x07 //!< Offset specified was past the end of the attribute
#define ATT_ERR_INSUFFICIENT_AUTHOR      0x08 //!< The attribute requires an authorization before it can be read or written
#define ATT_ERR_PREPARE_QUEUE_FULL       0x09 //!< Too many prepare writes have been queued
#define ATT_ERR_ATTR_NOT_FOUND           0x0a //!< No attribute found within the given attribute handle range
#define ATT_ERR_ATTR_NOT_LONG            0x0b //!< Attribute cannot be read or written using the Read Blob Request or Prepare Write Request
#define ATT_ERR_INSUFFICIENT_KEY_SIZE    0x0c //!< The Encryption Key Size used for encrypting this link is insufficient
#define ATT_ERR_INVALID_VALUE_SIZE       0x0d //!< The attribute value length is invalid for the operation
#define ATT_ERR_UNLIKELY                 0x0e //!< The attribute request that was requested has encountered an error that was very unlikely, and therefore could not be completed as requested
#define ATT_ERR_INSUFFICIENT_ENCRYPT     0x0f //!< The attribute requires encryption before it can be read or written
#define ATT_ERR_UNSUPPORTED_GRP_TYPE     0x10 //!< The attribute type is not a supported grouping attribute as defined by a higher layer specification
#define ATT_ERR_INSUFFICIENT_RESOURCES   0x11 //!< Insufficient Resources to complete the request


struct att_data_list {
	uint16_t num;
	uint16_t len;
	uint8_t **data;
};

struct att_range {
	uint16_t start;
	uint16_t end;
};


/* These functions do byte conversion */
static inline uint8_t att_get_u8(const void *ptr)
{
	const uint8_t *u8_ptr = ptr;
	return *u8_ptr;
}

static inline uint16_t att_get_u16(const void *ptr)
{
	const uint16_t *u16_ptr = ptr;
	return bt_get_le16(u16_ptr);
}

static inline uint32_t att_get_u32(const void *ptr)
{
	const uint32_t *u32_ptr = ptr;
	return bt_get_le32(u32_ptr);
}

static inline uint128_t att_get_u128(const void *ptr)
{
	DBG_Assert(FALSE);

	const uint128_t *u128_ptr = ptr;
	uint128_t dst;
	
	btoh128(u128_ptr, &dst);
	
	return dst;

}

static inline void att_put_u8(uint8_t src, void *dst)
{
	uint8_t *uint8_ptr = dst;
	//bt_put_unaligned(src, (uint8_t *) dst);
	*uint8_ptr = src;
}

static inline void att_put_u16(uint16_t src, void *dst)
{
	bt_put_le16(src, dst);
}

static inline void att_put_u32(uint32_t src, void *dst)
{
	bt_put_le32(src, dst);
}

static inline void att_put_u128(uint128_t src, void *dst)
{
	DBG_Assert(FALSE);

	uint128_t *d128 = dst;
	
	htob128(&src, d128);
	
}

static inline void att_put_uuid16(bt_uuid_t src, void *dst)
{

	att_put_u16(src.value.u16_val, dst);

}


static inline void att_put_uuid128(bt_uuid_t src, void *dst)
{
	DBG_Assert(FALSE);
	//att_put_u128(src.value.u128, dst);
}

static inline void att_put_uuid(bt_uuid_t src, void *dst)
{
	if (src.type == BT_UUID16)
		att_put_uuid16(src, dst);
	else
		att_put_uuid128(src, dst);
}

static inline bt_uuid_t att_get_uuid16(const void *ptr)
{
	bt_uuid_t uuid;
	
	bt_uuid16_create(&uuid, att_get_u16(ptr));
	
	return uuid;

}

static inline bt_uuid_t att_get_uuid128(const void *ptr)
{
	DBG_Assert(FALSE);

	bt_uuid_t uuid;
	uint128_t value;
	
	value  = att_get_u128(ptr);
	bt_uuid128_create(&uuid, value);
	
	return uuid;
}

struct att_data_list *att_data_list_alloc(uint16_t num, uint16_t len);
void att_data_list_free(struct att_data_list *list);

const char *att_ecode2str(uint8_t status);

uint16_t enc_read_by_grp_req(uint16_t start, uint16_t end, bt_uuid_t *uuid,
							 uint8_t *pdu, size_t len);
uint16_t dec_read_by_grp_req(const uint8_t *pdu, size_t len, uint16_t *start,
							 uint16_t *end, bt_uuid_t *uuid);
uint16_t enc_read_by_grp_resp(struct att_data_list *list, uint8_t *pdu,
							  size_t len);
uint16_t enc_find_by_type_req(uint16_t start, uint16_t end, bt_uuid_t *uuid,
							  const uint8_t *value, size_t vlen, uint8_t *pdu,
							  size_t len);
uint16_t dec_find_by_type_req(const uint8_t *pdu, size_t len, uint16_t *start,
							  uint16_t *end, bt_uuid_t *uuid, uint8_t *value, size_t *vlen);
uint16_t enc_find_by_type_resp(GSList *ranges, uint8_t *pdu, size_t len);
GSList *dec_find_by_type_resp(const uint8_t *pdu, size_t len);
struct att_data_list *dec_read_by_grp_resp(const uint8_t *pdu, size_t len);
uint16_t enc_read_by_type_req(uint16_t start, uint16_t end, bt_uuid_t *uuid,
							  uint8_t *pdu, size_t len);
uint16_t dec_read_by_type_req(const uint8_t *pdu, size_t len, uint16_t *start,
							  uint16_t *end, bt_uuid_t *uuid);
							  
uint16_t enc_read_by_type_resp(struct att_data_list *list, uint8_t *pdu,
							   size_t len);
uint16_t enc_write_cmd(uint16_t handle, const uint8_t *value, size_t vlen,
					   uint8_t *pdu, size_t len);
uint16_t dec_write_cmd(const uint8_t *pdu, size_t len, uint16_t *handle,
					   uint8_t **pValue, size_t *pLen);
struct att_data_list *dec_read_by_type_resp(const uint8_t *pdu, size_t len);
uint16_t enc_write_req(uint16_t handle, const uint8_t *value, size_t vlen,
					   uint8_t *pdu, size_t len);
uint16_t dec_write_req(const uint8_t *pdu, size_t len, uint16_t *handle,
					   uint8_t **pValue, size_t *pLen);
uint16_t enc_write_resp(uint8_t *pdu);
uint16_t dec_write_resp(const uint8_t *pdu, size_t len);
uint16_t enc_read_req(uint16_t handle, uint8_t *pdu, size_t len);
uint16_t enc_read_blob_req(uint16_t handle, uint16_t offset, uint8_t *pdu,
						   size_t len);
uint16_t dec_read_req(const uint8_t *pdu, size_t len, uint16_t *handle);
uint16_t dec_read_blob_req(const uint8_t *pdu, size_t len, uint16_t *handle,
						   uint16_t *offset);
uint16_t enc_read_resp(uint8_t *value, size_t vlen, uint8_t *pdu, size_t len);
uint16_t enc_read_blob_resp(uint8_t *value, size_t vlen, uint16_t offset,
							uint8_t *pdu, size_t len);
ssize_t dec_read_resp(const uint8_t *pdu, size_t len, uint8_t *value,
					  size_t vlen);
uint16_t enc_error_resp(uint8_t opcode, uint16_t handle, uint8_t status,
						uint8_t *pdu, size_t len);
uint16_t enc_find_info_req(uint16_t start, uint16_t end, uint8_t *pdu,
						   size_t len);
uint16_t dec_find_info_req(const uint8_t *pdu, size_t len, uint16_t *start,
						   uint16_t *end);
uint16_t enc_find_info_resp(uint8_t format, struct att_data_list *list,
							uint8_t *pdu, size_t len);
struct att_data_list *dec_find_info_resp(const uint8_t *pdu, size_t len,
										 uint8_t *format);
uint16_t enc_notification(uint16_t handle, uint8_t *value, size_t vlen,
						  uint8_t *pdu, size_t len);
uint16_t enc_indication(uint16_t handle, uint8_t *value, size_t vlen,
						uint8_t *pdu, size_t len);
uint16_t dec_indication(const uint8_t *pdu, size_t len, uint16_t *handle,
						uint8_t *value, size_t vlen);
uint16_t enc_confirmation(uint8_t *pdu, size_t len);

uint16_t enc_mtu_req(uint16_t mtu, uint8_t *pdu, size_t len);
uint16_t dec_mtu_req(const uint8_t *pdu, size_t len, uint16_t *mtu);
uint16_t enc_mtu_resp(uint16_t mtu, uint8_t *pdu, size_t len);
uint16_t dec_mtu_resp(const uint8_t *pdu, size_t len, uint16_t *mtu);

uint16_t enc_prep_write_req(uint16_t handle, uint16_t offset,
							const uint8_t *value, size_t vlen,
							uint8_t *pdu, size_t len);
uint16_t dec_prep_write_req(const uint8_t *pdu, size_t len, uint16_t *handle,
							uint16_t *offset, uint8_t *value, size_t *vlen);
uint16_t enc_prep_write_resp(uint16_t handle, uint16_t offset,
							 const uint8_t *value, size_t vlen,
							 uint8_t *pdu, size_t len);
uint16_t dec_prep_write_resp(const uint8_t *pdu, size_t len, uint16_t *handle,
							 uint16_t *offset, uint8_t *value,
							 size_t *vlen);
uint16_t enc_exec_write_req(uint8_t flags, uint8_t *pdu, size_t len);
uint16_t dec_exec_write_req(const uint8_t *pdu, size_t len, uint8_t *flags);
uint16_t enc_exec_write_resp(uint8_t *pdu);
uint16_t dec_exec_write_resp(const uint8_t *pdu, size_t len);

//added by Optek



#endif


