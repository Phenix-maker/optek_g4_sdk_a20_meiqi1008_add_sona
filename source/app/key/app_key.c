#include "os_config.h"

#include "c_def.h"
#include "debug.h"
#include "oem.h"

#include "remote.h"

#include "hal_gpio.h"

#include "app_key.h"
#include "app_key_oem.h"



/*
 * main board key map 
 *****************************************************************************************
 */

static const KEY_AD_ENTRY *oem_main_kbd_map[] = {
	oem_main_kbd_entry0,
	oem_main_kbd_entry1,
	oem_main_kbd_entry2
};

APP_KEY vAdMapMainKey (U8 ad_channel, U16 ad_val)
{
	KEY_AD_ENTRY *p ;
	APP_KEY key = APP_KEY_INVALID;

	for (p = oem_main_kbd_map[ad_channel]; p->key != APP_KEY_INVALID; p++)
	{
		if ( (ad_val >= p->ad_min) && (ad_val <= p->ad_max) )
		{
			key = p->key;
			break;
		}
	}

	return key;
}

const KEY_GPIO_MAP oem_main_gpio_map = {
	(KEY_GPIO_ENTRY *) oem_main_gpio_kbd_entry,
	(sizeof (oem_main_gpio_kbd_entry) / sizeof (oem_main_gpio_kbd_entry[0]))
};

void app_gpio_key_init(void)
{
	int i;
	KEY_GPIO_ENTRY * p = oem_main_gpio_map.key_gpio_entry;
	U16 entries = oem_main_gpio_map.entries;

	for (i=0; i<entries; i++)
	{
		hal_gpio_mux_func_select( p->gpio_index, MUX_SEL_GPIO_INPUT);
		p++;
	}
}

/**
 ****************************************************************************************
 * @brief Convert gpio pin status to app key id
 *
 * @param[in] gpio_group   group that gpios in
 * @param[in] gpio_status  gpios of this group's status
 * @param[in out] app_keys     buffer to store output keyids of the keys is/are in down
 * @param[in] max_simult   maximum simultaneous keys
 *
 * @return The number key in down status
 *
 ****************************************************************************************
 */
U8 vGpioMapMainKey(U32 gpio_group, U32 gpio_status, APP_KEY *app_keys, U8 max_simult)
{
	U16 i;
	KEY_GPIO_ENTRY * p = oem_main_gpio_map.key_gpio_entry;
	U16 entries = oem_main_gpio_map.entries;
	U32 gpio_mask;
	U8  keys_cnt = 0;

	for (i=0; i<entries; i++)
	{
		if (p->gpio_index >= (gpio_group << 5))
		{
			gpio_mask = 1 << (p->gpio_index - (gpio_group << 5));
		}

		if ((gpio_status & gpio_mask) == p->active_lever)
		{
			*app_keys++ = p->key;

			if (++keys_cnt > max_simult)
				break;
		}
		p++;
	}

	return keys_cnt;
}

#ifdef POWER_KEY_ENABLE
const KEY_GPIO_MAP oem_main_sreg_gpio_map = {
	(KEY_GPIO_ENTRY *) oem_main_sreg_gpio_kbd_entry,
	(sizeof (oem_main_sreg_gpio_kbd_entry) / sizeof (oem_main_sreg_gpio_kbd_entry[0]))
};

U8 vGpioMapMainSregKey(U32 gpio_group, U32 gpio_status, APP_KEY *app_keys, U8 max_simult)
{
	U16 i;
	KEY_GPIO_ENTRY * p = oem_main_sreg_gpio_map.key_gpio_entry;
	U16 entries = oem_main_sreg_gpio_map.entries;
	U32 gpio_mask;
	U8  keys_cnt = 0;

	for (i=0; i<entries; i++)
	{
		if (p->gpio_index >= (gpio_group-1))
		{
			gpio_mask = 1 << (p->gpio_index - (gpio_group-1));
		}

		if ((gpio_status & gpio_mask) == p->active_lever)
		{
			*app_keys++ = p->key;

			if (++keys_cnt > max_simult)
				break;
		}
		p++;
	}

	return keys_cnt;
}
#endif

const KEY_MIX_MAP oem_mix_map = {
	(KEY_MIX_ENTRY *) oem_main_mix_entry,
	(sizeof (oem_main_mix_entry) / sizeof (oem_main_mix_entry[0]))
};

/**
 ****************************************************************************************
 * @brief key mixing
 *
 * @param[in] app_keys     the keys is/are in down
 * @param[in] key_cnt      key count
 *
 * @return the key id after mixing
 *
 ****************************************************************************************
 */
APP_KEY vMainKeyMix(APP_KEY *app_keys, U8 key_cnt)
{
	U16 i,j,k;
	KEY_MIX_ENTRY * p = oem_mix_map.key_mix_entry;
	U16 entries = oem_mix_map.entries;
	APP_KEY key_id = APP_KEY_INVALID;

	if (key_cnt == 0)
		return APP_KEY_INVALID;

	for (i=0; i<entries; i++)
	{
		if (key_id == APP_KEY_INVALID)
		{
			//check dual keys mixing
			for (j=0; j<key_cnt; j++)
			{
				if (p->key_id1 == app_keys[j])
				{
					for (k=0; k<key_cnt; k++)
					{
						if (p->key_id2 == app_keys[k])
						{
							key_id = p->mix_key;
							break;
						}
					}
					if (key_id != APP_KEY_INVALID)
						break;				
				}
			}
		}
		else
		{
			//check 3(4,5...) keys mixing
			if (p->key_id1 == key_id)
			{
				for (k=0; k<key_cnt; k++)
				{
					if (p->key_id2 == app_keys[k])
					{
						key_id = p->mix_key;
						break;
					}
				}
			}
			else if (p->key_id2 == key_id)
			{
				for (k=0; k<key_cnt; k++)
				{
					if (p->key_id1 == app_keys[k])
					{
						key_id = p->mix_key;
						break;
					}
				}				
			}
		}

		p++;
	}

	return key_id;
}

/*
 * remote key map 
 *****************************************************************************************
 */

const KEY_REMOTE_MAP oem_remote_map = {
	(KEY_REMOTE_ENTRY *) oem_remote_entry,
	(sizeof (oem_remote_entry) / sizeof (oem_remote_entry[0]))
};

APP_KEY vRemoteMapMainKey(U32 rmtkey_code)
{
	U16 i;
	KEY_REMOTE_ENTRY * p = oem_remote_map.key_remote_entry;
	U16 entries = oem_remote_map.entries;
	APP_KEY key_id = APP_KEY_INVALID;

	for (i=0; i<entries; i++)
	{
		if (rmtkey_code  == p->rmtkey_val)
		{
			key_id = p->key_id;
			break;
		}
		p++;
	}

	return key_id;
}
