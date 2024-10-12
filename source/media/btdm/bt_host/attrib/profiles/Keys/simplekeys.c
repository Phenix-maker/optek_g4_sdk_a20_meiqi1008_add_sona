/*
	Copyright by Optek in 2015
*/

#include "c_def.h"
#include "debug.h"
#include "oem.h"

#include "bt_def.h"
#include "bluetooth.h"
#include "hci.h"
#include "hci_core.h"
#include "l2cap.h"
#include "../blueZ/uuid.h"
//#include "../blueZ/bluetooth.h"
//#include "../blueZ/sdp_lib.h"

#include "list.h"
#include "malloc.h"
#include "g_slist.h"

#include "../attrib/att.h"
#include "../attrib/gattrib.h"
#include "../attrib/gatt.h"
#include "../attrib/gatt_profile_uuid.h"

#include "simplekeys.h"

#include "../attrib/gatt-service.h"
#include "app_task.h"

#ifdef BTDM52_ENABLE

/*********************************************************************
 * Profile Attributes - variables
 */

// SK Service attribute
static const bt_uuid_t skService = { ATT_BT_UUID_SIZE, SK_SERV_UUID };

static const bt_uuid_t skPressedUuid = { ATT_BT_UUID_SIZE, SK_KEYPRESSED_UUID };
static const bt_uuid_t skReleasedUuid = { ATT_BT_UUID_SIZE, SK_KEYRELEASED_UUID };
 

// Key Pressed State Characteristic
static uint8_t skKeyPressed = 0;
static uint8_t skKeyReleased = 0;

static uint8_t skNotificationEnable = 0;

// Key Pressed Characteristic Configs
static gattCharCfg_t skConfig[GATT_MAX_NUM_CONN];

// Key Pressed Characteristic User Description
static const uint8_t skCharUserDesp[16] = "Key State\0";

#define BLE_NOTIFY_DEBUG

/*********************************************************************
 * Profile Attributes - Table
 */

//static gattAttribute_t simplekeysAttrTbl[SERVAPP_NUM_ATTR_SUPPORTED] = 
static gattAttribute_t simplekeysAttrTbl[] = 
{
  // Simple Keys Service
  { 
    //{ ATT_BT_UUID_SIZE, GATT_PRIM_SVC_UUID }, /* type */
	  &gattPrimSvc,
    GATT_PROP_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8_t *)&skService                       /* pValue */
  },

    // Characteristic Declaration for Keys
    { 
      //{ ATT_BT_UUID_SIZE, GATT_CHARAC_UUID },
		  &gattCharac,
      GATT_PROP_READ, 
      0,
#ifdef BLE_NOTIFY_DEBUG
      &gattNotifyProps							//key notify 
#else
	  &gattWriteNoRespProps					//Key write
#endif

    },

      // Characteristic Value- Key Pressed
      { 
        //{ ATT_BT_UUID_SIZE, SK_KEYPRESSED_UUID },
			  &skPressedUuid,
        0, 
        0, 
        &skKeyPressed 
      },


#ifndef BLE_NOTIFY_DEBUG
		{ 
				  //{ ATT_BT_UUID_SIZE, GATT_CHARAC_UUID },
				  &gattCharac,
					  GATT_PROP_READ, 
					  0,
					  &gattWriteNoRespProps 
		},
			  
		// Characteristic Value- Key Pressed
		{ 
					  //{ ATT_BT_UUID_SIZE, SK_KEYPRESSED_UUID },
					  &skReleasedUuid,
						  0, 
						  0, 
						  &skKeyReleased 
		},
#endif	//BLE_NOTIFY_DEBUG

      // Characteristic configuration
      { 
        //{ ATT_BT_UUID_SIZE, GATT_CLIENT_CHARAC_CFG_UUID },
			&gattClientCharacCFG,
		GATT_PROP_READ | GATT_PROP_WRITE,
        0, 
        (uint8_t *)skConfig 
      },

