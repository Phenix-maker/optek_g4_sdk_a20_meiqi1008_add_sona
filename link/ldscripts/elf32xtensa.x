/* This linker script generated from xt-genldscripts.tpp for LSP nort */
/* Linker Script for default link */

/*
	flash rom version
*/


/*for not supporting upgrade*/
/*DSP_CORE_START = 0x00000;*/

/*for supporting upgrade*/
DSP_CORE_START = 0x80000;

MEMORY
{
	/*optek gen4 soc*/
	/*
	section    len
	iram       0x30000(192K)
	irom       0x8000(32K)
	drom       0x4000(16K)
	dram       0x78000(480K)
	psram      (depends on chip)
	*/

	i_ram_vector :                 org = 0x40000000, len = 0x400
	i_ram_xa_libc :                org = 0x40000400, len = 0x2000
	i_ram :                        org = 0x40000400+0x2000, len = 0x2FC00 - 0x2000
/*
	i_ram_xa_dts_dmp_codec :       org = 0x40000400+0x2000, len = 0xD280
	i_ram_xa_ec3_codec :           org = 0x40000400+0x2000, len = 0xD280
	i_ram :                        org = 0x40000400+0x2000+0xD280, len = 0x2FC00-0x2000-0xD280
*/
	i_rom :                        org = 0x48000000, len = 0x60000
	d_rom :                        org = 0x3F800000, len = 0x40000

/*	d_ram :                        org = 0x3FDC0000, len = (0x78000-0x400)*/
/*	d_ram :                        org = 0x3FDC0000, len = (0x78000-0x400-0x10)*/

	/*0x78000-configTOTAL_HEAP_SIZE-stream_memory_bss-codec_memory_bss-0x400*/ 
	/*0x78000=480K*/
	/*configTOTAL_HEAP_SIZE=0x6000=24K*/
	/*stream_memory_bss=0x14000=80K*/
	/*codec_memory_bss=0x34000=208K*/
/*	d_ram :                        org = 0x3FDC0000, len = (0x78000-0x6000-0x14000-0x34000-0x400)*/
	/*stream_memory_bss=0x1C000=112K*/
/*	d_ram :                        org = 0x3FDC0000, len = (0x78000-0x6000-0x1C000-0x34000-0x400)*/
	/*stream_memory_bss=0x20000=128K*/
/*	d_ram :                        org = 0x3FDC0000, len = (0x78000-0x6000-0x20000-0x34000-0x400)*/
	/*stream_memory_bss=0x20000=136K*/
/*	d_ram :                        org = 0x3FDC0000, len = (0x78000-0x6000-0x22000-0x34000-0x400)*/
	
/*	d_ram :                        org = 0x3FDC0000, len = (0x78000-0x400-0x10-0x22000)*/
/*	d_ram :                        org = 0x3FDC0000, len = (0x78000-0x400-0x10-0x20000)*/
/*	d_ram :                        org = 0x3FDC0000, len = (0x78000-0x400-0x10-0x18000)*/		/*bt mode and DD+ function both open*/
/*	d_ram :                        org = 0x3FDC0000, len = (0x78000-0x400-0x10-0x14000)*/
	d_ram :                        org = 0x3FDC0000, len = (0x78000-0x400-0x10-0x10000)

/*
	dram memory can used iram memory
	iram memory setting at reset.S file
	default setting : 192k iram, 480k dram
	example:()
	if  64k iram, 608k dram
	d_ram :                        org = 0x3FDC0000 - 0x20000, len = (0x78000-0x400-0x10-0x10000 + 0x20000)
	
*/


	d_ram_boot :                   org = 0x3FDC0000+0x78000-0x400-0x10, len = 0x10
	d_ram_stack :                  org = 0x3FDC0000+0x78000-0x400, len = 0x400


	/* iram */
/*	rom_boot :                     org = 0x40000400, len = 0xC00
	rom_vector :                   org = 0x40001000, len = 0x400
	rom_text :                     org = 0x40001400, len = 0x100000 - 0x1000
*/


	/*text ram of fpga only*/
/*	rom_boot :                     org = 0x6C000000, len = 0xC00
	rom_vector :                   org = 0x6C000C00, len = 0x400
	rom_text :                     org = 0x6C001000, len = 0x100000 - 0x1000
*/


	/*sFlash for one project*/
/*	rom_boot :                     org = 0x6A000000, len = 0xC00
	rom_vector :                   org = 0x6A000C00, len = 0x400
	rom_xa_libc_text :             org = 0x6A001000, len = 0x2000
	rom_text :                     org = 0x6A001000+0x2000, len = 0x100000 - 0x1000
*/


	/*psram for one project*/
/*	rom_boot :                     org = 0x70000000, len = 0xC00
	rom_vector :                   org = 0x70000C00, len = 0x400
	rom_xa_libc_text :             org = 0x70001000, len = 0x2000
	rom_text :                     org = 0x70001000+0x2000, len = 0x200000 - 0x1000 - 0x2000
*/


	/*sFlash for multi projects*/
	rom :                          org = 0x6A000000, len = 0x200000
	reset_dsp_core_boot :          org = 0x6A000000, len = 0x400
	reset_mcu_core_boot :          org = 0x6A000400, len = 0x400
	rom_upgrade_boot :             org = 0x6A000800, len = 0x7F000

	rom_dsp_upgrade_project_boot : org = 0x6A000800, len = 0x7F000 - 0x800
	rom_mcu_upgrade_project_boot : org = 0x6A000800, len = 0x7F000 - 0x800

	rom_boot_data :                org = 0x6A050000, len = 0x1000
	rom_boot :                     org = 0x6A051000, len = 0xC00
	rom_vector :                   org = 0x6A051C00, len = 0x400

	boot_offset :                  org = 0x2000, len = 0

	boot_copy_loader_rodata :      org = ORIGIN(boot_offset)+0x6A050000, len = 0x10

	xa_dd_ac3_dec_lib_rodata :     org = ORIGIN(boot_offset)+0x6A050010, len = 0x1ebc
	xa_dd_ac3_dec_lib_txt :        org = ORIGIN(boot_offset)+0x6A051ef0, len = 0x65c0

	xa_dts_dmp_dec_lib_rodata :    org = ORIGIN(boot_offset)+0x6A050010+0x84D0, len = 0x1dd70
	xa_dts_dmp_dec_lib_txt :       org = ORIGIN(boot_offset)+0x6A050010+0x84D0+0x1dd90, len = 0xb0ac

	xa_dd_ec3_dec_lib_rodata :     org = ORIGIN(boot_offset)+0x6A050010+0x84D0+0x28F00, len = 0x5a50
	xa_dd_ec3_dec_lib_txt :        org = ORIGIN(boot_offset)+0x6A050010+0x84D0+0x28F00+0x5a80, len = 0xd25c

	rom_xa_libc_text :             org = ORIGIN(boot_offset)+0x6A050010+0x84D0+0x28F00+0x12D00, len = 0x2000
	rom_text :                     org = ORIGIN(boot_offset)+0x6A052010+0x84D0+0x28F00+0x12D00, len = 0x200000-0x82010-0x84D0-0x28F00-0x12D00

	rom_xa_libc_text :             org = ORIGIN(boot_offset)+0x6A050010, len = 0x2000
	rom_text :                     org = ORIGIN(boot_offset)+0x6A050010+0x2000, len = 0x200000-0x50010-0x2000


	/*psram for multi projects*/
	/*for copying the sFlash data of psram bin to psram and running at psram*/
/*	rom :                          org = 0x70000000, len = 0x200000
	reset_dsp_core_boot :          org = 0x70000000, len = 0x400
	reset_mcu_core_boot :          org = 0x70000400, len = 0x400
	rom_upgrade_boot :             org = 0x70000800, len = 0x7F000

	rom_dsp_upgrade_project_boot : org = 0x70000800, len = 0x7F000 - 0x800
	rom_mcu_upgrade_project_boot : org = 0x70000800, len = 0x7F000 - 0x800

	rom_boot_data :                org = 0x70050000, len = 0x1000
	rom_boot :                     org = 0x70051000, len = 0xC00
	rom_vector :                   org = 0x70051C00, len = 0x400

	boot_offset :                  org = 0x2000, len = 0

	boot_copy_loader_rodata :      org = ORIGIN(boot_offset)+0x70050000, len = 0x10

	xa_dd_ac3_dec_lib_rodata :     org = ORIGIN(boot_offset)+0x70050010, len = 0x1ebc
	xa_dd_ac3_dec_lib_txt :        org = ORIGIN(boot_offset)+0x70051ef0, len = 0x65c0

	xa_dts_dmp_dec_lib_rodata :    org = ORIGIN(boot_offset)+0x70050010+0x84D0, len = 0x1dd70
	xa_dts_dmp_dec_lib_txt :       org = ORIGIN(boot_offset)+0x70050010+0x84D0+0x1dd90, len = 0xb0ac

	xa_dd_ec3_dec_lib_rodata :     org = ORIGIN(boot_offset)+0x70050010+0x84D0+0x28F00, len = 0x5a50
	xa_dd_ec3_dec_lib_txt :        org = ORIGIN(boot_offset)+0x70050010+0x84D0+0x28F00+0x5a80, len = 0xd25c

	rom_xa_libc_text :             org = ORIGIN(boot_offset)+0x70050010+0x84D0+0x28F00+0x12D00, len = 0x2000
	rom_text :                     org = ORIGIN(boot_offset)+0x70052010+0x84D0+0x28F00+0x12D00, len = 0x200000-0x82010-0x84D0-0x28F00-0x12D00

	rom_xa_libc_text :             org = ORIGIN(boot_offset)+0x70050010, len = 0x2000
	rom_text :                     org = ORIGIN(boot_offset)+0x70050010+0x2000, len = 0x200000-0x50010-0x2000
*/



	/* special section in  sFlash */
	bt_data_1 :                    org = 0x6A048000, len = 0x1000
	bt_data_2 :                    org = 0x6A049000, len = 0x1000
	upgrade_hw_set_rodata :        org = 0x6A04A000, len = 0x1000

	optek_link_user_data :         org = 0x6A04B000, len = 0x1000

	rom_save_data2 :               org = 0x6A04C000, len = 0x1000
	rom_save_data1 :               org = 0x6A04D000, len = 0x1000

/*	xa_libc_text :                 org = 0x70eaa400, len = 0x19c0*/



	/*DRAM*/
/*	stream_memory_bss :            org = 0x3FDC0000+0x78000-0x400-0x34000-0x14000, len = 0x14000*/
/*	codec_memory_bss :             org = 0x3FDC0000+0x78000-0x400-0x34000, len = 0x34000*/ /*only for ddplus71 test*/

/*	stream_memory_bss :            org = 0x3FDC0000+0x78000-0x400-0x34000-0x1C000, len = 0x1C000*/
/*	codec_memory_bss :             org = 0x3FDC0000+0x78000-0x400-0x34000, len = 0x34000*/ /*only for ddplus71 test*/


/*	stream_memory_bss :            org = 0x3FDC0000+0x78000-0x400-0x10-0x22000, len = 0x22000*/
/*	stream_memory_bss :            org = 0x3FDC0000+0x78000-0x400-0x10-0x20000, len = 0x20000*/
/*	stream_memory_bss :            org = 0x3FDC0000+0x78000-0x400-0x10-0x18000, len = 0x18000*/
/*	stream_memory_bss :            org = 0x3FDC0000+0x78000-0x400-0x10-0x14000, len = 0x14000*/
	stream_memory_bss :            org = 0x3FDC0000+0x78000-0x400-0x10-0x10000  + 16*1024, len = 0x10000 - 16*1024



	/* for debug */
	dump_memory_bss :              org = 0x70600000, len = 0x100000


/*	stream_memory_bss :            org = 0x70450000, len = 0x2B0000*/
	codec_slow_memory_bss :        org = 0x70700000, len = 0x100000
	psram_bss :                    org = 0x70800000, len = 0x20000


	DDPLUS_PERSISTENT_BSS :        org = 0x708CE400, len = 0x13E00
	DDPLUS_SCRATCH_BSS :           org = 0x708E2200, len = 0x11E00
	DDPLUS_OUTPUT_BSS :            org = 0x708F4000, len = 0xC000


	WindowVectors.text_seg :              org = 0x40000000, len = 0x178
	sram1_seg :                           org = (0x40000000 + 0x0000017C-0x4), len = 0x4
	Level2InterruptVector.text_seg :      org = (0x40000000 + 0x0000017C), len = 0x1C
	sram3_seg :                           org = (0x40000000 + 0x000001a0-0x8), len = 0x8
	Level3InterruptVector.text_seg :      org = (0x40000000 + 0x000001a0), len = 0x38
	sram5_seg :                           org = (0x40000000 + 0x000001e0-0x8), len = 0x8
	Level4InterruptVector.text_seg :      org = (0x40000000 + 0x000001e0), len = 0x38


	sram7_seg :                           org = (0x40000000 + 0x00000220-0x8), len = 0x8
	Level5InterruptVector.text_seg :      org = (0x40000000 + 0x00000220), len = 0x38

/*
	sram9_seg :                           org = (0x40000000 + 0x00000200-0x8), len = 0x8
	DebugExceptionVector.text_seg :       org = (0x40000000 + 0x00000200), len = 0x38	
*/

	sram11_seg :                          org = (0x40000000 + 0x00000260-0x8), len = 0x8
	NMIExceptionVector.text_seg :         org = (0x40000000 + 0x00000260), len = 0x38	
	sram13_seg :                          org = (0x40000000 + 0x0000029c-0x4), len = 0x4
	KernelExceptionVector.text_seg :      org = (0x40000000 + 0x0000029c), len = 0x1C
	sram15_seg :                          org = (0x40000000 + 0x000002bc-0x4), len = 0x4
	UserExceptionVector.text_seg :        org = (0x40000000 + 0x000002bc), len = 0x1C
	sram17_seg :                          org = (0x40000000 + 0x000002dc-0x4), len = 0x4
	DoubleExceptionVector.text_seg :      org = (0x40000000 + 0x000002dc), len = 0x1C

}


