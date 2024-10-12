#include "os_config.h"

#ifdef LCD_ST7032
#include "hw_timer.h"
#include "hw_spi.h"
#include "hal_gpio.h"

#include "flash_boot.h"
#include "LCDdevice.h"
#include "LCD_ST7032.h"
#include "app_string_oem.h"

//#define RTOS
//#define LCD_TEST

#define ST7032_FUNC_ENABLE
#define DBG_Printf

#ifdef RTOS
void *mutex_spi;
#define OS_MUTEX_QSPI_LOCK
#define OS_MUTEX_QSPI_UNLOCK
#endif


#define LCD_1062_NEW


#define	LCD_CONTRAST_MAXIMUM    0x00
#define	LCD_CONTRAST_MEDIMUM    0x10
#define	LCD_CONTRAST_MINIMUM    0x1f
#define	LCD_CONTRAST            LCD_CONTRAST_MEDIMUM

#define Lcd_Reset_Pin_High      Lcd_Reset_Write( 1 )
#define Lcd_Reset_Pin_Low       Lcd_Reset_Write( 0 )

#define	Lcd_Cs_Enable		
#define	Lcd_Cs_Disable

#define	Lcd_Rs_Command          Lcd_Rs_Write(0); \
                                delayus(1)

#define	Lcd_Rs_Data             Lcd_Rs_Write(1); \
                                delayus(1)

#define Lcd_Write_Command(x)    Spi_Write_Bits(x); \
                                delayus(10)

#define Lcd_Write_Data(x)       Spi_Write_Bits(x); \
                                delayus(100)


#define CS_ENABLE
#define CS_DISABLE
								

#define SPI_SELECT             spi_transfer_size_set(8, 0)


//the ICON ram is 0x42
#define ICON_REC_MASK           0x08
#define ICON_REP_ALL_MASK       0x04
#define ICON_REP_ONE_MASK       0x02

//ICON ram address is 0x40
#define ICON_STEREO_MASK        0x10
#define ICON_EQ_MASK            0x08
#define ICON_ONE_MASK           0x04
#define ICON_TWO_MASK           0x02
#define ICON_THREE_MASK         0x01

//ICON ram address is 0x44
#define ICON_SLEEP_MASK         0x01

//ICON ram address is 0x48
#define ICON_REPEAT_MASK        0x01

//ICON ram address is 0x4A
#define ICON_RP_ONE_MASK        0x01

//ICON ram address is 0x4B
#define ICON_RP_ALL_MASK        0x10

//ICON ram address is 0x4E
#define ICON_PROG_MASK          0x01

//ICON ram address is 0x4F
#define ICON_TIMER_MASK         0x10


static U8 ram_seg0;


void Lcd_Rs_Write(U16 Logic)
{
	#ifdef ST7032_FUNC_ENABLE
	if ( Logic )
	{
		LCD_RS_PIN_HIGH;
	}
	else
	{
		LCD_RS_PIN_LOW;
	}
	#endif
}

void Lcd_Reset_Write(U16 Logic)
{
	#ifdef ST7032_FUNC_ENABLE

	#endif
}

extern I16 pre_freq;

void Lcd_Write_Byte(U16 Data)
{
	#ifdef ST7032_FUNC_ENABLE

	#endif
}

void ST7032_init(void)
{
	DBG_Printf("%s\n\r", __func__);
	#ifdef ST7032_FUNC_ENABLE
	//RS pin
	LCD_RS_PIN_INIT;
	LCD_RS_PIN_HIGH;

	ram_seg0 = 0;
	#endif
}

