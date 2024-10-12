#ifndef __AUDIO_MA12070P__
#define __AUDIO_MA12070P__


//GPIO setting for MA12070P chip pins
#define ENABLE_PIN          GPIO0_25_INDEX
//init gpio pin to output mode
#define ENABLE_PIN_INIT     hal_gpio_mux_func_select(ENABLE_PIN,MUX_SEL_GPIO_OUTPUT)
//set gpio pin to high level
#define ENABLE_PIN_HIGH     hal_gpio_output_level_set(ENABLE_PIN,GPIO_HIGH)
//set gpio pin to low level
#define ENABLE_PIN_LOW      hal_gpio_output_level_set(ENABLE_PIN,GPIO_LOW)

#define MUTE_PIN            GPIO0_24_INDEX
//init gpio pin to output mode
#define MUTE_PIN_INIT       hal_gpio_mux_func_select(MUTE_PIN,MUX_SEL_GPIO_OUTPUT)
//set gpio pin to high level
#define MUTE_PIN_HIGH       hal_gpio_output_level_set(MUTE_PIN,GPIO_HIGH)
//set gpio pin to low level
#define MUTE_PIN_LOW        hal_gpio_output_level_set(MUTE_PIN,GPIO_LOW)


#define POWER_PIN           GPIO_UNDEFINE_INDEX
//init gpio pin to output mode
#define POWER_PIN_INIT      hal_gpio_mux_func_select(POWER_PIN,MUX_SEL_GPIO_OUTPUT)
//set gpio pin to high level
#define POWER_PIN_HIGH      hal_gpio_output_level_set(POWER_PIN,GPIO_HIGH)
//set gpio pin to low level
#define POWER_PIN_LOW       hal_gpio_output_level_set(POWER_PIN,GPIO_LOW)

#define ERROR_PIN           GPIO0_20_INDEX
//init gpio pin to input mode
#define ERROR_PIN_INIT      hal_gpio_mux_func_select(ERROR_PIN,MUX_SEL_GPIO_INPUT)

#define ERROR_PIN_STATUS    hal_gpio_input_level_read(ERROR_PIN);


#define DEV_PHY_ADDR     0x40 
#define DEV_PHY_ADDR_R   0x42  
//reg config
//#define USED_REG_SET_ENABLE

#ifdef USED_REG_SET_ENABLE

/****************************************************
 * Read / Write Access (Power Mode Settings)
 ****************************************************/

//Power_Mode_Control
//regester: 0x00
//default:  0x3D
/****************************************************
reserved_0_3bit: bit 3:0
    Reserved
*   
PM_man锛歜it 5:4
    Manual selected power mode. These two bits 
    can be used selecting the Power Mode of the 
    device when it is in manual Power Mode 
    control.

    00
    Reserved

    01
    Power Mode 1

    10
    Power Mode 2

    11
    Power Mode 3
* 
manualPM: bit 6
    Select manual Power Mode control. 
    Default the device will operate in automatic Power Mode control. 
    This bit can be set to 1 if manual Power Mode control is required.

    0
    automatic Power Mode control

    1
    manual Power Mode control

* 
reserved_7bit: bit 7
    Reserved
* 
*****************************************************/
enum {
    POWER_MODE_0 = 0,
    POWER_MODE_1 = 1,
    POWER_MODE_2 = 2,
    POWER_MODE_3 = 3
};

typedef volatile struct{
    U8 reserved_0_3bit      : 4;
    U8 pm_man               : 2;//00 Reserved; 01 Power Mode 1; 10 Power Mode 2; 11 Power Mode 3
    U8 manual_pm            : 1;
    U8 reserved_7bit        : 1;

}CHIP_MA12070P_POWER_MODE_CONTROL;


//Threshold for Power Mode change PM1=>PM2
//regester: 0x01
//default锛� 0x3C
/****************************************************
Mthr_1to2: bit 7:0
    Threshold value for PM1=>PM2 change. 
    This value will set the threshold for when automatic Power Mode changes from PM1 to PM2. 
    It can be programmed from 0 - 255; 
    this maps to 0 output power 鈥� max output power.
*****************************************************/
typedef volatile struct{
    U8 mthr_1to2      : 7;

}CHIP_MA12070P_POWER_MODE_PM1_to_PM2;


//Threshold for Power Mode change PM2=>PM1
//regester: 0x02
//default:  0x32
/****************************************************
Mthr_2to1: bit 7:0
    Threshold value for PM2=>PM1 change. 
    This value will set the threshold for when automatic Power Mode changes from PM2 to PM1. 
    It can be programmed from 0 - 255; 
    this maps to 0 output power 鈥� max output power.
*****************************************************/
typedef volatile struct{
    U8 mthr_2to1      : 7;

}CHIP_MA12070P_POWER_MODE_PM2_to_PM1;