/* Default entry point: */
/*for one project*/
/*ENTRY(_ResetVector)*/

/*for multi projects*/
ENTRY(_Reset_DSP_Core)


GROUP (

/*First,search lib path by customer */
lib/libhal.a
lib/libc.a
lib/libgcc.a
lib/libm.a
lib/libsim.a
/*lib/libgdbio.a*/


/*second, search default lib path by compiler*/
/*
libhal.a 
libc.a 
libgcc.a 
libm.a
libsim.a
libgdbio.a 
*/


/*customer libs*/
lib/Optek_lib_freeRTOS.a
/*lib/Optek_lib_freeRTOS_debug.a*/

lib/optek_lib_spdif_data_parse.a

lib/optek_lib_usb_host_msc.a

lib/optek_lib_usb_device.a

lib/optek_lib_cec.a

lib/libxa_sbc_dec.a
lib/opteklib_sbc_enc.a

lib/xa_msbc_dec.a 
lib/xa_msbc_enc.a

lib/opteklib_btdm_host.a
/*lib/opteklib_btdm52_controller.a*/
lib/opteklib_btdm52_controller_t1.a
/*lib/libfs_sector_512_folder_99_no_fs_janus.a*/
/*lib/libfs_sector_512_folder_99_fs_janus.a*/

/*suport one fs lib*/
lib/Optek_lib_one_fs.a

/*suport two fs lib*/
/*lib/Optek_lib_two_fs.a*/

lib/Optek_lib_dsp_dapp_Q30.a

lib/xa_mp3_dec.a

lib/libxa_lc3_codec.a
lib/optek_lib_dsp_d2as.a

/*lib/libeq_16bit.a*/
/*lib/libeq_24bit.a*/
/*lib/libMackieWoofer-24091701.a*/
/*lib/libMackieWoofer-24092102.a*/
lib/libMackieWoofer-24101102.a
)