void ST7032_open(void)
{
	U8 i;
	U8 j;

	DBG_Printf("%s\n\r", __func__);
	#ifdef ST7032_FUNC_ENABLE

	#ifdef RTOS
	U32 ret;
	ret = OS_MUTEX_QSPI_LOCK;
	DBG_Assert( ret == TRUE );
	#endif

	hw_spi_mode_set(SPI_MODE_0);
	SPI_SELECT;
	Lcd_Cs_Enable;

	//	Function Set (N,S,CG)
	//	N=1, 2 lines; DL=1, 8bit bus; DH=0, 5*8 font; IS=0, Normal instruction
	Lcd_Rs_Command;

	#ifdef LCD_1062_NEW
 	Lcd_Write_Command(0x39); //FUNCTION SET 001,DL,N,DH,IS2,IS1
	delayus(10);
 	Lcd_Write_Command(0x1c);
	delayus(10);
 	Lcd_Write_Command(0x79);
	delayus(10);
 	Lcd_Write_Command(0x5f);
	delayus(10);
 	Lcd_Write_Command(0x6b);
	delayus(10);
 	Lcd_Write_Command(0x01);
	delayus(10);
 	Lcd_Write_Command(0x0c);
	delayus(10);
	#else
	Lcd_Write_Command(0x38);
	delayus(30);

	//Set cursor moving and display shift
	Lcd_Write_Command(0x04);
	delayus(30);

	// IS=1, extension instruction
	Lcd_Write_Command(0x39);
	delayus(30);

	// Bias selection/Internal OSC frequency adjust.
	// BS=1, 1/4 bais; F2~0=(1,0,0), frequence 183HZ 
	Lcd_Write_Command(0x1F);
	delayus(30);

	// Contrast set(low byte)
	// C3-C0(bit3-bit0)
	Lcd_Write_Command(0x7A);
	delayus(30);

	// Power/ICON/Contrast control
	// Ion=1(bit3), ICON display on; Bon=1(bit2), booster circuit turn on; C5(bit1),C4(bit0), Contrast set(high byte).
	Lcd_Write_Command(0x5E);
	delayus(30);

	// Follower control
	// Fon = 1, Rab(2~0)=(0,1,0)
	Lcd_Write_Command(0x6A);
	delayms(200);

	// Display ON/OFF control
	// D=1, Display On; C,B=0,0, Cursor OFF
	Lcd_Write_Command(0x0C);
	delayus(30);

	// Clear Display
	Lcd_Write_Command(0x01);
	delayms(2);

	// Return Home
	Lcd_Write_Command(0x02);
	delayus(30);
	//delayus(1500);

	// Entery Mode set
	// I/D=1, ;S=0, ;
	Lcd_Write_Command(0x06);
	delayus(30);
	#endif



	// IS=0, disable extension instruction
	Lcd_Write_Command(0x38);
	delayus(10);

	// IS=0, disable extension instruction
	Lcd_Rs_Command;
	Lcd_Write_Command(0x38);
	delayus(10);

	//clear DDRAM for part of dd ram of the chip
	Lcd_Rs_Command;
	Lcd_Write_Command(0x80); //hw 1ST Line for the screen of LCD
	delayus(10);
	Lcd_Rs_Data;
	for(i=0; i<0x10; i++)
	{
		Lcd_Write_Data(0x20);
	}
	delayus(10);
	Lcd_Rs_Command;
	Lcd_Write_Command(0xC0); //hw 2nd Line  for the screen of LCD
	delayus(10);
	Lcd_Rs_Data;
	for(i=0; i<0x10; i++)
	{
		Lcd_Write_Data(0x20);
	}

	delayus(10);

	//clear CGRAM
	Lcd_Rs_Command;
	Lcd_Write_Command(0x40);
	delayus(10);
	Lcd_Rs_Data;
	for(i=0; i<0x08;i++)
	{
		for (j=0; j<8; j++)
		{
			Lcd_Write_Data(0);
		}
	}

	delayus(10);

	// IS=1, enable extension instruction
	Lcd_Rs_Command;
	Lcd_Write_Command(0x39);
	delayus(10);

	//clear ICON RAM
	Lcd_Rs_Command;
	Lcd_Write_Command(0x40);
	delayus(10);
	Lcd_Rs_Data;
	for(i=0; i<0x10; i++)
	{
		Lcd_Write_Data(0x00);
	}


	delayus(10);


	// IS=0, disable extension instruction
	Lcd_Rs_Command;
	Lcd_Write_Command(0x38);
	delayus(10);

	//set CGRAM
	Lcd_Rs_Command;
	Lcd_Write_Command(0x40);
	delayus(10);
	Lcd_Rs_Data;

	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x00);

	// play /1
	Lcd_Write_Data(0x10);
	Lcd_Write_Data(0x18);
	Lcd_Write_Data(0x1C);
	Lcd_Write_Data(0x1E);
	Lcd_Write_Data(0x1C);
	Lcd_Write_Data(0x18);
	Lcd_Write_Data(0x10);
	Lcd_Write_Data(0x00);

	// pause /2
	Lcd_Write_Data(0x11);
	Lcd_Write_Data(0x19);
	Lcd_Write_Data(0x1D);
	Lcd_Write_Data(0x1F);
	Lcd_Write_Data(0x1D);
	Lcd_Write_Data(0x19);
	Lcd_Write_Data(0x11);
	Lcd_Write_Data(0x00);

	// Fi. /3
	Lcd_Write_Data(0x1C);
	Lcd_Write_Data(0x10);
	Lcd_Write_Data(0x11);
	Lcd_Write_Data(0x1C);
	Lcd_Write_Data(0x11);
	Lcd_Write_Data(0x11);
	Lcd_Write_Data(0x11);
	Lcd_Write_Data(0x00);

	// le. /4
	Lcd_Write_Data(0x10);
	Lcd_Write_Data(0x10);
	Lcd_Write_Data(0x17);
	Lcd_Write_Data(0x15);
	Lcd_Write_Data(0x17);
	Lcd_Write_Data(0x14);
	Lcd_Write_Data(0x17);
	Lcd_Write_Data(0x00);

	Lcd_Write_Data(0x08);
	Lcd_Write_Data(0x08);
	Lcd_Write_Data(0x08);
	Lcd_Write_Data(0x08);
	Lcd_Write_Data(0x08);
	Lcd_Write_Data(0x08);
	Lcd_Write_Data(0x0A);
	Lcd_Write_Data(0x00);

	//2���� icon  /6
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x04);
	Lcd_Write_Data(0x0E);
	Lcd_Write_Data(0x1F);
	Lcd_Write_Data(0x00);

	//o.  /7
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x0C);
	Lcd_Write_Data(0x12);
	Lcd_Write_Data(0x12);
	Lcd_Write_Data(0x12);
	Lcd_Write_Data(0x0D);
	Lcd_Write_Data(0x00);

	//Battery icon 8
	Lcd_Write_Data(0x04);
	Lcd_Write_Data(0x1F);
	Lcd_Write_Data(0x11);
	Lcd_Write_Data(0x11);
	Lcd_Write_Data(0x13);
	Lcd_Write_Data(0x15);
	Lcd_Write_Data(0x1F);
	Lcd_Write_Data(0x00);

	delayus(10);


	// Display ON/OFF control
	// D=1, Display On; C,B=0,0, Cursor OFF, blink off
	Lcd_Rs_Command;
	Lcd_Write_Command(0x0C);
	delayus(10);

	// IS=1, enable extension instruction
	Lcd_Rs_Command;
	Lcd_Write_Command(0x39);
	delayus(10);

	//Lcd_Cs_Disable;

	#ifdef RTOS
	ret = OS_MUTEX_QSPI_UNLOCK;
	DBG_Assert( ret == TRUE );
	#endif

	#ifdef LCD_TEST
	lcd_test();
	#endif

	#endif
}

