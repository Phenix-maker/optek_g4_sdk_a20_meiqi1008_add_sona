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
#include "../bluez/uuid.h"
//#include "../blueZ/bluetooth.h"
//#include "../blueZ/sdp_lib.h"

#include "list.h"
#include "malloc.h"
#include "g_slist.h"

#include "../attrib/att.h"
#include "../attrib/gattrib.h"
#include "../attrib/gatt.h"
#include "../attrib/gatt_profile_uuid.h"

#include "../attrib/gatt-service.h"
#include "proxreporter.h"
#include "app_task.h"

#ifdef BTDM52_ENABLE
/*********************************************************************
 * CONSTANTS
 */

// Profile Parameters
#define PP_LINK_LOSS_ALERT_LEVEL         0  // RW uint8 - Profile Attribute value
#define PP_IM_ALERT_LEVEL                1  // RW uint8 - Profile Attribute value
#define PP_TX_POWER_LEVEL                2  // RW int8 - Profile Attribute value

// Alert Level Values
#define PP_ALERT_LEVEL_NO               0x00
#define PP_ALERT_LEVEL_LOW              0x01
#define PP_ALERT_LEVEL_HIGH             0x02
  



/*********************************************************************
 * CONSTANTS
 */
#define PP_DEFAULT_TX_POWER               0
#define PP_DEFAULT_PATH_LOSS              0x7F

#define SERVAPP_NUM_ATTR_SUPPORTED        5



/*********************************************************************
 * Profile Attributes - variables
 */

// Link Loss Service
static const bt_uuid_t linkLossService = { ATT_BT_UUID_SIZE, LINK_LOSS_SERV_UUID };
static const bt_uuid_t alertLevelUuid = { ATT_BT_UUID_SIZE, ALERT_LEVEL_UUID };



// Alert Level attribute
// This attribute enumerates the level of alert.
static uint8_t llAlertLevel = PP_ALERT_LEVEL_NO;

// Immediate Alert Service
static const bt_uuid_t imAlertService = { ATT_BT_UUID_SIZE, IMMEDIATE_ALERT_SERV_UUID };


// Alert Level attribute
// This attribute enumerates the level of alert.
static uint8_t imAlertLevel = PP_ALERT_LEVEL_NO;

// Tx Power Level Service
static const bt_uuid_t txPwrLevelService = { ATT_BT_UUID_SIZE, TX_PWR_LEVEL_SERV_UUID };
static const bt_uuid_t txPwrLevelUuid = { ATT_BT_UUID_SIZE, TX_PWR_LEVEL_UUID };
 

// Tx Power Level attribute
// This attribute represents the range of transmit power levels in dBm with
// a range from -20 to +20 to a resolution of 1 dBm.
static int8_t txPwrLevel = PP_DEFAULT_TX_POWER;

// Tx Power Level Characteristic Configs
static gattCharCfg_t txPwrLevelConfig[GATT_MAX_NUM_CONN];


/*********************************************************************
 * Profile Attributes - Table
 */
// Link Loss Service Atttribute Table
static gattAttribute_t linkLossAttrTbl[] = 
{
  // Link Loss service
  { 
    //{ ATT_BT_UUID_SIZE, GATT_PRIM_SVC_UUID }, /* type */
	  &gattPrimSvc,
    GATT_PROP_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8_t *)&linkLossService                 /* pValue */
  },

    // Characteristic Declaration
    { 
      //{ ATT_BT_UUID_SIZE, GATT_CHARAC_UUID },
		  &gattCharac,
      GATT_PROP_READ, 
      0,
      &gattReadWriteProps 
    },

      // Alert Level attribute
      { 
        //{ ATT_BT_UUID_SIZE, ALERT_LEVEL_UUID },
		&alertLevelUuid,
        GATT_PROP_READ | GATT_PROP_WRITE, 
        0, 
        &llAlertLevel 
      },
};

// Immediate Alert Service Atttribute Table
static gattAttribute_t imAlertAttrTbl[] = 
{
  // Immediate Alert service
  { 
    //{ ATT_BT_UUID_SIZE, GATT_PRIM_SVC_UUID }, /* type */
	  &gattPrimSvc,
    GATT_PROP_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8_t *)&imAlertService                  /* pValue */
  },

    // Characteristic Declaration
    { 
      //{ ATT_BT_UUID_SIZE, GATT_CHARAC_UUID },
		  &gattCharac,
      GATT_PROP_READ, 
      0,
      &gattWriteNoRespProps 
    },

      // Alert Level attribute
      { 
        //{ ATT_BT_UUID_SIZE, ALERT_LEVEL_UUID },
			  &alertLevelUuid,
        GATT_PROP_WRITE, 
        0, 
        &imAlertLevel 
      },
};