SECTIONS
{
	/*sFlash for multi projects*/
	.reset_dsp_core_text :
	{
		*(.Reset_DSP_Boot.text);
		. = ALIGN(4);
		*(.Reset_DSP_Boot.literal);
		. = ALIGN(4);	

		*(.flash_boot_switch_text.literal);
		. = ALIGN(4);
		*(.flash_boot_switch_text);
	} > reset_dsp_core_boot


	.reset_mcu_core_text :
	{
		*(.Reset_MCU_Boot.text);
		. = ALIGN(4);
		*(.Reset_MCU_Boot.literal);
		. = ALIGN(4);	
	} > reset_mcu_core_boot


/*
	.rom_upgrade_boot :
	{
		*(.rom_upgrade_boot)
	} > rom_upgrade_boot
*/

	.boot_text :
	{
		*(.ResetVector.text);
		. = ALIGN(4);
		*(.ResetVector.literal);
		. = ALIGN(4);

		*(.flash_boot_text.literal);
		. = ALIGN(4);
		*(.flash_boot_text);

		*(.boot_upgrade_text);
		. = ALIGN(4);
		*(.boot_upgrade_text.literal);
		. = ALIGN(4);
	} > rom_boot


	.boot_copy_loader_rodata :
	{
		*(.boot_copy_loader_rodata)
	} > boot_copy_loader_rodata

	.vector :
	{
	} > rom_vector


/*	.WindowVectors.text :*/
	.WindowVectors.text : AT (LOADADDR(.vector) + SIZEOF(.vector))
	{
		__TEXT_ROM_START1 = LOADADDR(.vector) + SIZEOF(.vector);
		__TEXT_START1 = ABSOLUTE(.);
		_WindowVectors_text_start = ABSOLUTE(.);
		*(.WindowVectors.text)
		. = ALIGN(4);
		_WindowVectors_text_end = ABSOLUTE(.);
		__TEXT_START1_END = ABSOLUTE(.);
	} > WindowVectors.text_seg

/*	.Level2InterruptVector.literal :*/
	.Level2InterruptVector.literal : AT (LOADADDR(.WindowVectors.text) + SIZEOF(.WindowVectors.text))
	{
		__TEXT_ROM_START2 = LOADADDR(.WindowVectors.text) + SIZEOF(.WindowVectors.text);
		__TEXT_START2 = ABSOLUTE(.);
		_Level2InterruptVector_literal_start = ABSOLUTE(.);
		*(.Level2InterruptVector.literal)
		. = ALIGN(4);
		_Level2InterruptVector_literal_end = ABSOLUTE(.);
		__TEXT_START2_END = ABSOLUTE(.);
	} > sram1_seg

/*	.Level2InterruptVector.text :*/
	.Level2InterruptVector.text : AT (LOADADDR(.Level2InterruptVector.literal) + SIZEOF(.Level2InterruptVector.literal))
	{
		__TEXT_ROM_START3 = LOADADDR(.Level2InterruptVector.literal) + SIZEOF(.Level2InterruptVector.literal);
		__TEXT_START3 = ABSOLUTE(.);
		_Level2InterruptVector_text_start = ABSOLUTE(.);
		*(.Level2InterruptVector.text)
		. = ALIGN(4);
		_Level2InterruptVector_text_end = ABSOLUTE(.);
		__TEXT_START3_END = ABSOLUTE(.);
	} > Level2InterruptVector.text_seg

/*	.Level3InterruptVector.literal :*/
	.Level3InterruptVector.literal : AT (LOADADDR(.Level2InterruptVector.text) + SIZEOF(.Level2InterruptVector.text))
	{
		__TEXT_ROM_START4 = LOADADDR(.Level2InterruptVector.text) + SIZEOF(.Level2InterruptVector.text);
		__TEXT_START4 = ABSOLUTE(.);
		_Level3InterruptVector_literal_start = ABSOLUTE(.);
		*(.Level3InterruptVector.literal)
		. = ALIGN(4);
		_Level3InterruptVector_literal_end = ABSOLUTE(.);
		__TEXT_START4_END = ABSOLUTE(.);
	} > sram3_seg

/*	.Level3InterruptVector.text :*/
	.Level3InterruptVector.text : AT (LOADADDR(.Level3InterruptVector.literal) + SIZEOF(.Level3InterruptVector.literal))
	{
		__TEXT_ROM_START5 = LOADADDR(.Level3InterruptVector.literal) + SIZEOF(.Level3InterruptVector.literal);
		__TEXT_START5 = ABSOLUTE(.);
		_Level3InterruptVector_text_start = ABSOLUTE(.);
		*(.Level3InterruptVector.text)
		. = ALIGN(4);
		_Level3InterruptVector_text_end = ABSOLUTE(.);
		__TEXT_START5_END = ABSOLUTE(.);
	} > Level3InterruptVector.text_seg

/*	.Level4InterruptVector.literal :*/
	.Level4InterruptVector.literal : AT (LOADADDR(.Level3InterruptVector.text) + SIZEOF(.Level3InterruptVector.text))
	{
		__TEXT_ROM_START6 = LOADADDR(.Level3InterruptVector.text) + SIZEOF(.Level3InterruptVector.text);
		__TEXT_START6 = ABSOLUTE(.);
		_Level4InterruptVector_literal_start = ABSOLUTE(.);
		*(.Level4InterruptVector.literal)
		. = ALIGN(4);
		_Level4InterruptVector_literal_end = ABSOLUTE(.);
		__TEXT_START6_END = ABSOLUTE(.);
	} > sram5_seg

/*	.Level4InterruptVector.text :*/
	.Level4InterruptVector.text : AT (LOADADDR(.Level4InterruptVector.literal) + SIZEOF(.Level4InterruptVector.literal))
	{
		__TEXT_ROM_START7 = LOADADDR(.Level4InterruptVector.literal) + SIZEOF(.Level4InterruptVector.literal);
		__TEXT_START7 = ABSOLUTE(.);
		_Level4InterruptVector_text_start = ABSOLUTE(.);
		*(.Level4InterruptVector.text)
		. = ALIGN(4);
		_Level4InterruptVector_text_end = ABSOLUTE(.);
		__TEXT_START7_END = ABSOLUTE(.);
	} > Level4InterruptVector.text_seg

/*	.Level5InterruptVector.literal :*/
	.Level5InterruptVector.literal : AT (LOADADDR(.Level4InterruptVector.text) + SIZEOF(.Level4InterruptVector.text))
	{
		__TEXT_ROM_START8 = LOADADDR(.Level4InterruptVector.text) + SIZEOF(.Level4InterruptVector.text);
		__TEXT_START8 = ABSOLUTE(.);
		_Level5InterruptVector_literal_start = ABSOLUTE(.);
		*(.Level5InterruptVector.literal)
		. = ALIGN(4);
		_Level5InterruptVector_literal_end = ABSOLUTE(.);
		__TEXT_START8_END = ABSOLUTE(.);
	} > sram7_seg

/*	.Level5InterruptVector.text :*/
	.Level5InterruptVector.text : AT (LOADADDR(.Level5InterruptVector.literal) + SIZEOF(.Level5InterruptVector.literal))
	{
		__TEXT_ROM_START9 = LOADADDR(.Level5InterruptVector.literal) + SIZEOF(.Level5InterruptVector.literal);
		__TEXT_START9 = ABSOLUTE(.);
		_Level5InterruptVector_text_start = ABSOLUTE(.);
		*(.Level5InterruptVector.text)
		. = ALIGN(4);
		_Level5InterruptVector_text_end = ABSOLUTE(.);
		__TEXT_START9_END = ABSOLUTE(.);
	} > Level5InterruptVector.text_seg

/*	.DebugExceptionVector.literal :*/
/*	.DebugExceptionVector.literal : AT (LOADADDR(.Level5InterruptVector.text) + SIZEOF(.Level5InterruptVector.text))*/
/*	.DebugExceptionVector.literal : AT (LOADADDR(.Level4InterruptVector.text) + SIZEOF(.Level4InterruptVector.text))*/
/*	{*/
/*		__TEXT_ROM_START10 = LOADADDR(.Level5InterruptVector.text) + SIZEOF(.Level5InterruptVector.text);*/
/*		__TEXT_ROM_START10 = LOADADDR(.Level4InterruptVector.text) + SIZEOF(.Level4InterruptVector.text);
		__TEXT_START10 = ABSOLUTE(.);
		_DebugExceptionVector_literal_start = ABSOLUTE(.);
		*(.DebugExceptionVector.literal)
		. = ALIGN(4);
		_DebugExceptionVector_literal_end = ABSOLUTE(.);
		__TEXT_START10_END = ABSOLUTE(.);
	} > sram9_seg*/

/*	.DebugExceptionVector.text :*/
/*	.DebugExceptionVector.text : AT (LOADADDR(.DebugExceptionVector.literal) + SIZEOF(.DebugExceptionVector.literal))*/
/*	{
		__TEXT_ROM_START11 = LOADADDR(.DebugExceptionVector.literal) + SIZEOF(.DebugExceptionVector.literal);
		__TEXT_START11 = ABSOLUTE(.);
		_DebugExceptionVector_text_start = ABSOLUTE(.);
		*(.DebugExceptionVector.text)
		. = ALIGN(4);
		_DebugExceptionVector_text_end = ABSOLUTE(.);
		__TEXT_START11_END = ABSOLUTE(.);
	} > DebugExceptionVector.text_seg*/

/*	.NMIExceptionVector.literal :*/
/*	.NMIExceptionVector.literal : AT (LOADADDR(.DebugExceptionVector.text ) + SIZEOF(.DebugExceptionVector.text))*/
	.NMIExceptionVector.literal : AT (LOADADDR(.Level5InterruptVector.text ) + SIZEOF(.Level5InterruptVector.text))
	{
/*		__TEXT_ROM_START12 = LOADADDR(.DebugExceptionVector.text) + SIZEOF(.DebugExceptionVector.text);*/
		__TEXT_ROM_START12 = LOADADDR(.Level5InterruptVector.text) + SIZEOF(.Level5InterruptVector.text);
		__TEXT_START12 = ABSOLUTE(.);
		_NMIExceptionVector_literal_start = ABSOLUTE(.);
		*(.NMIExceptionVector.literal)
		. = ALIGN(4);
		_NMIExceptionVector_literal_end = ABSOLUTE(.);
		__TEXT_START12_END = ABSOLUTE(.);
	} > sram11_seg

/*	.NMIExceptionVector.text :*/
	.NMIExceptionVector.text : AT (LOADADDR(.NMIExceptionVector.literal) + SIZEOF(.NMIExceptionVector.literal))
	{
		__TEXT_ROM_START13 = LOADADDR(.NMIExceptionVector.literal) + SIZEOF(.NMIExceptionVector.literal);
		__TEXT_START13 = ABSOLUTE(.);
		_NMIExceptionVector_text_start = ABSOLUTE(.);
		*(.NMIExceptionVector.text)
		. = ALIGN(4);
		_NMIExceptionVector_text_end = ABSOLUTE(.);
		__TEXT_START13_END = ABSOLUTE(.);
	} > NMIExceptionVector.text_seg

/*	.KernelExceptionVector.literal :*/
	.KernelExceptionVector.literal : AT (LOADADDR(.NMIExceptionVector.text ) + SIZEOF(.NMIExceptionVector.text))
	{
		__TEXT_ROM_START14 = LOADADDR(.NMIExceptionVector.text) + SIZEOF(.NMIExceptionVector.text);
		__TEXT_START14 = ABSOLUTE(.);
		_KernelExceptionVector_literal_start = ABSOLUTE(.);
		*(.KernelExceptionVector.literal)
		. = ALIGN(4);
		_KernelExceptionVector_literal_end = ABSOLUTE(.);
		__TEXT_START14_END = ABSOLUTE(.);
	} > sram13_seg

/*	.KernelExceptionVector.text :*/
	.KernelExceptionVector.text : AT (LOADADDR(.KernelExceptionVector.literal) + SIZEOF(.KernelExceptionVector.literal))
	{
		__TEXT_ROM_START15 = LOADADDR(.KernelExceptionVector.literal) + SIZEOF(.KernelExceptionVector.literal);
		__TEXT_START15 = ABSOLUTE(.);
		_KernelExceptionVector_text_start = ABSOLUTE(.);
		*(.KernelExceptionVector.text)
		. = ALIGN(4);
		_KernelExceptionVector_text_end = ABSOLUTE(.);
		__TEXT_START15_END = ABSOLUTE(.);
	} > KernelExceptionVector.text_seg

/*	.UserExceptionVector.literal :*/
	.UserExceptionVector.literal : AT (LOADADDR(.KernelExceptionVector.text) + SIZEOF(.KernelExceptionVector.text))
	{
		__TEXT_ROM_START16 = LOADADDR(.KernelExceptionVector.text) + SIZEOF(.KernelExceptionVector.text);
		__TEXT_START16 = ABSOLUTE(.);
		_UserExceptionVector_literal_start = ABSOLUTE(.);
		*(.UserExceptionVector.literal)
		. = ALIGN(4);		
		_UserExceptionVector_literal_end = ABSOLUTE(.);
		__TEXT_START16_END = ABSOLUTE(.);
	} > sram15_seg

/*	.UserExceptionVector.text :*/
	.UserExceptionVector.text : AT (LOADADDR(.UserExceptionVector.literal) + SIZEOF(.UserExceptionVector.literal))
	{
		__TEXT_ROM_START17 = LOADADDR(.UserExceptionVector.literal) + SIZEOF(.UserExceptionVector.literal);
		__TEXT_START17 = ABSOLUTE(.);
		_UserExceptionVector_text_start = ABSOLUTE(.);
		*(.UserExceptionVector.text)
		. = ALIGN(4);
		_UserExceptionVector_text_end = ABSOLUTE(.);
		__TEXT_START17_END = ABSOLUTE(.);
	} > UserExceptionVector.text_seg

/*	.DoubleExceptionVector.literal :*/
	.DoubleExceptionVector.literal : AT (LOADADDR(.UserExceptionVector.text) + SIZEOF(.UserExceptionVector.text))
	{
		__TEXT_ROM_START18 = LOADADDR(.UserExceptionVector.text) + SIZEOF(.UserExceptionVector.text);
		__TEXT_START18 = ABSOLUTE(.);
		_DoubleExceptionVector_literal_start = ABSOLUTE(.);
		*(.DoubleExceptionVector.literal)
		. = ALIGN(4);
		_DoubleExceptionVector_literal_end = ABSOLUTE(.);
		__TEXT_START18_END = ABSOLUTE(.);
	} > sram17_seg

/*	.DoubleExceptionVector.text :*/
	.DoubleExceptionVector.text : AT (LOADADDR(.DoubleExceptionVector.literal ) + SIZEOF(.DoubleExceptionVector.literal))
	{
		__TEXT_ROM_START19 = LOADADDR(.DoubleExceptionVector.literal) + SIZEOF(.DoubleExceptionVector.literal);
		__TEXT_START19 = ABSOLUTE(.);
		_DoubleExceptionVector_text_start = ABSOLUTE(.);
		*(.DoubleExceptionVector.text)
		. = ALIGN(4);
		_DoubleExceptionVector_text_end = ABSOLUTE(.);
		__TEXT_START19_END = ABSOLUTE(.);
	} > DoubleExceptionVector.text_seg



/*	.vector_text_msg :*/
	.vector_text_msg (LOADADDR(.DoubleExceptionVector.text ) + SIZEOF(.DoubleExceptionVector.text)) :
	{
		. = ALIGN(4);
		_vector_text_msg_start = ABSOLUTE(.);

		LONG(_WindowVectors_text_start);
		LONG(_WindowVectors_text_end);
		LONG(LOADADDR(.WindowVectors.text));

		LONG(_Level2InterruptVector_literal_start);
		LONG(_Level2InterruptVector_literal_end);
		LONG(LOADADDR(.Level2InterruptVector.literal));

		LONG(_Level2InterruptVector_text_start);
		LONG(_Level2InterruptVector_text_end);
		LONG(LOADADDR(.Level2InterruptVector.text));

		LONG(_Level3InterruptVector_literal_start );
		LONG(_Level3InterruptVector_literal_end);
		LONG(LOADADDR(.Level3InterruptVector.literal));

		LONG(_Level3InterruptVector_text_start );
		LONG(_Level3InterruptVector_text_end );
		LONG(LOADADDR(.Level3InterruptVector.text));

		LONG(_Level4InterruptVector_literal_start);
		LONG(_Level4InterruptVector_literal_end);
		LONG(LOADADDR(.Level4InterruptVector.literal));

		LONG(_Level4InterruptVector_text_start);
		LONG(_Level4InterruptVector_text_end);
		LONG(LOADADDR(.Level4InterruptVector.text));

		LONG(_Level5InterruptVector_literal_start);
		LONG(_Level5InterruptVector_literal_end);
		LONG(LOADADDR(.Level5InterruptVector.literal));

		LONG(_Level5InterruptVector_text_start);
		LONG(_Level5InterruptVector_text_end);
		LONG(LOADADDR(.Level5InterruptVector.text));

/*
		LONG(_DebugExceptionVector_literal_start);
		LONG(_DebugExceptionVector_literal_end);
		LONG(LOADADDR(.DebugExceptionVector.literal));

		LONG(_DebugExceptionVector_text_start);
		LONG(_DebugExceptionVector_text_end);
		LONG(LOADADDR(.DebugExceptionVector.text));
*/

		LONG(_NMIExceptionVector_literal_start);
		LONG(_NMIExceptionVector_literal_end);
		LONG(LOADADDR(.NMIExceptionVector.literal));

		LONG(_NMIExceptionVector_text_start);
		LONG(_NMIExceptionVector_text_end);
		LONG(LOADADDR(.NMIExceptionVector.text));

		LONG(_KernelExceptionVector_literal_start );
		LONG(_KernelExceptionVector_literal_end );
		LONG(LOADADDR(.KernelExceptionVector.literal));

		LONG(_KernelExceptionVector_text_start);
		LONG(_KernelExceptionVector_text_end);
		LONG(LOADADDR(.KernelExceptionVector.text));	

		LONG(_UserExceptionVector_literal_start);
		LONG(_UserExceptionVector_literal_end);
		LONG(LOADADDR(.UserExceptionVector.literal));

		LONG(_UserExceptionVector_text_start );
		LONG(_UserExceptionVector_text_end);
		LONG(LOADADDR(.UserExceptionVector.text));

		LONG(_DoubleExceptionVector_literal_start);
		LONG(_DoubleExceptionVector_literal_end);
		LONG(LOADADDR(.DoubleExceptionVector.literal));	

		LONG(_DoubleExceptionVector_text_start);
		LONG(_DoubleExceptionVector_text_end);
		LONG(LOADADDR(.DoubleExceptionVector.text));

		LONG(0);
		LONG(0);
		LONG(0);

		. = ALIGN(4);
		_vector_text_msg_end = ABSOLUTE(.);
/*	} > srom*/
	} > rom_vector


	.rom_dummy :
	{
		*(.rom_dummy)
	} > rom_text


	.btdm_rodata :
	{
		. = ALIGN(4);
		__TEXT_START34 = ABSOLUTE(.);

/*		Debug/lib_btdm5.a (.rodata)*/

/*		/FPGA/lib_btdm/bin/Optek_HiFi3_Dev_02/Debug/lib_btdm.a (.rodata)*/

/*		../../../../lib_btdm/bin/Optek_HiFi3_Dev_02/Debug/lib_btdm.a (.rodata)*/

		. = ALIGN(4);
		__TEXT_START34_END = ABSOLUTE(.);
	} > rom_text
/*	} > btdm_rom*/


	.rom_dumm_xa_libc_text :
	{
		*(.rom_dumm_xa_libc_text)
	} > rom_xa_libc_text

/*	.xa_libc_text :*/
/*	.xa_libc_text (LOADADDR(.rom_dumm_xa_libc_text) + SIZEOF(.rom_dumm_xa_libc_text)) :*/
	.xa_libc_text : AT (LOADADDR(.rom_dumm_xa_libc_text) + SIZEOF(.rom_dumm_xa_libc_text))
	{
		. = ALIGN(16);
		__xa_libc_text_rom_start = LOADADDR(.rom_dumm_xa_libc_text) + SIZEOF(.rom_dumm_xa_libc_text);
		__xa_libc_text_ram_start = ABSOLUTE(.);

		*(.xa_libc_text)

/*		lib/libc.a (.literal .text)
		lib/libgcc.a (.literal .text)
		lib/libgdbio.a (.literal .text)
		lib/libhal.a (.literal .text)
*/


		/* from libc.a and from libgcc.a */
		lib/memcmp.o (.literal .text)
		lib/memmove.o (.literal .text)
		lib/memcpy.o (.literal .text)
		lib/memset.o (.literal .text)


		lib/_addsubdf3.o (.literal .text)
		lib/_muldf3.o (.literal .text)
		lib/_cmpdf2.o (.literal .text)
		lib/_fixdfsi.o (.literal .text)
		lib/_fixdfdi.o (.literal .text)
		lib/_fixunsdfsi.o (.literal .text)
		lib/_divdi3.o (.literal .text)
		lib/vec_memcpy.o (.literal .text)
		lib/vec_memset.o (.literal .text)
		lib/softfloat_cmn_df.o (.literal .text)
		lib/softfloat_div_df.o (.literal .text)

		lib/_divsi3.o (.literal .text)
		lib/_modsi3.o (.literal .text)
		lib/_udivsi3.o (.literal .text)
		lib/_umodsi3.o (.literal .text)
		lib/_umulsidi3.o (.literal .text)
		lib/_ashldi3.o (.literal .text)
		lib/_floatsidf.o (.literal .text)
		lib/softfloat_cmn.o (.literal .text)
		lib/softfloat_cmn_div.o (.literal .text)

		lib/ieee754divs.o (.literal .text)
		lib/_truncdfsf2.o (.literal .text)
		lib/_extendsfdf2.o (.literal .text)
		lib/_muldi3.o (.literal .text)
		lib/_udivdi3.o (.literal .text)
		lib/_umoddi3.o (.literal .text)
		lib/abs.o (.literal .text)

		/*it is used by the function `datmos_downmixer_init',
		calculate_proper_downmix_mode' and
		datmos_downmixer_get_num_of_out_channels'*/
		/*source/common/dpf_log.o (.literal .text)*/
		/*lib/dpf_log.o (.literal .text)*/

		KEEP(*(.init))
		KEEP(*(.init.literal))

		. = ALIGN(16);

		__xa_libc_text_ram_end = ABSOLUTE(.);
/*	} > rom_text*/
/*	} > rom_xa_libc_text*/
	} > i_ram_xa_libc
	

	.rom_dummy_dd_ac3_dec_lib_rodata :
	{
		*(.rom_dummy_dd_ac3_dec_lib_rodata)
	} > xa_dd_ac3_dec_lib_rodata

	/*use 0x1ebc bytes of space*/
	.xa_dd_ac3_dec_lib_rodata :
	{
		. = ALIGN(4);

		__xa_dd_ac3_dec_lib_rodata_ram_start = ABSOLUTE(.);

		*(.xa_dd_ac3_dec_lib_rodata)

		__xa_dd_ac3_dec_lib_rodata_ram_end = ABSOLUTE(.);
		. = ALIGN(4);
	} > xa_dd_ac3_dec_lib_rodata


	.rom_dummy_dd_ac3_dec_lib_txt :
	{
		*(.rom_dummy_dd_ac3_dec_lib_txt)
	} > xa_dd_ac3_dec_lib_txt

	/*use 0x65c4 bytes of space*/
/*	.xa_dd_ac3_dec_lib_txt :*/
	.xa_dd_ac3_dec_lib_txt : AT (LOADADDR(.rom_dummy_dd_ac3_dec_lib_txt) + SIZEOF(.rom_dummy_dd_ac3_dec_lib_txt))
	{
		. = ALIGN(4);
		__xa_dd_ac3_dec_lib_text_rom_start = LOADADDR(.rom_dummy_dd_ac3_dec_lib_txt) + SIZEOF(.rom_dummy_dts_dmp_dec_lib_txt);
		__xa_dd_ac3_dec_lib_text_ram_start = ABSOLUTE(.);

		*(.xa_dd_ac3_dec_lib_txt)

		. = ALIGN(4);
		__xa_dd_ac3_dec_lib_text_ram_end = ABSOLUTE(.);
/*	} > xa_dd_ac3_dec_lib_txt*/
	} > i_ram_xa_dd_ac3_codec


	.rom_dummy_dts_dmp_lib_rodata :
	{
		*(.rom_dummy_dts_dmp_lib_rodata)
	} > xa_dts_dmp_dec_lib_rodata

	/*use 0x1dd70 bytes of space*/
	.xa_dts_dmp_dec_lib_rodata :
	{
		. = ALIGN(4);
/*		__xa_dts_dmp_dec_lib_rodata_rom_start = LOADADDR(.rom_dummy_dts_dmp_lib_rodata) + SIZEOF(.rom_dummy_dts_dmp_lib_rodata);*/
		__xa_dts_dmp_dec_lib_rodata_rom_start = ABSOLUTE(.);

		*(.xa_dts_dmp_dec_lib_rodata)

		. = ALIGN(4);
		__xa_dts_dmp_dec_lib_rodata_rom_end = ABSOLUTE(.);
	} > xa_dts_dmp_dec_lib_rodata


	.rom_dummy_dts_dmp_dec_lib_txt :
	{
		*(.rom_dummy_dts_dmp_dec_lib_txt)
	} > xa_dts_dmp_dec_lib_txt

	/*use 0xb0ac bytes of space*/
/*	.xa_dts_dmp_dec_lib_txt :*/
	.xa_dts_dmp_dec_lib_txt : AT (LOADADDR(.rom_dummy_dts_dmp_dec_lib_txt) + SIZEOF(.rom_dummy_dts_dmp_dec_lib_txt))
	{
		. = ALIGN(4);
		__xa_dts_dmp_dec_lib_text_rom_start = LOADADDR(.rom_dummy_dts_dmp_dec_lib_txt) + SIZEOF(.rom_dummy_dts_dmp_dec_lib_txt);
		__xa_dts_dmp_dec_lib_text_ram_start = ABSOLUTE(.);

		*(.xa_dts_dmp_dec_lib_txt)

		. = ALIGN(4);
		__xa_dts_dmp_dec_lib_text_ram_end = ABSOLUTE(.);
/*	} > xa_dts_dmp_dec_lib_txt*/
	} > i_ram_xa_dts_dmp_codec


	.rom_dummy_codec_rodata :
	{
		*(.rom_dummy_codec_rodata)
	} > rom_text


	.rom_dummy_dd_ec3_dec_lib_rodata :
	{
		*(.rom_dummy_dd_ec3_dec_lib_rodata)
	} > xa_dd_ec3_dec_lib_rodata

	/*use 0xd25c bytes of space*/
	.xa_dd_ec3_dec_lib_rodata :
	{
		. = ALIGN(4);
/*		__xa_dd_ec3_dec_lib_rodata_rom_start = LOADADDR(.rom_dummy_dd_ec3_dec_lib_rodata) + SIZEOF(.rom_dummy_dd_ec3_dec_lib_rodata);*/
		__xa_dd_ec3_dec_lib_rodata_rom_start = ABSOLUTE(.);

		*(.xa_dd_ec3_dec_lib_rodata)

		. = ALIGN(4);
		__xa_dd_ec3_dec_lib_rodata_rom_end = ABSOLUTE(.);
	} > xa_dd_ec3_dec_lib_rodata


	.rom_dummy_dd_ec3_dec_lib_txt :
	{
		*(.rom_dummy_dd_ec3_dec_lib_txt)
	} > xa_dd_ec3_dec_lib_txt

	/*use 0xd260 bytes of space*/
/*	.xa_dd_ec3_dec_lib_txt :*/
	.xa_dd_ec3_dec_lib_txt : AT (LOADADDR(.rom_dummy_dd_ec3_dec_lib_txt) + SIZEOF(.rom_dummy_dd_ec3_dec_lib_txt))
	{
		. = ALIGN(4);
		__xa_dd_ec3_dec_lib_text_rom_start = LOADADDR(.rom_dummy_dd_ec3_dec_lib_txt) + SIZEOF(.rom_dummy_dd_ec3_dec_lib_txt);
		__xa_dd_ec3_dec_lib_text_ram_start = ABSOLUTE(.);

		*(.xa_dd_ec3_dec_lib_txt)

		. = ALIGN(4);
		__xa_dd_ec3_dec_lib_text_ram_end = ABSOLUTE(.);
/*	} > xa_dd_ec3_dec_lib_txt*/
	} > i_ram_xa_dd_ec3_codec


	.upgrade_hw_set_rodata :
	{
		*(.upgrade_hw_set_rodata)
	} > upgrade_hw_set_rodata


	.boot_rodata :
	{
		*(.boot_rodata)
	} > rom_boot_data


	.rom_dummy_all_rodata :
	{
		*(.rom_dummy_rodata4)
	} > rom_text

/*	.all_rodata :*/
	.all_rodata (LOADADDR(.rom_dummy_all_rodata) + SIZEOF(.rom_dummy_all_rodata)) :
/*	.all_rodata : AT (LOADADDR(.rom_dummy_all_rodata) + SIZEOF(.rom_dummy_all_rodata))*/
	{
		. = ALIGN(4);
		__all_rodata_rom_start = LOADADDR(.rom_dummy_all_rodata) + SIZEOF(.rom_dummy_all_rodata);
		__all_rodata_ram_start = ABSOLUTE(.);

		*(.rodata)
		*(.rodata.*)

		*(.clib.rodata)
		*(.clib.rodata.*)

		*(.rtos.rodata)
		*(.rtos.rodata.*)

		. = ALIGN(4);
		__all_rodata_ram_end = ABSOLUTE(.);

	} > rom_text
/*	} > d_ram*/
/*	} > psram_text*/


/*	.btdm_data :*/
	.btdm_data (LOADADDR(.all_rodata) + SIZEOF(.all_rodata)) :
/*	.btdm_data : AT (LOADADDR(.all_rodata) + SIZEOF(.all_rodata))*/
	{
		. = ALIGN(4);
		__btdm_data_rom_start = LOADADDR(.all_rodata) + SIZEOF(.all_rodata);
		__btdm_data_start = ABSOLUTE(.);
/*
		Debug/lib_btdm5.a (.data)
*/
/*		/FPGA/lib_btdm/bin/Optek_HiFi3_Dev_02/Debug/lib_btdm.a (.data)*/

/*		../../../../lib_btdm/bin/Optek_HiFi3_Dev_02/Debug/lib_btdm.a (.data)*/

		. = ALIGN(4);
		__btdm_data_end = ABSOLUTE(.);

/*	} > d_ram*/
	} > rom_text


/*	.data :*/
/*	.data (LOADADDR(.btdm_data) + SIZEOF(.btdm_data)) :*/
/*	.data : AT (LOADADDR(.btdm_data) + SIZEOF(.btdm_data))*/
	.data : AT (LOADADDR(.btdm_data) + SIZEOF(.btdm_data))
	{
		. = ALIGN(8);
		__data_rom_start = LOADADDR(.btdm_data) + SIZEOF(.btdm_data);
		__data_ram_start = ABSOLUTE(.);

		*(.data)
		*(.data.*)

		*(.eh_frame)
		*(.eh_frame.*)


		*(.clib.data)
		*(.clib.data.*)


		*(.rtos.data)
		*(.data.data.*)

		. = ALIGN(8);
		__data_ram_end = ABSOLUTE(.);

	} > d_ram
/*	} > rom_text*/


/*	.flash_prog_text :*/
/*	.flash_prog_text (LOADADDR(.data) + SIZEOF(.data)) :*/
	.flash_prog_text : AT (LOADADDR(.data) + SIZEOF(.data))
	{
		. = ALIGN(4);
		__flash_prog_rom_start = LOADADDR(.data) + SIZEOF(.data);
		__flash_prog_ram_start = ABSOLUTE(.);

/*		source/common/app_flash_program.o (.literal .text)
		source/common/SpiFlash_W25QXX.o (.literal .text)*/

		*(.flash_prog_text.literal)
		*(.flash_prog_text.text)
		*(.flash_prog_text)

		. = ALIGN(4);
		__flash_prog_ram_end = ABSOLUTE(.);
/*	} > rom_text*/
	} > i_ram


/*	.libkernel_text :*/
/*	.libkernel_text (LOADADDR(.flash_prog_text) + SIZEOF(.flash_prog_text)) :*/
	.libkernel_text : AT (LOADADDR(.flash_prog_text) + SIZEOF(.flash_prog_text))
	{
		. = ALIGN(4);
		__libkernel_rom_start = LOADADDR(.flash_prog_text) + SIZEOF(.flash_prog_text);
		__libkernel_ram_start = ABSOLUTE(.);

/*		libkernel.a (.literal .text)*/

		*(.libkernel_text.literal)
		*(.libkernel_text.text)
		*(.libkernel_text)

		lib/Optek_lib_freeRTOS.a (.literal .text)

		lib/libhal.a (.literal .text)


		lib/libxa_sbc_dec.a (.literal .text)
		lib/opteklib_sbc_enc.a (.literal .text)

		lib/xa_msbc_dec.a (.literal .text)
		lib/xa_msbc_enc.a (.literal .text)

/*
		source/os/threadRTOS/xtensa_vectors.o (.literal .text)
		source/os/threadRTOS/xtensa_context.o (.literal .text)
*/

/*
		source/os/freeRTOS/Xtensa/xtensa_vectors.o (.literal .text)
		source/os/freeRTOS/Xtensa/xtensa_context.o (.literal .text)
*/

/*
		source/os/freeRTOS/portable/XCC/Xtensa/xtensa_vectors.o (.literal .text)
		source/os/freeRTOS/portable/XCC/Xtensa/xtensa_context.o (.literal .text)
*/


/*		source/common/windowspill_asm.o (.literal .text)*/

		lib/windowspill_asm.o (.literal .text)

/*		source/common/xtensa_timer1.o (.literal .text)*/

		. = ALIGN(4);
		__libkernel_ram_end = ABSOLUTE(.);
/*	} > rom_text*/
	} > i_ram


/*	.btdm_text :*/
	.btdm_text (LOADADDR(.libkernel_text) + SIZEOF(.libkernel_text)) :
/*	.btdm_text : AT (LOADADDR(.libkernel_text) + SIZEOF(.libkernel_text))*/
	{
		. = ALIGN(4);
		__TEXT_START34 = ABSOLUTE(.);
/*
		Debug/lib_btdm5.a (.literal .text)
*/
/*		/FPGA/lib_btdm/bin/Optek_HiFi3_Dev_02/Debug/lib_btdm.a (.literal .text)*/

/*		../../../../lib_btdm/bin/Optek_HiFi3_Dev_02/Debug/lib_btdm.a (.literal .text)*/

		. = ALIGN(4);
		__TEXT_START34_END = ABSOLUTE(.);
	} > rom_text
/*	} > btdm_rom*/
/*	} > srom*/


	/*sona dsp mem set*/
	/* 
	if open SONA_DSP_MALLOC_L2 
	*no load in rom_text
	.sona_L2_text (LOADADDR(.btdm_text) + SIZEOF(.btdm_text)) :
	{
		. = ALIGN(4);
		__sona_L2_text_rom_start = LOADADDR(.btdm_text) + SIZEOF(.btdm_text);
		__sona_L2_text_ram_start = ABSOLUTE(.);

		*(.sona_L2_text.literal)
		*(.sona_L2_text.text)
		*(.sona_L2_text)

		. = ALIGN(4);
		__sona_L2_text_rom_end = ABSOLUTE(.);
	} > rom_text

	*load in i_ram
	.sona_L2_text : AT (LOADADDR(.btdm_text) + SIZEOF(.btdm_text))
	{
		. = ALIGN(4);
		__sona_L2_text_rom_start = LOADADDR(.btdm_text) + SIZEOF(.btdm_text);
		__sona_L2_text_ram_start = ABSOLUTE(.);

		*(.sona_L2_text.literal)
		*(.sona_L2_text.text)
		*(.sona_L2_text)

		. = ALIGN(4);
		__sona_L2_text_rom_end = ABSOLUTE(.);
	} > i_ram


	*load in psram_bss
	.sona_L2_text : AT (LOADADDR(.btdm_text) + SIZEOF(.btdm_text))
	{
		. = ALIGN(4);
		__sona_L2_text_rom_start = LOADADDR(.btdm_text) + SIZEOF(.btdm_text);
		__sona_L2_text_ram_start = ABSOLUTE(.);

		*(.sona_L2_text.literal)
		*(.sona_L2_text.text)
		*(.sona_L2_text)

		. = ALIGN(4);
		__sona_L2_text_rom_end = ABSOLUTE(.);
	} > psram_bss


	*no load in rom_text
	.sona_L1_text (LOADADDR(.sona_L2_text) + SIZEOF(.sona_L2_text)) :
	{
		. = ALIGN(4);
		__sona_L1_text_rom_start = LOADADDR(.sona_L2_text) + SIZEOF(.sona_L2_text);
		__sona_L1_text_ram_start = ABSOLUTE(.);
        
		*(.sona_L1_text.literal)
		*(.sona_L1_text.text)
		*(.sona_L1_text)

		. = ALIGN(4);
		__sona_L1_text_rom_end = ABSOLUTE(.);
	} > rom_text


	*load in i_ram
	.sona_L1_text : AT (LOADADDR(.sona_L2_text) + SIZEOF(.sona_L2_text))
	{
		. = ALIGN(4);
		__sona_L1_text_rom_start = LOADADDR(.sona_L2_text) + SIZEOF(.sona_L2_text);
		__sona_L1_text_ram_start = ABSOLUTE(.);
        
		*(.sona_L1_text.literal)
		*(.sona_L1_text.text)
		*(.sona_L1_text)

		. = ALIGN(4);
		__sona_L1_text_rom_end = ABSOLUTE(.);
    } > i_ram


	*load in psram_bss
	.sona_L1_text : AT (LOADADDR(.sona_L2_text) + SIZEOF(.sona_L2_text))
	{
		. = ALIGN(4);
		__sona_L1_text_rom_start = LOADADDR(.sona_L2_text) + SIZEOF(.sona_L2_text);
		__sona_L1_text_ram_start = ABSOLUTE(.);
        
		*(.sona_L1_text.literal)
		*(.sona_L1_text.text)
		*(.sona_L1_text)

		. = ALIGN(4);
		__sona_L1_text_rom_end = ABSOLUTE(.);
  	} > psram_bss

	*/


/*	.sona_L1_text (LOADADDR(.sona_L2_text) + SIZEOF(.sona_L2_text)) :*/
	.sona_L1_text : AT (LOADADDR(.btdm_text) + SIZEOF(.btdm_text))
	{
		. = ALIGN(4);
		__sona_L1_text_rom_start = LOADADDR(.btdm_text) + SIZEOF(.btdm_text);
		__sona_L1_text_ram_start = ABSOLUTE(.);
        
		*(.sona_L1_text.literal)
		*(.sona_L1_text.text)
		*(.sona_L1_text)

		. = ALIGN(4);
		__sona_L1_text_rom_end = ABSOLUTE(.);
/*	} > rom_text*/
    } > i_ram
/*  } > psram_bss*/    

/*	.sona_dsp_text (LOADADDR(.sona_L1_text) + SIZEOF(.sona_L1_text)) :*/
	.sona_dsp_text : AT (LOADADDR(.sona_L1_text) + SIZEOF(.sona_L1_text))
	{
		. = ALIGN(4);
		__sona_dsp_text_rom_start = LOADADDR(.sona_L1_text) + SIZEOF(.sona_L1_text);
		__sona_dsp_text_ram_start = ABSOLUTE(.);

		/*lib/libMackieWoofer-24091701.a (.literal .text)*/
		lib/libMackieWoofer-24092102.a (.literal .text)

		. = ALIGN(4);
		__sona_dsp_text_rom_end = ABSOLUTE(.);
/*	} > rom_text*/
    } > i_ram
/*  } > psram_bss*/


/*	.all_text :*/
	.all_text (LOADADDR(.sona_dsp_text) + SIZEOF(.sona_dsp_text)) :
/*	.all_text : AT (LOADADDR(.sona_dsp_text) + SIZEOF(.sona_dsp_text))*/
	{
		. = ALIGN(4);
		__all_text_rom_start = LOADADDR(.sona_dsp_text) + SIZEOF(.sona_dsp_text);
		__all_text_ram_start = ABSOLUTE(.);


		KEEP(*(.init))
		KEEP(*(.init.literal))

/*
		source/os/threadRTOS/*.o (.literal .text)
		source/os/freeRTOS/*.o (.literal .text)
*/

/*		*(.literal .text .literal.* .text.* .stub .gnu.warning .gnu.linkonce.literal.* .gnu.linkonce.t.*.literal .gnu.linkonce.t.*)*/
		*(.literal .text .literal.* .text.*)


/*		*(.eh_frame.literal)
		*(.eh_frame.text)*.
*/

		. = ALIGN(4);
		__all_text_ram_end  = ABSOLUTE(.);
	
/*	} > i_ram*/
	} > rom_text
/*	} > psram_text*/
	


/*	.internal_ram_1_text :*/
	.internal_ram_1_text : AT (LOADADDR(.all_text) + SIZEOF(.all_text))
	{
		. = ALIGN(4);
		__internal_ram_1_rom_start = LOADADDR(.all_text) + SIZEOF(.all_text);
		__internal_ram_1_ram_start = ABSOLUTE(.);

		*(.internal_ram_1_text.literal)
		*(.internal_ram_1_text.text)
		*(.internal_ram_1_text)

		*(.bt_host_internal_ram_text.literal)
		*(.bt_host_internal_ram_text.text)
		*(.bt_host_internal_ram_text)

/*		*(.eh_frame.literal)
		*(.eh_frame.text)
*/


/*		*(.dsp_critical_sec_text.literal)
		*(.dsp_critical_sec_text.text)
		*(.dsp_critical_sec_text)
*/

		. = ALIGN(4);
		/*better in iram part*/
		*(.RWIP_TEXT.literal)
		*(.RWIP_TEXT.text)
		*(.RWIP_TEXT)

		*(.UART_TEXT.literal)
		*(.UART_TEXT.text)
		*(.UART_TEXT)

		*(.HCI_TL_TEXT.literal)
		*(.HCI_TL_TEXT.text)
		*(.HCI_TL_TEXT)

		*(.HCI_TEXT.literal)
		*(.HCI_TEXT.text)
		*(.HCI_TEXT)

		*(.H4TL_TEXT.literal)
		*(.H4TL_TEXT.text)
		*(.H4TL_TEXT)

		*(.HCI_FC_TEXT.literal)
		*(.HCI_FC_TEXT.text)
		*(.HCI_FC_TEXT)

		*(.CO_UTILS_TEXT.literal)
		*(.CO_UTILS_TEXT.text)
		*(.CO_UTILS_TEXT)

		*(.KE_QUEUE_TEXT.literal)
		*(.KE_QUEUE_TEXT.text)
		*(.KE_QUEUE_TEXT)

		*(.KE_TASK_TEXT.literal)
		*(.KE_TASK_TEXT.text)
		*(.KE_TASK_TEXT)

		*(.KE_TIMER_TEXT.literal)
		*(.KE_TIMER_TEXT.text)
		*(.KE_TIMER_TEXT)


		/*must in iram part*/
		*(.RWIP_DRIVER_SLEEP_TEXT.literal)
		*(.RWIP_DRIVER_SLEEP_TEXT.text)
		*(.RWIP_DRIVER_SLEEP_TEXT)

		*(.KE_MSG_TEXT.literal)
		*(.KE_MSG_TEXT.text)
		*(.KE_MSG_TEXT)

		*(.KE_EVENT_TEXT.literal)
		*(.KE_EVENT_TEXT.text)
		*(.KE_EVENT_TEXT)

		*(.KE_MEM_TEXT.literal)
		*(.KE_MEM_TEXT.text)
		*(.KE_MEM_TEXT)

		*(.KE_EVENT_SCH_TEXT.literal)
		*(.KE_EVENT_SCH_TEXT.text)
		*(.KE_EVENT_SCH_TEXT)	

		*(.KE_TASK_SCH_TEXT.literal)
		*(.KE_TASK_SCH_TEXT.text)
		*(.KE_TASK_SCH_TEXT)						

		*(.RF_RIPPLE_TEXT.literal)
		*(.RF_RIPPLE_TEXT.text)
		*(.RF_RIPPLE_TEXT)

		/*common part*/
		*(.LC3_COMMON_TXT.literal)
		*(.LC3_COMMON_TXT.text)
		*(.LC3_COMMON_TXT)

		*(.LC3_DEC_ARI_TXT.literal)
		*(.LC3_DEC_ARI_TXT.text)
		*(.LC3_DEC_ARI_TXT)


		*(.LC3_DEC_ARI_TXT.literal)
		*(.LC3_DEC_ARI_TXT.text)
		*(.LC3_DEC_ARI_TXT)

		/*decode part*/
		*(.LC3_DEC_IDCT_TXT.literal)
		*(.LC3_DEC_IDCT_TXT.text)
		*(.LC3_DEC_IDCT_TXT)

		*(.LC3_DEC_TNS_TXT.literal)
		*(.LC3_DEC_TNS_TXT.text)
		*(.LC3_DEC_TNS_TXT)

		*(.LC3_DEC_LPTF_TXT.literal)
		*(.LC3_DEC_LPTF_TXT.text)
		*(.LC3_DEC_LPTF_TXT)

		*(.LC3_DEC_NOISE_FILLING_TXT.literal)
		*(.LC3_DEC_NOISE_FILLING_TXT.text)
		*(.LC3_DEC_NOISE_FILLING_TXT)

		*(.LC3_DEC_GOLBAL_GAIN_TXT.literal)
		*(.LC3_DEC_GOLBAL_GAIN_TXT.text)
		*(.LC3_DEC_GOLBAL_GAIN_TXT)

		*(.LC3_DEC_ENTROPY_TXT.literal)
		*(.LC3_DEC_ENTROPY_TXT.text)
		*(.LC3_DEC_ENTROPY_TXT)	

		*(.LC3_DEC_PLC_UPDATE_TXT.literal)
		*(.LC3_DEC_PLC_UPDATE_TXT.text)
		*(.LC3_DEC_PLC_UPDATE_TXT)	

		*(.LC3_DEC_SNS_QUANTIZE_SCF_TXT.literal)
		*(.LC3_DEC_SNS_QUANTIZE_SCF_TXT.text)
		*(.LC3_DEC_SNS_QUANTIZE_SCF_TXT)

		*(.LC3_DEC_SCF_SCALING_TXT.literal)
		*(.LC3_DEC_SCF_SCALING_TXT.text)
		*(.LC3_DEC_SCF_SCALING_TXT)

		*(.LC3_DEC_RESIDUAL_TXT.literal)
		*(.LC3_DEC_RESIDUAL_TXT.text)
		*(.LC3_DEC_RESIDUAL_TXT)

		*(.LC3_DEC_PLC_NOISE_SUB_TXT.literal)
		*(.LC3_DEC_PLC_NOISE_SUB_TXT.text)
		*(.LC3_DEC_PLC_NOISE_SUB_TXT)				

		*(.LC3_DEC_PLC_UPDATE_AFTER_IMDCT_TXT.literal)
		*(.LC3_DEC_PLC_UPDATE_AFTER_IMDCT_TXT.text)
		*(.LC3_DEC_PLC_UPDATE_AFTER_IMDCT_TXT)

		*(.LC3_DEC_COMMON_TXT.literal)
		*(.LC3_DEC_COMMON_TXT.text)
		*(.LC3_DEC_COMMON_TXT)

		*(.LC3_DEC_PLC_MAIN_TXT.literal)
		*(.LC3_DEC_PLC_MAIN_TXT.text)
		*(.LC3_DEC_PLC_MAIN_TXT)

		*(.LC3_DEC_PLC_APPLY_TXT.literal)
		*(.LC3_DEC_PLC_APPLY_TXT.text)
		*(.LC3_DEC_PLC_APPLY_TXT)	

		*(.LC3_DEC_PLC_CLASSIFY_TXT.literal)
		*(.LC3_DEC_PLC_CLASSIFY_TXT.text)
		*(.LC3_DEC_PLC_CLASSIFY_TXT)

		*(.LC3_DEC_API_TXT.literal)
		*(.LC3_DEC_API_TXT.text)
		*(.LC3_DEC_API_TXT)	

		*(.LC3_DEC_LC3_TXT.literal)
		*(.LC3_DEC_LC3_TXT.text)
		*(.LC3_DEC_LC3_TXT)

		/*encode part*/
		*(.LC3_ENC_ARI_TXT.literal)
		*(.LC3_ENC_ARI_TXT.text)
		*(.LC3_ENC_ARI_TXT)

		*(.LC3_ENC_MDCT_TXT.literal)
		*(.LC3_ENC_MDCT_TXT.text)
		*(.LC3_ENC_MDCT_TXT)

		*(.LC3_ENC_TNS_TXT.literal)
		*(.LC3_ENC_TNS_TXT.text)
		*(.LC3_ENC_TNS_TXT)

		*(.LC3_ENC_LPTF_TXT.literal)
		*(.LC3_ENC_LPTF_TXT.text)
		*(.LC3_ENC_LPTF_TXT)

		*(.LC3_ENC_ATTACK_TXT.literal)
		*(.LC3_ENC_ATTACK_TXT.text)
		*(.LC3_ENC_ATTACK_TXT)

		*(.LC3_ENC_CUTOFF_TXT.literal)
		*(.LC3_ENC_CUTOFF_TXT.text)
		*(.LC3_ENC_CUTOFF_TXT)

		*(.LC3_EN_ENTROPY_TXT.literal)
		*(.LC3_EN_ENTROPY_TXT.text)
		*(.LC3_EN_ENTROPY_TXT)

		*(.LC3_ENC_ESTIMATE_GLOBAL_GAIN_TXT.literal)
		*(.LC3_ENC_ESTIMATE_GLOBAL_GAIN_TXT.text)
		*(.LC3_ENC_ESTIMATE_GLOBAL_GAIN_TXT)

		*(.LC3_ENC_NOISE_FACTOR_TXT.literal)
		*(.LC3_ENC_NOISE_FACTOR_TXT.text)
		*(.LC3_ENC_NOISE_FACTOR_TXT)

		*(.LC3_ENC_OLPA_TXT.literal)
		*(.LC3_ENC_OLPA_TXT.text)
		*(.LC3_ENC_OLPA_TXT)

		*(.LC3_ENC_QUANTIZE_TXT.literal)
		*(.LC3_ENC_QUANTIZE_TXT.text)
		*(.LC3_ENC_QUANTIZE_TXT)

		*(.LC3_ENC_RESAMPLE_TXT.literal)
		*(.LC3_ENC_RESAMPLE_TXT.text)
		*(.LC3_ENC_RESAMPLE_TXT)

		*(.LC3_ENC_SCALE_SIGNAL_TXT.literal)
		*(.LC3_ENC_SCALE_SIGNAL_TXT.text)
		*(.LC3_ENC_SCALE_SIGNAL_TXT)

		*(.LC3_ENC_SNS_COMPUTE_SCF_TXT.literal)
		*(.LC3_ENC_SNS_COMPUTE_SCF_TXT.text)
		*(.LC3_ENC_SNS_COMPUTE_SCF_TXT)

		*(.LC3_ENC_SNS_QUANTIZE_TXT.literal)
		*(.LC3_ENC_SNS_QUANTIZE_TXT.text)
		*(.LC3_ENC_SNS_QUANTIZE_TXT)

		*(.LC3_ENC_RESIDUAL_TXT.literal)
		*(.LC3_ENC_RESIDUAL_TXT.text)
		*(.LC3_ENC_RESIDUAL_TXT)		

		*(.LC3_ENC_LPC2RC_TXT.literal)
		*(.LC3_ENC_LPC2RC_TXT.text)
		*(.LC3_ENC_LPC2RC_TXT)

		*(.LC3_ENC_ADJUST_GLOBAL_GAIN_TXT.literal)
		*(.LC3_ENC_ADJUST_GLOBAL_GAIN_TXT.text)
		*(.LC3_ENC_ADJUST_GLOBAL_GAIN_TXT)

		*(.LC3_ENC_LC3_TXT.literal)
		*(.LC3_ENC_LC3_TXT.text)
		*(.LC3_ENC_LC3_TXT)

		*(.LC3_ENC_API_TXT.literal)
		*(.LC3_ENC_API_TXT.text)
		*(.LC3_ENC_API_TXT)				

		*(.LC3_ENC_LC3_TXT.literal)
		*(.LC3_ENC_LC3_TXT.text)
		*(.LC3_ENC_LC3_TXT)

		*(.LC3_ENC_COMMON_TXT.literal)
		*(.LC3_ENC_COMMON_TXT.text)
		*(.LC3_ENC_COMMON_TXT)

		. = ALIGN(4);
		__internal_ram_1_ram_end = ABSOLUTE(.);

	} > i_ram
/*	} > rom_1*/


/*	.interal_ram_1_text :*/
	.interal_ram_1_text : AT (LOADADDR(.internal_ram_1_text) + SIZEOF(.internal_ram_1_text))
	{
		. = ALIGN(4);
		__interal_ram_1_rom_start = LOADADDR(.internal_ram_1_text) + SIZEOF(.internal_ram_1_text);
		__interal_ram_1_ram_start = ABSOLUTE(.);

		*(.interal_ram_1_text.literal)
		*(.interal_ram_1_text.text)
		*(.interal_ram_1_text)

/*		*(.eh_frame.literal)
		*(.eh_frame.text)
*/
		. = ALIGN(4);
		__interal_ram_1_ram_end = ABSOLUTE(.);

	} > i_ram
/*	} > rom_1*/


/*	.dsrc_data_tbl :*/
/*	.dsrc_data_tbl (LOADADDR(.interal_ram_1_text) + SIZEOF(.interal_ram_1_text)) :*/
	.dsrc_data_tbl : AT (LOADADDR(.interal_ram_1_text) + SIZEOF(.interal_ram_1_text))
	{
		. = ALIGN(8);
		__dsrc_data_rom_start = LOADADDR(.interal_ram_1_text) + SIZEOF(.interal_ram_1_text);
		__dsrc_data_ram_start = ABSOLUTE(.);
		*(.dsrc_data_tbl)
		. = ALIGN(8);
		__dsrc_data_ram_end = ABSOLUTE(.);

/*	} > DSRC_DATA_TBL*/
	} > d_ram
/*	} > sdram_text*/


/*	.dsp_critical_sec_text :*/
/*	.dsp_critical_sec_text (LOADADDR(.dsrc_data_tbl) + SIZEOF(.dsrc_data_tbl)) :*/
	.dsp_critical_sec_text : AT (LOADADDR(.dsrc_data_tbl) + SIZEOF(.dsrc_data_tbl))
	{
		. = ALIGN(8);

		__dsp_critical_sec_rom_start = LOADADDR(.dsrc_data_tbl) + SIZEOF(.dsrc_data_tbl);
		__dsp_critical_sec_ram_start = ABSOLUTE(.);

		*(.dsp_critical_sec_text.literal)
		*(.dsp_critical_sec_text.text)
		*(.dsp_critical_sec_text)

		. = ALIGN(8);
		__dsp_critical_sec_ram_end = ABSOLUTE(.);

	} > i_ram
/*	} > rom_1*/


/*	.fs_search_file_text :*/
	.fs_search_file_text (LOADADDR(.dsp_critical_sec_text) + SIZEOF(.dsp_critical_sec_text)) :
/*	.fs_search_file_text : AT (LOADADDR(.dsp_critical_sec_text ) + SIZEOF(.dsp_critical_sec_text))*/
	{
		. = ALIGN(4);
		__fs_search_file_text_rom_start = LOADADDR(.dsp_critical_sec_text) + SIZEOF(.dsp_critical_sec_text);
		__fs_search_file_text_ram_start = ABSOLUTE(.);

		*(.fs_search_file_text.literal)
		*(.fs_search_file_text.text)
		*(.fs_search_file_text)

		. = ALIGN(4);
		__TEXT_FS_SEARCH_FILE_RAM_END = ABSOLUTE(.);
	} > rom_text


	.btdm_bss (NOLOAD):
	{
		. = ALIGN(4);

/*		Debug/lib_btdm5.a (.bss)*/

/*		/FPGA/lib_btdm/bin/Optek_HiFi3_Dev_02/Debug/lib_btdm.a (.bss)*/

/*		../../../../lib_btdm/bin/Optek_HiFi3_Dev_02/Debug/lib_btdm.a (.bss)*/

		. = ALIGN(4);
/*	} > btdm_bss_seg*/
	} > d_ram


	.internal_ram_boot_bss (NOLOAD):
	{
		. = ALIGN(4);

		*(.internal_ram_boot_bss)
		*(.internal_ram_boot_bss.*)

		. = ALIGN(4);

	} > d_ram_boot


	.codec_slow_memory_bss (NOLOAD) :
	{
		. = ALIGN (16);

		*(.codec_slow_memory_bss)
		*(.codec_slow_memory_bss.*)

		. = ALIGN (16);
	} > codec_slow_memory_bss


	.all_bss (NOLOAD):
	{
		. = ALIGN (16);
		_bss_start = ABSOLUTE(.);

		*(.bss)
		*(.bss.*)

		. = ALIGN (16);
		_bss_end = ABSOLUTE(.);
		
	} > d_ram
/*	} > all_bss*/
/*	} > rom_text*/


	.clib_bss (NOLOAD):
	{
		. = ALIGN (4);
		_clib_bss_start = ABSOLUTE(.);

		*(.text._init_reent_bss)
		*(.text._init_reent_bss.*)

		*(COMMON)
		*(.clib.bss)
		*(.clib.bss*)
	    
		*(.clib.percpu.bss)
		*(.clib.percpu.bss*)

		*(.rtos.percpu.bss)
		*(.rtos.percpu.bss*)

		*(.rtos.bss)
		*(.rtos.bss*)

		. = ALIGN (4);
		_clib_bss_end = ABSOLUTE(.);
		_end = ABSOLUTE(.);

	} > d_ram
/*	} > all_bss*/
/*	} > rom_text*/


	.usb_fs_bss (NOLOAD):
	{
		. = ALIGN(4);

		*(.usb_fs_bss)
		*(.usb_fs_bss.*)
		
		. = ALIGN(4);

	} > d_ram
/*	} > psram_bss*/

	.usb_folder_bss (NOLOAD):
	{
		. = ALIGN(4);

		*(.usb_folder_bss)
		*(.usb_folder_bss.*)

		. = ALIGN(4);
/*	} > usb_folder_bss*/
	} > d_ram
/*	} > psram_bss*/


	.usb_fs_delete_bss (NOLOAD):
	{
		. = ALIGN(4);

		*(.usb_fs_delete_bss)
		*(.usb_fs_delete_bss.*)

		. = ALIGN(4);
/*	} > usb_fs_delete_bss*/
	} > d_ram
/*	} > psram_bss*/


	.usbhost_bss (NOLOAD):
	{
		. = ALIGN(8);
	
		*(.usbhost_bss)
		*(.usbhost_bss.*)
		*(.usbDev_bss)
		*(.usbDev_bss.*)

		. = ALIGN(8);

	} > d_ram
/*	} > psram_bss*/


	.iphone_bss (NOLOAD):
	{
		. = ALIGN(4);

		*(.iphone_bss)
		*(.iphone_bss.*)

		. = ALIGN(4);
	} > d_ram
/*	} > psram_bss*/


	.dsp_bss (NOLOAD):
	{
		. = ALIGN(4);

		*(.d_dsp_bss)
		*(.d_dsp_bss.*)

		. = ALIGN(4);
	} > d_ram


/*	_heap_sentry = 0x3fe20000;*/
/*	_heap_sentry = 0x70FF0000;*/

	__stack = ORIGIN(d_ram_stack) + LENGTH(d_ram_stack);
	/*__stack = 0x70200000;*/

	VECTOR_BASE_ADDR = ORIGIN(i_ram_vector);
}