void ST7032_close(void)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef ST7032_FUNC_ENABLE

	#endif
}

void ST7032_fini(void)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef ST7032_FUNC_ENABLE

	#endif
}

void ST7032_clear_icon(void)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef ST7032_FUNC_ENABLE
	U16 i;

	Lcd_Cs_Enable;
	Lcd_Rs_Command;
	Lcd_Write_Command(0x40);
	delayus(100);
	Lcd_Rs_Data;
	for( i = 0; i < 0x10; i ++)
	{
		Lcd_Write_Data(0x00);
	}

	ram_seg0 = 0;
	Lcd_Cs_Disable;
	delayus(100);
	#endif
}

void ST7032_clear_line(U16 line)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef ST7032_FUNC_ENABLE
	U16 i , len ,cmd ;
	
	DBG_Assert( line < MAX_LCD_MODULE_LINE );

	if ( line == LINE1 )
	{
		cmd = 0x80;
		len = 0x10;
	}
	else if ( line == LINE2 )
	{
		cmd = 0xC0;
		len = 0x10;
	}
	else 
	{
		cmd = 0x80;
		len = 0x20;
	}

	Lcd_Cs_Enable;
	Lcd_Rs_Command;
	Lcd_Write_Command(cmd);
	delayus(100);
	Lcd_Rs_Data;
	for( i = 0; i < len; i ++)
	{
		Lcd_Write_Data(0x20);
	}

	Lcd_Cs_Disable;
	delayus(100);

	#endif
}

void ST7032_clear (LCD_RANGE lcd_range)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef ST7032_FUNC_ENABLE

	#ifdef RTOS
	int ret;
	ret = OS_MUTEX_QSPI_LOCK;
	DBG_Assert( ret == TRUE );
	#endif


	switch (lcd_range)
	{
	case LINE1:
		ST7032_clear_line (LINE1);
		break;
	case LINE2:
		ST7032_clear_line (LINE2);
		break;
	case LINE12:
	case LINE123:
		ST7032_clear_line (LINE1);
		ST7032_clear_line (LINE2);
		break;
	case ICON:
		ST7032_clear_icon ();
		break;
	case SCREEN:
		ST7032_clear_line (LINE1);
		ST7032_clear_line (LINE2);
		ST7032_clear_icon ();
		break;
	default :
		printf_error("lcd_range setting\n\r");
		break;
	}

	#ifdef RTOS
	ret = OS_MUTEX_QSPI_UNLOCK;
	DBG_Assert( ret == TRUE );
	#endif

	#endif

}

