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

#include "att.h"
#include "gattrib.h"
#include "gatt.h"
#include "gatt_profile_uuid.h"

#include "gatt-service.h"

#ifdef BTDM52_ENABLE
/*********************************************************************
 * CONSTANTS
 */

// Device Information Service Parameters
#define DEVINFO_SYSTEM_ID                 0
#define DEVINFO_MODEL_NUMBER              1
#define DEVINFO_SERIAL_NUMBER             2
#define DEVINFO_FIRMWARE_REV              3
#define DEVINFO_HARDWARE_REV              4
#define DEVINFO_SOFTWARE_REV              5
#define DEVINFO_MANUFACTURER_NAME         6
#define DEVINFO_11073_CERT_DATA           7
#define DEVINFO_PNP_ID                    8

// IEEE 11073 authoritative body values
#define DEVINFO_11073_BODY_EMPTY          0
#define DEVINFO_11073_BODY_IEEE           1
#define DEVINFO_11073_BODY_CONTINUA       2
#define DEVINFO_11073_BODY_EXP            254

// System ID length
#define DEVINFO_SYSTEM_ID_LEN             8

  // PnP ID length
#define DEVINFO_PNP_ID_LEN                7


/*********************************************************************
 * Profile Attributes - variables
 */

// Device Information Service attribute
static const bt_uuid_t devInfoService = { ATT_BT_UUID_SIZE, DEVINFO_SERV_UUID };

// System ID characteristic
static const bt_uuid_t systemIDUuid = { ATT_BT_UUID_SIZE, SYSTEM_ID_UUID };
static const uint8_t devInfoSystemId[DEVINFO_SYSTEM_ID_LEN] = {7, 6, 5, 1, 2, 3, 4, 0};

// Model Number String characteristic
static const bt_uuid_t modelNumberUuid = { ATT_BT_UUID_SIZE, MODEL_NUMBER_UUID };
static const uint8_t devInfoModelNumber[] = "Model Number ABC";

// Serial Number String characteristic
static const bt_uuid_t serialNumberUuid = { ATT_BT_UUID_SIZE, SERIAL_NUMBER_UUID };
static const uint8_t devInfoSerialNumber[] = "SN 1283746";

// Firmware Revision String characteristic
static const bt_uuid_t firmwareRevUuid = { ATT_BT_UUID_SIZE, FIRMWARE_REV_UUID };
static const uint8_t devInfoFirmwareRev[] = "Firmware Rev 1.1";

// Hardware Revision String characteristic
static const bt_uuid_t hardwareRevUuid = { ATT_BT_UUID_SIZE, HARDWARE_REV_UUID };
static const uint8_t devInfoHardwareRev[] = "Hardware Rev 1.0";

// Software Revision String characteristic
static const bt_uuid_t softwareRevUuid = { ATT_BT_UUID_SIZE, SOFTWARE_REV_UUID };
static const uint8_t devInfoSoftwareRev[] = "Software Rev 1.1";

// Manufacturer Name String characteristic
static const bt_uuid_t manafactureNameUuid = { ATT_BT_UUID_SIZE, MANUFACTURER_NAME_UUID };
static const uint8_t devInfoMfrName[] = "Manufacturer: Optek";

// IEEE 11073-20601 Regulatory Certification Data List characteristic
static const bt_uuid_t IEEE11073CertUuid = { ATT_BT_UUID_SIZE, IEEE_11073_CERT_DATA_UUID };
#if 0
static const uint8_t devInfo11073Cert[] =
{
  DEVINFO_11073_BODY_EXP,      // authoritative body type
  0x00,                       // authoritative body structure type
                              // authoritative body data follows below:
  'e', 'x', 'p', 'e', 'r', 'i', 'm', 'e', 'n', 't', 'a', 'l'
};
#endif

static const uint8_t devInfo11073Cert[] = {  'e', 'x', 'p', 'e', 'r', 'i', 'm', 'e', 'n', 't', 'a', 'l'};


// System ID characteristic
static const bt_uuid_t PnpIDUuid = { ATT_BT_UUID_SIZE, PNP_ID_UUID };

#if 0
static const uint8_t devInfoPnpId[DEVINFO_PNP_ID_LEN] =
{
  1,                                      // Vendor ID source (1=Bluetooth SIG)
  0x000D,		//LO_UINT16(0x000D), HI_UINT16(0x000D),   // Vendor ID (Texas Instruments)
  0x0000,		//LO_UINT16(0x0000), HI_UINT16(0x0000),   // Product ID (vendor-specific)
  0x0110		//LO_UINT16(0x0110), HI_UINT16(0x0110)    // Product version (JJ.M.N)
};
#endif