/*
CODEC_MEMORY_START            = ORIGIN(d_ram_codec);
*/
/*
ROM_DSP_CORE_START_ADDR      = ORIGIN(rom_dsp_jump_to_upgrade);
ROM_MCU_START_ADDR           = ORIGIN(rom_mcu_jump_to_upgrade);
*/


ROM_DSP_START_ADDR           = ORIGIN(rom_dsp_start);

ROM_BOOT_START_ADDR          = ORIGIN(rom_boot);

ROM_MCU_START_ADDR           = ORIGIN(rom_mcu_start);


/*
BOOT_TAG_START                = ORIGIN(rom_boot_data);
BOOT_TAG_END                  = ORIGIN(rom_boot_data) + LENGTH(rom_boot_data);
*/
BOOT_TAG_START                = ORIGIN(rom_boot_data);
BOOT_TAG_END                  = ORIGIN(rom_boot_data) + LENGTH(rom_boot_data);

ROM_DSP_UPGRADE_PRJOJECT_START_ADDR
                              = ORIGIN(rom_dsp_upgrade_project_boot);  

ROM_START_ADDR                = ORIGIN(rom);
ROM_BOOT_ADDR                 = ORIGIN(rom_boot);


SFLASH_START_ADDR             = ORIGIN(rom_boot);

