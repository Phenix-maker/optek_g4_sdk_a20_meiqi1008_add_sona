#ifndef _GATT_H_
#define _GATT_H_

#include "blueZ/sdp.h"

/* GATT Profile Attribute types */
#define GATT_PRIM_SVC_UUID		0x2800
#define GATT_SND_SVC_UUID		0x2801
#define GATT_INCLUDE_UUID		0x2802
#define GATT_CHARAC_UUID		0x2803

/* GATT Characteristic Types */
#define GATT_CHARAC_DEVICE_NAME			0x2A00
#define GATT_CHARAC_APPEARANCE			0x2A01
#define GATT_CHARAC_PERIPHERAL_PRIV_FLAG	0x2A02
#define GATT_CHARAC_RECONNECTION_ADDRESS	0x2A03
#define GATT_CHARAC_PERIPHERAL_PREF_CONN	0x2A04
#define GATT_CHARAC_SERVICE_CHANGED		0x2A05

/* GATT Characteristic Descriptors */
#define GATT_CHARAC_EXT_PROPER_UUID	0x2900
#define GATT_CHARAC_USER_DESC_UUID	0x2901
#define GATT_CLIENT_CHARAC_CFG_UUID	0x2902
#define GATT_SERVER_CHARAC_CFG_UUID	0x2903
#define GATT_CHARAC_FMT_UUID		0x2904
#define GATT_CHARAC_AGREG_FMT_UUID	0x2905
#define GATT_CHARAC_VALID_RANGE_UUID	0x2906
#define GATT_EXTERNAL_REPORT_REFERENCE	0x2907
#define GATT_REPORT_REFERENCE		0x2908

/* Client Characteristic Configuration bit field */
#define GATT_CLIENT_CHARAC_CFG_NOTIF_BIT	0x0001
#define GATT_CLIENT_CHARAC_CFG_IND_BIT		0x0002

//added by Optek

/** @defgroup GATT_PROP_BITMAPS_DEFINES GATT Characteristic Properties Bit Fields
 * @{
 */

#define GATT_PROP_BCAST                  0x01 //!< Permits broadcasts of the Characteristic Value
#define GATT_PROP_READ                   0x02 //!< Permits reads of the Characteristic Value
#define GATT_PROP_WRITE_NO_RSP           0x04 //!< Permits writes of the Characteristic Value without response
#define GATT_PROP_WRITE                  0x08 //!< Permits writes of the Characteristic Value with response
#define GATT_PROP_NOTIFY                 0x10 //!< Permits notifications of a Characteristic Value without acknowledgement
#define GATT_PROP_INDICATE               0x20 //!< Permits indications of a Characteristic Value with acknowledgement
#define GATT_PROP_AUTHEN                 0x40 //!< Permits signed writes to the Characteristic Value
#define GATT_PROP_EXTENDED               0x80 //!< Additional characteristic properties are defined in the Characteristic Extended Properties Descriptor

/** @} End GATT_PROP_BITMAPS_DEFINES */

/** @defgroup GATT_EXT_PROP_BITMAPS_DEFINES GATT Characteristic Extended Properties Bit Fields
 * @{
 */

#define GATT_EXT_PROP_RELIABLE_WRITE     0x0001 //!< Permits reliable writes of the Characteristic Value
#define GATT_EXT_PROP_WRITABLE_AUX       0x0002 //!< Permits writes to the characteristic descriptor

/** @} End GATT_EXT_PROP_BITMAPS_DEFINES */

/** @defgroup GATT_CLIENT_CFG_BITMAPS_DEFINES GATT Client Characteristic Configuration Bit Fields
 * @{
 */

#define GATT_CLIENT_CFG_NOTIFY           0x0001 //!< The Characteristic Value shall be notified
#define GATT_CLIENT_CFG_INDICATE         0x0002 //!< The Characteristic Value shall be indicated

/** @} End GATT_CLIENT_CFG_BITMAPS_DEFINES */

/** @defgroup GATT_SERV_CFG_BITMAPS_DEFINES GATT Server Characteristic Configuration Bit Fields
 * @{
 */

#define GATT_SERV_CFG_BCAST              0x0001 //!< The Characteristic Value shall be broadcast when the server is in the broadcast procedure if advertising data resources are available

/** @} End GATT_SERV_CFG_BITMAPS_DEFINES */

#define GATT_CFG_NO_OPERATION            0x0000 // No operation

/** @defgroup GATT_FORMAT_TYPES_DEFINES GATT Characteristic Format Types
 * @{
 */

