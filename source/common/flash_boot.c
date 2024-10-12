// Copyright (C) Optek. 2008
//
//! \file flash_boot.c
//! \brief
//! \version 0.1
//! \date Feb, 2007
//!
//!
//
#include "c_def.h"
#include "debug.h"
#include "oem.h"

#include "regmap.h"

#include "mem_reloc.h"

#include "hw_gpio.h"
#include "hw_uart.h"

#include "CFasm_memcpy.h"
#include "flash_boot.h"


#ifdef SPI_FLASH_ENABLE
#ifdef SPI_FLASH_WINBOND
#include "SpiFlash_W25QXX.h"
#endif
#endif


//#define LIBKERNEL_TEXT_LOAD

//#define ALL_TEXT_LOAD
//#define ALL_RODATA_LOAD

#ifdef DOLBY_DDPLUS71_DEC_ENABLE
//#define DDPLUS71_LIB_RODATA_LOADER
#define OPTEK_LIB_DSP_EC3_TEXT_LOADER
#endif

#ifdef DTS_DMP_DEC_EANBLE
#define OPTEK_LIB_DTS_DMP_TEXT_LOADER
#endif

//#define ATMOS_CODEC_LIB_RODATA_LOAD
//#define ATMOS_CODEC_LIB_TEXT_LOAD

//#define XA_MAT_DEC_LIB_TEXT_LOADER
//#define XA_RSRC_DEC_LIB_TEXT_LOADER
//#define XA_TRUEHD_DEC_LIB_TEXT_LOADER

//#undef BOOT_FROM_FLASH


//#pragma section boot_copy_code ".copyflash"
//#pragma use_section boot_copy_code InitSdram, CopyFlash


void System_Stack_Space_Init(void) __FLASH_BOOT_TEXT;
void Sdram_Init(void) __FLASH_BOOT_TEXT;

void SpiFlash_Enable(void) __FLASH_BOOT_TEXT;
void SpiFlash_4Bit_Enable(void) __FLASH_BOOT_TEXT;

void Flash_Boot_Copy(void) __FLASH_BOOT_TEXT;
void Flash_Boot_Switch(void) __FLASH_BOOT_TEXT;
void DataCache_Clean(void) __INTERNAL_RAM_TEXT;

void main_power_on(void) __FLASH_BOOT_TEXT;
void flash_boot_atmos_xa_lib_text_copy(void)  __FLASH_BOOT_TEXT;
void flash_boot_atmos_xa_lib_rodata_copy(void)  __FLASH_BOOT_TEXT;


/* Normal code externs */
extern U32 __internal_ram_1_rom_start[];
extern U32 __internal_ram_1_ram_start[];
extern U32 __internal_ram_1_ram_end[];

extern U32 __interal_ram_1_rom_start[];
extern U32 __interal_ram_1_ram_start[];
extern U32 __interal_ram_1_ram_end[];

extern U32 __libkernel_rom_start[];
extern U32 __libkernel_ram_start[];
extern U32 __libkernel_ram_end[];

extern U32 __flash_prog_rom_start[];
extern U32 __flash_prog_ram_start[];
extern U32 __flash_prog_ram_end[];

extern U32 __data_rom_start[];
extern U32 __data_ram_start[];
extern U32 __data_ram_end[];

extern U32 __all_text_rom_start[];
extern U32 __all_text_ram_start[];
extern U32 __all_text_ram_end[];

extern U32 _bss_start[];
extern U32 _bss_end[];

extern U32 _clib_bss_start[];
extern U32 _clib_bss_end[];

extern U32 __xa_libc_text_rom_start[];
extern U32 __xa_libc_text_ram_start[];
extern U32 __xa_libc_text_ram_end[];

extern U32 __all_rodata_rom_start[];
extern U32 __all_rodata_ram_start[];
extern U32 __all_rodata_ram_end[];

extern U32 __codec_rodata_rom_start[];
extern U32 __codec_rodata_ram_start[];
extern U32 __codec_rodata_ram_end[];


extern U32 __xa_dd_ac3_dec_lib_rodata_ram_start[];
extern U32 __xa_dd_ac3_dec_lib_rodata_ram_start[];
extern U32 __xa_dd_ac3_dec_lib_rodata_ram_end[];

extern U32 __xa_dd_ac3_dec_lib_text_rom_start[];
extern U32 __xa_dd_ac3_dec_lib_text_ram_start[];
extern U32 __xa_dd_ac3_dec_lib_text_ram_end[];

extern U32 __xa_dd_ec3_dec_lib_rodata_rom_start[];
extern U32 __xa_dd_ec3_dec_lib_rodata_ram_start[];
extern U32 __xa_dd_ec3_dec_lib_rodata_ram_end[];

extern U32 __xa_dd_ec3_dec_lib_text_rom_start[];
extern U32 __xa_dd_ec3_dec_lib_text_ram_start[];
extern U32 __xa_dd_ec3_dec_lib_text_ram_end[];

extern U32 __xa_dts_dmp_dec_lib_rodata_rom_start[];
extern U32 __xa_dts_dmp_dec_lib_rodata_ram_start[];
extern U32 __xa_dts_dmp_dec_lib_rodata_ram_end[];

extern U32 __xa_dts_dmp_dec_lib_text_rom_start[];
extern U32 __xa_dts_dmp_dec_lib_text_ram_start[];
extern U32 __xa_dts_dmp_dec_lib_text_ram_end[];


extern U32 __xa_libc_text_rom_start[];
extern U32 __xa_libc_text_ram_start[];
extern U32 __xa_libc_text_ram_end[];

extern U32 __xa_dapv2_lib_text_rom_start[];
extern U32 __xa_dapv2_lib_text_start[];
extern U32 __xa_dapv2_lib_text_end[];

extern U32 __xa_mat_dec_lib_text_rom_start[];
extern U32 __xa_mat_dec_lib_text_start[];
extern U32 __xa_mat_dec_lib_text_end[];

extern U32 __xa_ddplus_udc_lib_text_rom_start[];
extern U32 __xa_ddplus_udc_lib_text_start[];
extern U32 __xa_ddplus_udc_lib_text_end[];

extern U32 __xa_dmx_lib_text_rom_start[];
extern U32 __xa_dmx_lib_text_start[];
extern U32 __xa_dmx_lib_text_end[];