void ST7032_display_char_fixed_len (const U8 *buf, U16 line, U16 spos, U16 epos)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef ST7032_FUNC_ENABLE

	#endif
}



void ST7032_display_char (const U8 *buf, U16 line, U16 pos)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef ST7032_FUNC_ENABLE

	U8 temp;
	U8 i;
	
	#ifdef RTOS
	ret = OS_MUTEX_QSPI_LOCK;
	DBG_Assert( ret == TRUE );
	#endif

	SPI_SELECT;

	DBG_Assert(line < MAX_LCD_MODULE_LINE);
	DBG_Assert(pos < MAX_LCD_MODULE_POS);


	Lcd_Cs_Enable;

	if (line == LINE1)
	{
		temp = 0x80;
	}
	else if (line == LINE2)
	{
		temp = 0xC0;
	}
	else
	{
		printf_error("line setting\n\r");
		temp = 0x80;
	}


	Lcd_Rs_Command;
	Lcd_Write_Command(temp);
	delayus(10);

	Lcd_Rs_Data;
	for (i=0; i<pos; i++)
	{
		Lcd_Write_Data(0x20); //space
	}

	while ( (*buf != 0) && (pos < MAX_LCD_MODULE_POS) )
	{
		Lcd_Write_Data(*buf);
		pos++;
		buf++;
	}

	while (pos < MAX_LCD_MODULE_POS)
	{
		Lcd_Write_Data(0x20); //space
		pos++;

	}

	delayus(10);


	Lcd_Cs_Disable;

	#ifdef RTOS
	ret = OS_MUTEX_QSPI_UNLOCK;
	DBG_Assert( ret == TRUE );
	#endif

	#endif
}


void ST7032_display_icon (LCD_ICON lcd_icon, U32 param)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef ST7032_FUNC_ENABLE

	#endif
}

void ST7032_display_screen(void)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef ST7032_FUNC_ENABLE

	U8 i;

	//display DDRAM
	Lcd_Rs_Command;
	Lcd_Write_Command(0x80);
	delayus(100);
	Lcd_Rs_Data;
	for(i=0; i<0x10; i++)
	{
		Lcd_Write_Data('A');
	}

	delayus(100);

	//display DDRAM
	Lcd_Rs_Command;
	Lcd_Write_Command(0xC0);
	delayus(100);
	Lcd_Rs_Data;
	for(i=0; i<0x10; i++)
	{
		Lcd_Write_Data('A');
	}

	delayus(100);

	//display ICON RAM
	Lcd_Rs_Command;
	Lcd_Write_Command(0x40);
	delayus(100);
	Lcd_Rs_Data;
	for(i=0; i<0x10;i++)
	{
		Lcd_Write_Data(0x1F);
	}

	#endif
}


#ifdef LCD_TEST

void lcd_test(void)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef ST7032_FUNC_ENABLE

	char string_buf0[16] = "                ";
	char string_buf1[16] = "ABCDEFGHIJKLMNOP";
	char string_buf2[16] = "abceefghijklmnop";


	#ifdef LCD_TEST
	U8 i;
	U8 j;

	while (1)
	{
		//clear DDRAM
		Lcd_Rs_Command;
		Lcd_Write_Command(0x80); //set dd ram address
		delayus(10);
		Lcd_Rs_Data;
		#ifdef SEND_CONTINUOUS_DATA
		Spi_Write_Bytes(string_buf0, 16);
		#else
		for(i=0; i<0x10; i++)
		{
			Lcd_Write_Data(0x20);
		}
		#endif

		delayus(10);

		//clear DDRAM
		Lcd_Rs_Command;
		Lcd_Write_Command(0xC0); //set dd ram address
		delayus(10);
		Lcd_Rs_Data;
		#ifdef SEND_CONTINUOUS_DATA
		Spi_Write_Bytes(string_buf0, 16);
		#else
		for(i=0; i<0x10; i++)
		{
			Lcd_Write_Data(0x20);
		}
		#endif //SEND_CONTINUOUS_DATA

		delayus(10);

		//claer CGRAM
		Lcd_Rs_Command;
		Lcd_Write_Command(0x40);
		delayus(10);
		Lcd_Rs_Data;
		for(i=0; i<0x08;i++)
		{
			for (j=0; j<8; j++)
			{
				Lcd_Write_Data(0);
			}
		}

		delayus(10);


		// IS=1, extension instruction
		Lcd_Rs_Command;
		Lcd_Write_Command(0x39);
		delayus(10);

		//clear ICON RAM
		Lcd_Rs_Command;
		Lcd_Write_Command(0x40);
		delayus(10);
		Lcd_Rs_Data;
		for(i=0; i<0x10; i++)
		{
			Lcd_Write_Data(0x00);
		}

		delayus(10);

		// IS=0, disable extension instruction
		Lcd_Rs_Command;
		Lcd_Write_Command(0x38);
		delayus(10);


		//display DDRAM
		Lcd_Rs_Command;
		Lcd_Write_Command(0x80);
		delayus(10);
		Lcd_Rs_Data;
		#ifdef SEND_CONTINUOUS_DATA
		Spi_Write_Bytes(string_buf1, 15);
		#else
		for(i=0; i<0x0F; i++)
		{
			Lcd_Write_Data('A'+i);
		}

		delayus(10);

		//display DDRAM
		Lcd_Rs_Command;
		Lcd_Write_Command(0xC0);
		delayus(10);
		Lcd_Rs_Data;		
		#ifdef SEND_CONTINUOUS_DATA
		Spi_Write_Bytes(string_buf2, 15);
		#else
		for(i=0; i<0x0F; i++)
		{
			Lcd_Write_Data('a'+i);
		}
		#endif

		#endif

		delayus(10);
	}

	#endif

	#endif
}
#endif //LCD_TEST

