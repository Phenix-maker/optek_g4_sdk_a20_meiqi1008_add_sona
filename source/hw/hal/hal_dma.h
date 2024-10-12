#ifndef __HAL_DMA_H__
#define __HAL_DMA_H__



/*
 ****************************************************************************************
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
/**
 * @enum enumDMA_ID
 * @brief DMA connection is avalible from 0 to 9
 */
typedef enum {
	DMA_ID_0 = 0,
	DMA_ID_1 = 1,
	DMA_ID_2 = 2,
	DMA_ID_3 = 3,
	DMA_ID_4 = 4,
	DMA_ID_5 = 5,
	DMA_ID_6 = 6,
	DMA_ID_7 = 7,
	DMA_ID_8 = 8,
	DMA_ID_9 = 9,
	DMA_ID_MAX
} enumDMA_ID;


/**
 * @enum enumDMA_TRIG
 * @brief DMA hardware connection
 */
typedef enum {	
	SOURCE_DMA_UNKNOWN = 0,
	SOURCE_DMA_MI2S_IN = 1,
	SOURCE_DMA_ADC = 2,
	SOURCE_DMA_IIS_ST_IN = 3,
	SOURCE_DMA_DA_PP = 4,
	SOURCE_DMA_SPDIF_IN = 5,
	SOURCE_DMA_IIS_ST_OUT = 6,
	SOURCE_DMA_SD_WR = 7,
	SOURCE_DMA_SD_RD = 8,
	SOURCE_DMA_UART_0_TX = 9,
	SOURCE_DMA_UART_0_RX = 10,
	SOURCE_DMA_SPI_TX = 13,
	SOURCE_DMA_DIGIT_MIC_IN = 14,
	SOURCE_DMA_SPI_RX = 15,
	SOURCE_DMA_MAX
} enumDMA_TRIG;


/**
 * @enum enumDMA_TRANS_BYTEs
 * @brief DMA hardware transfer unit
 */
typedef enum {
	DMA_TRANS_1BYTE = 0,
	DMA_TRANS_2BYTEs = 1,
	DMA_TRANS_4BYTEs = 2,
	DMA_TRANS_BYTEs_MAX
} enumDMA_TRANS_BYTEs;


/**
 * @enum enumDMA_TRANS_BYTEs
 * @brief DMA hardware transfer burst size
 */
typedef enum {
	DMA_BURST_SINGLE = 0,
	DMA_BURST_FOUR = 3,
	DMA_BURST_EIGHT = 5,
	DMA_BURST_SIXTEEN = 7,
	DMA_TRANS_BURST_MAX
} enumDMA_TRANS_BURST_SIZE;


/**
 * @struct DMA_subCNTR
 * @brief DMA sub control register
 */
typedef struct {
	u32 size         :3;
	u32 burst_size   :3;
	u32 saddr_inc    :1;
	u32 daddr_inc    :1;
	u32 source       :4;
	u32 start        :1;
	u32 channel_enb  :1;
	u32 int_enb      :1;
} DMA_subCNTR;


/**
 * @struct DMA_CNTRs
 * @brief DMA control register
 */
typedef volatile struct {
	U32 source;
	U32 res1;
	U32 dest;
	U32 res2;
	U32 count;
	U32 res3;
	DMA_subCNTR sub_cntr;
} DMA_CNTRs;


/**
 ****************************************************************************************
 * CONSTANT DEFINITIONS
 ****************************************************************************************
 */
#define AUDIO_OUTPUT_DMA_ID     DMA_ID_0
#define HDMI_IN_RX_DMA_ID       DMA_ID_1
#define SPDIF_IN_RX_DMA_ID      DMA_ID_1
#define ARC_IN_RX_DMA_ID        DMA_ID_1
#define AUX_IN_RX_DMA_ID        DMA_ID_1
#define USB_IN_DMA_ID			DMA_ID_2

#define UART0_RX_DMA_ID         DMA_ID_4
#define UART0_TX_DMA_ID         DMA_ID_5

#define SPI_TX_DMA_ID           DMA_ID_6
#define SPI_RX_DMA_ID           DMA_ID_7

#define DMA_ADDR_FIXED         	0
#define DMA_ADDR_INC           	1

#define DMA_CHANNEL(ID) 		(1<<ID)



#define DMA_TRANSMIT_SIZE_BYTE     		0
#define DMA_TRANSMIT_SIZE_HALFWORD 		1
#define DMA_TRANSMIT_SIZE_WORD     		2

#define DMA_SRC_ADDR_FIXED         0
#define DMA_SRC_ADDR_INC           1