//Threshold for Power Mode change PM2=>PM3
//regester: 0x03
//default:  0x5A
/****************************************************
Mthr_2to3: bit 7:0
    Threshold value for PM2=>PM3 change. 
    This value will set the threshold for when automatic Power Mode changes from PM2 to PM3. 
    It can be programmed from 0 - 255; 
    this maps to 0 output power 鈥� max output power.
*****************************************************/
typedef volatile struct{
    U8 mthr_2to3      : 7;

}CHIP_MA12070P_POWER_MODE_PM2_to_PM3;

//Threshold for Power Mode change PM3=>PM2
//regester: 0x04
//default:  0x50
/****************************************************
Mthr_3to2: bit 7:0
    Threshold value for PM3=>PM2 change. 
    This value will set the threshold for when automatic Power Mode changes from PM3 to PM2. 
    It can be programmed from 0 - 255; 
    this maps to 0 output power 鈥� max output power.
*****************************************************/
typedef volatile struct{
    U8 mthr_3to2      : 7;

}CHIP_MA12070P_POWER_MODE_PM3_to_PM2;

//Soft-clipping and over-current protection latching
//regester: 0x0A
//default:  0x0C
/****************************************************
*
reserved_0bit: bit 0
    Reserved
*
ocp_latch_en: bit 1
    High to use permanently latching OCP.
*
reserved_2_6bit: bit 6:2
    Reserved
*
lf_clamp_en: bit 7
    Enables soft-clipping. 
    High to enable. 
    Low to disable.
*
*****************************************************/
typedef volatile struct{
    U8 reserved_0bit        : 1;
    U8 ocp_latch_en         : 1;
    U8 reserved_2_6bit      : 5;
    U8 lf_clamp_en          : 1;

}CHIP_MA12070P_SOFT_CLIPING_AND_OVER_CURRENT_PROTECTION_LATCHING;





/****************************************************
 * Read / Write Access (Power Mode Profile Settings)
 ****************************************************/

//Custom Power Mode Profile setting
//regester: 0x1D
//default:  0x00
/****************************************************
*
PMprofile: bit 2:0
    Power Mode Profile select. 
    With this register the user can selects the appropriate Power Mode Profile.

    000
    Power Mode Profile 0

    001
    Power Mode Profile 1

    010
    Power Mode Profile 2

    011
    Power Mode Profile 3

    100
    Power Mode Profile 4
*
reserved_3_7bit: bit 7:3
    Reserved
*
*****************************************************/
enum {
    POWER_MODE_PROFILE_0    = 0,
    POWER_MODE_PROFILE_1    = 1,
    POWER_MODE_PROFILE_2    = 2,
    POWER_MODE_PROFILE_3    = 3,
    POWER_MODE_PROFILE_4    = 4,
};

typedef volatile struct{

    U8 pmprofile            : 3;
    U8 reserved_3_7bit      : 5;

}CHIP_MA12070P_CUSTOM_POWER_MODE_PROFILE_SETTING;



//Power Mode Profile configuration
//regester: 0x1E
//default:  0x2F
/****************************************************
*
PM1_man: bit 1:0
    Custom profile PM1 content

    00
    Assign scheme A to PM1

    01
    Assign scheme B to PM1

    10
    Assign scheme C to PM1

    11
    Assign scheme D to PM1
*
PM1_man: bit 3:2
    Custom profile PM2 content

    00
    Assign scheme A to PM2

    01
    Assign scheme B to PM2

    10
    Assign scheme C to PM2

    11
    Assign scheme D to PM2
*
PM3_man: bit 5:4
    Custom profile PM1 content

    00
    Assign scheme A to PM3

    01
    Assign scheme B to PM3

    10
    Assign scheme C to PM3

    11
    Assign scheme D to PM3
*
reserved_6_7bit: bit 7:6
    Reserved
*
*****************************************************/
typedef volatile struct{
    U8 pm1_man              : 2;
    U8 pm2_man              : 2;
    U8 pm3_man              : 2;
    U8 reserved_6_7bit      : 2;

}CHIP_MA12070P_POWER_MODE_PROFILE_CONFIGURATION;

//Over-current protection latch clear
//regester: 0x20
//default:  0x1F
/****************************************************
*
reserved_0_6bit: bit 6:0
    Reserved
*
ocp_latch_clear
    Clears over current protection latch. 
    A low to high toggle clears the current OCP latched 
    condition.
*
*****************************************************/
typedef volatile struct{
    U8 reserved_0_6bit      : 7;
    U8 ocp_latch_clear      : 1;

}CHIP_MA12070P_OVER_CURRENT_PROTECTION_LATCH_CLEAR;