void ST7032_display_full(void)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef ST7032_FUNC_ENABLE

	#endif	
}

void ST7032_display_source(U8 audio_source)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef ST7032_FUNC_ENABLE
	switch (audio_source)
	{
		case SOURCE_AUX:
			app_lcd_device.display_char(STRING_SOURCE_MODE_AUX, 0, 4);
			app_lcd_device.display_char(STRING_NULL, 1, 0);
			break;

		case SOURCE_ARC:
			app_lcd_device.display_char(STRING_SOURCE_MODE_ARC, 0, 4);
			app_lcd_device.display_char(STRING_NULL, 1, 0);
			break;

		case SOURCE_HDMI:
			app_lcd_device.display_char(STRING_SOURCE_MODE_HDMI, 0, 4);
			app_lcd_device.display_char(STRING_NULL, 1, 0);
			break;

		case SOURCE_COAXIAL:
			app_lcd_device.display_char(STRING_SOURCE_MODE_COAX, 0, 2);
			app_lcd_device.display_char(STRING_NULL, 1, 0);
			break;

		case SOURCE_OPTICAL:
			app_lcd_device.display_char(STRING_SOURCE_MODE_OPTICAL, 0, 2);
			app_lcd_device.display_char(STRING_NULL, 1, 0);
			break;			

		case SOURCE_BT:
			app_lcd_device.display_char(STRING_SOURCE_MODE_BT, 0, 1);
			app_lcd_device.display_char(STRING_NULL, 1, 0);
			break;

		case SOURCE_USB_MSC:
			app_lcd_device.display_char(STRING_SOURCE_MODE_USB_MSC, 0, 2);
			app_lcd_device.display_char(STRING_NULL, 1, 0);
			break;

		case SOURCE_USB_SPEAKER:
			app_lcd_device.display_char(STRING_SOURCE_MODE_USB_SPEAKER, 0, 0);
			app_lcd_device.display_char(STRING_NULL, 1, 0);
			break;

		case SOURCE_INTERNAL:
			app_lcd_device.display_char(STRING_SOURCE_MODE_INTERNAL, 0, 1);
			app_lcd_device.display_char(STRING_NULL, 1, 0);
			break;

		default:
			app_lcd_device.display_char(STRING_NULL, 0, 0);
			app_lcd_device.display_char(STRING_NULL, 1, 0);
			break;
	}
	#endif
}

void ST7032_display_music_play(U32 music_level)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef ST7032_FUNC_ENABLE

	#endif
}

void ST7032_display_dim_set(U8 val)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef ST7032_FUNC_ENABLE

	#endif	
}

void ST7032_display_clear_icon(void)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef ST7032_FUNC_ENABLE

	#endif
}

void ST7032_display_icon_partial(void)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef ST7032_FUNC_ENABLE

	#endif
}

void ST7032_display_led_light_set(U8 num, U8 con)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef ST7032_FUNC_ENABLE

	#endif
}

void ST7032_display_level(U16 Level_L, U16 Level_R,U8 index)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef ST7032_FUNC_ENABLE

	#endif
}

#endif //LCD_ST7032
