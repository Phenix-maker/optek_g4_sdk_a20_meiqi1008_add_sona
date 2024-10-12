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

#ifdef BTDM52_ENABLE
/*********************************************************************
 * Profile Attributes - Table
 */

static const bt_uuid_t genaralAttrService = { ATT_BT_UUID_SIZE, GENERAL_ATTR_SERV_UUID };
static const uint16_t generalAccessServiceChange[2] = {0x0001, 0xFFFF};


static gattCharCfg_t generalAttrClientCharCfg[GATT_MAX_NUM_CONN];



static gattAttribute_t generalAttrTbl[] =
{
  // General Attribute Service
  {
    //{ ATT_BT_UUID_SIZE, GATT_PRIM_SVC_UUID }, /* type */
	  &gattPrimSvc,
    GATT_PROP_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8_t *)&genaralAttrService                     /* pValue */
  },

  // Characteristic Declaration Device Name
  {
      //{ ATT_BT_UUID_SIZE, GATT_CHARAC_UUID },
		  &gattCharac,
		  GATT_PROP_READ,
		  0,
		  (uint8_t *)&gattNotifyProps
    },
	
	// Characteristic Value- Device Name
	{ 
        //{ ATT_BT_UUID_SIZE, GATT_CHARAC_SERVICE_CHANGED },
			&gattCharacServiceChanged,
			0, 
			0, 
			(uint8_t *) &generalAccessServiceChange[0] 
	  },

      // General Attr Client Characteristic Configuration
      {
		  //{ ATT_BT_UUID_SIZE, GATT_CLIENT_CHARAC_CFG_UUID },
			  &gattClientCharacCFG,
			  GATT_PROP_READ | GATT_PROP_WRITE,
			  0,
			  (uint8_t *) &generalAttrClientCharCfg
      },


};


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint8_t genericAttrReadAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
                             uint8_t *pValue, uint8_t *pLen, uint16_t offset, uint8_t maxLen );
static bStatus_t genericAttrWriteAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
                                  uint8_t *pValue, uint8_t len, uint16_t offset );
//static void battNotifyCB( linkDBItem_t *pLinkItem );
//static uint8_t battMeasure( void );
//static void battNotifyLevel( void );

/*********************************************************************
 * PROFILE CALLBACKS
 */
// Battery Service Callbacks
const gattServiceCBs_t genericAttrCBs =
{
  NULL,
  NULL,
  genericAttrReadAttrCB,  // Read callback function pointer
  genericAttrWriteAttrCB, // Write callback function pointer
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
bStatus_t genericAttr_AddService( void )
{
  uint8_t status;

  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg( INVALID_CONNHANDLE, generalAttrClientCharCfg );

  // Register GATT attribute list and CBs with GATT Server App
  status = GATTServApp_RegisterService( generalAttrTbl,
                                        GATT_NUM_ATTRS( generalAttrTbl ),
                                        &genericAttrCBs );

  return ( status );
}


static uint8_t genericAttrReadAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
							  uint8_t *pValue, uint8_t *pLen, uint16_t offset, uint8_t maxLen )
{
	//asm ("break 1,1");
	bStatus_t status = SUCCESS;
	
	// Make sure it's not a blob operation (no attributes in the profile are long)

	

	*pLen = 0;
	status = ATT_ERR_READ_NOT_PERMITTED;

  return ( status );	
}

static bStatus_t genericAttrWriteAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
                                  uint8_t *pValue, uint8_t len, uint16_t offset )
{
	//asm ("break 1,1");
	return SUCCESS;
}
#endif