//Audio in mode
//regester: 0x25
//default:  0x10
/****************************************************
*
reserved_0_4bit: bit 4:0
    Reserved
*
audio_in_mode : bit 6:5
    Audio input mode. Sets the input mode of the amplifier. 
    This means the amplifier overall gain setting.

    00
    Audio in mode 0: 20dB gain

    01
    Audio in mode 1: 26dB gain

*
*****************************************************/
typedef volatile struct{
    U8 reserved_0_4bit      : 5;
    U8 audio_in_mode        : 2;
    U8 reserved_7bit        : 1;

}CHIP_MA12070P_AUDIO_IN_MODE;

//DC protection
//regester: 0x26
//default:  0x05
/****************************************************
*
reserved_0_1bit: bit 1:0
    Reserved
*
Eh_dcShdn : bit 2
    Enables or disables DC protection. 
    High to enable. 
    Low to disable.
*
reserved_3_7bit: bit 7:3
    Reserved
*
*****************************************************/
typedef volatile struct{
    U8 reserved_0_1bit      : 2;
    U8 eh_dc_shdn           : 1;
    U8 reserved_3_7bit      : 5;

}CHIP_MA12070P_DC_PROTECTION;


//Audio in mode overwrite
//regester: 0x27
//default:  0x08
/****************************************************
*
reserved_0_4bit: bit 4:0
    Reserved
*
audio_in_mode_ext : bit 5
    Enables audio in mode default overwrite. 
    High to enable. 
    Low to disable. 
    Should enabled for address 0x25 to have effect.
*
reserved_6_7bit: bit 7:6
    Reserved
*
*****************************************************/
typedef volatile struct{
    U8 reserved_0_4bit          : 5;
    U8 audio_in_mode_ext        : 1;
    U8 reserved_6_7bit          : 2;

}CHIP_MA12070P_AUDIO_IN_MODE_OVERWRITE;

//Error handler clear
//regester: 0x2D
//default:  0x30
/****************************************************
*
reserved_0_1bit: bit 1:0
    Reserved
*
eh_clear : bit 2
    Clears error handler. 
    A low-to-high-to-low 
    toggle clears the error handler.
*
reserved_3_7bit: bit 7:3
    Reserved
*
*****************************************************/
typedef volatile struct{
    U8 reserved_0_1bit          : 2;
    U8 eh_clear                 : 1;
    U8 reserved_3_7bit          : 5;

}CHIP_MA12070P_ERROR_HANDLER_CLEAR;




/****************************************************
 * Read / Write Access (I2S format configuration)
 ****************************************************/

//PCM word format 锛圛2S format configuration + Volume control and limiter锛�
//regester: 0x35
//default:  0x01
/****************************************************
*
i2s_format : bit 2:0
    000
    i2s standard

    001
    Left justified

    100
    Right justified 16bits

    110
    Right justified 18bits

    000 锛燂�?
    Right justified 20bits

    111
    Right justified 24bits
*
Processor bypass mux
audio_proc_enable: bit 3
    0
    Bypass the audio processor

    1
    Use the audio processor
*
Limiter release time
audio_proc_attack: bit 5:4
    00
    Slow release time

    01
    Normal release time

    10
    Fast release time
*
Limiter attack time control
audio_proc_release锛� bit 7:6
    00
    Slow attack time

    01
    Normal attack time

    10
    Fast attack time

*
*****************************************************/
enum {
    I2S_STANDARD                = 0,
    LEFT_JUSTIFIED              = 1,
    RIGHT_JUSTIFIED_16BIT       = 4,
    RIGHT_JUSTIFIED_18BIT       = 6,
    RIGHT_JUSTIFIED_20BIT       = 0,
    RIGHT_JUSTIFIED_24BIT       = 7
};

enum {
    SLOW_TIME   = 0,
    NORMAL_TIME = 1,
    FAST_TIME   = 2
};

typedef volatile struct{
    U8 i2s_format               : 3;
    U8 audio_proc_enable        : 1;
    U8 audio_proc_attack        : 2;
    U8 audio_proc_release       : 2;

}CHIP_MA12070P_I2S_FORMAT_CONFIG_AND_VOLUME_CONTROL_LIMITER_0X35;

