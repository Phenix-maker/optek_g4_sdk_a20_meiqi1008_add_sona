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
 * Profile Attributes - Table
 */

static const bt_uuid_t genaralAccessService = { ATT_BT_UUID_SIZE, GENERAL_ACCESS_SERV_UUID };

static const uint8_t generalDevName[16] = {"  OPTEK BLE TEST1\0"};
static const uint16_t generalAccessAppearance = 0x0000;


#define DEFAULT_MIN_CONN_INTERVAL     0x0006  // 100 milliseconds
#define DEFAULT_MAX_CONN_INTERVAL     0x0C80  // 4 seconds

#define MIN_CONN_INTERVAL             0x0006
#define MAX_CONN_INTERVAL             0x0C80


#define MIN_SLAVE_LATENCY             0
#define MAX_SLAVE_LATENCY             500

#define MIN_TIMEOUT_MULTIPLIER        0x000a
#define MAX_TIMEOUT_MULTIPLIER        0x0c80

//  0B  50  00  A0  00  00  00  E8  03	read from ti demo
static const uint16_t generalAccessConnParms[4] = {0x0050, 0x00A0, 0x0000, 0x03E8};

static gattAttribute_t generalAccessTbl[] =
{
  // General Access Service
  {
    //{ ATT_BT_UUID_SIZE, GATT_PRIM_SVC_UUID }, /* type */
    &gattPrimSvc,
	GATT_PROP_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8_t *)&genaralAccessService                     /* pValue */
  },

  // Characteristic Declaration Device Name
	{
      //{ ATT_BT_UUID_SIZE, GATT_CHARAC_UUID },
	  &gattCharac,
      GATT_PROP_READ,
      0,
      (uint8_t *)&gattReadProps
    },

	// Characteristic Value- Device Name
	{ 
        //{ ATT_BT_UUID_SIZE, GATT_CHARAC_DEVICE_NAME },
		  &gattCharacDevName,
			GATT_PROP_READ, 
			0, 
			generalDevName 
    },

	// Characteristic Declaration Appearance
	  {
		  //{ ATT_BT_UUID_SIZE, GATT_CHARAC_UUID },
			  &gattCharac,
			  GATT_PROP_READ,
			  0,
			  (uint8_t *)&gattReadProps
		},
		
		// Characteristic Value- Appearance
		{ 
			//{ ATT_BT_UUID_SIZE, GATT_CHARAC_APPEARANCE },
			    &gattCharacAppearance,
				GATT_PROP_READ, 
				0, 
				(uint8_t *) &generalAccessAppearance 
		  },


		  // Characteristic Declaration Connection Parms
		  {
			  //{ ATT_BT_UUID_SIZE, GATT_CHARAC_UUID },
				  &gattCharac,
				  GATT_PROP_READ,
				  0,
				  (uint8_t *)&gattReadProps
			},
			
			// Characteristic Value- Connection Parms
			{ 
				//{ ATT_BT_UUID_SIZE, GATT_CHARAC_PERIPHERAL_PREF_CONN },
					&gattCharacPeriPrefConn,
					GATT_PROP_READ, 
					0, 
					(uint8_t *) &generalAccessConnParms[0] 
			  },

};


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint8_t geneticAcessReadAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
                             uint8_t *pValue, uint8_t *pLen, uint16_t offset, uint8_t maxLen );
static bStatus_t geneticAcessWriteAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
                                  uint8_t *pValue, uint8_t len, uint16_t offset );

/*********************************************************************
 * PROFILE CALLBACKS
 */
// Battery Service Callbacks
const gattServiceCBs_t geneticAcessCBs =
{
  NULL,
  NULL,
  geneticAcessReadAttrCB,  // Read callback function pointer
  geneticAcessWriteAttrCB, // Write callback function pointer
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
bStatus_t geneticAcess_AddService( void )
{
  uint8_t status;

  // Register GATT attribute list and CBs with GATT Server App
  status = GATTServApp_RegisterService( generalAccessTbl,
                                        GATT_NUM_ATTRS( generalAccessTbl ),
                                        &geneticAcessCBs );

  return ( status );
}


static uint8_t geneticAcessReadAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
							  uint8_t *pValue, uint8_t *pLen, uint16_t offset, uint8_t maxLen )
{
	bStatus_t status = SUCCESS;
	
	// Make sure it's not a blob operation (no attributes in the profile are long)
	if ( offset > 0 )
	{
		return ( ATT_ERR_ATTR_NOT_LONG );
	}
	
	uint16_t uuid = pAttr->pType->value.u16_val; 
	
	if ( uuid == GATT_CHARAC_DEVICE_NAME )
	{
		*pLen = sizeof(generalDevName);
		CFasm_memcpy( pValue, pAttr->pValue, sizeof(generalDevName) );
	}
	else if ( uuid == GATT_CHARAC_APPEARANCE )
	{
		*pLen = sizeof(generalAccessAppearance);
		CFasm_memcpy( pValue, pAttr->pValue, sizeof(generalAccessAppearance) );
	}
	else if ( uuid == GATT_CHARAC_PERIPHERAL_PREF_CONN )
	{
		*pLen = sizeof(generalAccessConnParms);
		CFasm_memcpy( pValue, pAttr->pValue, sizeof(generalAccessConnParms) );
	}
	else
	{
		*pLen = 0;
		status = ATT_ERR_ATTR_NOT_FOUND;
	}
	
  return ( status );
}

static bStatus_t geneticAcessWriteAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
                                  uint8_t *pValue, uint8_t len, uint16_t offset )
{
	DBG_Assert(FALSE);
	return SUCCESS;
}
#endif