extern U32 __xa_rsrc_lib_text_rom_start[];
extern U32 __xa_rsrc_lib_text_start[];
extern U32 __xa_xa_rsrc_lib_text_end[];

extern U32 __xa_oar_lib_text_rom_start[];
extern U32 __xa_oar_lib_text_start[];
extern U32 __xa_oar_lib_text_end[];

extern U32 __xa_truehd_dec_lib_text_rom_start[];
extern U32 __xa_truehd_dec_lib_text_start[];
extern U32 __xa_truehd_dec_lib_text_end[];

extern U32 __xa_fader_lib_text_rom_start[];
extern U32 __xa_fader_lib_text_start[];
extern U32 __xa_fader_lib_text_end[];

extern U32 __xa_dlb_intrinsics_lib_text_rom_start[];
extern U32 __xa_dlb_intrinsics_lib_text_start[];
extern U32 __xa_dlb_intrinsics_lib_text_end[];


extern U32 __xa_dapv2_lib_rodata_rom_start[];
extern U32 __xa_dapv2_lib_rodata_start[];
extern U32 __xa_dapv2_lib_rodata_end[];

extern U32 __xa_mat_dec_lib_rodata_rom_start[];
extern U32 __xa_mat_dec_lib_rodata_start[];
extern U32 __xa_mat_dec_lib_rodata_end[];

extern U32 __xa_ddplus_udc_lib_rodata_rom_start[];
extern U32 __xa_ddplus_udc_lib_rodata_start[];
extern U32 __xa_ddplus_udc_lib_rodata_end[];

extern U32 __xa_dmx_lib_rodata_rom_start[];
extern U32 __xa_dmx_lib_rodata_start[];
extern U32 __xa_dmx_lib_rodata_end[];

extern U32 __xa_rsrc_lib_rodata_rom_start[];
extern U32 __xa_rsrc_lib_rodata_start[];
extern U32 __xa_rsrc_lib_rodata_end[];

extern U32 __xa_oar_lib_rodata_rom_start[];
extern U32 __xa_oar_lib_rodata_start[];
extern U32 __xa_oar_lib_rodata_end[];

extern U32 __xa_truehd_dec_lib_rodata_rom_start[];
extern U32 __xa_truehd_dec_lib_rodata_start[];
extern U32 __xa_truehd_dec_lib_rodata_end[];

extern U32 __xa_fader_lib_rodata_rom_start[];
extern U32 __xa_fader_lib_rodata_start[];
extern U32 __xa_fader_lib_rodata_end[];

extern U32 __xa_dlb_intrinsics_lib_rodata_rom_start[];
extern U32 __xa_dlb_intrinsics_lib_rodata_start[];
extern U32 __xa_dlb_intrinsics_lib_rodata_end[];

extern U32 __xa_libc_rodata_rom_start[];
extern U32 __xa_libc_rodata_text_start[];
extern U32 __xa_libc_rodata_end[];

extern U32 __dsrc_data_ram_start[];
extern U32 __dsrc_data_ram_end[];
extern U32 __dsrc_data_rom_start[];
extern U32 __dsp_critical_sec_rom_start[];
extern U32 __dsp_critical_sec_ram_start[];
extern U32 __dsp_critical_sec_ram_end[];


extern U32 SYSTEM_STACK_START[];
extern U32 SYSTEM_STACK_END[];

U8 app_defalt_poweron_souce(void);

#define SYSTEM_STACK_SPACE_TEST

void System_Stack_Space_Init(void)
{

}

/*******************************************************************************
 *
 *   FUNCTION NAME - Sdram_Init
 *
 *   ARGUMENTS     - None
 *
 *   RETURN VALUE  - None
 *
 *******************************************************************************
 *
 *   DESCRIPTION
 *   This function initializes the SDRAM.
 *
 ******************************************************************************/

#ifdef OPTEK_SOC2_VERSION
#define SDRAM_PIN_GPIO1_MASK     0xFFFFF800
#define SDRAM_PIN_GPIO2_MASK     0x000FFFFF
#else
#define SDRAM_PIN_GPIO1_MASK     0xFFFFF800
#define SDRAM_PIN_GPIO2_MASK     0xFFFFFFFF
#endif

#define SDRAM_CKE_PIN        GPIO43_MASK

#define SDRAM_CKE_LOW       GPIO1_FUNCTION_ENABLE2(SDRAM_CKE_PIN); \
							GPIO1_OUTPUT_ENABLE(SDRAM_CKE_PIN); \
							GPIO1_OUTPUT_CLR(SDRAM_CKE_PIN)

#define SDRAM_CKE_HIGH      GPIO1_FUNCTION_ENABLE2(SDRAM_CKE_PIN); \
							GPIO1_OUTPUT_ENABLE(SDRAM_CKE_PIN); \
							GPIO1_OUTPUT_SET(SDRAM_CKE_PIN)

#define SDRAM_CKE_INPUT     GPIO1_FUNCTION_ENABLE2(SDRAM_CKE_PIN); \
							GPIO1_OUTPUT_DISABLE(SDRAM_CKE_PIN)

void Sdram_Init(void)
{

}

void SpiFlash_Enable(void)
{

}

//Enable 4 bit Flash
#define SPI_FLASH_BUSY_MASK    0x01

void SpiFlash_4Bit_Enable(void)
{
	volatile U8 temp, status;
	volatile U8 i,j;

	temp = ADDR_SPI_STATUS2;

	for (i=0; i<3; i++)
	{
		if ( (temp & 0x02) == 0x00 )
		{
			//spi flash is 1-bit mode, set it to 4-bit
			ADDR_SPI_WR_ENABLE = 0x00;

			SFLASH_QUAD_ENABLE;
		
			for (j=0; j<100; j++)
			{
				delay_loop(48);
				status = ADDR_SPI_STATUS & SPI_FLASH_BUSY_MASK;
				if (!(status == SPI_FLASH_BUSY_MASK))
				{
					break;
				}
			}

			temp = ADDR_SPI_STATUS2;
		}
		else
		{
			break;
		}
	}
}

extern U32 SDRAM_START_ADDR[];
extern U32 SFLASH_START_ADDR[];

void DataCache_Clean(void)
{

}