//(I2S format configuration + Volume control and limiter锛�
//regester: 0x36
//default:  0x01
/****************************************************
*
Clocking edge of the serial clock signal (SCK) 
i2s_sck_pol: bit 0
    0
    Serial data (SDX) and word select (WS) are changing at rising edge of the serial clock signal (SCK). 
    The MA12070P will capture data at the falling edge of the serial clock signal SCK

    1
    Serial data (SDX) and word select (WS) are changing at falling edge of the serial clock signal (SCK). 
    The MA12070P will capture data at the rising edge of the serial clock signal SCK

*
Pairing of data words
i2s_ws_pol: bit 1
    0
    First word of a simultaneously sampled PCM data pair is transmitted while word select (WS) is low

    1
    First word of a simultaneously sampled PCM data pair is transmitted while word select (WS) is high

*
Bit order of PCM data words
i2s_order: bit 2
    0
    Most significant bit of the PCM data word is transmitted first

    1
    Least significant bit of the PCM data word is transmitted first

*
Number of data bits per frame
i2s_framesize: bit 4:3
    00
    64 serial clock (SCK) cycles are present in each period of the word select signal (WS)

    01
    48 serial clock (SCK) cycles are present in each period of the word select signal (WS)

    10
    32 serial clock (SCK) cycles are present in each period of the word select signal (WS)

*
Left/right order of PCM words
i2s_rightfirst锛� bit 5
    0
    Left PCM data word (of a simultaneously sampled PCM data pair) is send first

    1
    Right PCM data word (of a simultaneously sampled PCM data pair) is send first

*
Limiter bypass mux
audio_proc_limiterEnable: bit 6
    0
    Bypass the limiter

    1
    Use the limiter

*
Mute mux control
audio_proc_mute:bit 7
    0
    Play audio

    1
    Mute audio

*
*****************************************************/
typedef volatile struct{
    U8 i2s_sck_pol                      : 1;
    U8 i2s_ws_pol                       : 1;
    U8 i2s_order                        : 1;
    U8 i2s_framesize                    : 2;
    U8 i2s_rightfirst                   : 1;
    U8 audio_proc_limiterEnable         : 1;
    U8 audio_proc_mute                  : 1;

}CHIP_MA12070P_I2S_FORMAT_CONFIG_AND_VOLUME_CONTROL_LIMITER_0X36;


/****************************************************
 * Read / Write Access (Volume control and limiter)
 ****************************************************/

//Master integer dB volume
//regester: 0x40
//default:  0x18
/****************************************************
*
vol_db_master: bit 7:0
    Control of integer value master dB volume (see Table 8-9 for mapping overview)

*
*****************************************************/
typedef volatile struct{
    U8 vol_db_master          : 8;

}CHIP_MA12070P_MASTER_INTEGER_DB_VOLUME;


//Master fract dB volume
//regester: 0x41
//default:  0x00
/****************************************************
*
vol_db_master: bit 1:0
    Control of fractional value dB volume (see Table 8-9 for mapping overview)

*
reserved_2_7bit: bit 7:2
    Reserved
*
*****************************************************/
typedef volatile struct{
    U8 vol_lsb_master           : 2;
    U8 reserved_2_7bit          : 6;

}CHIP_MA12070P_MASTER_FRACT_DB_VOLUME;


//Ch0L integer dB volume
//regester: 0x42
//default:  0x18
/****************************************************
*
vol_db_ch0: bit 7:0
    Control of integer value ch0L dB volume (see Table 8-9 for mapping overview)

*
*****************************************************/
typedef volatile struct{
    U8 vol_db_ch0           : 8;

}CHIP_MA12070P_CH0L_INTEGER_DB_VOLUME;

//Ch0R integer dB volume
//regester: 0x43
//default:  0x18
/****************************************************
*
vol_db_ch1: bit 7:0
    Control of integer value ch0R dB volume (see Table 8-9 for mapping overview)

*
*****************************************************/
typedef volatile struct{
    U8 vol_db_ch1           : 8;

}CHIP_MA12070P_CH0R_INTEGER_DB_VOLUME;

//Ch1L integer dB volume
//regester: 0x44
//default:  0x18
/****************************************************
*
vol_db_ch2: bit 7:0
    Control of integer value ch1L dB volume (see Table 8-9 for mapping overview)

*
*****************************************************/
typedef volatile struct{
    U8 vol_db_ch2           : 8;

}CHIP_MA12070P_CH1L_INTEGER_DB_VOLUME;

//Ch1R integer dB volume
//regester: 0x45
//default:  0x18
/****************************************************
*
vol_db_ch3: bit 7:0
    Control of integer value ch1R dB volume (see Table 8-9 for mapping overview)

*
*****************************************************/
typedef volatile struct{
    U8 vol_db_ch3           : 8;

}CHIP_MA12070P_CH1R_INTEGER_DB_VOLUME;