I_ROM_START_ADDR              =  ORIGIN(i_rom);

D_ROM_START_ADDR              =  ORIGIN(d_rom);

/*
DDPLUS_PERSISTENT_START      = ORIGIN(DDPLUS_PERSISTENT_BSS);
DDPLUS_SCRATCH_START         = ORIGIN(DDPLUS_SCRATCH_BSS);
DDPLUS_OUTPUT_START          = ORIGIN(DDPLUS_OUTPUT_BSS);
*/


AUX_STREAM_START           = ORIGIN(stream_memory_bss);
AUX_STREAM_END             = ORIGIN(stream_memory_bss) + LENGTH(stream_memory_bss);

/*
AUX_STREAM_START             = 0x70E00000;
AUX_STREAM_END               = 0x70E10000;
*/

/*
AUX_STREAM_START             = 0x70E00000;
AUX_STREAM_END               = 0x70F00000;
*/


SPDIF_STREAM_START           = ORIGIN(stream_memory_bss);
SPDIF_STREAM_END             = ORIGIN(stream_memory_bss) + LENGTH(stream_memory_bss);

/*
SPDIF_STREAM_START           = 0x70E00000;
SPDIF_STREAM_END             = 0x70E18000;
*/

/*
SPDIF_STREAM_START           = 0x70700000;
SPDIF_STREAM_END             = 0x70720000;
*/