#define GATT_FORMAT_BOOL                 0x01 //!< Unsigned 1 bit; 0 = false, 1 = true
#define GATT_FORMAT_2BIT                 0x02 //!< Unsigned 2 bit integer
#define GATT_FORMAT_NIBBLE               0x03 //!< Unsigned 4 bit integer
#define GATT_FORMAT_UINT8                0x04 //!< Unsigned 8 bit integer
#define GATT_FORMAT_UINT12               0x05 //!< Unsigned 12 bit integer
#define GATT_FORMAT_UINT16               0x06 //!< Unsigned 16 bit integer
#define GATT_FORMAT_UINT24               0x07 //!< Unsigned 24 bit integer
#define GATT_FORMAT_UINT32               0x08 //!< Unsigned 32 bit integer
#define GATT_FORMAT_UINT48               0x09 //!< Unsigned 48 bit integer
#define GATT_FORMAT_UINT64               0x0a //!< Unsigned 64 bit integer
#define GATT_FORMAT_UINT128              0x0b //!< Unsigned 128 bit integer
#define GATT_FORMAT_SINT8                0x0c //!< Signed 8 bit integer
#define GATT_FORMAT_SINT12               0x0d //!< Signed 12 bit integer
#define GATT_FORMAT_SINT16               0x0e //!< Signed 16 bit integer
#define GATT_FORMAT_SINT24               0x0f //!< Signed 24 bit integer
#define GATT_FORMAT_SINT32               0x10 //!< Signed 32 bit integer
#define GATT_FORMAT_SINT48               0x11 //!< Signed 48 bit integer
#define GATT_FORMAT_SINT64               0x12 //!< Signed 64 bit integer
#define GATT_FORMAT_SINT128              0x13 //!< Signed 128 bit integer
#define GATT_FORMAT_FLOAT32              0x14 //!< IEEE-754 32 bit floating point
#define GATT_FORMAT_FLOAT64              0x15 //!< IEEE-754 64 bit floating point
#define GATT_FORMAT_SFLOAT               0x16 //!< IEEE-11073 16 bit SFLOAT
#define GATT_FORMAT_FLOAT                0x17 //!< IEEE-11073 32 bit FLOAT
#define GATT_FORMAT_DUINT16              0x18 //!< IEEE-20601 format
#define GATT_FORMAT_UTF8S                0x19 //!< UTF-8 string
#define GATT_FORMAT_UTF16S               0x1a //!< UTF-16 string
#define GATT_FORMAT_STRUCT               0x1b //!< Opaque structure

/** @} End GATT_FORMAT_TYPES_DEFINES */

/** @defgroup GATT_NS_TYPES_DEFINES GATT Namespace Types
 * @{
 */

#define GATT_NS_NONE                     0x00 //!< No namespace
#define GATT_NS_BT_SIG                   0x01 //!< Bluetooth SIG namespace

/** @} End GATT_NS_TYPES_DEFINES */

/** @defgroup GATT_NS_BT_DESC_DEFINES GATT Bluetooth Namespace Descriptions
 * @{
 */

#define GATT_NS_BT_DESC_UNKNOWN          0x0000 //!< The description is unknown

/** @} End GATT_NS_BT_DESC_DEFINES */

// All profile services bit fields
#define GATT_ALL_SERVICES                0xFFFFFFFF

// GATT Services bit fields
#define GATT_SERVICE                     0x00000001

// GATT Server Parameters
#define GATT_PARAM_NUM_PREPARE_WRITES    0 // RW  uint8


#define GAP_DEVICE_NAME_LEN                     (20+1)

// Privacy Flag States
#define GAP_PRIVACY_DISABLED                    0x00
#define GAP_PRIVACY_ENABLED                     0x01

// GAP GATT Server Parameters
#define GGS_DEVICE_NAME_ATT                     0   // RW  uint8[GAP_DEVICE_NAME_LEN]
#define GGS_APPEARANCE_ATT                      1   // RW  uint16
#define GGS_PERI_PRIVACY_FLAG_ATT               2   // RW  uint8
#define GGS_RECONNCT_ADDR_ATT                   3   // RW  uint8[B_ADDR_LEN]
#define GGS_PERI_CONN_PARAM_ATT                 4   // RW  sizeof(gapPeriConnectParams_t)
#define GGS_PERI_PRIVACY_FLAG_PROPS             5   // RW  uint8
#define GGS_W_PERMIT_DEVICE_NAME_ATT            6   // W   uint8
#define GGS_W_PERMIT_APPEARANCE_ATT             7   // W   uint8
#define GGS_W_PERMIT_PRIVACY_FLAG_ATT           8   // W   uint8

// GAP Services bit fields
#define GAP_SERVICE                             0x00000001

// Attribute ID used with application's callback when attribute value is changed OTA
#define GGS_DEVICE_NAME_ID                      0
#define GGS_APPEARANCE_ID                       1

#define MAX_NUM_LL_CONN                       3
#define GATT_MAX_NUM_CONN                ( MAX_NUM_LL_CONN + 1 )



typedef void (*gatt_cb_t) (GSList *l, guint8 status, gpointer user_data);

struct gatt_primary {
	char uuid[MAX_LEN_UUID_STR + 1];
	gboolean changed;
	struct att_range range;
};

struct gatt_included {
	char uuid[MAX_LEN_UUID_STR + 1];
	uint16_t handle;
	struct att_range range;
};