/*******************************************************************************
 *
 *   FUNCTION NAME - CopyFlash
 *
 *   ARGUMENTS     - None
 *
 *   RETURN VALUE  - None
 *
 *******************************************************************************
 *
 *   DESCRIPTION
 *   This function copies sections from Flash memory to SDRAM and SRAM, as
 *   per sections defined in the memory control file.
 *
 ******************************************************************************/

extern U8 SDRAM_ICON_RODATA_START[];
extern U8 SDRAM_ICON_RODATA_END[];
extern U8 ROM_ICON_RODATA_START[];
extern U8 ROM_ICON_RODATA_END[];

extern U8 SDRAM_FONT_RODATA_START[];
extern U8 SDRAM_FONT_RODATA_END[];
extern U8 ROM_FONT_RODATA_START[];
extern U8 ROM_FONT_RODATA_END[];


#ifdef ATMOS_CODEC_ENABLE
void flash_boot_atmos_xa_lib_text_copy(void)
{
	volatile U32 count;
	volatile U32 len;

	#ifdef ATMOS_CODEC_LIB_TEXT_LOAD
	
	#if 0
	//.xa_dapv2_lib_text
	if (!(__xa_dapv2_lib_text_start == __xa_dapv2_lib_text_rom_start))
	{
		len = __xa_dapv2_lib_text_end - __xa_dapv2_lib_text_start;
		#if 0
		CFasm_memcpy(__xa_dapv2_lib_text_start, __xa_dapv2_lib_text_rom_start, len*4);
		#else
		for (count = 0; count < len; count++)
		{
			__xa_dapv2_lib_text_start[count] = __xa_dapv2_lib_text_rom_start[count];
		}
		#endif
	}
	#endif

	#ifdef XA_MAT_DEC_LIB_TEXT_LOADER
	//.xa_mat_dec_lib_text
	if (!(__xa_mat_dec_lib_text_start == __xa_mat_dec_lib_text_rom_start))
	{
	    len = __xa_mat_dec_lib_text_end - __xa_mat_dec_lib_text_start;
		#if 0
		CFasm_memcpy(__xa_mat_dec_lib_text_start, __xa_mat_dec_lib_text_rom_start, len*4);
		#else
		for (count = 0; count < len; count++)
		{
			__xa_mat_dec_lib_text_start[count] = __xa_mat_dec_lib_text_rom_start[count];
		}
		#endif
	}
	#endif

	#if 0
	//.xa_ddplus_udc_lib_text
	if (!(__xa_ddplus_udc_lib_text_start == __xa_ddplus_udc_lib_text_rom_start))
	{
	    len = __xa_ddplus_udc_lib_text_end - __xa_ddplus_udc_lib_text_start;
		#if 0
		CFasm_memcpy(__xa_ddplus_udc_lib_text_start, __xa_ddplus_udc_lib_text_rom_start, len*4);
		#else
		for (count = 0; count < len; count++)
		{
			__xa_ddplus_udc_lib_text_start[count] = __xa_ddplus_udc_lib_text_rom_start[count];
		}
		#endif
	}
	#endif

	#if 0
	//.xa_dmx_lib_text
	if (!(__xa_dmx_lib_text_start == __xa_dmx_lib_text_rom_start))
	{
	    len = __xa_dmx_lib_text_end - __xa_dmx_lib_text_start;
		#if 0
		CFasm_memcpy(__xa_dmx_lib_text_start, __xa_dmx_lib_text_rom_start, len*4);
		#else
		for (count = 0; count < len; count++)
		{
			__xa_dmx_lib_text_start[count] = __xa_dmx_lib_text_rom_start[count];
		}
		#endif
	}
	#endif

	#ifdef XA_RSRC_DEC_LIB_TEXT_LOADER
	//.xa_rsrc_lib_text
	if (!(__xa_rsrc_lib_text_start == __xa_rsrc_lib_text_rom_start))
	{
	    len = __xa_xa_rsrc_lib_text_end - __xa_rsrc_lib_text_start;
		#if 0
		CFasm_memcpy(__xa_rsrc_lib_text_start, __xa_rsrc_lib_text_rom_start, len*4);
		#else
		for (count = 0; count < len; count++)
		{
			__xa_rsrc_lib_text_start[count] = __xa_rsrc_lib_text_rom_start[count];
		}
		#endif
	}
	#endif

	#if 0
	//.xa_oar_lib_text
	if (!(__xa_oar_lib_text_start == __xa_oar_lib_text_rom_start))
	{
		len = __xa_oar_lib_text_end - __xa_oar_lib_text_start;
		#if 0
		CFasm_memcpy(__xa_oar_lib_text_start, __xa_oar_lib_text_rom_start, len*4);
		#else
		for (count = 0; count < len; count++)
		{
			__xa_oar_lib_text_start[count] = __xa_oar_lib_text_rom_start[count];
		}
		#endif
	}
	#endif

	#ifdef XA_TRUEHD_DEC_LIB_TEXT_LOADER
	//.xa_truehd_dec_lib_text
	if (!(__xa_truehd_dec_lib_text_start == __xa_truehd_dec_lib_text_rom_start))
	{
		len = __xa_truehd_dec_lib_text_end - __xa_truehd_dec_lib_text_start;
		#if 0
		CFasm_memcpy(__xa_truehd_dec_lib_text_start, __xa_truehd_dec_lib_text_rom_start, len*4);
		#else
		for (count = 0; count < len; count++)
		{
			__xa_truehd_dec_lib_text_start[count] = __xa_truehd_dec_lib_text_rom_start[count];
		}
		#endif
 	}
	#endif

	#if 0
	//.xa_fader_lib_text
	if (!(__xa_fader_lib_text_start == __xa_fader_lib_text_rom_start))
	{
	    len = __xa_fader_lib_text_end - __xa_fader_lib_text_start;
		#if 0
		CFasm_memcpy(__xa_fader_lib_text_start, __xa_fader_lib_text_rom_start, len*4);
		#else
		for (count = 0; count < len; count++)
		{
			__xa_fader_lib_text_start[count] = __xa_fader_lib_text_rom_start[count];
		}
		#endif
 	}
	#endif

	#if 0
	//.xa_dlb_intrinsics_lib_text
	if (!(__xa_dlb_intrinsics_lib_text_start == __xa_dlb_intrinsics_lib_text_rom_start))
	{
	    len = __xa_dlb_intrinsics_lib_text_end - __xa_dlb_intrinsics_lib_text_start;
		#if 0
		CFasm_memcpy(__xa_dlb_intrinsics_lib_text_start, __xa_dlb_intrinsics_lib_text_rom_start, len*4);
		#else
		for (count = 0; count < len; count++)
		{
			__xa_dlb_intrinsics_lib_text_start[count] = __xa_dlb_intrinsics_lib_text_rom_start[count];
		}
		#endif
	}
	#endif

	#endif //ATMOS_CODEC_LIB_TEXT_LOAD
}