/*
SPDIF_STREAM_START           = 0x70300000;
SPDIF_STREAM_END             = 0x70500000;
*/


RECORD_FIFO_BUF_START        = SPDIF_STREAM_START;
RECORD_FIFO_BUF_END          = SPDIF_STREAM_END;


USB_SPEAKER_STREAM_START     = ORIGIN(stream_memory_bss) + LENGTH(stream_memory_bss) - 0x3000;
USB_SPEAKER_STREAM_END       = ORIGIN(stream_memory_bss) + LENGTH(stream_memory_bss);

BT_SBC_STREAM_START          = ORIGIN(stream_memory_bss) + LENGTH(stream_memory_bss) - 0x3000;
BT_SBC_STREAM_END            = ORIGIN(stream_memory_bss) + LENGTH(stream_memory_bss);


MP3_STREAM_START             = ORIGIN(stream_memory_bss) + LENGTH(stream_memory_bss) - 0x3000;
MP3_STREAM_END               = ORIGIN(stream_memory_bss) + LENGTH(stream_memory_bss);
/*
MP3_STREAM_START             = ORIGIN(stream_memory_bss);
MP3_STREAM_END               = ORIGIN(stream_memory_bss) + LENGTH(stream_memory_bss);
*/

/*XA_MEMTYPE_COPY_START        = 0x70400000;*/