#define DMA_DEST_ADDR_FIXED        0
#define DMA_DEST_ADDR_INC          1

//I2S
#define SOURCE_DMA0	    		0
#define SOURCE_DMA1     		1
#define SOURCE_DMA2     		2	
#define SOURCE_DMA3     		3

//CLASS D
#define SOURCE_DMA4     		4
#define SOURCE_DMA_CLASSD     	4



#define DMA_COUNT              	10
#define DMA_CNTRs_SIZE         	32
#define DMA_CONFIG_BASE_ADDR   	0x12000000
#define DMA_GCNTR              	REG(DMA_CONFIG_BASE_ADDR + 32 * 12)

#define DMA_0_SOURCE		   	REG(DMA_CONFIG_BASE_ADDR + 32 * 0)
#define DMA_0_DEST			   	REG(DMA_CONFIG_BASE_ADDR + 32 * 0 + 8)
#define DMA_0_COUNT			   	REG(DMA_CONFIG_BASE_ADDR + 32 * 0 + 16)
#define DMA_0_CNTR			   	REG(DMA_CONFIG_BASE_ADDR + 32 * 0 + 24)

#define DMA_1_SOURCE		   	REG(DMA_CONFIG_BASE_ADDR + 32 * 1)
#define DMA_1_DEST			   	REG(DMA_CONFIG_BASE_ADDR + 32 * 1 + 8)
#define DMA_1_COUNT			   	REG(DMA_CONFIG_BASE_ADDR + 32 * 1 + 16)
#define DMA_1_CNTR			   	REG(DMA_CONFIG_BASE_ADDR + 32 * 1 + 24)

#define DMA_4_SOURCE			REG(DMA_CONFIG_BASE_ADDR + 32 * 4)
#define DMA_4_DEST				REG(DMA_CONFIG_BASE_ADDR + 32 * 4 + 8)
#define DMA_4_COUNT				REG(DMA_CONFIG_BASE_ADDR + 32 * 4 + 16)
#define DMA_4_CNTR				REG(DMA_CONFIG_BASE_ADDR+ 32 * 4 + 24)

#define DMA_5_SOURCE			REG(DMA_CONFIG_BASE_ADDR + 32 * 5)
#define DMA_5_DEST				REG(DMA_CONFIG_BASE_ADDR + 32 * 5 + 8)
#define DMA_5_COUNT				REG(DMA_CONFIG_BASE_ADDR + 32 * 5 + 16)
#define DMA_5_CNTR				REG(DMA_CONFIG_BASE_ADDR+ 32 * 5 + 24)

#define DMA_7_SOURCE			REG(DMA_CONFIG_BASE_ADDR + 32 * 7)
#define DMA_7_DEST				REG(DMA_CONFIG_BASE_ADDR + 32 * 7 + 8)
#define DMA_7_COUNT				REG(DMA_CONFIG_BASE_ADDR + 32 * 7 + 16)
#define DMA_7_CNTR				REG(DMA_CONFIG_BASE_ADDR+ 32 * 7 + 24)


/*
 ****************************************************************************************
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
typedef void (*dma_done_callback) (void);

void hal_dma_isr(void *param);

void hal_dma_module_init(void);


void hal_dma_transfer_init(enumDMA_ID dma_id, U32 *dst, U32 *src, enumDMA_TRIG dma_trig, enumDMA_TRANS_BYTEs size,
							enumDMA_TRANS_BURST_SIZE b_size, BOOL src_inc, BOOL dst_inc, BOOL int_enb, dma_done_callback callback);

void hal_dma_close(enumDMA_ID dma_id);

static __inline__ void hal_dma_send_data(enumDMA_ID dma_id, U32 *buf, U32 size)
{
	//sanity
	DBG_Assert(dma_id < DMA_ID_MAX);
	DBG_Assert(buf != NULL);

	DMA_CNTRs *dma_cntr = (DMA_CNTRs *) (DMA_CONFIG_BASE_ADDR + dma_id*DMA_CNTRs_SIZE);

	dma_cntr->source = buf;
	dma_cntr->count = size;
}

static __inline__ void hal_dma_receive_data(enumDMA_ID dma_id, U32 *buf, U32 size)
{
	//sanity
	DBG_Assert(dma_id < DMA_ID_MAX);
	DBG_Assert(buf != NULL);

	DMA_CNTRs *dma_cntr = (DMA_CNTRs *) (DMA_CONFIG_BASE_ADDR + dma_id*DMA_CNTRs_SIZE);

	dma_cntr->dest = buf;
	dma_cntr->count = size;
}


#endif //__HAL_DMA_H__
