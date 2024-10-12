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


#include "list.h"
#include "malloc.h"
#include "g_slist.h"

#include "att.h"
#include "gattrib.h"
#include "gatt.h"

#include "gatt-service.h"

#include "../attrib/profiles/GenericAccess/genericAccess.h"
#include "../attrib/profiles/GenericAttr/genericAttr.h"
#include "../attrib/profiles/Batt/battservice.h"
#include "../attrib/profiles/keys/simplekeys.h"
#include "../attrib/profiles/Proximity/proxreporter.h"

#ifdef BTDM52_ENABLE

gattService_t gattService[MAX_GATT_SERVICES];


void GATTServApp_InitCharCfg( uint16_t connHandle, gattCharCfg_t *charCfgTbl )
{
	//DBG_Assert(FALSE);
	memset(charCfgTbl,0,sizeof(gattCharCfg_t)*GATT_MAX_NUM_CONN);
}


bStatus_t GATTServApp_RegisterService( gattAttribute_t *pAttrs, uint16_t numAttrs,
											 gattServiceCBs_t *pServiceCBs )
{
	uint16_t i,gattServiceFreeIndex,start_handles;

	gattServiceFreeIndex = 0xFFFF;
	start_handles = 0;

	for (i=0;i<MAX_GATT_SERVICES;i++) {
		if (gattService[i].startHandle == 0x0) {
			gattServiceFreeIndex = i;
			break;
		}
	}

	if (gattServiceFreeIndex == 0xFFFF) {
		DBG_Printf ("Too many GATT services\n\r");
		DBG_Assert (FALSE);
		return FAILURE;
	}

	if (gattServiceFreeIndex > 0) {
		start_handles = gattService[gattServiceFreeIndex-1].startHandle +  gattService[gattServiceFreeIndex-1].numAttrs;
	}
	else {
		start_handles = 1;
	}

	gattService[gattServiceFreeIndex].pGattAttribute = pAttrs;
	gattService[gattServiceFreeIndex].pfnServiceCB = pServiceCBs;
	gattService[gattServiceFreeIndex].startHandle = start_handles;
	gattService[gattServiceFreeIndex].numAttrs = numAttrs;

	for (i=0;i<numAttrs;i++) {
		pAttrs[i].handle=start_handles+i;
	}
	
	return SUCCESS;
}

void bt_dump_init(void);

bStatus_t GATTServApp_init(void)
{
	bt_dump_init();				//debug
	memset (gattService,0,sizeof(gattService));
}

bStatus_t	GATTServApp_open(void)
{
	uint8_t status;

	status = geneticAcess_AddService();
	DBG_Assert (status == SUCCESS);

	status = genericAttr_AddService();
	DBG_Assert (status == SUCCESS);

	//status = devInfo_AddService();
	//DBG_Assert (status == SUCCESS);

	//status = ProxReporter_AddService(PP_LINK_LOSS_SERVICE | PP_IM_ALETR_SERVICE | PP_TX_PWR_LEVEL_SERVICE);
	//DBG_Assert (status == SUCCESS);

	status = Batt_AddService();
	DBG_Assert (status == SUCCESS);

	status = simplekeys_AddService();
	status = reference_time_update_AddService();
	DBG_Assert (status == SUCCESS);

	return status;
}

bStatus_t	GATTServApp_close(void)
{
	
}

bStatus_t	GATTServApp_fini(void)
{
	memset (gattService,0,sizeof(gattService));
}

/* BLE ADV*/
const u8 ble_name[] = {"OPTEK BLE\0"};


u8 create_adv_data(u8 *ptr,u8 *ble_name,u8 name_len)
{
	u8 ad_len = 0, flags = 0;

	flags = LE_AD_GENERAL | LE_AD_NO_BREDR;

	if (flags) {
		//BT_DBG("adv flags 0x%x\r\n", flags);

		ptr[0] = 2;
		ptr[1] = EIR_FLAGS;
		ptr[2] = flags;

		ad_len += 3;
		ptr += 3;
	}

	if (name_len > 0) {
		size_t max_len = HCI_MAX_AD_LENGTH - ad_len - 2;

		if (name_len > max_len) {
			name_len = max_len;
			ptr[1] = EIR_NAME_SHORT;
		} else
			ptr[1] = EIR_NAME_COMPLETE;

		ptr[0] = name_len + 1;

		CFasm_memcpy(ptr + 2, ble_name, name_len);

		ad_len += (name_len + 2);
		ptr += (name_len + 2);
	}

#if 1
	*ptr++ = 0x05;
	*ptr++ = 0x02;
	*ptr++ = 0x0F;
	*ptr++ = 0x18;
	*ptr++ = 0xE0;
	*ptr++ = 0xFF;
 
	ad_len += 6;
#endif

	return ad_len;
}

u8 create_ad(struct hci_dev *hdev, u8 *ptr)
{
	return create_adv_data(ptr, ble_name, strlen(ble_name));

}


U16 bt_hci_dfault_mtu_cb(void)
{
	return 0x80;//23   max is 512
}

U16 bt_hci_appmax_mtu_cb(void)
{
	return 0x80; //max is 512
}

#endif