//(Volume control and limiter锛�
//regester: 0x46
//default:  0x00
/****************************************************
*
Ch0L fract dB volume
vol_lsb_ch0锛� bit 1:0
    Control of fractional value ch0L dB volume (see Table 8-9 for mapping overview)

*
Ch0R fract dB volume
vol_lsb_ch1: bit 3:2
    Control of fractional value ch0R dB volume (see Table 8-9 for mapping overview)

*
Ch1L fract dB volume
vol_lsb_ch2锛� bit 5:4
    Control of fractional value ch1L dB volume (see Table 8-9 for mapping overview)

*
Ch1R fract dB volume
vol_lsb_ch3: bit 7:6
    Control of fractional value ch1R dB volume (see Table 8-9 for mapping overview)

*
*****************************************************/
typedef volatile struct{
    U8 vol_lsb_ch0                      : 2;
    U8 vol_lsb_ch1                      : 2;
    U8 vol_lsb_ch2                      : 2;
    U8 vol_lsb_ch3                      : 2;

}CHIP_MA12070P_VOLUME_CONTROL_LIMITER_0X46;


//Ch0L integer dBFS limiter
//regester: 0x47
//default:  0x18
/****************************************************
*
thr_db_ch0: bit 7:0
    Control of integer value ch0L dBFS limiter threshold (see section 鈥淟imiter鈥�)

*
*****************************************************/
typedef volatile struct{
    U8 thr_db_ch0           : 8;

}CHIP_MA12070P_CH0L_INTEGER_DBFS_LIMITER;


//Ch0R integer dBFS limiter
//regester: 0x48
//default:  0x18
/****************************************************
*
thr_db_ch1: bit 7:0
    Control of integer value ch0R dBFS limiter threshold (see section 鈥淟imiter鈥�)

*
*****************************************************/
typedef volatile struct{
    U8 thr_db_ch1           : 8;

}CHIP_MA12070P_CH0R_INTEGER_DBFS_LIMITER;


//Ch1L integer dBFS limiter
//regester: 0x49
//default:  0x18
/****************************************************
*
thr_db_ch2: bit 7:0
    Control of integer value ch1L dBFS limiter threshold (see section 鈥淟imiter鈥�)

*
*****************************************************/
typedef volatile struct{
    U8 thr_db_ch2           : 8;

}CHIP_MA12070P_CH1L_INTEGER_DBFS_LIMITER;


//Ch1R integer dBFS limiter
//regester: 0x4A
//default:  0x18
/****************************************************
*
thr_db_ch3: bit 7:0
    Control of integer value ch0R dBFS limiter threshold (see section 鈥淟imiter鈥�)

*
*****************************************************/
typedef volatile struct{
    U8 thr_db_ch3           : 8;

}CHIP_MA12070P_CH1R_INTEGER_DBFS_LIMITER;


//(Volume control and limiter锛�
//regester: 0x4B
//default:  0x00
/****************************************************
*
Ch0L fract dBFS limiter
thr_lsb_ch0锛� bit 1:0
    Control of fractional value ch0L dBFS limiter threshold (see section 鈥淟imiter鈥�)

*
Ch0R fract dBFS limiter
thr_lsb_ch1: bit 3:2
    Control of fractional value ch0R dBFS limiter threshold (see section 鈥淟imiter鈥�)

*
Ch1L fract dBFS limiter
thr_lsb_ch2锛� bit 5:4
    Control of fractional value ch1L dBFS limiter threshold (see section 鈥淟imiter鈥�)

*
Ch1R fract dBFS limiter
thr_lsb_ch3: bit 7:6
    Control of fractional value ch1R dBFS limiter threshold (see section 鈥淟imiter鈥�)

*
*****************************************************/
typedef volatile struct{
    U8 thr_lsb_ch0                      : 2;
    U8 thr_lsb_ch1                      : 2;
    U8 thr_lsb_ch2                      : 2;
    U8 thr_lsb_ch3                      : 2;

}CHIP_MA12070P_VOLUME_CONTROL_LIMITER_0X4B;




/****************************************************
 * Read Only Access (Volume control and limiter monitor)
 ****************************************************/

//(Volume control and limiter monitor)
//regester: 0x7E
//default:  0x00
/****************************************************
*
Indicates if clipping occurs on the VLP output signals
audio_proc_clip_mon: bit 3:0
    bit 0 (1)
    clipping is present on channel 0L

    bit 1 (1)
    clipping is present on channel 0R

    bit 2 (1)
    clipping is present on channel 1L

    bit 3 (1)
    clipping is present on channel 1R

*
Indicates if limiters are active
audio_proc_limiter_mon: bit 7:4
    bit 4 (1)
    limiter is active on channel 0L

    bit 5 (1)
    limiter is active on channel 0R

    bit 6 (1)
    limiter is active on channel 1L

    bit 7 (1)
    limiter is active on channel 1R

*
****************************************************/
typedef volatile struct{
    U8 audio_proc_clip_mon          : 4;
    U8 audio_proc_limiter_mon       : 4;

}CHIP_MA12070P_VOLUME_CONTROL_AND_LIMITER_MONITOR_0X7E;