XA_MEMTYPE_PERSIST_START     = 0x70500000;
XA_MEMTYPE_PERSIST_END       = 0x70500000+0x1E800;
	
XA_MEMTYPE_SCRATCH_START     = 0x70500000+0x1E800;
XA_MEMTYPE_SCRATCH_END       = 0x70500000+0x1E800+0xC800;

XA_MEMTYPE_INPUT_START       = 0x70500000+0x1E800+0xC800;
XA_MEMTYPE_INPUT_END         = 0x70500000+0x1E800+0xC800+0x8200;

XA_MEMTYPE_OUTPUT_START      = 0x70500000+0x1E800+0xC800+0x8200;
XA_MEMTYPE_OUTPUT_END        = 0x70500000+0x1E800+0xC800+0x8200+0x6000;


DUMP_BUFFER_ADDR             = ORIGIN(dump_memory_bss);

BT_DATA_1                    = ORIGIN(bt_data_1);
BT_DATA_2                    = ORIGIN(bt_data_2);

ROM_SAVE_DATA1_START          = ORIGIN(rom_save_data1);
ROM_SAVE_DATA1_END            = ORIGIN(rom_save_data1) + LENGTH(rom_save_data1);

ROM_SAVE_DATA2_START          = ORIGIN(rom_save_data2);
ROM_SAVE_DATA2_END            = ORIGIN(rom_save_data2) + LENGTH(rom_save_data2);