#if 0
      // Characteristic User Description
      { 
        //{ ATT_BT_UUID_SIZE, GATT_CHARAC_USER_DESC_UUID },
		&gattCharacUserDesc,
        GATT_PROP_READ, 
        0, 
        skCharUserDesp 
      },
#endif
      
};



/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint8_t simplekeysReadAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
                             uint8_t *pValue, uint8_t *pLen, uint16_t offset, uint8_t maxLen );
static bStatus_t simplekeysWriteAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
                                  uint8_t *pValue, uint8_t len, uint16_t offset );
//static void battNotifyCB( linkDBItem_t *pLinkItem );
//static uint8_t battMeasure( void );
//static void battNotifyLevel( void );

/*********************************************************************
 * PROFILE CALLBACKS
 */
// Battery Service Callbacks
const gattServiceCBs_t simplekeysCBs =
{
  NULL,
  NULL,
  simplekeysReadAttrCB,  // Read callback function pointer
  simplekeysWriteAttrCB, // Write callback function pointer
  NULL             // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      Batt_AddService
 *
 * @brief   Initializes the Battery Service by registering
 *          GATT attributes with the GATT server.
 *
 * @return  Success or Failure
 */
bStatus_t simplekeys_AddService( void )
{
  uint8_t status;

  skNotificationEnable = 0;

  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg( INVALID_CONNHANDLE, skConfig );

  // Register GATT attribute list and CBs with GATT Server App
  status = GATTServApp_RegisterService( simplekeysAttrTbl,
                                        GATT_NUM_ATTRS( simplekeysAttrTbl ),
                                        &simplekeysCBs );

  return ( status );
}


static uint8_t simplekeysReadAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
							  uint8_t *pValue, uint8_t *pLen, uint16_t offset, uint8_t maxLen )
{
	bStatus_t status = SUCCESS;
	uint16_t uuid = pAttr->pType->value.u16_val; 
	
	// Make sure it's not a blob operation (no attributes in the profile are long)
	if ( offset > 0 )
	{
		*pLen = 0;
		return ( ATT_ERR_ATTR_NOT_LONG );
	}
	

	*pLen = 0;
	status = ATT_ERR_ATTR_NOT_FOUND;

	
  return ( status );
}

void app_cmd_ble_key_pressed(u8 key);
void app_cmd_ble_key_released(u8 key);

static bStatus_t simplekeysWriteAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
                                  uint8_t *pValue, uint8_t len, uint16_t offset )
{
	bStatus_t status = SUCCESS;
	uint16_t uuid = pAttr->pType->value.u16_val; 
	uint8_t key;
	
	
	// Make sure it's not a blob operation (no attributes in the profile are long)
	if ( offset > 0 )
	{
		return ( ATT_ERR_ATTR_NOT_LONG );
	}
	
	switch ( uuid )
	{ 
	case SK_KEYPRESSED_UUID:
		key = pValue[0];		
		func_w_one_param_msgSend(app_cmd_ble_key_pressed,(u32) key);
		break;
	case SK_KEYRELEASED_UUID:
		key = pValue[0];
		func_w_one_param_msgSend(app_cmd_ble_key_released,(u32) key);		
		break;
		
	case GATT_CLIENT_CHARAC_CFG_UUID:
		//0x0001	notified
		//0x0002	indicated
		skNotificationEnable = pValue[0] & 0x01;

		//DBG_Assert(FALSE);
		break;
		
	default:
		// Should never get here!
		status = ATT_ERR_ATTR_NOT_FOUND;
		break;
	}
	
	
	return ( status );
}

struct _GAttrib *find_att_sess_only_by_index(int index);
size_t att_get_session_mtu (GAttrib *session);

void att_send_data(void *pIO, struct sk_buff *sk_att);
void att_key_notification_send (u32 parm1, u32 parm2, u32 parm3);