/****************************************************
 * Read Only Access (Monitor Channel 0 and Channel 1)
 ****************************************************/

//Monitor register channel 0 (Frequency and Power Mode)
//regester: 0x60
//default:  0x00
/****************************************************
*
dcu_mon0.PM_mon: bit 1:0
    Power mode monitor channel 0. 
    Monitor to read out in which Power Mode channel 0 of the device is currently operating in.

*
reserved_2_3bit: bit 3:2
    Reserved

*
dcu_mon0.freqMode: bit 6:4
    Frequency mode monitor channel 0. 
    Register to read out in which frequency mode channel 0 of the device is currently operating in.

*
reserved_7bit: bit 7
    Reserved

*
****************************************************/

typedef volatile struct{
    U8 dcu_mon0_PM_mon          : 2;
    u8 reserved_2_3bit          : 2;
    U8 dcu_mon0_freqMode        : 3;
    u8 reserved_7bit            : 1;

}CHIP_MA12070P_MONITOR_REGISTER_CH0_FREQ_AND_PM;


//Monitor register channel 0
//regester: 0x61
//default:  0x00
/****************************************************
*
OCP Monitor channel 0: bit 0
    Channel 0 over current protection monitor. 
    Monitor to read out if an over current protection event has occurred.

*
dcu_mon0.Vcfly1_ok: bit 1
    Channel 0 Cfly1 protection monitor. 
    Monitor to read out if Cfly1 for channel 0 is ok.

*
dcu_mon0.Vcfly2_ok: bit 2
    Channel 0 Cfly2 protection monitor. 
    Monitor to read out if Cfly2 for channel 0 is ok.

*
dcu_mon0.pvdd_ok: bit 3
    Channel 0 PVDD monitor. 
    Monitor to read out if PVDD for channel 0 is ok.

*
dcu_mon0.vdd_ok: bit 4
    Channel 0 VDD monitor. 
    Monitor to read out if VDD for channel 0 is ok.

*
dcu_mon0.mute: bit 5
    Channel 0 mute monitor. 
    Monitor to read out if channel 0 is in mute or in unmute.

*
reserved_6_7bit: bit 7:6
    Reserved

*
****************************************************/

typedef volatile struct{
    U8 ocp_monitor_ch0          : 1;
    u8 dcu_mon0_Vcfly1_ok       : 1;
    U8 dcu_mon0_Vcfly2_ok       : 1;
    U8 dcu_mon0_pvdd_ok         : 1;
    u8 dcu_mon0_vdd_ok          : 1;
    U8 dcu_mon0_mute            : 1;
    u8 reserved_6_7bit          : 2;

}CHIP_MA12070P_MONITOR_REGISTER_CH0_CTL;


//Monitor register channel 0 (Modulation Index)
//regester: 0x62
//default:  0x00
/****************************************************
*
dcu_mon0.M_mon: bit 7:0
    Channel 0 modulation index monitor. 
    Monitor to read out live modulation index. 
    Modulation index from 0 to 1 maps on the 8-bits register from 0 to 255.

*
****************************************************/
typedef volatile struct{
    U8 dcu_mon0_M_mon          : 8;

}CHIP_MA12070P_MONITOR_REGISTER_CH0_MODULATION_INDEX;





//Monitor register channel 1 (Frequency and Power Mode)
//regester: 0x64
//default:  0x00
/****************************************************
*
dcu_mon1.PM_mon: bit 1:0
    Power mode monitor channel 1. 
    Monitor to read out in which Power Mode channel 1 of the device is currently operating in.

*
reserved_2_3bit: bit 3:2
    Reserved

*
dcu_mon1.freqMode: bit 6:4
    Frequency mode monitor channel 1. 
    Register to read out in which frequency mode channel 1 of the device is currently operating in.

*
reserved_7bit: bit 7
    Reserved

*
****************************************************/

typedef volatile struct{
    U8 dcu_mon1_PM_mon          : 2;
    u8 reserved_2_3bit          : 2;
    U8 dcu_mon1_freqMode        : 3;
    u8 reserved_7bit            : 1;

}CHIP_MA12070P_MONITOR_REGISTER_CH1_FREQ_AND_PM;