static const uint8_t devInfoPnpId[] = {'1','0','0','0','D','0','0','0','0','0','1','1','1','0'};


/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t devInfoAttrTbl[] =
{
  // Device Information Service
  {
    //{ ATT_BT_UUID_SIZE, GATT_PRIM_SVC_UUID }, /* type */
	  &gattPrimSvc,
    GATT_PROP_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8_t *)&devInfoService                /* pValue */
  },

    // System ID Declaration
    {
      //{ ATT_BT_UUID_SIZE, GATT_CHARAC_UUID },
	  &gattCharac,
      GATT_PROP_READ,
      0,
      &gattReadProps
    },

      // System ID Value
      {
        //{ ATT_BT_UUID_SIZE, SYSTEM_ID_UUID },
		  &systemIDUuid,
        GATT_PROP_READ,
        0,
        (uint8_t *) devInfoSystemId
      },

    // Model Number String Declaration
    {
      //{ ATT_BT_UUID_SIZE, GATT_CHARAC_UUID },
			&gattCharac,
     GATT_PROP_READ,
      0,
      &gattReadProps
    },

      // Model Number Value
      {
        //{ ATT_BT_UUID_SIZE, MODEL_NUMBER_UUID },
		&modelNumberUuid,
	 GATT_PROP_READ,
        0,
        (uint8_t *) devInfoModelNumber
      },

    // Serial Number String Declaration
    {
      //{ ATT_BT_UUID_SIZE, GATT_CHARAC_UUID },
			&gattCharac,
      GATT_PROP_READ,
      0,
      &gattReadProps
    },

      // Serial Number Value
      {
        //{ ATT_BT_UUID_SIZE, SERIAL_NUMBER_UUID },
		&serialNumberUuid,
        GATT_PROP_READ,
        0,
        (uint8_t *) devInfoSerialNumber
      },

    // Firmware Revision String Declaration
    {
      //{ ATT_BT_UUID_SIZE, GATT_CHARAC_UUID },
			&gattCharac,
      GATT_PROP_READ,
      0,
      &gattReadProps
    },

      // Firmware Revision Value
      {
        //{ ATT_BT_UUID_SIZE, FIRMWARE_REV_UUID },
		&firmwareRevUuid,
        GATT_PROP_READ,
        0,
        (uint8_t *) devInfoFirmwareRev
      },

    // Hardware Revision String Declaration
    {
      //{ ATT_BT_UUID_SIZE, GATT_CHARAC_UUID },
			&gattCharac,
     GATT_PROP_READ,
      0,
      &gattReadProps
    },

      // Hardware Revision Value
      {
        //{ ATT_BT_UUID_SIZE, HARDWARE_REV_UUID },
		&hardwareRevUuid,
        GATT_PROP_READ,
        0,
        (uint8_t *) devInfoHardwareRev
      },

    // Software Revision String Declaration
    {
      //{ ATT_BT_UUID_SIZE, GATT_CHARAC_UUID },
			&gattCharac,
      GATT_PROP_READ,
      0,
      &gattReadProps
    },

      // Software Revision Value
      {
        //{ ATT_BT_UUID_SIZE, SOFTWARE_REV_UUID },
		&softwareRevUuid,
        GATT_PROP_READ,
        0,
        (uint8_t *) devInfoSoftwareRev
      },

    // Manufacturer Name String Declaration
    {
      //{ ATT_BT_UUID_SIZE, GATT_CHARAC_UUID },
			&gattCharac,
      GATT_PROP_READ,
      0,
      &gattReadProps
    },

      // Manufacturer Name Value
      {
        //{ ATT_BT_UUID_SIZE, MANUFACTURER_NAME_UUID },
	&manafactureNameUuid,
	 GATT_PROP_READ,
        0,
        (uint8_t *) devInfoMfrName
      },

    // IEEE 11073-20601 Regulatory Certification Data List Declaration
    {
      //{ ATT_BT_UUID_SIZE, GATT_CHARAC_UUID },
			&gattCharac,
     GATT_PROP_READ,
      0,
      &gattReadProps
    },

      // IEEE 11073-20601 Regulatory Certification Data List Value
      {
        //{ ATT_BT_UUID_SIZE, IEEE_11073_CERT_DATA_UUID },
	&IEEE11073CertUuid,
	 GATT_PROP_READ,
        0,
        (uint8_t *) devInfo11073Cert
      },

    // PnP ID Declaration
    {
      //{ ATT_BT_UUID_SIZE, GATT_CHARAC_UUID },
			&gattCharac,
      GATT_PROP_READ,
      0,
      &gattReadProps
    },

      // PnP ID Value
      {
        //{ ATT_BT_UUID_SIZE, PNP_ID_UUID },
		&PnpIDUuid,
        GATT_PROP_READ,
        0,
        (uint8_t *) devInfoPnpId
      }
};


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint8_t devInfoReadAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
                             uint8_t *pValue, uint8_t *pLen, uint16_t offset, uint8_t maxLen );