// Tx Power Level Service Atttribute Table
static gattAttribute_t txPwrLevelAttrTbl[] = 
{
  // Tx Power Level service
  { 
    //{ ATT_BT_UUID_SIZE, GATT_PRIM_SVC_UUID }, /* type */
	  &gattPrimSvc,
    GATT_PROP_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8_t *)&txPwrLevelService               /* pValue */
  },

    // Characteristic Declaration
    { 
      //{ ATT_BT_UUID_SIZE, GATT_CHARAC_UUID },
		  &gattCharac,
      GATT_PROP_READ, 
      0,
      &gattReadNotifyProps 
    },

      // Tx Power Level attribute
      { 
        //{ ATT_BT_UUID_SIZE, TX_PWR_LEVEL_UUID },
		&txPwrLevelUuid,
        GATT_PROP_READ, 
        0, 
        (uint8_t *)&txPwrLevel 
      },

      // Characteristic configuration
      { 
        //{ ATT_BT_UUID_SIZE, GATT_CLIENT_CHARAC_CFG_UUID },
			&gattClientCharacCFG,
        GATT_PROP_READ | GATT_PROP_WRITE, 
        0, 
        (uint8_t *)txPwrLevelConfig 
      },

};


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint8_t proxReporterReadAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
                             uint8_t *pValue, uint8_t *pLen, uint16_t offset, uint8_t maxLen );
static bStatus_t proxReporterWriteAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
                                  uint8_t *pValue, uint8_t len, uint16_t offset );
//static void battNotifyCB( linkDBItem_t *pLinkItem );
//static uint8_t battMeasure( void );
//static void battNotifyLevel( void );

/*********************************************************************
 * PROFILE CALLBACKS
 */
// Battery Service Callbacks
const gattServiceCBs_t proxReporterCBs =
{
  NULL,
  NULL,
  proxReporterReadAttrCB,  // Read callback function pointer
  proxReporterWriteAttrCB, // Write callback function pointer
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
bStatus_t ProxReporter_AddService( uint32_t services )
{

  uint8_t status;
  
  if ( services & PP_LINK_LOSS_SERVICE )
  {
	  // Register Link Loss attribute list and CBs with GATT Server App  
	  status = GATTServApp_RegisterService( linkLossAttrTbl, 
		  GATT_NUM_ATTRS( linkLossAttrTbl ),
		  &proxReporterCBs );
  }
  
  if ( ( status == SUCCESS ) && ( services & PP_IM_ALETR_SERVICE ) )
  {
	  // Register Link Loss attribute list and CBs with GATT Server App
	  status = GATTServApp_RegisterService( imAlertAttrTbl, 
		  GATT_NUM_ATTRS( imAlertAttrTbl ),
		  &proxReporterCBs );
  }
  
  if ( ( status == SUCCESS )  && ( services & PP_TX_PWR_LEVEL_SERVICE ) )
  {
	  
	  // Initialize Client Characteristic Configuration attributes
	  GATTServApp_InitCharCfg( INVALID_CONNHANDLE, txPwrLevelConfig );
	  
	  // Register with Link DB to receive link status change callback
	  //VOID linkDB_Register( proxReporter_HandleConnStatusCB );  
	  
	  
	  // Register Tx Power Level attribute list and CBs with GATT Server App
	  status = GATTServApp_RegisterService( txPwrLevelAttrTbl, 
		  GATT_NUM_ATTRS( txPwrLevelAttrTbl ),
		  &proxReporterCBs );
  }
  
  return ( status );


}


static uint8_t proxReporterReadAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
							  uint8_t *pValue, uint8_t *pLen, uint16_t offset, uint8_t maxLen )
{
	bStatus_t status = SUCCESS;
	
	// Make sure it's not a blob operation (no attributes in the profile are long)
	if ( offset > 0 )
	{
		return ( ATT_ERR_ATTR_NOT_LONG );
	}
	
	uint16_t uuid = pAttr->pType->value.u16_val; 
	
	switch (uuid) {
	case ALERT_LEVEL_UUID:
	case TX_PWR_LEVEL_UUID:
        *pLen = 1;
        pValue[0] = *pAttr->pValue;
        break;
		
	default:
        // Should never get here!
        *pLen = 0;
        status = ATT_ERR_ATTR_NOT_FOUND;
        break;
	}
	
	return ( status );
	return SUCCESS;
}

void app_cmd_ble_debug_vol(u8 level);

static bStatus_t proxReporterWriteAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
                                  uint8_t *pValue, uint8_t len, uint16_t offset )
{
	bStatus_t status = SUCCESS;
	uint16_t uuid = pAttr->pType->value.u16_val; 
	uint8_t level;

	
	// Make sure it's not a blob operation (no attributes in the profile are long)
	if ( offset > 0 )
	{
		return ( ATT_ERR_ATTR_NOT_LONG );
	}

		switch ( uuid )
		{ 
		case ALERT_LEVEL_UUID:
			level = pValue[0];
			pAttr->pValue[0] = level;

			func_w_one_param_msgSend(app_cmd_ble_debug_vol,(u32) level);

			if( pAttr->pValue == &llAlertLevel ) {
				//notify = PP_LINK_LOSS_ALERT_LEVEL;
				DBG_Printf ("Loss Alert Level %d\n\r",level);
			}
			else {
				//notify = PP_IM_ALERT_LEVEL;
				DBG_Printf ("Immediate Alert Alert Level %d\n\r",level);
			}			
			break;
			
		case GATT_CLIENT_CHARAC_CFG_UUID:
			DBG_Assert(FALSE);
			break;
			
		default:
			// Should never get here!
			status = ATT_ERR_ATTR_NOT_FOUND;
			break;
		}

	
		return ( status );
}

#endif