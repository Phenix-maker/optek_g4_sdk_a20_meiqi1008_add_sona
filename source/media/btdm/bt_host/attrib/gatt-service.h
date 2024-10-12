#ifndef GATT_SERVICE_H
#define GATT_SERVICE_H

/*********************************************************************
 * MACROS
 */

// The number of attribute records in a given attribute table
#define GATT_NUM_ATTRS( attrs )          ( sizeof( attrs ) / sizeof( gattAttribute_t ) )

// The handle of a service is the handle of the first attribute
#define GATT_SERVICE_HANDLE( attrs )     ( (attrs)[0].handle )

// The handle of the first included service (i = 1) is the value of the second attribute
#define GATT_INCLUDED_HANDLE( attrs, i ) ( *((uint16 *)((attrs)[(i)].pValue)) )


/*********************************************************************
 * TYPEDEFS
 */
#define bStatus_t bool
#define INVALID_CONNHANDLE			0xFFFF

#define SUCCESS						0x00
#define FAILURE						0x01


#define MAX_GATT_SERVICES			0x0C



/**
 * @defgroup GATT_SERV_APP_CB_API GATT Server App Callback API Functions
 *
 * @{
 */

/**
 * @brief   Callback function prototype to read an attribute value.
 *
 * @param   pConn - connection chan request was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be read (to be returned)
 * @param   pLen - length of data (to be returned)
 * @param   offset - offset of the first octet to be read
 * @param   maxLen - maximum length of data to be read
 *
 * @return  SUCCESS: Read was successfully.<BR>
 *          Error, otherwise: ref ATT_ERR_CODE_DEFINES.<BR>
 */
typedef bStatus_t (*pfnGATTReadAttrCB_t)( GAttrib *pConn, gattAttribute_t *pAttr,
                                          uint8_t *pValue, uint8_t *pLen, uint16_t offset,
                                          uint8_t maxLen );
/**
 * @brief   Callback function prototype to write an attribute value.
 *
 * @param   pConn - connection chan request was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   pLen - length of data
 * @param   offset - offset of the first octet to be written
 *
 * @return  SUCCESS: Write was successfully.<BR>
 *          Error, otherwise: ref ATT_ERR_CODE_DEFINES.<BR>
 */
typedef bStatus_t (*pfnGATTWriteAttrCB_t)( GAttrib *pConn, gattAttribute_t *pAttr,
                                           uint8_t *pValue, uint8_t len, uint16_t offset );
/**
 * @brief   Callback function prototype to authorize a Read or Write operation
 *          on a given attribute.
 *
 * @param   pConn - connection chan request was received on
 * @param   pAttr - pointer to attribute
 * @param   opcode - request opcode (ATT_READ_REQ or ATT_WRITE_REQ)
 *
 * @return  SUCCESS: Operation authorized.<BR>
 *          ATT_ERR_INSUFFICIENT_AUTHOR: Authorization required.<BR>
 */
typedef bStatus_t (*pfnGATTAuthorizeAttrCB_t)( GAttrib *pConn, gattAttribute_t *pAttr,
                                               uint8_t opcode );


typedef void (*GAttribServiceStart)(void);
typedef void (*GAttribServiceStop)(void);



//added by Optek
//typedef void (*GDestroyNotify)(gpointer user_data);
/**
 * GATT Structure for service callback functions - must be setup by the application
 * and used when GATTServApp_RegisterService() is called.
 */
typedef struct
{
  GAttribServiceStart  pfnAttrServiceStart;			//BLE conn
  GAttribServiceStop	pfnAttrServiceStop;		//BLE disconn
  pfnGATTReadAttrCB_t pfnReadAttrCB;           //!< Read callback function pointer
  pfnGATTWriteAttrCB_t pfnWriteAttrCB;         //!< Write callback function pointer
  pfnGATTAuthorizeAttrCB_t pfnAuthorizeAttrCB; //!< Authorization callback function pointer
} gattServiceCBs_t;



typedef struct
{
	gattAttribute_t		*pGattAttribute;
	uint16_t			startHandle;
	uint16_t			numAttrs;
	gattServiceCBs_t	*pfnServiceCB;
} gattService_t;

extern gattService_t gattService[MAX_GATT_SERVICES];

/**
 * @brief   Initialize the client characteristic configuration table.
 *
 *          Note: Each client has its own instantiation of the Client
 *                Characteristic Configuration. Reads/Writes of the Client
 *                Characteristic Configuration only only affect the
 *                configuration of that client.
 *
 * @param   connHandle - connection handle (0xFFFF for all connections).
 * @param   charCfgTbl - client characteristic configuration table.
 *
 * @return  none
 */
extern void GATTServApp_InitCharCfg( uint16_t connHandle, gattCharCfg_t *charCfgTbl );


/**
 * @brief   Register a service's attribute list and callback functions with
 *          the GATT Server Application.
 *
 * @param   pAttrs - Array of attribute records to be registered
 * @param   numAttrs - Number of attributes in array
 * @param   pServiceCBs - Service callback function pointers
 *
 * @return  SUCCESS: Service registered successfully.<BR>
 *          INVALIDPARAMETER: Invalid service field.<BR>
 *          FAILURE: Not enough attribute handles available.<BR>
 *          bleMemAllocError: Memory allocation error occurred.<BR>
 */
extern bStatus_t GATTServApp_RegisterService( gattAttribute_t *pAttrs, uint16_t numAttrs,
                                              gattServiceCBs_t *pServiceCBs );


#endif