static bStatus_t devInfoWriteAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
                                  uint8_t *pValue, uint8_t len, uint16_t offset );
//static void battNotifyCB( linkDBItem_t *pLinkItem );
//static uint8_t battMeasure( void );
//static void battNotifyLevel( void );

/*********************************************************************
 * PROFILE CALLBACKS
 */
// Battery Service Callbacks
const gattServiceCBs_t devInfoCBs =
{
  NULL,
  NULL,
  devInfoReadAttrCB,  // Read callback function pointer
  devInfoWriteAttrCB, // Write callback function pointer
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
bStatus_t devInfo_AddService( void )
{
  uint8_t status;

  // Register GATT attribute list and CBs with GATT Server App
  status = GATTServApp_RegisterService( devInfoAttrTbl,
                                        GATT_NUM_ATTRS( devInfoAttrTbl ),
                                        &devInfoCBs );

  return ( status );
}


static uint8_t devInfoReadAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
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
	case SYSTEM_ID_UUID:
		*pLen = sizeof(devInfoSystemId);
		CFasm_memcpy( pValue, pAttr->pValue, sizeof(devInfoSystemId) );
		break;
	case MODEL_NUMBER_UUID:
		*pLen = sizeof(devInfoModelNumber);
		CFasm_memcpy( pValue, pAttr->pValue, sizeof(devInfoModelNumber) );
		break;
	case SERIAL_NUMBER_UUID:
		*pLen = sizeof(devInfoSerialNumber);
		CFasm_memcpy( pValue, pAttr->pValue, sizeof(devInfoSerialNumber) );
		break;
	case FIRMWARE_REV_UUID:
		*pLen = sizeof(devInfoFirmwareRev);
		CFasm_memcpy( pValue, pAttr->pValue, sizeof(devInfoFirmwareRev) );
		break;
	case HARDWARE_REV_UUID:
		*pLen = sizeof(devInfoHardwareRev);
		CFasm_memcpy( pValue, pAttr->pValue, sizeof(devInfoHardwareRev) );
		break;
	case SOFTWARE_REV_UUID:
		*pLen = sizeof(devInfoSoftwareRev);
		CFasm_memcpy( pValue, pAttr->pValue, sizeof(devInfoSoftwareRev) );
		break;
	case MANUFACTURER_NAME_UUID:
		*pLen = sizeof(devInfoMfrName);
		CFasm_memcpy( pValue, pAttr->pValue, sizeof(devInfoMfrName) );
		break;
	case IEEE_11073_CERT_DATA_UUID:
		//*pLen = sizeof(devInfo11073Cert);
		//CFasm_memcpy( pValue, pAttr->pValue, sizeof(devInfo11073Cert) );
		*pLen = sizeof(devInfoMfrName);
		CFasm_memcpy( pValue, pAttr->pValue, sizeof(devInfoMfrName) );
		break;
	case PNP_ID_UUID:
		//*pLen = sizeof(devInfoPnpId);
		//CFasm_memcpy( pValue, pAttr->pValue, sizeof(devInfoPnpId) );
		*pLen = sizeof(devInfoMfrName);
		CFasm_memcpy( pValue, pAttr->pValue, sizeof(devInfoMfrName) );
		break;
	default:
		DBG_Assert (FALSE);
		*pLen = 0;
		status = ATT_ERR_ATTR_NOT_FOUND;
		break;
	}
 
	return ( status );

}	

static bStatus_t devInfoWriteAttrCB( GAttrib *pConn, gattAttribute_t *pAttr,
                                  uint8_t *pValue, uint8_t len, uint16_t offset )
{
	asm ("break 1,1");
	return SUCCESS;
}
#endif