void flash_boot_atmos_xa_lib_rodata_copy(void)
{
	volatile U32 count;
	volatile U32 len;

	#ifdef ATMOS_CODEC_LIB_RODATA_LOAD
	//.xa_dapv2_lib_rodata
	if ( (__xa_dapv2_lib_rodata_start != __xa_dapv2_lib_rodata_rom_start) )
	{
	    len = __xa_dapv2_lib_rodata_end - __xa_dapv2_lib_rodata_start;
		#if 0
		CFasm_memcpy(__xa_dapv2_lib_rodata_start, __xa_dapv2_lib_rodata_rom_start, len*4);
		#else
		for (count = 0; count < len; count++)
		{
			__xa_dapv2_lib_rodata_start[count] = __xa_dapv2_lib_rodata_rom_start[count];
		}
		#endif
	}

	//.xa_mat_dec_lib_rodata
	if ( (__xa_mat_dec_lib_rodata_start != __xa_mat_dec_lib_rodata_rom_start) )
	{
	    len = __xa_mat_dec_lib_rodata_end - __xa_mat_dec_lib_rodata_start;
		#if 0
		CFasm_memcpy(__xa_mat_dec_lib_rodata_start, __xa_mat_dec_lib_rodata_rom_start, len*4);
		#else
		for (count = 0; count < len; count++)
		{
			__xa_mat_dec_lib_rodata_start[count] = __xa_mat_dec_lib_rodata_rom_start[count];
		}
		#endif
	}

	//.xa_ddplus_udc_lib_rodata
	if (!(__xa_ddplus_udc_lib_rodata_start == __xa_ddplus_udc_lib_rodata_rom_start))
	{
	    len = __xa_ddplus_udc_lib_rodata_end - __xa_ddplus_udc_lib_rodata_start;
		#if 0
		CFasm_memcpy(__xa_ddplus_udc_lib_rodata_start, __xa_ddplus_udc_lib_rodata_rom_start, len*4);
		#else
		for (count = 0; count < len; count++)
		{
			__xa_ddplus_udc_lib_rodata_start[count] = __xa_ddplus_udc_lib_rodata_rom_start[count];
		}
		#endif
 	}

	//.xa_dmx_lib_rodata
	if (!(__xa_dmx_lib_rodata_start == __xa_dmx_lib_rodata_rom_start))
	{
		len = __xa_dmx_lib_rodata_end - __xa_dmx_lib_rodata_start;
		#if 0
		CFasm_memcpy(__xa_dmx_lib_rodata_start, __xa_dmx_lib_rodata_rom_start, len*4);
		#else
		for (count = 0; count < len; count++)
		{
			__xa_dmx_lib_rodata_start[count] = __xa_dmx_lib_rodata_rom_start[count];
		}
		#endif
 	}

	//.xa_dapv2_lib_rodata
	if (!(__xa_rsrc_lib_rodata_start == __xa_rsrc_lib_rodata_rom_start))
	{
		len = __xa_rsrc_lib_rodata_end - __xa_rsrc_lib_rodata_start;
		#if 0
		CFasm_memcpy(__xa_rsrc_lib_rodata_start, __xa_rsrc_lib_rodata_rom_start, len*4);
		#else
		for (count = 0; count < len; count++)
		{
			__xa_rsrc_lib_rodata_start[count] = __xa_rsrc_lib_rodata_rom_start[count];
		}
		#endif
 	}

	//.xa_oar_lib_rodata
	if (!(__xa_oar_lib_rodata_start == __xa_oar_lib_rodata_rom_start))
	{
	    len = __xa_oar_lib_rodata_end - __xa_oar_lib_rodata_start;
		#if 0
		CFasm_memcpy(__xa_oar_lib_rodata_start, __xa_oar_lib_rodata_rom_start, len*4);
		#else
		for (count = 0; count < len; count++)
		{
			__xa_oar_lib_rodata_start[count] = __xa_oar_lib_rodata_rom_start[count];
		}
		#endif
	}

	//.xa_truehd_dec_lib_rodata
	if (!(__xa_truehd_dec_lib_rodata_start == __xa_truehd_dec_lib_rodata_rom_start))
	{
	    len = __xa_truehd_dec_lib_rodata_end - __xa_truehd_dec_lib_rodata_start;
		#if 0
		CFasm_memcpy(__xa_truehd_dec_lib_rodata_start, __xa_truehd_dec_lib_rodata_rom_start, len*4);
		#else
		for (count = 0; count < len; count++)
		{
			__xa_truehd_dec_lib_rodata_start[count] = __xa_truehd_dec_lib_rodata_rom_start[count];
		}
		#endif
	}  

	//.xa_fader_lib_rodata
	if (!(__xa_fader_lib_rodata_start == __xa_fader_lib_rodata_rom_start))
	{
		len = __xa_fader_lib_rodata_end - __xa_fader_lib_rodata_start;
		#if 0
		CFasm_memcpy(__xa_fader_lib_rodata_start, __xa_fader_lib_rodata_rom_start, len*4);
		#else
		for (count = 0; count < len; count++)
		{
			__xa_fader_lib_rodata_start[count] = __xa_fader_lib_rodata_rom_start[count];
		}
		#endif
	}  

	//.xa_dlb_intrinsics_lib_rodata
	if (!(__xa_dlb_intrinsics_lib_rodata_start == __xa_dlb_intrinsics_lib_rodata_rom_start))
	{
		len = __xa_dlb_intrinsics_lib_rodata_end - __xa_dlb_intrinsics_lib_rodata_start;
		#if 0
		CFasm_memcpy(__xa_dlb_intrinsics_lib_rodata_start, __xa_dlb_intrinsics_lib_rodata_rom_start, len*4);
		#else
		for (count = 0; count < len; count++)
		{
			__xa_dlb_intrinsics_lib_rodata_start[count] = __xa_dlb_intrinsics_lib_rodata_rom_start[count];
		}
		#endif
	}

	//.xa_libc_rodata
	if (!(__xa_libc_rodata_text_start == __xa_libc_rodata_rom_start))
	{
		len = __xa_libc_rodata_end - __xa_libc_rodata_text_start;
		#if 0
		CFasm_memcpy(__xa_libc_rodata_text_start, __xa_libc_rodata_rom_start, len*4);
		#else
		for (count = 0; count < len; count++)
		{
			__xa_libc_rodata_text_start[count] = __xa_libc_rodata_rom_start[count];
		}
		#endif
 	}
 	#endif
}
#endif //ATMOS_CODEC_ENABLE