const U8 STRING_OPEN1[] = {"CD    Open\0\0"};
void att_key_notification_send1(void)
{
	att_key_notification_send (STRING_OPEN1,8,NULL);

}
void att_key_notification_send (u32 parm1, u32 parm2, u32 parm3)
{
	GAttrib *session;
	struct sk_buff *gatt_sk_buf;
	u16 mtu;
	u16 handle;
	
	
	session = find_att_sess_only_by_index(0);
	if (session == NULL) {
		DBG_Printf ("No gatt service\n\r");
		return;	
	}

	if (skNotificationEnable == 0x0) {
		DBG_Printf ("Notify is not configed\n\r");
		return;	
	}

	//handle = simplekeysAttrTbl[0].handle;
	handle = simplekeysAttrTbl[2].handle;
	
	if (handle == 0) {
		DBG_Assert (FALSE);
		DBG_Printf ("No disp service\n\r");
		return;	
	}

	mtu = att_get_session_mtu(session);
	gatt_sk_buf = bt_skb_alloc((MAX_BT_SEND_SKB-BT_SKB_RESERVE), GFP_ATOMIC);
	
	gatt_sk_buf->len = enc_notification(handle, (uint8_t *) parm1, parm2, (uint8_t *) gatt_sk_buf->data, mtu);
	
	if (gatt_sk_buf->len > mtu) {
		DBG_Printf ("Notify message is too long %d mtu %d\n\r", gatt_sk_buf->len,mtu);
		//cut some
		gatt_sk_buf->len = mtu;
	}
	
	att_send_data(session, gatt_sk_buf);
}


/*********************************************************************
 * Profile Attributes - variables
 */

// RTU Service attribute
static const bt_uuid_t rtuService = { ATT_BT_UUID_SIZE, APP_SERV_UUID };

// RTU characteristic
static const bt_uuid_t phone_send_rtuTime_Uuid = { ATT_BT_UUID_SIZE, APP_SERV_W_UUID };
//static const bt_uuid_t rtuTime_send_phone_Uuid = { ATT_BT_UUID_SIZE, APP_SERV_W_UUID/*APP_SERV_R_UUID */};
static const bt_uuid_t rtuTime_send_phone_Uuid = { ATT_BT_UUID_SIZE, APP_SERV_R_UUID };
static const bt_uuid_t rtuTime_notify_Uuid = { ATT_BT_UUID_SIZE, APP_SERV_N_UUID };


// RTU State Characteristic
static uint8_t rtuTime[32];
static uint8_t rtu_Update_State_Time[32];
static uint8_t rtu_notify_State_Time[32];

static uint8_t rtuNotificationEnable = 0;

// RTU Characteristic Configs
static gattCharCfg_t rtuConfig[GATT_MAX_NUM_CONN];

// Key Pressed Characteristic User Description
static const uint8_t LDRCCharUserDesc[22] = "Linking Digital RC\0";

void att_key_notification_send_ldrc (u32 parm1, u32 parm2, u32 parm3);