struct gatt_char {
	char uuid[MAX_LEN_UUID_STR + 1];
	uint16_t handle;
	uint8_t properties;
	uint16_t value_handle;
};


//added by Optek


/**
 * GATT Attribute Type format.
 */
#if 0
typedef struct
{
  uint8_t len;         //!< Length of UUID
  const uint8_t *uuid; //!< Pointer to UUID
} gattAttrType_t;
#endif

#define gattAttrType_t bt_uuid_t;

typedef struct attAttribute_t
{
	bt_uuid_t *pType;
	//bt_uuid_t type;
 	uint8_t permissions;   //!< Attribute permissions
	uint16_t handle;       //!< Attribute handle - assigned internally by attribute server
	uint8_t* const pValue; //!< Attribute value - encoding of the octet array is defined in 
	//!< the applicable profile. The maximum length of an attribute 
	//!< value shall be 512 octets.
} gattAttribute_t;


/**
 * GATT Structure for Client Characteristic Configuration.
 */
typedef struct
{
  uint16_t connHandle; //!< Client connection handle
  uint8_t  value;      //!< Characteristic configuration value for this client
} gattCharCfg_t;



guint gatt_discover_primary(GAttrib *attrib, bt_uuid_t *uuid, gatt_cb_t func,
							gpointer user_data);

unsigned int gatt_find_included(GAttrib *attrib, uint16_t start, uint16_t end,
					gatt_cb_t func, gpointer user_data);

guint gatt_discover_char(GAttrib *attrib, uint16_t start, uint16_t end,
					bt_uuid_t *uuid, gatt_cb_t func,
					gpointer user_data);

guint gatt_read_char(GAttrib *attrib, uint16_t handle, GAttribResultFunc func,
							gpointer user_data);

guint gatt_write_char(GAttrib *attrib, uint16_t handle, uint8_t *value,
					size_t vlen, GAttribResultFunc func,
					gpointer user_data);

guint gatt_discover_char_desc(GAttrib *attrib, uint16_t start, uint16_t end,
				GAttribResultFunc func, gpointer user_data);

//guint gatt_write_cmd(GAttrib *attrib, uint16_t handle, uint8_t *value, int vlen,
//				GDestroyNotify notify, gpointer user_data);

guint gatt_read_char_by_uuid(GAttrib *attrib, uint16_t start, uint16_t end,
				bt_uuid_t *uuid, GAttribResultFunc func,
				gpointer user_data);

guint gatt_exchange_mtu(GAttrib *attrib, uint16_t mtu, GAttribResultFunc func,
							gpointer user_data);

gboolean gatt_parse_record(const sdp_record_t *rec,
					uuid_t *prim_uuid, uint16_t *psm,
					uint16_t *start, uint16_t *end);


//added by Optek
gboolean gatt_primary_all_cb(GAttrib *attrib, guint8 *pdu, guint16 len);
gboolean gatt_read_by_type_resp(GAttrib *attrib, guint8 *pdu, guint16 len);
gboolean gatt_read_by_type_req(GAttrib *attrib, guint8 *pdu, guint16 len);
gboolean gatt_find_info_req(GAttrib *attrib, guint8 *pdu, guint16 len);
gboolean gatt_find_by_type_req(GAttrib *attrib, guint8 *pdu, guint16 len);
gboolean gatt_write_req(GAttrib *attrib, guint8 *pdu, guint16 len);
gboolean gatt_write_cmd(GAttrib *attrib, guint8 *pdu, guint16 len);
gboolean gatt_read_req(GAttrib *attrib, guint8 *pdu, guint16 len);
gboolean gatt_read_blob_req(GAttrib *attrib, guint8 *pdu, guint16 len);


extern const bt_uuid_t  gattPrimSvc;
extern const bt_uuid_t  gattSndSvc;
extern const bt_uuid_t  gattInclude;
extern const bt_uuid_t  gattCharac;
extern const bt_uuid_t   gattCharacDevName;
extern const bt_uuid_t   gattCharacAppearance;
extern const bt_uuid_t   gattCharacPeriPrivFlag;
extern const bt_uuid_t   gattCharacReconnAddr;
extern const bt_uuid_t   gattCharacPeriPrefConn;
extern const bt_uuid_t   gattCharacServiceChanged;


extern const bt_uuid_t   gattCharacExtProper;
extern const bt_uuid_t   gattCharacUserDesc;
extern const bt_uuid_t   gattClientCharacCFG;
extern const bt_uuid_t   gattServerCharacCFG;
extern const bt_uuid_t   gattCharacFMT;
extern const bt_uuid_t   gattCharacAgregFMT;
extern const bt_uuid_t   gattCharacValidRande;
extern const bt_uuid_t   gattExternalReportRef;
extern const bt_uuid_t   gattCharacReportRef;

extern const uint8_t gattReadProps;
extern const uint8_t gattNotifyProps;
extern const uint8_t gattReadNotifyProps;
extern const uint8_t gattReadWriteProps;
extern const uint8_t gattWriteNoRespProps;
#endif
