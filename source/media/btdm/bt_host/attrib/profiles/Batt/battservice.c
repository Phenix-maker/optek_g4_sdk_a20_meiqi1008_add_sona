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
#include "uuid.h"
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
 * CONSTANTS
 */

// Battery Service Get/Set Parameters
#define BATT_PARAM_LEVEL                0
#define BATT_PARAM_CRITICAL_LEVEL       1
#define BATT_PARAM_SERVICE_HANDLE       2
#define BATT_PARAM_BATT_LEVEL_IN_REPORT 3

// Callback events
#define BATT_LEVEL_NOTI_ENABLED         1
#define BATT_LEVEL_NOTI_DISABLED        2

// HID Report IDs for the service
#define HID_RPT_ID_BATT_LEVEL_IN        4  // Battery Level input report ID


// Battery Service attribute
static const bt_uuid_t battService = { ATT_BT_UUID_SIZE, BATT_SERV_UUID };
static const bt_uuid_t battLevelUuid = { ATT_BT_UUID_SIZE, BATT_LEVEL_UUID };

// Battery level characteristic
static uint8_t battLevel = 100;
static gattCharCfg_t battLevelClientCharCfg[GATT_MAX_NUM_CONN];

//define in hiddev.h
#define HID_REPORT_REF_LEN          2    // HID Report Reference Descriptor
#define HID_REPORT_TYPE_INPUT       1

// HID Report Reference characteristic descriptor, battery level
static uint8_t hidReportRefBattLevel[HID_REPORT_REF_LEN] =
             { HID_RPT_ID_BATT_LEVEL_IN, HID_REPORT_TYPE_INPUT };

/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t battAttrTbl[] =
{
  // Battery Service
  {
    //{ ATT_BT_UUID_SIZE, GATT_PRIM_SVC_UUID }, /* type */
	  &gattPrimSvc,
    GATT_PROP_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8_t *)&battService                     /* pValue */
  },

    // Battery Level Declaration
    {
      //{ ATT_BT_UUID_SIZE, GATT_CHARAC_UUID },
		  &gattCharac,
      GATT_PROP_READ,
      0,
      &gattReadProps
    },

      // Battery Level Value
      {
        //{ ATT_BT_UUID_SIZE, BATT_LEVEL_UUID },
		&battLevelUuid,
        GATT_PROP_READ,
        0,
        &battLevel
      },

      // Battery Level Client Characteristic Configuration
      {
        //{ ATT_BT_UUID_SIZE, GATT_CLIENT_CHARAC_CFG_UUID },
		&gattClientCharacCFG,
        GATT_PROP_READ | GATT_PROP_WRITE,
        0,
        (uint8_t *) &battLevelClientCharCfg
      },

      // HID Report Reference characteristic descriptor, batter level input
      {
        //{ ATT_BT_UUID_SIZE, GATT_REPORT_REFERENCE },
        &gattCharacReportRef,
		GATT_PROP_READ,
        0,
        hidReportRefBattLevel
      }

};


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint8_t battReadAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
                             uint8_t *pValue, uint8_t *pLen, uint16_t offset, uint8_t maxLen );
static bStatus_t battWriteAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
                                  uint8_t *pValue, uint8_t len, uint16_t offset );
//static void battNotifyCB( linkDBItem_t *pLinkItem );
static uint8_t battMeasure( void );
static void battNotifyLevel( void );

/*********************************************************************
 * PROFILE CALLBACKS
 */
// Battery Service Callbacks
const gattServiceCBs_t battCBs =
{
  NULL,
  NULL,
  battReadAttrCB,  // Read callback function pointer
  battWriteAttrCB, // Write callback function pointer
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
bStatus_t Batt_AddService( void )
{
  uint8_t status;

  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg( INVALID_CONNHANDLE, battLevelClientCharCfg );

  // Register GATT attribute list and CBs with GATT Server App
  status = GATTServApp_RegisterService( battAttrTbl,
                                        GATT_NUM_ATTRS( battAttrTbl ),
                                        &battCBs );

  return ( status );
}


static uint8_t battReadAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
							  uint8_t *pValue, uint8_t *pLen, uint16_t offset, uint8_t maxLen )
{
	bStatus_t status = SUCCESS;
	uint16_t uuid = pAttr->pType->value.u16_val; 
	
	// Make sure it's not a blob operation (no attributes in the profile are long)
	if ( offset > 0 )
	{
		return ( ATT_ERR_ATTR_NOT_LONG );
	}
	
	
	// Measure battery level if reading level
	if ( uuid == BATT_LEVEL_UUID )
	{		
		*pLen = 1;
		pValue[0] = 80;				//80%
	}
	else if ( uuid == GATT_REPORT_REFERENCE )
	{
		*pLen = HID_REPORT_REF_LEN;

		CFasm_memcpy( pValue, pAttr->pValue, HID_REPORT_REF_LEN );
	}
	else
	{
		*pLen = 0;
		status = ATT_ERR_ATTR_NOT_FOUND;
	}
	
  return ( status );

}

static bStatus_t battWriteAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
                                  uint8_t *pValue, uint8_t len, uint16_t offset )
{
	asm ("break 1,1");
	return SUCCESS;
}
#endif