/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t rtuAttrTbl[] = 
{
	  //Service
	  { 
	    //{ ATT_BT_UUID_SIZE, GATT_PRIM_SVC_UUID }, /* type */
		&gattPrimSvc,
	    GATT_PROP_READ | GATT_PROP_WRITE | GATT_PROP_NOTIFY | GATT_PROP_WRITE_NO_RSP,                         /* permissions */
	    0,                                        /* handle */
	    (uint8_t *)&rtuService                       /* pValue */
	  },
      // Characteristic Declaration
      { 
        //{ ATT_BT_UUID_SIZE, GATT_CHARAC_UUID },
  	  	&gattCharac,
  	  	GATT_PROP_WRITE | GATT_PROP_WRITE_NO_RSP,
        0,
        //&gattReadProps
        &gattWriteNoRespProps
      },
  
      // Characteristic Time Update Control Point
      { 
		&phone_send_rtuTime_Uuid,
		GATT_PROP_WRITE | GATT_PROP_WRITE_NO_RSP,
        0, 
        rtuTime 
      },

      // Characteristic Declaration
      { 
        //{ ATT_BT_UUID_SIZE, GATT_CHARAC_UUID },
  	  	&gattCharac,
  	  	GATT_PROP_READ, 
        0,
        &gattReadProps
      },
  
      // Characteristic Time Update Control Point
      { 
	    &rtuTime_send_phone_Uuid,
		GATT_PROP_READ, 
        0, 
        rtu_Update_State_Time 
      },
        
      { 
  	  	&gattCharac,
  	  	GATT_PROP_NOTIFY, 
        0,
        &gattNotifyProps
      },
  
      { 
	    &rtuTime_notify_Uuid,
		GATT_PROP_NOTIFY, 
        0, 
        rtu_notify_State_Time 
      },

/*
	// Characteristic configuration
	{ 
		//{ ATT_BT_UUID_SIZE, GATT_CLIENT_CHARAC_CFG_UUID },
		&gattClientCharacCFG,
		GATT_PROP_READ | GATT_PROP_WRITE,
		0, 
		(uint8_t *)rtuConfig 
	},

	// Characteristic User Description
	{ 
		//{ ATT_BT_UUID_SIZE, GATT_CHARAC_USER_DESC_UUID },
		&gattCharacUserDesc,
		GATT_PROP_READ, 
		0, 
		LDRCCharUserDesc 
	},
*/
};

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint8_t RTUReadAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
                             uint8_t *pValue, uint8_t *pLen, uint16_t offset, uint8_t maxLen );
static bStatus_t RTUWriteAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
                                  uint8_t *pValue, uint8_t len, uint16_t offset );
//static void battNotifyCB( linkDBItem_t *pLinkItem );
//static uint8_t battMeasure( void );
//static void battNotifyLevel( void );

/*********************************************************************
 * PROFILE CALLBACKS
 */
// RTU Callbacks
const gattServiceCBs_t RTUCBs =
{
  NULL,
  NULL,
  RTUReadAttrCB,  // Read callback function pointer
  RTUWriteAttrCB, // Write callback function pointer
  NULL             // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      Batt_AddService
 *
 * @brief   Initializes the Battery Service by registering
 *          GATT attributes with the GATT server.
 *
 * @return  Success or Failure
 */
bStatus_t reference_time_update_AddService( void )
{
  uint8_t status;

  rtuNotificationEnable = 0;
/*
  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg( INVALID_CONNHANDLE, rtuConfig );
*/
  // Register GATT attribute list and CBs with GATT Server App
  status = GATTServApp_RegisterService( rtuAttrTbl,
                                        GATT_NUM_ATTRS( rtuAttrTbl ),
                                        &RTUCBs );

  return ( status );
}

U8 bleread_buf[16];
static uint8_t RTUReadAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
							  uint8_t *pValue, uint8_t *pLen, uint16_t offset, uint8_t maxLen )
{
	bStatus_t status = SUCCESS;
	uint16_t uuid = pAttr->pType->value.u16_val; 
	uint16_t i;
	
	DBG_Printf("APP read uuid:0x%x\n", uuid);
	
	// Make sure it's not a blob operation (no attributes in the profile are long)
	if ( offset > 0 )
	{
		return ( ATT_ERR_ATTR_NOT_LONG );
	}

	// Measure battery level if reading level
	if ( uuid == 0xF002 )
	{
		DBG_Printf("data:");
		*pLen = 16;
		for (i=0; i<16; i++)
		{
		 	pValue[i] = bleread_buf[i];
			DBG_Printf("0x%x, ", bleread_buf[i]);
		}
		DBG_Printf("\n");
	}
/*
	else if ( uuid == GATT_REPORT_REFERENCE )
	{
		*pLen = HID_REPORT_REF_LEN;

		CFasm_memcpy( pValue, pAttr->pValue, HID_REPORT_REF_LEN );
	}
*/
	else
	{
		*pLen = 0;
		status = ATT_ERR_ATTR_NOT_FOUND;
	}

	//att_key_notification_send_ldrc(bleread_buf, 10, NULL);
	
/*
	// Measure read current time value
	if ( uuid == TIME_UPDATE_STATE_UUID )
	{		
		//*pLen = 10;
		//timeBuf = pValue;

	}
	else
	{
		*pLen = 0;
		status = ATT_ERR_ATTR_NOT_FOUND;
	}
*/	
  return ( status );

}