void Flash_Boot_Copy(void)
{
	volatile U32 count;
	volatile U32 len;

	//.xa_libc_text
	if ( !( (U32) __xa_libc_text_ram_start == (U32) __xa_libc_text_rom_start) )
	{
		len = __xa_libc_text_ram_end - __xa_libc_text_ram_start;

		for (count=0; count < len; count++)
		{
			__xa_libc_text_ram_start[count] = __xa_libc_text_rom_start[count];
		}
	}

	if ( !( (U32) __internal_ram_1_ram_start == (U32) __internal_ram_1_rom_start) )
	{
		//.internal_ram_1_text
		len = __internal_ram_1_ram_end - __internal_ram_1_ram_start;
		for (count = 0; count < len; count++)
		{
			__internal_ram_1_ram_start[count] = __internal_ram_1_rom_start[count];
		}
	}

	if ( !( (U32) __interal_ram_1_ram_start == (U32) __interal_ram_1_rom_start) )
	{
		len = __interal_ram_1_ram_end - __interal_ram_1_ram_start;
		for (count=0; count < len; count++)
		{
			__interal_ram_1_ram_start[count] = __interal_ram_1_rom_start[count];
		}
	}

	if ( !( (U32) __flash_prog_ram_start == (U32) __flash_prog_rom_start) )
	{
		//.flash_prog_text
		len = __flash_prog_ram_end - __flash_prog_ram_start;
		for (count = 0; count < len; count++)
		{
			__flash_prog_ram_start[count] = __flash_prog_ram_end[count];
		}
	}


	//LIBKERNEL_TEXT_LOAD
	if ( !( (U32)__libkernel_ram_start == (U32)__libkernel_rom_start) )
	{
		//.libkernel_text
		len = __libkernel_ram_end - __libkernel_ram_start;

		for (count = 0; count < len; count++)
		{
			__libkernel_ram_start[count] = __libkernel_rom_start[count];
		}
	}


	#ifdef CODEC_TEXT_LOAD
	//.codec_text
	len = __TEXT_START25_END - __TEXT_START25;
	CFasm_memcpy(__TEXT_START25, __TEXT_ROM_START25, len*4);
	#endif


	//ALL_TEXT_LOAD
	if ( !( (U32)__all_text_ram_start == (U32)__all_text_rom_start) )
	{
		//.all_text
		len = __all_text_ram_end - __all_text_ram_start;

		for (count = 0; count < len; count++)
		{
			__all_text_ram_start[count] = __all_text_rom_start[count];
		}
	}

	//.dsrc_data_tbl
	if (!(__dsrc_data_ram_start == __dsrc_data_rom_start))
	{
		len = __dsrc_data_ram_end - __dsrc_data_ram_start;
		CFasm_memcpy(__dsrc_data_ram_start, __dsrc_data_rom_start, len*4);
	}

	//.dsp_critical_sec_text
	if (!(__dsp_critical_sec_ram_start == __dsp_critical_sec_rom_start))
	{
		len = __dsp_critical_sec_ram_end - __dsp_critical_sec_ram_start;
		CFasm_memcpy(__dsp_critical_sec_ram_start, __dsp_critical_sec_rom_start, len*4);
	}
	
	if ( !( (U32)__libkernel_ram_start == (U32)__libkernel_rom_start) )
	{
		//.libkernel_rodata
		len = __libkernel_ram_end - __libkernel_ram_start;

		for (count = 0; count < len; count++)
		{
			__libkernel_ram_start[count] = __libkernel_rom_start[count];
		}
	}


	#ifdef UPGRADE_RODATA_LOAD
	//.upgrade_rodata
	len = __TEXT_START26_END - __TEXT_START26;
	CFasm_memcpy(__TEXT_START26, __TEXT_ROM_START26, len*4);
	#endif


	//ALL_RODATA_LOAD
	if ( !( (U32) __all_rodata_ram_start == (U32) __all_rodata_rom_start) )
	{
		//.all_rodata
		len = __all_rodata_ram_end - __all_rodata_ram_start;

		for (count = 0; count < len; count++)
		{
			__all_rodata_ram_start[count] = __all_rodata_rom_start[count];
		}
	}


	if ( !( (U32) __data_ram_start == (U32) __data_rom_start) )
	{
		//.data
		len = __data_ram_end - __data_ram_start;
		for (count=0; count < len; count++)
		{
			__data_ram_start[count] = __data_rom_start[count];
		}
	}


	//.bss
	len = _clib_bss_end - _clib_bss_start; //0x1B628 bytes
	REG_MISC_STATUS = len;
	for (count = 0; count < len; count++)
	{
		_clib_bss_start[count] = 0;
	}

    void flash_libxa_dd_ac3_dec_text_copy(void);
    flash_libxa_dd_ac3_dec_text_copy();

	asm("nop");
}

void flash_libxa_ec3_dec_text_copy(void)
{
#ifdef OPTEK_LIB_DSP_EC3_TEXT_LOADER
	volatile U32 count;
	volatile U32 len;

	if ( !( (U32)__xa_dd_ec3_dec_lib_text_ram_start == (U32)__xa_dd_ec3_dec_lib_text_rom_start) )
	{
		len = __xa_dd_ec3_dec_lib_text_ram_end - __xa_dd_ec3_dec_lib_text_ram_start;
		for (count = 0; count < len; count++)
		{
			__xa_dd_ec3_dec_lib_text_ram_start[count] = __xa_dd_ec3_dec_lib_text_rom_start[count];
		}
	}
#endif
}