//Monitor register channel 1
//regester: 0x65
//default:  0x00
/****************************************************
*
OCP Monitor channel 1: bit 0
    Channel 1 over current protection monitor. 
    Monitor to read out if an over current protection event has occurred.

*
dcu_mon1.Vcfly1_ok: bit 1
    Channel 1 Cfly1 protection monitor. 
    Monitor to read out if Cfly1 for channel 1 is ok.

*
dcu_mon1.Vcfly2_ok: bit 2
    Channel 1 Cfly2 protection monitor. 
    Monitor to read out if Cfly2 for channel 1 is ok.

*
dcu_mon1.pvdd_ok: bit 3
    Channel 1 PVDD monitor. 
    Monitor to read out if PVDD for channel 1 is ok.

*
dcu_mon1.vdd_ok: bit 4
    Channel 1 VDD monitor. 
    Monitor to read out if VDD for channel 1 is ok.

*
dcu_mon1.mute: bit 5
    Channel 1 mute monitor. 
    Monitor to read out if channel 1 is in mute or in unmute.

*
reserved_6_7bit: bit 7:6
    Reserved

*
****************************************************/

typedef volatile struct{
    U8 ocp_monitor_ch1          : 1;
    u8 dcu_mon1_Vcfly1_ok       : 1;
    U8 dcu_mon1_Vcfly2_ok       : 1;
    U8 dcu_mon1_pvdd_ok         : 1;
    u8 dcu_mon1_vdd_ok          : 1;
    U8 dcu_mon1_mute            : 1;
    u8 reserved_6_7bit          : 2;

}CHIP_MA12070P_MONITOR_REGISTER_CH1_CTL;


//Monitor register channel 1 (Modulation Index)
//regester: 0x66
//default:  0x00
/****************************************************
*
dcu_mon1.M_mon: bit 7:0
    Channel 1 modulation index monitor. 
    Monitor to read out live modulation index. 
    Modulation index from 0 to 1 maps on the 8-bits register from 0 to 255.

*
****************************************************/
typedef volatile struct{
    U8 dcu_mon1_M_mon          : 8;

}CHIP_MA12070P_MONITOR_REGISTER_CH1_MODULATION_INDEX;




/****************************************************
 * Read Only Access (Error Register Monitoring)
 ****************************************************/

//Error accumulated register
//regester: 0x6D
//default:  0x00
/****************************************************
*
error_acc: bit 7:0
    Error monitor register. 
    Gives the accumulated status of every potential error source. 
    This register should be cleared by using the error handler clear register.
    
    All bits will be 0 in default/normal operation and 1 when triggered

    bit 0: 
    flying capacitor over-voltage error
    
    bit 1: 
    over-current protection

    bit 2: 
    pll error

    bit 3: 
    PVDD under-voltage protection

    bit 4: 
    over-temperature warning

    bit 5: 
    over-temperature error

    bit 6: 
    pin-to-pin low impedance protection

    bit 7: 
    DC protection

*
****************************************************/
typedef volatile struct{
    U8 error_acc          : 8;

}CHIP_MA12070P_ERROR_ACCUMULATED_REGISTER;


//Monitor MSEL register
//regester: 0x75
//default:  0x00
/****************************************************
*
msel_mon: bit 2:0
    MSEL[2:0] monitor register. 
    Monitor to read out which output configuration the device is in:BTL, SE, BTL/SE or PBTL

*
reserved_3_7bit: bit 7:3
    Reserved
*
****************************************************/
typedef volatile struct{
    U8 msel_mon             : 3;
    U8 reserved_3_7bit      : 5;

}CHIP_MA12070P_MONITOR_MSEL_REGISTER;


//Error register
//regester: 0x7C
//default:  0x00
/****************************************************
*
error: bit 7:0
    Error monitor register. 
    Gives the live status of every potential error source.
    
    All bits will be 0 in default/normal operation and 1 when triggered

    bit 0: 
    flying capacitor over-voltage error
    
    bit 1: 
    over-current protection

    bit 2: 
    pll error

    bit 3: 
    PVDD under-voltage protection

    bit 4: 
    over-temperature warning

    bit 5: 
    over-temperature error

    bit 6: 
    pin-to-pin low impedance protection

    bit 7: 
    DC protection

*
****************************************************/
typedef volatile struct{
    U8 error          : 8;

}CHIP_MA12070P_ERROR_REGISTER;
#endif