OPTEK_LINK_USER_DATA		 = ORIGIN(optek_link_user_data);


/*
XA_DTS_DMP_CODEC_SRC_START   = ORIGIN(xa_dts_dmp_dec_lib_txt);
XA_DTS_DMP_CODEC_SRC_END     = ORIGIN(xa_dts_dmp_dec_lib_txt) + LENGTH(xa_dts_dmp_dec_lib_txt);
*/

/*
XA_DTS_DMP_CODEC_DST_START   = ORIGIN(i_ram_xa_dts_dmp_codec);
XA_DTS_DMP_CODEC_DST_END     = ORIGIN(i_ram_xa_dts_dmp_codec) + LENGTH(i_ram_xa_dts_dmp_codec);
__xa_dts_dmp_dec_lib_text_ram_start = XA_DTS_DMP_CODEC_DST_START;
__xa_dts_dmp_dec_lib_text_ram_end   =  XA_DTS_DMP_CODEC_DST_END;
*/

/*
XA_EC3_CODEC_SRC_START       = ORIGIN(xa_dd_ec3_dec_lib_txt);
XA_EC3_CODEC_SRC_END         = ORIGIN(xa_dd_ec3_dec_lib_txt) + LENGTH(xa_dd_ec3_dec_lib_txt);
*/

/*
XA_EC3_CODEC_DST_START       = ORIGIN(i_ram_xa_ec3_codec);
XA_EC3_CODEC_DST_END         = ORIGIN(i_ram_xa_ec3_codec) + LENGTH(i_ram_xa_ec3_codec);
__xa_dd_ec3_dec_lib_text_ram_start = XA_EC3_CODEC_DST_START;
__xa_dd_ec3_dec_lib_text_ram_end   =  XA_EC3_CODEC_DST_END;
*/

UPGRADE_HW_SET_RODATA_ADDR   = ORIGIN(upgrade_hw_set_rodata);