extern U32 __xa_dd_ac3_dec_lib_text_rom_start[];
extern U32 __xa_dd_ac3_dec_lib_text_ram_start[];
extern U32 __xa_dd_ac3_dec_lib_text_ram_end[];

void flash_libxa_dd_ac3_dec_text_copy(void)
{
#ifdef OPTEK_LIB_DD_AC3_TEXT_LOADER
	volatile U32 count;
	volatile U32 len;

	if ( !( (U32) __xa_dd_ac3_dec_lib_text_ram_start == (U32) __xa_dd_ac3_dec_lib_text_rom_start) )
	{
		len = __xa_dd_ac3_dec_lib_text_ram_end - __xa_dd_ac3_dec_lib_text_ram_start;
		for (count = 0; count < len; count++)
		{
			__xa_dd_ac3_dec_lib_text_ram_start[count] = __xa_dd_ac3_dec_lib_text_rom_start[count];
		}
	}
#endif
}

void flash_libxa_dts_dmp_dec_text_copy(void)
{
#ifdef OPTEK_LIB_DTS_DMP_TEXT_LOADER
	volatile U32 count;
	volatile U32 len;

	if ( !( (U32) __xa_dts_dmp_dec_lib_text_ram_start == (U32) __xa_dts_dmp_dec_lib_text_rom_start) )
	{
		len = __xa_dts_dmp_dec_lib_text_ram_end - __xa_dts_dmp_dec_lib_text_ram_start;
		for (count = 0; count < len; count++)
		{
			__xa_dts_dmp_dec_lib_text_ram_start[count] = __xa_dts_dmp_dec_lib_text_rom_start[count];
		}
	}
#endif
}


#ifdef DSP_SONA_MODULE_ENABLE

extern U32 __sona_dsp_text_rom_start[];
extern U32 __sona_dsp_text_ram_start[];
extern U32 __sona_dsp_text_rom_end[];

#ifdef SONA_DSP_MALLOC_L2
extern U32 __sona_L2_text_rom_start[];
extern U32 __sona_L2_text_ram_start[];
extern U32 __sona_L2_text_rom_end[];
#endif

extern U32 __sona_L1_text_rom_start[];
extern U32 __sona_L1_text_ram_start[];
extern U32 __sona_L1_text_rom_end[];

void flash_lib_sona_code_copy(void)
{
	volatile U32 count;
	volatile U32 len;
	volatile U32 *pRamStart;
	volatile U32 *pRomStart;

	//SONA_RODATA_LOAD
	#ifdef SONA_DSP_MALLOC_L2
	pRamStart = __sona_L2_text_ram_start;
	pRomStart = __sona_L2_text_rom_start;
	DBG_Assert( (pRamStart != NULL) ||
				(pRomStart != NULL) );
	if ( !( (U32) __sona_L2_text_ram_start == (U32) __sona_L2_text_rom_start) )
	{
		len = __sona_L2_text_rom_end - __sona_L2_text_ram_start;
		#if 1
		for (count = 0; count < len; count++)
		{
			pRamStart[count] = pRomStart[count];
		}
		#else
		//CFasm_memcpy(pRamStart, pRomStart, len*4);
		memcpy(pRamStart, pRomStart, len*4);
		#endif

	}
	#endif

	pRamStart = __sona_L1_text_ram_start;
	pRomStart = __sona_L1_text_rom_start;
	DBG_Assert( (pRamStart != NULL) ||
				(pRomStart != NULL) );
	if ( !( (U32) __sona_L1_text_ram_start == (U32) __sona_L1_text_rom_start) )
	{
		len = __sona_L1_text_rom_end - __sona_L1_text_ram_start;

		#if 1
		for (count = 0; count < len; count++)
		{
			pRamStart[count] = pRomStart[count];
		}
        #else
    	//CFasm_memcpy(pRamStart, pRomStart, len*4);
    	memcpy(pRamStart, pRomStart, len*4);
        #endif
	}

 	pRamStart = __sona_dsp_text_ram_start;
	pRomStart = __sona_dsp_text_rom_start;
	DBG_Assert( (pRamStart != NULL) ||
				(pRomStart != NULL) );
	if ( !( (U32) __sona_dsp_text_ram_start == (U32) __sona_dsp_text_rom_start) )
	{
		len = __sona_dsp_text_rom_end - __sona_dsp_text_ram_start;

		#if 1
		for (count = 0; count < len; count++)
		{
			pRamStart[count] = pRomStart[count];
		}
        #else
    	//CFasm_memcpy(pRamStart, pRomStart, len*4);
    	memcpy(pRamStart, pRomStart, len*4);
        #endif
	}
}
#endif //SONA_DSP_ENABLE



#define UPGRADE_PROJECT_ENABLE

#ifdef UPGRADE_PROJECT_ENABLE

//#define UPGRADE_PROJECT_FIRST_START

#undef DEBUG_GPIO_PIN0_ENABLE

#define LOOP_1_MS_TIME_CNT            1000
//#define LOOP_1_S_TIME_CNT             (LOOP_1_MS_TIME_CNT*10)
#define LOOP_1_S_TIME_CNT             (LOOP_1_MS_TIME_CNT)
#define LOOP_10_S_TIME_CNT            (LOOP_1_S_TIME_CNT*2)

#define GPIO_OUT_DELAY_TIME_BASE_CNT    1


#ifdef UPGRADE_PROJECT_FIRST_START
U8 upgrade_normal_state;
U8 upgrade_flag_from_app;
#endif


extern U8 sram_boot_config[16];
extern const U8 flash_boot_config[16];

extern U32 BOOT_TAG_START[];

void Flash_Boot_Switch(void) __attribute__ ((section (".flash_boot_switch_text")));