/*************************************************************************
* ********************************************************************** *
*          VLP Mapping from register values to gain and level            *
* ********************************************************************** *
*                                                                        *
* Integer dB              Fractional dB                                  *
* register setting        register setting            VLP Gain/Level dB  *
* dec Hex                     dec hex                                    *
* 0 (0x00)                    0 (0x0)                     24.00          *
* 0 (0x00)                    1 (0x1)                     23.75          *
* 鈥� 鈥�                         鈥� 鈥�                         鈥�              *
* 22 (0x16)                   3 (0x3)                     1.25           *
* 23 (0x17)                   0 (0x0)                     1.00           *
* 23 (0x17)                   1 (0x1)                     0.75           *
* 23 (0x17)                   2 (0x2)                     0.50           *
* 23 (0x17)                   3 (0x3)                     0.25           *
* 24 (0x18)                   0 (0x0)                     0.00           *
* 24 (0x18)                   1 (0x1)                     -0.25          *
* 24 (0x18)                   2 (0x2)                     -0.50          *
* 24 (0x18)                   3 (0x3)                     -0.75          *
* 25 (0x19)                   0 (0x0)                     -1.00          *
* 鈥� 鈥�                         鈥� 鈥�                         鈥�              *
* 167 (0xA7)                  3 (0x3)                     -143.75        *
* 168 (0xA8)                  0 (0x0)                     -144.00        *
* 鈥� 鈥�                         鈥� 鈥�                         鈥�              *
* 255 (0xFF)                  2 (0x2)                     -144.00        *
* 255 (0xFF)                  3 (0x3)                     -144.00        *
* ********************************************************************** *
*************************************************************************/


/*
* function: void Audio_MA12070P_VLP_gain_input_Integer_Fractional_reg_value_output(float vlp_gain, U8 *p_integer_db, U8 *p_fractional_db)
* input: float vlp_gain, U8 *p_integer_db, U8 *p_fractional_db
  vlp_gain: 24.00dB ~ -144.00dB
  *p_integer_db: 0~255
  *p_fractional_db: 0/1/2/3
* output: U8 *p_integer_db, U8 *p_fractional_db
* 
* */
void Audio_MA12070P_VLP_gain_input_Integer_Fractional_reg_value_output(float vlp_gain, U8 *p_integer_db, U8 *p_fractional_db);
void Audio_MA12070P_VLP_gain_input_Integer_Fractional_reg_value_output_R(float vlp_gain, U8 *p_integer_db, U8 *p_fractional_db);


void Audio_Dev_MA12070P_Init(void);
void Audio_Dev_MA12070P_Open(void);
void Audio_Dev_MA12070P_Set_Channel(U8 channel);
void Audio_Dev_MA12070P_Set_Volume(U8 volume);
void Audio_Dev_MA12070P_Set_Speaker(U8 speaker, U8 attenuation);
void Audio_Dev_MA12070P_Set_Bass_Treble(U8 bass, U8 treble);
void Audio_Dev_MA12070P_Set_eq_mode(U8 mode);
void Audio_Dev_MA12070P_Set_Mute(U8 con);
void Audio_Dev_MA12070P_Set_AM_Mode(U8 con, U32 kFreq);

void Audio_Dev_MA12070P_Init_R(void);
void Audio_Dev_MA12070P_Open_R(void);
void Audio_Dev_MA12070P_Set_Channel_R(U8 channel);
void Audio_Dev_MA12070P_Set_Volume_R(U8 volume);
void Audio_Dev_MA12070P_Set_Speaker_R(U8 speaker, U8 attenuation);
void Audio_Dev_MA12070P_Set_Bass_Treble_R(U8 bass, U8 treble);
void Audio_Dev_MA12070P_Set_eq_mode_R(U8 mode);
void Audio_Dev_MA12070P_Set_Mute_R(U8 con);
void Audio_Dev_MA12070P_Set_AM_Mode_R(U8 con, U32 kFreq);


#define AUDIO_DEV_MA12070P      Audio_Dev_MA12070P_Init, \
								Audio_Dev_MA12070P_Open,	\
								Audio_Dev_MA12070P_Set_Channel, \
								Audio_Dev_MA12070P_Set_Volume, \
								Audio_Dev_MA12070P_Set_Bass_Treble, \
                                Audio_Dev_MA12070P_Set_eq_mode, \
								Audio_Dev_MA12070P_Set_Mute, \
								Audio_Dev_MA12070P_Set_AM_Mode  //,\
								//Audio_Dev_MA12070P_Set_Speaker

#define AUDIO_DEV_MA12070P_R    Audio_Dev_MA12070P_Init_R, \
								Audio_Dev_MA12070P_Open_R,	\
								Audio_Dev_MA12070P_Set_Channel_R, \
								Audio_Dev_MA12070P_Set_Volume_R, \
								Audio_Dev_MA12070P_Set_Bass_Treble_R, \
                                Audio_Dev_MA12070P_Set_eq_mode_R, \
								Audio_Dev_MA12070P_Set_Mute_R, \
								Audio_Dev_MA12070P_Set_AM_Mode_R  //,\
								//Audio_Dev_MA12070P_Set_Speaker                                

#endif /* __AUDIO_MA12070P__ */