void app_ble_update_rxdata(U8 *p);

uint8_t Phoneble_rxBuf[16];

static bStatus_t RTUWriteAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
                                  uint8_t *pValue, uint8_t len, uint16_t offset )
{
	bStatus_t status = SUCCESS;
	uint16_t uuid = pAttr->pType->value.u16_val; 
	uint8_t i;
	DBG_Printf("APP write uuid:0x%x,dlen:%d\n", uuid, len );
	
	memset(Phoneble_rxBuf,0,sizeof(Phoneble_rxBuf));
	// Make sure it's not a blob operation (no attributes in the profile are long)
	if ( offset > 0 )
	{
		return ( ATT_ERR_ATTR_NOT_LONG );
	}
	
	switch ( uuid )
	{ 
		case 0xF001:
			DBG_Printf("data:\n");
			for(i=0; i<len; i++)
			{
				Phoneble_rxBuf[i] = pValue[i];
				DBG_Printf("0x%x, ", Phoneble_rxBuf[i]);
			}
			DBG_Printf("\n");	

			func_w_one_param_msgSend(app_ble_update_rxdata, Phoneble_rxBuf);
			
			break;
			
	/*	case GATT_CLIENT_CHARAC_CFG_UUID:
			//0x0001	notified
			//0x0002	indicated
			rtuNotificationEnable = pValue[0] & 0x01;

			//DBG_Assert(FALSE);
			break;*/
			
		default:
			// Should never get here!
			status = ATT_ERR_ATTR_NOT_FOUND;
			//DBG_Printf("BLE write status==false\n");
			break;
	}
	
	//DBG_Printf("BLE write status==TRUE\n");
	return ( status );
}


struct _GAttrib *find_att_sess_only_by_index(int index);
size_t att_get_session_mtu (GAttrib *session);

void att_send_data(void *pIO, struct sk_buff *sk_att);


void att_key_notification_send_ldrc (u32 parm1, u32 parm2, u32 parm3)
{
	GAttrib *session;
	struct sk_buff *gatt_sk_buf;
	u16 mtu;
	u16 handle;
	
	DBG_Printf("BLE notify enter\n\r");


	session = find_att_sess_only_by_index(1);
	if (session == NULL) {
		DBG_Printf ("No nore than 1 service\n\r");
		//return;	
	}

	session = find_att_sess_only_by_index(0);
	if (session == NULL) {
		DBG_Printf ("No gatt service\n\r");
		return;	
	}
		DBG_Printf ("session:0x%x\n\r",session);

/*
	if (skNotificationEnable == 0x0) {
		DBG_Printf ("Notify is not configed\n\r");
		return;	
	}
*/
	//handle = simplekeysAttrTbl[0].handle;
	handle = rtuAttrTbl[6].handle;
	DBG_Printf ("handle:0x%x\n\r",handle);
	if (handle == 0) {
		DBG_Assert (FALSE);
		DBG_Printf ("No disp service\n\r");
		return;	
	}

	mtu = att_get_session_mtu(session);
	
	gatt_sk_buf = bt_skb_alloc((MAX_BT_SEND_SKB-BT_SKB_RESERVE), GFP_ATOMIC);
	
	gatt_sk_buf->len = enc_notification(handle, (uint8_t *) parm1, parm2, (uint8_t *) gatt_sk_buf->data, mtu);
	
	if (gatt_sk_buf->len > mtu) {
		DBG_Printf ("Notify message is too long %d mtu %d\n\r", gatt_sk_buf->len,mtu);
		//cut some
		gatt_sk_buf->len = mtu;
	}
	
	att_send_data(session, gatt_sk_buf);
}

#endif




