#include "os_config.h"

#include "c_def.h"
#include "debug.h"


#include "regmap.h"

#include "stdlib.h"

#include "reg_ipcore.h"
#include "hw_timer.h"
#include "optekrf_reg.h"
#include "optekrf.h"
#include "div_tone2410.h"
//#include "dco_8psk2410.h"
//#include "div_8psk2410.h"
//#include "am_8psk2410.h"
#include "am_ham_dpd.h"
#include "am_pm.h"
#include "debug.h"


#ifdef BTDM52_ENABLE

#define __STATIC static
//#define TX_TEST
//#define RX_TEST
#define printf_warn


/**
****************************************************************************************
 * DEFINES
 ****************************************************************************************
 **/

#if 0
#define IP_RADIOCNTL0_ADDR   0x17000070
#define IP_RADIOCNTL0_OFFSET 0x00000070
#define IP_RADIOCNTL0_INDEX  0x0000001C
#define IP_RADIOCNTL0_RESET  0x00000002

/// Macro to read a common ip register
#define REG_IP_RD(addr)              (*(volatile uint32_t *)(addr))

/// Macro to write a common ip register
#define REG_IP_WR(addr, value)       (*(volatile uint32_t *)(addr)) = (value)


__INLINE uint8_t ip_radiocntl0_spicomp_getf(void)
{
    uint32_t localVal = REG_IP_RD(IP_RADIOCNTL0_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}

__INLINE uint8_t ip_radiocntl0_spigo_getf(void)
{
    uint32_t localVal = REG_IP_RD(IP_RADIOCNTL0_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

__INLINE void ip_radiocntl0_spigo_setf(uint8_t spigo)
{
    //ASSERT_ERR((((uint32_t)spigo << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_IP_WR(IP_RADIOCNTL0_ADDR, (REG_IP_RD(IP_RADIOCNTL0_ADDR) & ~((uint32_t)0x00000001)) | ((uint32_t)spigo << 0));
}
#endif


#define REG_EM_ET_SIZE	16
#define REG_EM_ET_BASE_ADDR 0x17010000
/// Exchange memory base address
#define EM_BASE_ADDR         REG_EM_ET_BASE_ADDR


/// Start of the common EM part
#define EM_COMMON_OFFSET     (0)

/*
 * EXCHANGE TABLE
 ****************************************************************************************
 */

/// Exchange table area definition
#define EM_EXCH_TABLE_LEN    16
#define EM_ET_OFFSET         (EM_COMMON_OFFSET)
#define EM_ET_END            (EM_ET_OFFSET + EM_EXCH_TABLE_LEN * REG_EM_ET_SIZE)
/*
 * FREQUENCY TABLE
 ****************************************************************************************
 */

/// Frequency table area definition
#define EM_FT_OFFSET         (EM_ET_END)

#if 1
/// Frequency table area definition - Word size in bytes i.e. 32-bit words
#define RF_FREQ_TABLE_ELT_SIZE                      sizeof(uint8_t)
/// Frequency table area definition - VCO sub-band word size in bytes i.e. 8-bit words
#define RF_VCO_TABLE_ELT_SIZE                       0
#else
/// Frequency table area definition - Word size in bytes i.e. 32-bit words
#define RF_FREQ_TABLE_ELT_SIZE                      sizeof(uint32_t)
/// Frequency table area definition - VCO sub-band word size in bytes i.e. 8-bit words
#define RF_VCO_TABLE_ELT_SIZE                       sizeof(uint8_t)
#endif

/// number of frequencies / Depends on RF target
#if 1//BT_EMB_PRESENT
/// Requires 80 x words for Frequency table + potential VCO sub-band table
#define EM_RF_FREQ_TABLE_LEN  (80 * RF_FREQ_TABLE_ELT_SIZE)
#define EM_RF_VCO_TABLE_LEN (80 * RF_VCO_TABLE_ELT_SIZE)
#elif BLE_EMB_PRESENT
/// Requires 40 x words for Frequency table + potential VCO sub-band table
#define EM_RF_FREQ_TABLE_LEN  (40 * RF_FREQ_TABLE_ELT_SIZE)
#define EM_RF_VCO_TABLE_LEN (40 * RF_VCO_TABLE_ELT_SIZE)
#endif // BT_EMB_PRESENT/BLE_EMB_PRESENT

#define EM_FT_END            (EM_FT_OFFSET + (EM_RF_VCO_TABLE_LEN + EM_RF_FREQ_TABLE_LEN) * sizeof(uint8_t))

/*
 * RF SW SPI
 ****************************************************************************************
 */
/// RF SW-Driven SPI transfers area definition 
#define EM_RF_SW_SPI_OFFSET     (EM_FT_END)



#define RF_EM_SPI_ADRESS              (EM_BASE_ADDR + EM_RF_SW_SPI_OFFSET)

#define ICYDM_SPIRD                   0x00
#define ICYDM_SPIWR                   0x80

#define ICYDM_PWR_TBL_SIZE            0x10
#define ICYDM_PA_TBL_MSK              0x0F
#define ICYDM_PA_PWR_MSK              0x1F

#define ICYDM_MAX_BURST_SIZE          0x80
#define ICYDM_INIT_TBL_18_SIZE        0xD0
#define ICYDM_INIT_TBL_20_SIZE        0xFC
#define ICYDM_INIT_TBL_30_SIZE        0xF7

#define ICYDM_RSSI_20dB_THRHLD        -20
#define ICYDM_RSSI_40dB_THRHLD        -40
#define ICYDM_RSSI_45dB_THRHLD        -45
#define ICYDM_RSSI_48dB_THRHLD        -48
#define ICYDM_RSSI_55dB_THRHLD        -55
#define ICYDM_RSSI_60dB_THRHLD        -60
#define ICYDM_RSSI_70dB_THRHLD        -70


#define REG_ICYDM_RD                rf_icydm_reg_rd
#define REG_ICYDM_WR                rf_icydm_reg_wr

/* The offset value given below is the offset to add to the frequency table index to
   get the value to be programmed in the radio for each channel                      */
#define ICYDM_FREQTAB_OFFSET          0   // Offset for Calypso radio

// TX max power
#define ICYDM_POWER_MIN                 0
#define ICYDM_POWER_MAX                0xFF
#define BTIPT_POWER_MSK                0x3F

// Workaround RSSI value issues identified in EBQ testing
#define RF_EBQ_RSSI_WORKAROUND          1
/**
****************************************************************************************
* MACROS
*****************************************************************************************
*/
// Max burst register
__STATIC uint8_t rf_icydm_reg_buf[ICYDM_MAX_BURST_SIZE + 8]; // max burst size + buffer controls, TODO


// IcyTRxDM register definitions and access functions
__STATIC uint32_t rf_icydm_reg_rd (uint32_t addr);
__STATIC void rf_icydm_reg_wr (uint32_t addr, uint32_t value);


/// IcyTRx EM Write Macro for HW driven SPI chained structures
#define RF_ICTRXDM_EM_WR(addr, val) \
    EM_WR((((uint32_t) (addr)) + REG_EM_ET_BASE_ADDR), (val))


/**
 ****************************************************************************************
 * @brief SPI access
 ***************************************************************************************
 */
__STATIC void rf_icydm_spi_tf(void)
{
    //launch SPI transfer
    ip_radiocntl0_spigo_setf(1);

    //wait for transfer to be completed
    while (!ip_radiocntl0_spicomp_getf());
}

/**
 ****************************************************************************************
 * @brief IcyTRxDM specific read access
 *
 * @param[in] addr    register address
 *
 * @return uint32_t value
 *****************************************************************************************
 */
__STATIC uint32_t rf_icydm_reg_rd (uint32_t addr)
{
    // Next Pointr to 0x0
    rf_icydm_reg_buf[0] = (uint8_t)(0);  // Next Pointer set to 0x0000 to stop the SPI Chained access
    rf_icydm_reg_buf[1] = (uint8_t)(0);

    //copy control and number of u32 to send
    rf_icydm_reg_buf[2] = (uint8_t)(ICYDM_SPIRD + 1);

    //copy address
    rf_icydm_reg_buf[3] = (uint8_t)(addr & 0x00FF);
    rf_icydm_reg_buf[4] = (uint8_t)((addr & 0xFF00) >> 8 );

    // Padding
    rf_icydm_reg_buf[5] = (uint8_t)(0);
    rf_icydm_reg_buf[6] = (uint8_t)(0);
    rf_icydm_reg_buf[7] = (uint8_t)(0);

    memcpy((void *)RF_EM_SPI_ADRESS, rf_icydm_reg_buf, 8);

    //do the transfer
    rf_icydm_spi_tf();

    return (uint32_t)(*((uint8_t *)(RF_EM_SPI_ADRESS + 8)));
}

/**
 ****************************************************************************************
 * @brief IcyTRxDM specific read access
 *
 * @param[in] addr    register address
 * @param[in] size    transfer size
 * @param[in] buf     pointer to the buffer to store data
 *
 * @return void
 ****************************************************************************************
 **/
__STATIC void rf_icydm_reg_burst_rd (uint16_t addr, uint8_t size, uint8_t *buf)
{
    rf_icydm_reg_buf[0] = (uint8_t)(0);  // Next Pointer set to 0x0000 to stop the SPI Chained access
    rf_icydm_reg_buf[1] = (uint8_t)(0);

    //copy control and number of u8 to read
    rf_icydm_reg_buf[2] = (uint8_t)(ICYDM_SPIRD + size);

    //copy address
    rf_icydm_reg_buf[3] = (uint8_t)(addr & 0x00FF);
    rf_icydm_reg_buf[4] = (uint8_t)((addr & 0xFF00) >> 8 );

    // Padding
    rf_icydm_reg_buf[5] = (uint8_t)(0);
    rf_icydm_reg_buf[6] = (uint8_t)(0);
    rf_icydm_reg_buf[7] = (uint8_t)(0);

    memcpy((void *)RF_EM_SPI_ADRESS, rf_icydm_reg_buf, 8);

    //do the transfer
    rf_icydm_spi_tf();

    for(int i =0;i < size;i++)
    {
        buf[i] = *((uint8_t *)(RF_EM_SPI_ADRESS + 8 + i));
    }    
}

/**
 ****************************************************************************************
 * @brief IcyTRxDM specific write access
 *
 * @param[in] addr    register address
 * @param[in] value   value to write
 *
 * @return uint32_t value
 ****************************************************************************************
 */
__STATIC void rf_icydm_reg_wr (uint32_t addr, uint32_t value)
{
    rf_icydm_reg_buf[0] = (uint8_t)(0);  // Next Pointer set to 0x0000 to stop the SPI Chained access
    rf_icydm_reg_buf[1] = (uint8_t)(0);

    //inversion for EM reading by U8 on ATL SPI side
    //copy control and number of u32 to send
    rf_icydm_reg_buf[2] = (uint8_t)(ICYDM_SPIWR + 1);

    //copy address
    rf_icydm_reg_buf[3] = (uint8_t)(addr & 0x00FF);
    rf_icydm_reg_buf[4] = (uint8_t)((addr & 0xFF00) >> 8 );

    // Padding
    rf_icydm_reg_buf[5] = (uint8_t)(0);
    rf_icydm_reg_buf[6] = (uint8_t)(0);
    rf_icydm_reg_buf[7] = (uint8_t)(0);

    // Byte to be written
    rf_icydm_reg_buf[8]= (uint8_t)value;

    memcpy((void *)RF_EM_SPI_ADRESS, rf_icydm_reg_buf, 9);

    //do the transfer
    rf_icydm_spi_tf();
}

/**
 ****************************************************************************************
 * @brief IcyTRxDM specific read access
 *
 * @param[in] addr    register address
 * @param[in] size    transfer size
 * @param[in] data    pointer to the data array
 *
 * @return uint32_t value
 ****************************************************************************************
 **/
__STATIC void rf_icydm_reg_burst_wr (uint16_t addr, uint8_t size, uint8_t *data)
{
    rf_icydm_reg_buf[0] = (uint8_t)(0);  // Next Pointer set to 0x0000 to stop the SPI Chained access
    rf_icydm_reg_buf[1] = (uint8_t)(0);

    //copy control and number of u8 to send
    rf_icydm_reg_buf[2] = (uint8_t)(ICYDM_SPIWR + size);

    //copy address
    rf_icydm_reg_buf[3] = (uint8_t)(addr & 0x00FF);
    rf_icydm_reg_buf[4] = (uint8_t)((addr & 0xFF00) >> 8 );

    // Padding
    rf_icydm_reg_buf[5] = (uint8_t)(0);
    rf_icydm_reg_buf[6] = (uint8_t)(0);
    rf_icydm_reg_buf[7] = (uint8_t)(0);

    for(int i =0;i < size + 2;i++)
    {
        rf_icydm_reg_buf[i + 8] = *(data + i);
    }

    memcpy((void *)RF_EM_SPI_ADRESS, rf_icydm_reg_buf, 8 + size);

    //do the transfer
    rf_icydm_spi_tf();
}

void rf_switch(U8 csem)
{
	U32 src_sw_value = 0;
    U8 csem_rd_value = 0;

	switch (csem)
	{
		case 0: src_sw_value = 0x04000000;              //optek rf
				break;
		case 1: src_sw_value = 0x0C000000;              //csem rf
				break;
		default: src_sw_value = 0x04000000;
				break;
	}

    rf_icydm_reg_burst_wr(0x524,4,&src_sw_value);
    csem_rd_value = rf_icydm_reg_rd(0x3FF);
}


#define MEM_TEST

//#define SPI_TEST
#define OPTEKRF_SPI_TEST
//#define OPTEKRF_TRX_TEST

#define OPTEKRF_TX_TEST                         //debug optekrf tx module

#define PA_SEL_BIT 7
#define ANT_SEL_BIT 6
#define BLE_4M_BIT 5

#if 0//moved to optekrf.h
#define OPTEKRF_TEST_BY_CEVA
#define OPTEKRF_RX_TEST_BY_REG
#define OPTEKRF_TEST_BY_OPTEK

#define TEST_BOARD_1
//#define TEST_BOARD_2

#ifdef TEST_BOARD_1
#define TEST_BOARD_1_TX
#endif

#ifdef TEST_BOARD_2
//#define TEST_BOARD_2_RX
#define TEST_BOARD_2_TX
#endif
#endif


//#define OPTEKRF_RX_2M_ONLY

void hw_optek_rf_test(void)
{
	U32 i = 0;
	U16 val_16b = 0;
	U8 val_8b = 0;
#if 1
    //btdm mdm init
    BTDM_MDM_MDM_CNTL_ADDR_CT = 0x0A;                       //enable btdm mdm and rx cfg sel
    BTDM_MDM_CORR_ERR_CNTL_ADDR_CT = 0x80000003;            //sync word from port
    //BTDM_MDM_CORR_ERR_CNTL_ADDR_CT = 0x00000003;            //sync word from apb, for sim
    //BTDM_MDM_TE_TIMEINIT_ADDR_CT = 0x003f03e2;              //edr sync timing, for 16MHz only

    //BTDM_MDM_FCS_IFMHZ_ADDR_CT = (0x1aab << 16) | 0x1d55;


    #ifdef OPTEKRF_RX_2M_ONLY
        BTDM_MDM_FCS_IFMHZ_ADDR_CT = (0xFAAB << 16) | 0xFAAB;       //-683(1MHz)->0xFD55, -1365(2MHz)->0xFAAB
    #else
        BTDM_MDM_FCS_IFMHZ_ADDR_CT = (0xFAAB << 16) | 0xFD55;       //-683(1MHz)->0xFD55, -1365(2MHz)->0xFAAB
    #endif 

#define OPTEKRF_TX_PWRUP_DELAY			0x30//0x10
#define OPTEKRF_RX_PWRUP_DELAY			0x30//0x10
#define OPTEKRF_FAST_PWRUP_DELAY		0x3                     //modem fast restart 3us

    BTDM_MDM_RX_STARTUPDEL_ADDR_CT = (OPTEKRF_FAST_PWRUP_DELAY << 24) | ((1 << 16) | OPTEKRF_RX_PWRUP_DELAY);		//for config 1 
    BTDM_MDM_TX_STARTUPDEL_ADDR_CT = ((4 << 16) | OPTEKRF_TX_PWRUP_DELAY);


    U32 mdm_rd_val[16];
    mdm_rd_val[0] = BTDM_MDM_MDM_CNTL_ADDR_CT;
    mdm_rd_val[1] = BTDM_MDM_CORR_ERR_CNTL_ADDR_CT;
    mdm_rd_val[2] = BTDM_MDM_TE_TIMEINIT_ADDR_CT;
    mdm_rd_val[3] = BTDM_MDM_FCS_IFMHZ_ADDR_CT;
    mdm_rd_val[4] = BTDM_MDM_RX_STARTUPDEL_ADDR_CT;
    mdm_rd_val[5] = BTDM_MDM_TX_STARTUPDEL_ADDR_CT;

    if ((mdm_rd_val[0] == 0x02) && (mdm_rd_val[1] == 0X80000003) && (mdm_rd_val[2] == 0x003f03e2) && (mdm_rd_val[3] == ((0x1aab << 16) | 0x1d55)))
    {
        //printf_warn("mdm reg wr succeed\r\n");
    }
    else
    {
        //printf_warn("mdm reg wr failed, 0x%x, 0x%x, 0x%x, 0x%x\r\n", mdm_rd_val[0], mdm_rd_val[1], mdm_rd_val[2], mdm_rd_val[3]);
    }
#endif


#define AM_9b_to_14b
#define RF_AM_9b

    OPTEKRF_MDM_CTRL *p_mdm_ctrl = (OPTEKRF_MDM_CTRL *) &val_8b;
    OPTEKRF_RX_RFGA_EFFECT_T *p_rx_rfga_effect_t = (OPTEKRF_RX_RFGA_EFFECT_T *) &val_8b;
    u32 am_a [7] = {0x06b73, 0x2fbc0, 0x0ec76, 0x3a1ca, 0x00fd6, 0x002a0, 0x000014};

    val_8b = 0;

    p_mdm_ctrl->am_sdm_en = 1;
    p_mdm_ctrl->dco_sdm_en = 1;


    p_mdm_ctrl->am_9b_14b = 1;
    p_mdm_ctrl->rf_am_9b = 1;                 //new gen rf ip

    #ifdef OPTEKRF_RX_2M_ONLY
        p_mdm_ctrl->rx_2m = 1;                  //rf IF is 2M
    #else
        p_mdm_ctrl->rx_2m = 0;
    #endif  //OPTEKRF_RX_2M_ONLY                   

   rf_icydm_reg_wr (OPTEKRF_MDM_CTRL_ADDR,val_8b);


    val_8b = 0;
    U8 val8b_rd = 0;

    p_rx_rfga_effect_t->rf_ga_effect_t = 16;
    p_rx_rfga_effect_t->rx_agc_avg_t = 0;
    p_rx_rfga_effect_t->tx_rampup_en = 1;
    p_rx_rfga_effect_t->tx_rampdn_en = 1;

    rf_icydm_reg_wr (OPTEKRF_RX_RFGA_EFFECT_T_ADDR,val_8b);
    val8b_rd = rf_icydm_reg_rd (OPTEKRF_RX_RFGA_EFFECT_T_ADDR);
    if (val8b_rd != 0xd0)
    {
        asm("break 1,1");
    }


    //init high pa and low pa am to am and am to pm table
    val_16b = 0x0;                                                      //hpa am am addr
    rf_icydm_reg_burst_wr(OPTEKRF_AM_TABLE_INDEX_ADDR,2,&val_16b);  
    
    //am am 9b

#if 1
    //no am am
    for (i=0; i<512; i++) {
        val_16b = (i << 3);
        rf_icydm_reg_burst_wr(OPTEKRF_AM_TABLE_DATA_ADDR,2,&val_16b);     //hpa  
    }

    for (i=0; i<512; i++) {
        val_16b = (i << 3);
        rf_icydm_reg_burst_wr(OPTEKRF_AM_TABLE_DATA_ADDR,2,&val_16b);     //lpa  
    }
#else
    //am am
    for (i=0; i<512; i++) {
        rf_icydm_reg_burst_wr(OPTEKRF_AM_TABLE_DATA_ADDR,2,&am_ham_dpd[i]);     //hpa  
    }

    for (i=0; i<512; i++) {
        rf_icydm_reg_burst_wr(OPTEKRF_AM_TABLE_DATA_ADDR,2,&am_ham_dpd[i]);     //lpa  
    }
#endif

    val_16b = 0x8000;                                                    //hpa am pm, 128 table   
    rf_icydm_reg_burst_wr(OPTEKRF_AM_TABLE_INDEX_ADDR,2,&val_16b);


#if 1
    //no am pm
    val_16b = 0x1;          //test                                                   
    for (i=0; i<128; i++) {
        //rf_icydm_reg_burst_wr(OPTEKRF_AM_TABLE_DATA_ADDR,2,&ham_pm_dpd[i]);         
        rf_icydm_reg_burst_wr(OPTEKRF_AM_TABLE_DATA_ADDR,2,&val_16b);                 //hpa
    }

    val_16b = 0x2;          //test
    for (i=0; i<128; i++) {
        //rf_icydm_reg_burst_wr(OPTEKRF_AM_TABLE_DATA_ADDR,2,&ham_pm_dpd[i]);         
        rf_icydm_reg_burst_wr(OPTEKRF_AM_TABLE_DATA_ADDR,2,&val_16b);                 //lpa 
    }
#else
    //am pm                                                 
    for (i=0; i<128; i++) {
        rf_icydm_reg_burst_wr(OPTEKRF_AM_TABLE_DATA_ADDR,2,&am_pm[i]);                 //hpa
    }

    for (i=0; i<128; i++) {
        rf_icydm_reg_burst_wr(OPTEKRF_AM_TABLE_DATA_ADDR,2,&am_pm[i]);                 //lpa 
    }
#endif   


   //verify

   
    //TODO

    volatile U32 val;
	int	ret;
	U32 rd_val;
	U32 test_err = 0;
    //U32 i;

	MISC_CLKGATE_CONTROLs *pGate;
	val = REG_MISC_CLKGATE;
	pGate = (MISC_CLKGATE_CONTROLs *) &val;
	pGate->AHB_BTDM_IF_gate = 1;						//open BTDM clk gate
    pGate->AHB_MDM_IF_pclk_gate = 1;                   //open MDM clk gate
	REG_MISC_CLKGATE = val;	


#if 0//manually gain control setting
    val = 0;
    MISC_RF_RX_GAIN_CONTROLs *pGain = (MISC_RF_RX_GAIN_CONTROLs *) &val;
	pGain->rx_ga_from_spi = 1;
    pGain->rx_rf_ga_from_spi = 6;
    pGain->rx_pga_ga_from_spi = 5;
    pGain->rolo_sel = 0;
	rf_icydm_reg_wr(OPTEKRF_RX_GAIN_ADDR,val);
#endif

	rf_switch(0);
	
#if 0//csem module hardware reset

    MISC_BTDM_CONTROLs *btdm_config;
    val = REG_MISC_BTDM;
    btdm_config = (MISC_BTDM_CONTROLs*) &val;

    btdm_config->bt_dbg_sel_2 &= ~(0x1<<2);
    btdm_config->bt_dbg_sel_2 &= ~(0x1<<1);
    btdm_config->bt_dbg_sel_2 &= ~(0x1<<0);
    REG_MISC_BTDM = val;
    delayms(10);

    btdm_config->bt_dbg_sel_2 |= (0x1<<1);
    REG_MISC_BTDM = val;
    delayms(10);

    btdm_config->bt_dbg_sel_2 |= (0x1<<0);
    REG_MISC_BTDM = val;
    delayms(10);

    btdm_config->bt_dbg_sel_2 |= (0x1<<2);
    REG_MISC_BTDM = val;
    delayms(10);
    
#endif

#if 0
// --------------- RADIOCNTL0 register ---------------
    // Set pointer SW SPI Drive access Pointer
    ip_radiocntl0_pack( /*uint16_t spiptr*/        (EM_RF_SW_SPI_OFFSET >> 2),
                        /*uint8_t  spicfg*/         0,
                        /*uint8_t  spifreq*/        1,
                        /*uint8_t  spigo*/          0);


// --------------- RADIOCNTL1 register ---------------
    /* RADIOCNTL1 */
    ip_radiocntl1_pack(/*uint8_t  forceagcen*/      0,
                        /*uint8_t  forceiq*/         0,
                        /*uint8_t  rxdnsl*/          0,
                        /*uint8_t  txdnsl*/          0,
                        /*uint16_t forceagclength*/  0x0,
                        /*uint8_t  syncpulsemode*/   1,
                        /*uint8_t  syncpulsesrc*/    1,
                        /*uint8_t  dpcorren*/        0,
                        /*uint8_t  jefselect*/       0,
                        /*uint8_t  xrfsel*/          0x07,
                        /*uint8_t  subversion*/      0x0);

#endif


    //rf_icydm_reg_wr(OPTEKRF_RFPLL_SETTING_ADDR,100);
#define TX_PWRUP_TIME     0x4c//0x59(release)
#define TX_PWRDN_TIME     0x3//0x5(release)
#define RX_PWRUP_TIME     0x4c//0x56(release)

    /* BT RADIOPWRUPDN */
    bt_radiopwrupdn_pack(   /*uint8_t rxpwrupct*/       RX_PWRUP_TIME,
                            /*uint8_t txpwrdnct*/       TX_PWRDN_TIME,
                            /*uint8_t txpwrupct*/       TX_PWRUP_TIME/*0x50*/);

    /* BLE RADIOTXRXTIM0 */
    ble_radiotxrxtim0_pack( /*uint8_t rfrxtmda0*/       9,
                            /*uint8_t rxpathdly0*/      5,
                            /*uint8_t txpathdly0*/      5);

    /* BLE RADIOTXRXTIM1 */
    ble_radiotxrxtim1_pack( /*uint8_t rfrxtmda1*/       5,
                            /*uint8_t rxpathdly1*/      5,
                            /*uint8_t txpathdly1*/      4);

#if 1
	/* BLE RADIOPWRUPDN0 */
    ble_radiopwrupdn0_pack(/*uint8_t syncposition0*/    0,
                            /*uint8_t rxpwrup0*/       RX_PWRUP_TIME,
                            /*uint8_t txpwrdn0*/       TX_PWRDN_TIME,
                            /*uint8_t txpwrup0*/       TX_PWRUP_TIME);

    /* BLE RADIOPWRUPDN1 */
    ble_radiopwrupdn1_pack(/*uint8_t syncposition1*/  0,
                            /*uint8_t rxpwrup1*/       RX_PWRUP_TIME,
                            /*uint8_t txpwrdn1*/       TX_PWRDN_TIME,
                            /*uint8_t txpwrup1*/       TX_PWRUP_TIME);

    /* BLE RADIOPWRUPDN2 */
    ble_radiopwrupdn2_pack(/*uint8_t syncposition2*/  0,
                            /*uint8_t rxpwrup2*/       RX_PWRUP_TIME,
                            /*uint8_t txpwrdn2*/       TX_PWRDN_TIME,
                            /*uint8_t txpwrup2*/       TX_PWRUP_TIME);

    /* BLE RADIOPWRUPDN3 */
    ble_radiopwrupdn3_pack(/*uint8_t txpwrdn3*/       TX_PWRDN_TIME,
                            /*uint8_t txpwrup3*/       TX_PWRUP_TIME);
#endif


    u32 optek_rf_setting_32b_tx [128];
    u32 optek_rf_reading_32b [128];

    /* rf power test param */
    #if 0
    //CLK
    //optek_rf_setting_32b_tx[0] = 0x003E020F;      //
    //optek_rf_setting_32b_tx[0] = 0x01220208;        //default
    //optek_rf_setting_32b_tx[0] = 0x003E03FF;        //trimed, 12pF
    //optek_rf_setting_32b_tx[0] = 0x003E033F;        //trimed, min
    //optek_rf_setting_32b_tx[0] = 0x812203FF;        //trimed
    //optek_rf_setting_32b_tx[0] = 0x012202AC;        //trimed
    //optek_rf_setting_32b_tx[0] = 0x012201AC;        //trimed
    optek_rf_setting_32b_tx[0] = 0x012201F4;        //trimed
    //LDO
    //optek_rf_setting_32b_tx[1] = 0x00000400;        //
    //optek_rf_setting_32b_tx[1] = 0x02492400;        //default
    optek_rf_setting_32b_tx[1] = 0x0249250B;        //
    //optek_rf_setting_32b_tx[1] = 0xF2492400;        //modified 1
    //optek_rf_setting_32b_tx[1] = 0xF249240B;        //modified 2
    //REF
    //optek_rf_setting_32b_tx[2] = 0x44888888;        //default
    optek_rf_setting_32b_tx[2] = 0xC4888888;        //max gain
    //PLL
    //optek_rf_setting_32b_tx[3] = 0x48924907;        //default
    //optek_rf_setting_32b_tx[3] = 0x48924938;        //change the relationship between 96MHz and feedback clk
    //optek_rf_setting_32b_tx[3] = 0xC8924907;        //fixed
    //optek_rf_setting_32b_tx[3] = 0x49924907;        //modified
    optek_rf_setting_32b_tx[3] = 0x64924FC7;        //
    //VCO
    //optek_rf_setting_32b_tx[4] = 0xCC5E8034;        //2400MHz
    //optek_rf_setting_32b_tx[4] = 0xCC688034;        //2448MHz
    //optek_rf_setting_32b_tx[4] = 0xCC200034;        //default
    optek_rf_setting_32b_tx[4] = 0xFF200032;        //
    //RXABB
    //optek_rf_setting_32b_tx[5] = 0x44668888;        //modified
    optek_rf_setting_32b_tx[5] = 0x04668888;        //default
    //optek_rf_setting_32b_tx[5] = 0xC4668888;        //enable PGA output
    //optek_rf_setting_32b_tx[5] = 0xDC668888;        //
    //TRX
    //optek_rf_setting_32b_tx[6] = 0x0430E0FF;        //modified 1
    //optek_rf_setting_32b_tx[6] = 0x0410EF80;        //default
    optek_rf_setting_32b_tx[6] = 0x0410EF4F;        //max pa power, manually control
    //optek_rf_setting_32b_tx[6] = 0x0430FF4F;        //max rx gain, manually control
    //optek_rf_setting_32b_tx[6] = 0x04300F80;        //modified 2
    //XCVR
    //optek_rf_setting_32b_tx[7] = 0xA0049C09;        //modified 1
    //optek_rf_setting_32b_tx[7] = 0xA0048409;        //modified 2, disable
    optek_rf_setting_32b_tx[7] = 0xA0048C09;        //default
    //optek_rf_setting_32b_tx[7] = 0xA0048C07;        //
    //optek_rf_setting_32b_tx[7] = 0xA0048409;            //20240105, disable ovf_unf
    //optek_rf_setting_32b_tx[7] = 0xA0048C0f;      //
    //optek_rf_setting_32b_tx[7] = 0xA0048009;        //modified 3
    //optek_rf_setting_32b_tx[7] = 0xA0048C87;        //modified 4
    //optek_rf_setting_32b_tx[7] = 0xA0048C07;        //for 1.1v port current test
    //optek_rf_setting_32b_tx[7] = 0xA0048C87;        //for 1.1v port current test
    #endif
    #if 1
    //CLK
    //optek_rf_setting_32b_tx[0] = 0x012201F4;        //trimed
    optek_rf_setting_32b_tx[0] = 0x01220284;        //trimed
    //LDO
    optek_rf_setting_32b_tx[1] = 0x03EDB60B;        //
    //REF
    optek_rf_setting_32b_tx[2] = 0x46F88F80;        //
    //PLL
    optek_rf_setting_32b_tx[3] = 0x64924FC7;        //
    //VCO
    optek_rf_setting_32b_tx[4] = 0xFF200032;        //
    //RXABB
    optek_rf_setting_32b_tx[5] = 0x046A8888;        //default
    //TRX
    optek_rf_setting_32b_tx[6] = 0x018A6F20;        //
    //XCVR
    optek_rf_setting_32b_tx[7] = 0xA0048C09;        //default
    #endif

#if 0
    //optek_rf_setting_32b_tx[0] = 0x01220128;      //TRIM
    optek_rf_setting_32b_tx[0] = 0x003E012F;        //48MHz->47.9999MHz
    //optek_rf_setting_32b_tx[0] = 0x003E022F;
#endif
#ifdef OPTEKRF_RX_TEST_BY_REG
    //for rx test
    //optek_rf_setting_32b_tx[0] = 0x003E03FF;        //CLK
    //optek_rf_setting_32b_tx[5] = 0xC4668888;        //RXABB, PGA
    optek_rf_setting_32b_tx[5] = 0x44668888;        //RXABB, mixed output
    //optek_rf_setting_32b_tx[5] = 0x84668888;        //RXABB, filter
    //optek_rf_setting_32b_tx[7] = 0xA0048437;        //XCVR
    optek_rf_setting_32b_tx[7] = 0xA0048C07;        //XCVR

    //optek_rf_setting_32b_tx[4] = 0xCC6C003C;        //VCO, 2449MHz, for test
    optek_rf_setting_32b_tx[6] = 0x0430EF80;        //TRX, default
#endif

    //param wr
    rf_icydm_reg_burst_wr( OPTEKRF_CLK_REG_CORE_ADDR, 4, &optek_rf_setting_32b_tx[0]);
    rf_icydm_reg_burst_wr( OPTEKRF_LDO_REG_CORE_ADDR, 4, &optek_rf_setting_32b_tx[1]);
    rf_icydm_reg_burst_wr( OPTEKRF_RFE_REG_ADDR, 4, &optek_rf_setting_32b_tx[2]);
    rf_icydm_reg_burst_wr( OPTEKRF_RFPLL_REG_ADDR, 4, &optek_rf_setting_32b_tx[3]);
    rf_icydm_reg_burst_wr( OPTEKRF_VCO_REG_ADDR, 4, &optek_rf_setting_32b_tx[4]);
    rf_icydm_reg_burst_wr( OPTEKRF_RXABB_REG_ADDR, 4, &optek_rf_setting_32b_tx[5]);
    rf_icydm_reg_burst_wr( OPTEKRF_TRX_REG_ADDR, 4, &optek_rf_setting_32b_tx[6]);
    rf_icydm_reg_burst_wr( OPTEKRF_XCVR_REG_ADDR, 4, &optek_rf_setting_32b_tx[7]);

    delayms(10);
    unsigned char optekrf_fco_rd[16];
    unsigned char optekrf_fco2_rd[16];
    U8 optekrf_fco_err = 0;
    U8 optekrf_fco2_err = 0;
    U8* pfvco = NULL;
    U8* pfvco2 = NULL;

    //tx
    pfvco = optekrf_fco;


    rf_icydm_reg_burst_wr( OPREKRF_FCVO_1_ADD, 16, pfvco);

    rf_icydm_reg_burst_rd( OPREKRF_FCVO_1_ADD, 16, &optekrf_fco_rd[0]);
    for (i=0; i<16; i++)
    {
        if (optekrf_fco_rd[i] != *(pfvco+i))
        {
            printf_warn("setting: %d, read: %d\r\n", *(pfvco+i), optekrf_fco_rd[i]);

            optekrf_fco_err++;
            asm("break 1,1");
        }
    }
    if (optekrf_fco_err == 0)
    {
        printf_warn("optekrf_fco wr & rd test successful\r\n");
    }

    
    //rx
    pfvco2 = optekrf_fco2;

    rf_icydm_reg_burst_wr( OPREKRF_FCVO2_1_ADD, 16, pfvco2);

    rf_icydm_reg_burst_rd( OPREKRF_FCVO2_1_ADD, 16, &optekrf_fco2_rd[0]);
    for (i=0; i<16; i++)
    {
        if (optekrf_fco2_rd[i] != *(pfvco2+i))
        {
            printf_warn("setting: %d, read: %d\r\n", *(pfvco2+i), optekrf_fco2_rd[i]);

            optekrf_fco2_err++;
            asm("break 1,1");
        }
    }
    if (optekrf_fco2_err == 0)
    {
        printf_warn("optekrf_fco wr & rd test successful\r\n");
    }


        U32 index = 0;
    #if 0//loading am to high power am table
        for (index=0; index<256; index++) {
            rf_icydm_reg_burst_wr((AM_HAM_ADDR + index*2),2,&am_ham_dpd[index]);
        }
    #endif

    //rf_icydm_reg_wr(OPTEKRF_LDOS_SETTING_ADDR,0x2a);        //rf ldo always on


    U16 wr_value1, wr_value2, rd_value[2];
    wr_value1 = 0;//10923;
    wr_value2 = 0;//10923;
#if 0
    rf_icydm_reg_burst_wr( OPTEKRF_RXIF_2M_DIV_ADDR_0, 2, &wr_value1);
    rf_icydm_reg_burst_wr( OPTEKRF_RXIF_DIV_ADDR_0, 2, &wr_value2);

    rf_icydm_reg_burst_rd( OPTEKRF_RXIF_2M_DIV_ADDR_0, 2, &rd_value[0]);
    rf_icydm_reg_burst_rd( OPTEKRF_RXIF_DIV_ADDR_0, 2, &rd_value[1]);

    if ((rd_value[0] == wr_value1) && (rd_value[1] == wr_value2))
    {
    	printf_warn("RXIF_2M_DIV & RXIF_DIV wr succeed\r\n");
    }
    else
    {
    	printf_warn("RXIF_2M_DIV & RXIF_DIV wr failed\r\n");
    	asm("break 1,1");
    }
#endif

    signed char optekrf_ga_edr_rd[16];
    U8 optekrf_ga_edr_err = 0;
    rf_icydm_reg_burst_wr( OPREKRF_DCO_GA_BTEDR_1_ADDR, 16, &optekrf_ga_edr[0]);

    rf_icydm_reg_burst_rd( OPREKRF_DCO_GA_BTEDR_1_ADDR, 16, &optekrf_ga_edr_rd[0]);
    for (i=0; i<16; i++)
    {
        if (optekrf_ga_edr_rd[i] != optekrf_ga_edr[i])
        {
            printf_warn("setting: %d, read: %d\r\n", optekrf_ga_edr[i], optekrf_ga_edr_rd[i]);
        }
        else
        {
            optekrf_ga_edr_err++;
        }
    }
    
    #if 1//rx agc adjust
    rf_icydm_reg_wr(OPTEKRF_RX_AGC_0db_ADJ_ADDR,8);
    #endif


    if (optekrf_ga_edr_err == 0)
    {
        printf_warn("optekrf_ga_edr wr & rd test successful\r\n");
    }


    U32 optekrf_debug;
    unsigned char mode, pa_sel, ant_sel, ble_4m;
    U8 optek_rf_setting[16];
#if 0
    pa_sel = (0x0 << PA_SEL_BIT);                           //1 -> hpa, 0 -> lpa
    ant_sel = (0x0 << ANT_SEL_BIT);                         //0 -> ANT0(normal), 1 -> ANT1
    ble_4m = (0x0 << BLE_4M_BIT);                           //enable ble4m fro 8psk 24m test

    //optekrf_debug = 0x80193456;                               //output toggle
    //optekrf_debug = 0x10193456;                               //output rom
    optekrf_debug = 0x10000000;                                 //am pm test

    rf_icydm_reg_burst_wr(0x524,4,&optekrf_debug);

    val_8b = 0x0;
    rf_icydm_reg_wr(OPTEKRF_PA_ANT_SEL_ADDR, val_8b);

    optek_rf_setting[0] = 0x4 | pa_sel | ant_sel | ble_4m;                    //bt 1MHz             
    //optek_rf_setting[0] = 0x7 | pa_sel | ant_sel | ble_4m;                      //bt edr 3MHz             
    optek_rf_setting[1] = 46;                       //channel
    optek_rf_setting[2] = 0xc0;//0x80;//0xFF;                     //power, 0.75->optekrf 0db
    optek_rf_setting[3] = 0x10;                     //tx startup

    rf_icydm_reg_burst_wr(0x800,4,&optek_rf_setting[0]);
#endif


#ifdef OPTEKRF_TEST_BY_CEVA
    return;                       //return for ceva test
#endif

#ifdef OPTEKRF_TEST_BY_OPTEK

    /*
    signed char optekrf_ga_edr_rd[16];
    U8 optekrf_ga_edr_err = 0;
    rf_icydm_reg_burst_wr( OPREKRF_DCO_GA_BTEDR_1_ADDR, 16, &optekrf_ga_edr[0]);

    rf_icydm_reg_burst_rd( OPREKRF_DCO_GA_BTEDR_1_ADDR, 16, &optekrf_ga_edr_rd[0]);
    for (i=0; i<16; i++)
    {
        if (optekrf_ga_edr_rd[i] != optekrf_ga_edr[i])
        {
            printf_warn("setting: %d, read: %d\r\n", optekrf_ga_edr[i], optekrf_ga_edr_rd[i]);
        }
        else
        {
            optekrf_ga_edr_err++;
        }
    }
    if (optekrf_ga_edr_err == 0)
    {
        printf_warn("optekrf_ga_edr wr & rd test successful\r\n");
    }*/

//     //32b toggle, 30b edge, 29b rom output, 28b rom _debug
//     //rom data [7:0] am, [15:8] dco, [20:16] div


//#define OPTEKRF_TX_TEST_FRAM_DBG_RAM
#ifdef OPTEKRF_TX_TEST_FRAM_DBG_RAM

    #define MAX_ROM_ELN 105600
    //#define MAX_ROM_ELN 13  
    U8 u8_val;   
    OPTEKRF_FM_AM_DT *pam_fm_dt = (OPTEKRF_FM_AM_DT *) &u8_val;
    OPTEKRF_DIV_DCO_DT *pam_dco_div = (OPTEKRF_DIV_DCO_DT *) &u8_val;
    unsigned char am = 64;
    unsigned char dco = 0;//255;
    unsigned char div = 19;


    pam_fm_dt->fm_dt = 3;                                           //default
    pam_fm_dt->am_dt = 4;
    rf_icydm_reg_wr(OPTEKRF_FM_AM_DT_ADDR,u8_val);

    pam_dco_div->div_dt = 4;
    pam_dco_div->dco_dt = 4;
    rf_icydm_reg_wr(OPTEKRF_DCO_DIV_DT_ADDR,u8_val);


    optekrf_debug = 0x12000000;                             //optekrf dbg ram enable, push to optek dbg ram                 
    //rf_icydm_reg_burst_wr(0x524,4,&optekrf_debug);

/*
    //push optekrf dgb data

    BTDM_MDM_VERSION_ADDR_CT = div << 16 | dco << 8 | am << 0;
    BTDM_MDM_VERSION_ADDR_CT = div << 16 | dco << 8 | am << 0;
    BTDM_MDM_VERSION_ADDR_CT = div << 16 | dco << 8 | am << 0;
    BTDM_MDM_VERSION_ADDR_CT = div << 16 | dco << 8 | am << 0;
    BTDM_MDM_VERSION_ADDR_CT = div << 16 | dco << 8 | am << 0;
    BTDM_MDM_VERSION_ADDR_CT = div << 16 | dco << 8 | am << 0;
    BTDM_MDM_VERSION_ADDR_CT = div << 16 | dco << 8 | am << 0;
    BTDM_MDM_VERSION_ADDR_CT = div << 16 | dco << 8 | am << 0;
    BTDM_MDM_VERSION_ADDR_CT = div << 16 | dco << 8 | am << 0;
    BTDM_MDM_VERSION_ADDR_CT = div << 16 | dco << 8 | am << 0;
    BTDM_MDM_VERSION_ADDR_CT = div << 16 | dco << 8 | am << 0;
    BTDM_MDM_VERSION_ADDR_CT = div << 16 | dco << 8 | am << 0;
    BTDM_MDM_VERSION_ADDR_CT = div << 16 | dco << 8 | am << 0;
    BTDM_MDM_VERSION_ADDR_CT = div << 16 | dco << 8 | am << 0;
    BTDM_MDM_VERSION_ADDR_CT = div << 16 | dco << 8 | am << 0;
    BTDM_MDM_VERSION_ADDR_CT = div << 16 | dco << 8 | am << 0;


    optekrf_debug = 0x11000000 + MAX_ROM_ELN;                     //pop optek dbg ram data                
    rf_icydm_reg_burst_wr(0x524,4,&optekrf_debug);
*/


#if 1//div_tone2410
    for (index=0; index<sizeof(div_tone2410); index++)
    {
        BTDM_MDM_VERSION_ADDR_CT = div_tone2410[index] << 16 | dco << 8 | am << 0;
    }

    optekrf_debug = 0x11000000 + (sizeof(div_tone2410) - 1);//MAX_ROM_ELN;                     //pop optek dbg ram data                
    //rf_icydm_reg_burst_wr(0x524,4,&optekrf_debug);
#endif

#if 0
    for (index=0; index<sizeof(dco_8psk2410); index++)
    {
        BTDM_MDM_VERSION_ADDR_CT = div_8psk2410[index] << 16 | dco_8psk2410[index] << 8 | am_8psk2410[index] << 0;
    }

    optekrf_debug = 0x11000000 + (sizeof(dco_8psk2410) - 1);//MAX_ROM_ELN;                     //pop optek dbg ram data                
    rf_icydm_reg_burst_wr(0x524,4,&optekrf_debug);
#endif


    pa_sel = (0x1 << PA_SEL_BIT);
    ant_sel = (0x1 << ANT_SEL_BIT);
    ble_4m = (0x0 << BLE_4M_BIT);                                           //enable ble4m

    //optek_rf_setting[0] = 0x4 | pa_sel | ant_sel | ble_4m;                    //bt 1MHz             
    optek_rf_setting[0] = 0x7 | pa_sel | ant_sel | ble_4m;                      //bt edr 3MHz             
    optek_rf_setting[1] = 8;                       //channel
    optek_rf_setting[2] = 0x40;//0xc0;                     //power, 0.75->optekrf 0db
    optek_rf_setting[3] = 0x10;                     //tx startup

    rf_icydm_reg_burst_wr(0x800,4,&optek_rf_setting[0]);                    //continouse loopback pop optekrf dgb data

	//asm ("waiti 1");

#else
    unsigned char am = 16;//64;
    unsigned char dco = 0;//255;
    unsigned char div = 19;
    //optekrf_debug = 0x20000000 | div << 16 | dco <<0 | am << 8;//for 0506 bin(reverse)
    optekrf_debug = 0x20000000 | div << 16 | dco << 8 | am << 0;//for 0508 bin

    //optekrf_debug[0] = 0x201FFFFF;
    rf_icydm_reg_burst_wr( 0x524, 4, &optekrf_debug);

    
    pa_sel = (0x1 << PA_SEL_BIT);
    ant_sel = (0x1 << ANT_SEL_BIT);
    ble_4m = (0x0 << BLE_4M_BIT);

    mode = 0x7 | pa_sel | ant_sel | ble_4m;
    unsigned char channel = 46;//2448MHz
    //unsigned char channel = 0;//2402MHz
    unsigned char power = 0xc0;
    unsigned char tx_startup = 0x10;
    unsigned char rx_startup = 0x20;                //TODO


    u8 optekrf_cmd [16];

#if ((defined TEST_BOARD_1_TX)||(defined TEST_BOARD_2_TX))
    optekrf_cmd [0] = mode;
    optekrf_cmd [1] = channel;
    optekrf_cmd [2] = power;
    optekrf_cmd [3] = tx_startup;

    rf_icydm_reg_burst_wr( OPTEKRF_RATE_TX_ADD, 4, optekrf_cmd);          //tx
#endif
#if ((defined TEST_BOARD_1_RX)||(defined TEST_BOARD_2_RX))
    optekrf_cmd [0] = mode;
    optekrf_cmd [1] = channel;
    optekrf_cmd [2] = rx_startup;

    rf_icydm_reg_burst_wr(OPTEKRF_RATE_RX_ADD, 3, optekrf_cmd);          //rx
#endif

    while (1);

    delayms(3);
#endif


#endif

    //asm("break 1,1");


    //printf_info("OPTEKRF_TRX_REG_ADDR val: 0x%x\r\n", optek_rf_setting_32b_tx[6]);

#if 0
    printf_warn("start am test\r\n");
    
    for (i=0; i<=255; i++)
    {
        am = i;
        optekrf_debug = 0x20000000 | div << 16 | dco <<0 | am << 8;
        rf_icydm_reg_burst_wr( 0x524, 4, &optekrf_debug);
        printf_info("am: %d\r\n", am);
        delayms(7000);
    }
#endif


    //while(1);

}



/*

#include <string.h>        // for memcpy
//#include "co_utils.h"      // common utility definition
//#include "co_math.h"       // common math functions
//#include "co_endian.h"     // endian definitions
#include "rf.h"            // RF interface
#include "em_map.h"        // RF area mapping

#include "rwip.h"          // for RF API structure definition
#include "reg_ipcore.h"    // DM core registers

#include "plf.h"           // Platform register
#include "flash.h"         // Flash interface
//#include "lcd.h"           // DBG interface
#include "reg_iqgen.h"     // Add DF Generator register h file here

#if (BLE_EMB_PRESENT)
#include "reg_blecore.h"   // ble core registers
#include "reg_em_ble_cs.h" // control structure definitions
#endif //BLE_EMB_PRESENT

#if (BT_EMB_PRESENT)
#include "reg_btcore.h"    // bt core registers
#include "reg_em_bt_cs.h"  // control structure definitions
#endif //BT_EMB_PRESENT
*/

#include "../api/rf.h"            // RF interface
#include "gallifrey_v3a/bt_dm/bt_dm.h"
#include "gallifrey_t22_v1a/bt_dm/bt_dm.h"


#define RF_ICYDM_REG_TBL_18_INIT           GALLIFREY_V3A_MAIN_REGS
#define RF_ICYDM_REG_TBL_18_ADPLL          GALLIFREY_V3A_ADPLL_REGS
#define RF_ICYDM_REG_TBL_18_SEQUENCER      GALLIFREY_V3A_SEQ_CODE
#define RF_ICYDM_REG_TBL_18_TX_PKT_HANDLER GALLIFREY_V3A_TXPH_CODE
#define RF_ICYDM_REG_TBL_18_RX_PKT_HANDLER GALLIFREY_V3A_RXPH_CODE

// icyTRxDM hash table
#define ICYDM_T22_V1A                  0x8779e61c 

 
/* --------------------------- TestChip gallifrey_v3a Init Tables ---------------------------------------------*/

#define RF_ICYDM_REG_TBL_18_INIT           GALLIFREY_V3A_MAIN_REGS
#define RF_ICYDM_REG_TBL_18_ADPLL          GALLIFREY_V3A_ADPLL_REGS
#define RF_ICYDM_REG_TBL_18_SEQUENCER      GALLIFREY_V3A_SEQ_CODE
#define RF_ICYDM_REG_TBL_18_TX_PKT_HANDLER GALLIFREY_V3A_TXPH_CODE
#define RF_ICYDM_REG_TBL_18_RX_PKT_HANDLER GALLIFREY_V3A_RXPH_CODE

__STATIC  const uint8_t RF_ICYDM_REG_TBL_18_GPIOS[] =
{
  0x1d,  0x1a,  0x01,  0x1b,
  0x1c,  0x29,  0x19,  0x16,
  0x17,  0x18
};

__STATIC  const uint8_t RF_ICYDM_REG_TBL_18_AGC[] =
{
    0x6b, 0x3f, 0xe9, 0x92,   0x1f, 0xf9, 0x9a, 0x9f,   0xf9, 0x83, 0x0f, 0xf9,   0x98, 0xcf, 0xf9, 0x82,     // 0x00*
    0x9f, 0xf5, 0x01, 0x0a,   0xc0, 0xd1, 0x2f, 0xf9,   0x81, 0xaf, 0xf9, 0x89,   0x9f, 0xf7, 0x00, 0x3c,     // 0x00*
    0xf8, 0x90, 0x8f, 0xf9,   0x9c, 0x0f, 0xc0, 0xd1,   0x3f, 0xd0, 0x32, 0x2a,   0x8c, 0xfa, 0xfa, 0xf9,     // 0x00*
    0xc8, 0xbf, 0xfd, 0x98,   0xcf, 0xf9, 0xc4, 0x0f,   0xc0, 0x05, 0xaa, 0x87,   0x00, 0x2c, 0xa1, 0xd1,     // 0x00*
    0xaf, 0xaf, 0x8f, 0x0f,   0xdc, 0x1b, 0xf0, 0xdd,   0xf0, 0xf8, 0xf9, 0xe2,   0x08, 0xdd, 0x81, 0x9f,     // 0x00*
    0x87, 0x05, 0xaa, 0x87,   0xd8, 0xcf, 0xe1, 0x0c,   0x0d, 0xc0, 0x18, 0x8e,   0xd0, 0x00, 0x0c, 0xe1,     // 0x00*
    0x00, 0x0d, 0xc0, 0x40,   0x1c, 0xfd, 0x8f, 0xaf,   0xa7, 0xc8, 0xdd, 0x02,   0xdf, 0x8d, 0x8f, 0x9f,     // 0x00*
    0x1f, 0x51, 0x05, 0x8a,   0xfd, 0xd1, 0xff, 0xf8,   0x3a, 0xca, 0xdc, 0x45,   0xf0, 0xdc, 0xfc, 0x18,     // 0x00*
    0x51, 0x09, 0x0a, 0xc0,   0x6a, 0xae, 0xd1, 0x05,   0x0a, 0xc0, 0x7a, 0x41,   0xc0, 0x1a, 0x9d, 0xa0,     // 0x00*
    0x00, 0xac, 0x13, 0x04,   0xac, 0xd1, 0x0d, 0x0a,   0xc0, 0x8a, 0x0f, 0xc0,   0x00, 0x00, 0x00, 0x00,     // 0x00*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x00*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x00*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x00*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x00*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x00*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x0e, 0x00, 0x00, 0x00,   0x00, 0x15, 0x00, 0x00,     // 0x01*
    0x00, 0x00, 0x00, 0x00,   0x1a, 0x29, 0x2a, 0x43,   0x4e, 0x39, 0x00, 0x58,   0x00, 0x00, 0x00, 0x00,     // 0x01*
    0x00, 0x01, 0x00, 0x00,   0x1c, 0x0d, 0x0d, 0x28,   0x0a, 0x00, 0x00, 0x00
};


/* --------------------------- TestChip gallifrey_t22_v1a Init Tables ---------------------------------------------*/

#define RF_ICYDM_REG_TBL_T22V1A_INIT           GALLIFREY_T22_V1A_MAIN_REGS
#define RF_ICYDM_REG_TBL_T22V1A_ADPLL          GALLIFREY_T22_V1A_ADPLL_REGS
#define RF_ICYDM_REG_TBL_T22V1A_SEQUENCER      GALLIFREY_T22_V1A_SEQ_CODE
#define RF_ICYDM_REG_TBL_T22V1A_TX_PKT_HANDLER GALLIFREY_T22_V1A_TXPH_CODE
#define RF_ICYDM_REG_TBL_T22V1A_RX_PKT_HANDLER GALLIFREY_T22_V1A_RXPH_CODE

__STATIC  const uint8_t RF_ICYDM_REG_TBL_T22V1A_GPIOS[] =
{
  0x1d,  0x1a,  0x01,  0x1b,
  0x1c,  0x29,  0x19,  0x16,
  0x17,  0x18
};

__STATIC  const uint8_t RF_ICYDM_REG_TBL_T22V1A_AGC[] =
{
  0x6b, 0x3f, 0xe9, 0x92,   0x1f, 0xf9, 0x9a, 0x9f,   0xf9, 0x83, 0x0f, 0xf8,   0x98, 0xcf, 0xf9, 0x84,     // 0x00*
  0x2f, 0xf8, 0x59, 0x1f,   0xa0, 0x00, 0x1c, 0xfd,   0x92, 0x1f, 0xf8, 0x9a,   0x9f, 0xf8, 0x79, 0x0f,     // 0x00*
  0xc0, 0x83, 0x0f, 0xf9,   0x98, 0xcf, 0xf9, 0x00,   0x1c, 0xfd, 0x43, 0xe1,   0xc0, 0x32, 0x2a, 0x8c,     // 0x00*
  0xfa, 0xfa, 0xf9, 0xc8,   0xbf, 0xfd, 0x98, 0xcf,   0xf9, 0xc4, 0x0f, 0xc0,   0x05, 0xaa, 0x87, 0x00,     // 0x00*
  0x0c, 0xf8, 0x03, 0xfd,   0xc6, 0x54, 0x2c, 0xa1,   0xd1, 0xaf, 0xaf, 0x8f,   0x0f, 0x13, 0x02, 0xcc,     // 0x00*
  0x01, 0xdf, 0x0d, 0xd0,   0x9f, 0x2f, 0x8e, 0x05,   0xaa, 0x87, 0xd8, 0x1f,   0xfd, 0xfa, 0xfa, 0xf8,     // 0x00*
  0x04, 0x2e, 0xc0, 0x71,   0xfc, 0xdd, 0x04, 0xfd,   0xf9, 0x7a, 0x58, 0xa0,   0xd8, 0x1f, 0xfd, 0xfa,     // 0x00*
  0x5a, 0xa0, 0x1c, 0xce,   0xd0, 0x00, 0x8c, 0xe1,   0x08, 0x0d, 0xc0, 0x94,   0x4d, 0x8f, 0x00, 0x0c,     // 0x00*
  0xd9, 0xf0, 0x08, 0xc0,   0x56, 0x1c, 0xfd, 0x8f,   0xaf, 0xa7, 0xc8, 0x3d,   0x03, 0xdf, 0x8d, 0x8f,     // 0x00*
  0x9f, 0x1f, 0x51, 0x05,   0x8a, 0xfd, 0xd1, 0xff,   0xf8, 0x3a, 0xca, 0xdc,   0x5b, 0xf0, 0xdc, 0xfc,     // 0x00*
  0x18, 0x51, 0x09, 0x0a,   0xc0, 0x6a, 0xae, 0xd1,   0x05, 0x0a, 0xc0, 0x7a,   0x41, 0xc0, 0x1a, 0x9d,     // 0x00*
  0xa0, 0x00, 0xac, 0x13,   0x04, 0xac, 0xd1, 0x0d,   0x0a, 0xc0, 0x8a, 0x0f,   0xc0, 0x00, 0x00, 0x00,     // 0x00*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x00*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x00*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x00*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x0f, 0x00, 0x00, 0x00,   0x00, 0x16, 0x00, 0x00,     // 0x01*
  0x00, 0x00, 0x00, 0x00,   0x1b, 0x2f, 0x3d, 0x59,   0x64, 0x49, 0x00, 0x6e,   0x00, 0x00, 0x00, 0x00,     // 0x01*
  0x00, 0x01, 0x00, 0x00,   0x23, 0x10, 0x10, 0x32,   0x0d, 0x00, 0x00, 0x00
};


void hw_csem_rf_test(void)
{
    uint32_t icydm_chip_id = 0;  // Default version is IcyTRXDM
    uint32_t icydm_hash    = 0;  // Default version is IcyTRXDM
    //uint8_t length = PARAM_LEN_RSSI_THR;

#if 1
// --------------- RADIOCNTL0 register ---------------
    // Set pointer SW SPI Drive access Pointer
    ip_radiocntl0_pack( /*uint16_t spiptr*/        (EM_RF_SW_SPI_OFFSET >> 2),
                        /*uint8_t  spicfg*/         0,
                        /*uint8_t  spifreq*/        1,
                        /*uint8_t  spigo*/          0);


// --------------- RADIOCNTL1 register ---------------
    /* RADIOCNTL1 */
    ip_radiocntl1_pack(/*uint8_t  forceagcen*/      0,
                        /*uint8_t  forceiq*/         0,
                        /*uint8_t  rxdnsl*/          0,
                        /*uint8_t  txdnsl*/          0,
                        /*uint16_t forceagclength*/  0x0,
                        /*uint8_t  syncpulsemode*/   1,
                        /*uint8_t  syncpulsesrc*/    1,
                        /*uint8_t  dpcorren*/        0,
                        /*uint8_t  jefselect*/       0,
                        /*uint8_t  xrfsel*/          0x07,
                        /*uint8_t  subversion*/      0x0);

#endif

#if !defined(RP_HWSIM_BYPASS)
    // Detect the RF version
    icydm_chip_id = rf_icydm_reg_rd(0x3FF);
    icydm_hash    = rf_icydm_reg_rd(0x3F8)|(rf_icydm_reg_rd(0x3F9)<<8)|(rf_icydm_reg_rd(0x3FA)<<16)|(rf_icydm_reg_rd(0x3FB)<<24);//ICYDM_T22_V1A;//
#else
    // Stick to v0x18 to prevent ASSERT
    icydm_chip_id = 0x18;
#endif

    // IcyTRx Static Register Initialization
  switch(icydm_chip_id)
  {
  case(0x18):
  // IcyTRxDM Static Register Initialization for TestChip version
    // TX/RX config setting
    {
      unsigned int i,blocks;
      blocks = (sizeof(RF_ICYDM_REG_TBL_18_INIT)-16)>>6;
      for(i=0;i<blocks;i++)
        rf_icydm_reg_burst_wr(i<<6, 64, (uint8_t *) &RF_ICYDM_REG_TBL_18_INIT[i<<6]);
      if(((sizeof(RF_ICYDM_REG_TBL_18_INIT)-16)%64)!=0){
        rf_icydm_reg_burst_wr(i<<6, (sizeof(RF_ICYDM_REG_TBL_18_INIT)-16)%64, (uint8_t *) &RF_ICYDM_REG_TBL_18_INIT[i<<6]);
      }
      rf_icydm_reg_burst_wr(sizeof(RF_ICYDM_REG_TBL_18_INIT)-4,4,(uint8_t *) &RF_ICYDM_REG_TBL_18_INIT[sizeof(RF_ICYDM_REG_TBL_18_INIT)-4]);
    }
       
    // Sequencer
    rf_icydm_reg_burst_wr(0xAD0, sizeof(RF_ICYDM_REG_TBL_18_SEQUENCER), (uint8_t *) &RF_ICYDM_REG_TBL_18_SEQUENCER[0]);

    // TX packet handler
    rf_icydm_reg_burst_wr(0xB50, sizeof(RF_ICYDM_REG_TBL_18_TX_PKT_HANDLER), (uint8_t *) &RF_ICYDM_REG_TBL_18_TX_PKT_HANDLER[0]);

    // RX packet handler
    rf_icydm_reg_burst_wr(0xBD0, sizeof(RF_ICYDM_REG_TBL_18_RX_PKT_HANDLER), (uint8_t *) &RF_ICYDM_REG_TBL_18_RX_PKT_HANDLER[0]);

    // AGC packet handler
    {
      unsigned int i;
      for(i=0;i<(sizeof(RF_ICYDM_REG_TBL_18_AGC)-64);i+=64)
        rf_icydm_reg_burst_wr(0xC50+i, 64, (uint8_t *) &RF_ICYDM_REG_TBL_18_AGC[i]);
      rf_icydm_reg_burst_wr(0xC50+i, sizeof(RF_ICYDM_REG_TBL_18_AGC)%64, (uint8_t *) &RF_ICYDM_REG_TBL_18_AGC[i]);
    }

     // APLL
    rf_icydm_reg_burst_wr(0xE00, sizeof(RF_ICYDM_REG_TBL_18_ADPLL), (uint8_t *) &RF_ICYDM_REG_TBL_18_ADPLL[0]);

    // GPIOS settings
    rf_icydm_reg_burst_wr(0x00A, 10, (uint8_t *) &RF_ICYDM_REG_TBL_18_GPIOS[0]);

    // End Init procedure
    rf_icydm_reg_wr(0x000, 0);
    rf_icydm_reg_wr(0x004, 1);
    rf_calib_adpll_v3a();
    rf_calib_dc_v3a();
    break;

  case(0x20):
    switch(icydm_hash){
      case ICYDM_T22_V1A:
         // IcyTRxDM Static Register Initialization for TestChip version
          // TX/RX config setting
          {
            unsigned int i,blocks;
            blocks = sizeof(RF_ICYDM_REG_TBL_T22V1A_INIT)>>6;
            for(i=0;i<blocks;i++)
              rf_icydm_reg_burst_wr(i<<6, 64, (uint8_t *) &RF_ICYDM_REG_TBL_T22V1A_INIT[i<<6]);
            if((sizeof(RF_ICYDM_REG_TBL_T22V1A_INIT)%64)!=0)
              rf_icydm_reg_burst_wr(i<<6, sizeof(RF_ICYDM_REG_TBL_T22V1A_INIT)%64, (uint8_t *) &RF_ICYDM_REG_TBL_T22V1A_INIT[i<<6]);
          }
             
          // Sequencer
          rf_icydm_reg_burst_wr(0xA00, sizeof(RF_ICYDM_REG_TBL_T22V1A_SEQUENCER), (uint8_t *) &RF_ICYDM_REG_TBL_T22V1A_SEQUENCER[0]);

          // TX packet handler
          rf_icydm_reg_burst_wr(0xA80, sizeof(RF_ICYDM_REG_TBL_T22V1A_TX_PKT_HANDLER), (uint8_t *) &RF_ICYDM_REG_TBL_T22V1A_TX_PKT_HANDLER[0]);

          // RX packet handler
          rf_icydm_reg_burst_wr(0xB40, sizeof(RF_ICYDM_REG_TBL_T22V1A_RX_PKT_HANDLER), (uint8_t *) &RF_ICYDM_REG_TBL_T22V1A_RX_PKT_HANDLER[0]);

          // AGC packet handler
          {
            unsigned int i;
            for(i=0;i<(sizeof(RF_ICYDM_REG_TBL_T22V1A_AGC)-64);i+=64)
              rf_icydm_reg_burst_wr(0xC00+i, 64, (uint8_t *) &RF_ICYDM_REG_TBL_T22V1A_AGC[i]);
            rf_icydm_reg_burst_wr(0xC00+i, sizeof(RF_ICYDM_REG_TBL_T22V1A_AGC)%64, (uint8_t *) &RF_ICYDM_REG_TBL_T22V1A_AGC[i]);
          }

           // APLL
          rf_icydm_reg_burst_wr(0xE00, sizeof(RF_ICYDM_REG_TBL_T22V1A_ADPLL), (uint8_t *) &RF_ICYDM_REG_TBL_T22V1A_ADPLL[0]);

          // GPIOS settings
          rf_icydm_reg_burst_wr(0x00A, 10, (uint8_t *) &RF_ICYDM_REG_TBL_T22V1A_GPIOS[0]);

          // End Init procedure
          rf_icydm_reg_wr(0x001, 0);
          rf_icydm_reg_wr(0x004, 1);
          rf_icydm_reg_wr(0x001, 0);
          rf_icydm_reg_wr(0x004, 1);
          rf_calib_adpll_t22_v1a();
          //rf_calib_dc_v3a();
          break;
      default:
         ASSERT_ERR(0);
         break;
    }
    break;

  default:
    ASSERT_ERR(0);
  break;
  }

   //gpio setting
    /*
    reg name  val     addr  
    gpio_0    0x16     0xA
    gpio_1    0x17     0xB
    gpio_2    0x18     0xC
    gpio_3    0x19     0xD

    gpio_5    0x1A     0xF 
    gpio_6    0x1B     0x10 
    gpio_7    0x1C     0x11
    gpio_8    0x1D     0x12
    gpio_9    0x1E     0x13
    */
    __STATIC  const uint8_t RF_ICYDM_REG_TBL_09_GPIOS[10] =
    {
    #if 1
      0x16,  0x17,  0x1E,  0x19,
      0x00,//no use
      0x1A,  0x1B,  0x1C,  0x1D,
      0x18
    #else      
      0x16,  0x17,  0x18,  0x19,
      0x00,//no use
      0x1A,  0x1B,  0x1C,  0x1D,
      0x1E
    #endif
    };


    rf_icydm_reg_burst_wr(0xA,  sizeof(RF_ICYDM_REG_TBL_09_GPIOS), RF_ICYDM_REG_TBL_09_GPIOS);



#if 0//REGISTER WR/RD TEST

  #define RD_LEN    10

  volatile char test_w_val[RD_LEN] = {0};
  volatile char test_r_val[RD_LEN] = {0};
  
  rf_icydm_reg_burst_rd(0xA, 10, test_r_val);
  
  #if 0//related functions
    rf_icydm_reg_wr(0x300,0);//1 byte per write
    rf_icydm_reg_32bit_wr(0x300,0x162800);//4 bytes per write
    rf_icydm_reg_burst_wr(0x300,  sizeof(ARRAY), (ARRAY));
    rf_icydm_reg_rd(0x300);//1 byte per read
    rf_icydm_reg_burst_rd(0xA, sizeof(ARRAY), (ARRAY));
  #endif

#endif//REGISTER WR/RD TEST

    switch(icydm_chip_id)
    {
		case(0x18):
			rf_icydm_reg_wr(0xe,0x11);//gpio4-> xo clk

			rf_icydm_reg_wr(0x1f2,0x3<<6);
			rf_icydm_reg_wr(0x1f3,0);
			rf_icydm_reg_wr(0x1f4,0x80);
			break;
		case(0x20):
			rf_icydm_reg_wr(0xe,0x11);//gpio4-> xo clk

		  #if 1
            //#define XO_DIV 3//48/3
            #define XO_DIV 4//48/4
            rf_icydm_reg_wr(0x200,XO_DIV&0X3<<6);
			rf_icydm_reg_wr(0x201,XO_DIV>>2);
			rf_icydm_reg_wr(0x202,0x80);
		  #else
			rf_icydm_reg_wr(0x200,0xff);
			rf_icydm_reg_wr(0x201,0xff);
			rf_icydm_reg_wr(0x202,0xff);
		  #endif
			break;
		default:
			break;
    }

    while(0)
    {
        //rf_icydm_reg_wr(0xa, 1);
        //rf_icydm_reg_wr(0xb, 1);
        //rf_icydm_reg_wr(0xc, 1);
        //rf_icydm_reg_wr(0xd, 1);

        rf_icydm_reg_wr(0xe, 1);
        rf_icydm_reg_wr(0xf, 1);
        rf_icydm_reg_wr(0x10, 1);
        rf_icydm_reg_wr(0x11, 1);
        //rf_icydm_reg_wr(0x12, 1);


        uint32_t val = rf_icydm_reg_rd(0xe);

        if (val != 1)
        {
            __asm("break 1,1");
        }

        timer_delayms(100);
        //rf_icydm_reg_wr(0xa, 2);
        //rf_icydm_reg_wr(0xb, 2);
        //rf_icydm_reg_wr(0xc, 2);
        //rf_icydm_reg_wr(0xd, 2);

        rf_icydm_reg_wr(0xe, 2);
        rf_icydm_reg_wr(0xf, 2);
        rf_icydm_reg_wr(0x10, 2);
        rf_icydm_reg_wr(0x11, 2);
        //rf_icydm_reg_wr(0x12, 2);

        val = rf_icydm_reg_rd(0xe);

        if (val != 2)
        {
            __asm("break 1,1");
        }        
        timer_delayms(100);

    }

    //rf_icydm_reg_wr(0xe, 0x12);

    //debug = rf_icydm_reg_rd(0xe);

}

void hw_rf_dcdc_vsel(U32 value)
{
	U32 val;
	AON_CTRL2_ST *reg2_val_cfg = (AON_CTRL2_ST *)&val;

	val = AON_CTRL2_ADDR;
	reg2_val_cfg->btdm_rf_dcdc_vsel = value;
	hw_hardware_aonreg_write((U32 *)&AON_CTRL2_ADDR, val);
}

void rf_ant_sel(U8 tx_sel, U8 rx_sel)
{
    U32 val = 0;
    OPTEKRF_PA_ANT_SEL* reg2_ant_sel = (OPTEKRF_PA_ANT_SEL*)&val;
    reg2_ant_sel->tx_ant = tx_sel;
    reg2_ant_sel->rx_ant = rx_sel;
    //rf_icydm_reg_burst_wr(OPTEKRF_PA_ANT_SEL_ADDR, 4, &val);
    rf_icydm_reg_wr(OPTEKRF_PA_ANT_SEL_ADDR, val);
}
#endif