void Flash_Boot_Switch(void)
{
	U32 i;
	//U8 wBuf[7];
	//U8 rBuf[7];
	U8 page_addr;
	U16 tmp;
	U8 jump_flag = FALSE;

	U16 *pFlashAddr = (U16 *)BOOT_TAG_START;
	U8 *pFlashBootConfig = (U8 *)flash_boot_config;

	#if 0
	//U32 *pAddr1 = (U32 *) (0x58008000 + 0x858);
	U8 *pAddr1 = (U8 *)BOOT_TAG_START;
	U8 *pAddr2 = (U8 *)(BOOT_TAG_START+4);

	for (i=0; i<8; i++)
	{
		rBuf[i] = *pAddr1++;
	}

	for (i=0; i<8; i++)
	{
		wBuf[i] = *pAddr2++;
	}

	asm("nop");
	#endif


	#if 0//def UPGRADE_PROJECT_FIRST_START
	upgrade_normal_state = FALSE; //for reading Ui message from dram
	upgrade_flag_from_app = FALSE;
	#endif


	#if 0//ndef BOOT_FROM_FLASH
	sram_boot_config[0] = 'U';
	sram_boot_config[1] = 'P';
	sram_boot_config[2] = 'G';

	#if 1
	sram_boot_config[3] = 'U';
	sram_boot_config[4] = 'S';
	sram_boot_config[5] = 'B';
	#else
	sram_boot_config[3] = ' ';
	sram_boot_config[4] = 'B';
	sram_boot_config[5] = 'T';
	#endif

	#if 0
	//erase all
	sram_boot_config[6] = 'E';
	sram_boot_config[7] = 'F';
	sram_boot_config[8] = 'A';
	#else
	//erase partial
	sram_boot_config[6] = 'E';
	sram_boot_config[7] = 'F';
	sram_boot_config[8] = 'P';
	#endif

	#endif


	#ifdef DEBUG_GPIO_PIN0_ENABLE
	/debug_pin_init();
	#endif

    #ifdef DEBUG_GPIO_PIN0_ENABLE
    /debug_pin_loop_with_one_param(LOOP_10_S_TIME_CNT, GPIO_OUT_DELAY_TIME_BASE_CNT);
    #endif


	#if 1//def BOOT_FROM_FLASH
 	//UPGUSBEFP
	//firt check boot tag in sram
	if ( (sram_boot_config[0] == 'U') && (sram_boot_config[1] == 'P') &&
		(sram_boot_config[2] == 'G') )
	{
		//debug_pin_test();
		
		#if 0
		if ( (sram_boot_config[3] == 'U') && (sram_boot_config[4] == 'S') &&
			(sram_boot_config[5] == 'B') )
		#endif
		#if 1
		if ( ( (sram_boot_config[3] == 'U') && (sram_boot_config[4] == 'S') &&
			(sram_boot_config[5] == 'B') ) ||
			( (sram_boot_config[3] == ' ') && (sram_boot_config[4] == 'B') &&
			(sram_boot_config[5] == 'T') ) )

		#endif
		{
			#if 0//def UPGRADE_PROJECT_FIRST_START
			//@app_main_data.media = MEDIA_USB_DEVICE;
			upgrade_normal_state = TRUE;
			upgrade_flag_from_app = TRUE;
			#endif

			if (sram_boot_config[6] == 'E' && sram_boot_config[7] == 'F' &&
				sram_boot_config[8] == 'A')
			{
				//@app_main_data.flashEraseMode = FLASH_ERASE_ALL;
				jump_flag = TRUE;
			}
			else if (sram_boot_config[6] == 'E' && sram_boot_config[7] == 'F' &&
				sram_boot_config[8] == 'P')
			{
				//@app_main_data.flashEraseMode = FLASH_ERASE_PARTIAL;
				jump_flag = TRUE;
			}
			else
			{
				asm("nop");
			}

			if (jump_flag == TRUE)
			{
				boot_dsp_upgrade_project_start();
			}

			//debug_pin_test();
			return;
		}

		#if 0//def SD_ENABLE
		else if (sram_boot_config[3] == ' ' && sram_boot_config[4] == 'S' &&
				sram_boot_config[5] == 'D')
		{
			#ifdef UPGRADE_PROJECT_FIRST_START
			app_main_data.media = MEDIA_SDMMC;
			upgrade_normal_state = TRUE;
			upgrade_flag_from_app = TRUE;
			#endif

			if (sram_boot_config[6] == 'E' && sram_boot_config[7] == 'F' &&
				sram_boot_config[8] == 'A')
			{
				app_main_data.flashEraseMode = FLASH_ERASE_ALL;
			}
			else
			{
				app_main_data.flashEraseMode = FLASH_ERASE_PARTIAL;
			}

			return;
		}
		#endif //SD_ENABLE

		#if 0//def BT_HCI_ENABLE
		else if (sram_boot_config[3] == ' ' && sram_boot_config[4] == 'B' &&
				sram_boot_config[5] == 'T')
		{
			#ifdef UPGRADE_PROJECT_FIRST_START
			app_main_data.media = MEDIA_BLUETOOTH;
			upgrade_normal_state = TRUE;
			upgrade_flag_from_app = TRUE;
			#endif

			if (sram_boot_config[6] == 'E' && sram_boot_config[7] == 'F' &&
				sram_boot_config[8] == 'A')
			{
				app_main_data.flashEraseMode = FLASH_ERASE_ALL;
			}
			else
			{
				app_main_data.flashEraseMode = FLASH_ERASE_PARTIAL;
			}

			return;
		}
		#endif //BT_HCI_ENABLE

		#ifdef UART_UPGRADE_ENABLE
		else if (sram_boot_config[3] == 'U' && sram_boot_config[4] == 'A' &&
				sram_boot_config[5] == 'T')
		{
			#ifdef UPGRADE_PROJECT_FIRST_START
			/app_main_data.media = MEDIA_UART_UPGRADE;
			upgrade_normal_state = TRUE;
			upgrade_flag_from_app = TRUE;
			#endif

			if (sram_boot_config[6] == 'E' && sram_boot_config[7] == 'F' &&
				sram_boot_config[8] == 'A')
			{
				app_main_data.flashEraseMode = FLASH_ERASE_ALL;
			}
			else
			{
				app_main_data.flashEraseMode = FLASH_ERASE_PARTIAL;
			}

			return;
		}
		#endif //UART_UPGRADE_ENABLE


		#ifdef UPGRADE_PROJECT_FIRST_START
		else if (sram_boot_config[3] == 'S' && sram_boot_config[4] == 'u' &&
				sram_boot_config[5] == 'c')
		{
			//app_main_data.media = MEDIA_UPGRADE;
			boot_mp3();
			return;
		}
		else if (sram_boot_config[3] == 'E' && sram_boot_config[4] == 'r' &&
				sram_boot_config[5] == 'r')
		{
			//app_main_data.media = MEDIA_UPGRADE;
			boot_mp3();
			return;
		}
		#endif //UPGRADE_PROJECT_FIRST_START
		
		#if 0
		else
		{
			asm("nop");
			//app_main_data.media = MEDIA_SDMMC;
		}
		#endif
	}
	#endif //BOOT_FROM_FLASH


	#if 0//def UPGRADE_PROJECT_FIRST_START
	#ifndef BOOT_FROM_FLASH
	upgrade_normal_state = FALSE; //for debug
	#endif
	#endif

	#if 0//def UPGRADE_PROJECT_FIRST_START
	if (sram_boot_config[3] == 'E' && sram_boot_config[4] == 'C'
		&& sram_boot_config[5] == 'O')
	{
		boot_standby_eco();
		return;
	}
	#endif


    #ifdef DEBUG_GPIO_PIN0_ENABLE
    /debug_pin_loop_with_one_param(LOOP_10_S_TIME_CNT/3, GPIO_OUT_DELAY_TIME_BASE_CNT*3);
    #endif


	#ifdef BOOT_FROM_FLASH
	//second check boot tag in flash
	if (pFlashBootConfig[0] == 'U' && pFlashBootConfig[1] == 'P' &&
		pFlashBootConfig[2] == 'G')
	{
		//debug_pin_test_1();
	
		#if 0//def POWER_RELAY_CONTROL
		POWER_ON;

		//delay for power
		for (i=0; i<100; i++)
		{
			asm("nop");
		}
		#endif

		#if 0
		if ( (pFlashBootConfig[3] == 'U') && (pFlashBootConfig[4] == 'S') &&
			(pFlashBootConfig[5] == 'B') )
		#endif
		#if 1
		if ( ( (pFlashBootConfig[3] == 'U') && (pFlashBootConfig[4] == 'S') &&
			(pFlashBootConfig[5] == 'B') ) ||
			( (pFlashBootConfig[3] == ' ' && pFlashBootConfig[4] == 'S' &&
				pFlashBootConfig[5] == 'D') ) )
		#endif
		{
			//@app_main_data.media = MEDIA_USB_DEVICE;

			if (pFlashBootConfig[6] == 'E' && pFlashBootConfig[7] == 'F' &&
				pFlashBootConfig[8] == 'A')
			{
				//@app_main_data.flashEraseMode = FLASH_ERASE_ALL;
				jump_flag = TRUE;
			}
			else if (pFlashBootConfig[6] == 'E' && pFlashBootConfig[7] == 'F' &&
				pFlashBootConfig[8] == 'P')
			{
				//@app_main_data.flashEraseMode = FLASH_ERASE_PARTIAL;
				jump_flag = TRUE;
			}
			else
			{
				asm("nop");
			}

			if (jump_flag == TRUE)
			{
				boot_dsp_upgrade_project_start();
			}

			return;
		}

		#if 0//def SD_ENABLE
		else if (pFlashBootConfig[3] == ' ' && pFlashBootConfig[4] == 'S' &&
				pFlashBootConfig[5] == 'D')
		{
			app_main_data.media = MEDIA_SDMMC;

			if (pFlashBootConfig[6] == 'E' && pFlashBootConfig[7] == 'F' &&
				pFlashBootConfig[8] == 'A')
			{
				app_main_data.flashEraseMode = FLASH_ERASE_ALL;
			}
			else
			{
				app_main_data.flashEraseMode = FLASH_ERASE_PARTIAL;
			}
		}
		#endif

		#if 0//def BT_HCI_ENABLE
		else if (pFlashBootConfig[3] == ' ' && pFlashBootConfig[4] == 'B' &&
				pFlashBootConfig[5] == 'T')
		{
			app_main_data.media = MEDIA_BLUETOOTH;

			if (pFlashBootConfig[6] == 'E' && pFlashBootConfig[7] == 'F' &&
				pFlashBootConfig[8] == 'A')
			{
				app_main_data.flashEraseMode = FLASH_ERASE_ALL;
				jump_flag = TRUE;
			}
			else if (pFlashBootConfig[6] == 'E' && pFlashBootConfig[7] == 'F' &&
				pFlashBootConfig[8] == 'P')
			{
				app_main_data.flashEraseMode = FLASH_ERASE_PARTIAL;
				jump_flag = TRUE;
			}
			else
			{
				asm("nop");
			}

			if (jump_flag == TRUE)
			{
				boot_dsp_upgrade_project_start();
			}
		}
		#endif

		#if 0
		else
		{
			//asm("nop");
			//app_main_data.media = MEDIA_SDMMC;
		}
		#endif
	}
	else
	#endif //BOOT_FROM_FLASH
	{
		#if 1//def BOOT_FROM_FLASH
		tmp = *pFlashAddr++;
		if (tmp == 0xFFFF)
		{
            #ifdef DEBUG_GPIO_PIN0_ENABLE
            //debug_pin_loop_with_one_param(LOOP_10_S_TIME_CNT/6, GPIO_OUT_DELAY_TIME_BASE_CNT*9);
            #endif

			//debug_pin_test_2();

			//app_main_data.media = MEDIA_USB_DEVICE;
			//app_main_data.media = MEDIA_SDMMC;
			//app_main_data.media = MEDIA_BLUETOOTH;
			//app_main_data.media = MEDIA_UART_UPGRADE;

			//app_main_data.flashEraseMode = FLASH_ERASE_PARTIAL;
			boot_dsp_upgrade_project_start();
		}
		else
		{
           #ifdef DEBUG_GPIO_PIN0_ENABLE
           //debug_pin_loop_with_one_param(LOOP_10_S_TIME_CNT/12, GPIO_OUT_DELAY_TIME_BASE_CNT*12);
           #endif

			//debug_pin_test_2();
			//@boot_mp3();
		}
		#else
		//DEBUG
		//app_main_data.media = MEDIA_USB_DEVICE;
		//app_main_data.media = MEDIA_SDMMC;
		//app_main_data.media = MEDIA_BLUETOOTH;
		//app_main_data.media = MEDIA_UART_UPGRADE;

		//app_main_data.flashEraseMode = FLASH_ERASE_ALL;
		//app_main_data.flashEraseMode = FLASH_ERASE_PARTIAL;
		#endif
	}
}

#endif //UPGRADE_PROJECT_ENABLE
