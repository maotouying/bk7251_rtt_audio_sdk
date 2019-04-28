#ifndef _I2S_PUB_H_
#define _I2S_PUB_H_

#define I2S_FAILURE                (1)
#define I2S_SUCCESS                (0)

#define I2S_DEV_NAME                "i2s"

#define I2S_CMD_MAGIC              (0xe280000)
enum
{
	I2S_CMD_UNIT_ENABLE = I2S_CMD_MAGIC + 1,
	I2S_CMD_SET_MSTEN,
	I2S_CMD_SELECT_MODE,
	I2S_CMD_SET_LRCK,
	I2S_CMD_SET_SCK_INV,
	I2S_CMD_SET_SCK_LSB,
	I2S_CMD_SET_SCK_SYNCLEN,
	I2S_CMD_SET_PCM_DLEN,	
	I2S_CMD_SET_FREQ_DATAWIDTH,
    I2S_CMD_RXINT_EN,
    I2S_CMD_TXINT_EN,
    I2S_CMD_RXOVR_EN,
    I2S_CMD_TXOVR_EN,
    I2S_CMD_RXFIFO_CLR_EN,
    I2S_CMD_TXFIFO_CLR_EN,
    I2S_CMD_RXINT_MODE,
    I2S_CMD_TXINT_MODE,
    I2S_CMD_GET_BUSY,
    I2S_CMD_ENABLE_INTERRUPT,
    I2S_CMD_DISABLE_INTERRUPT,
    I2S_CMD_MASTER_ENABLE,
    I2S_CMD_SLAVE_ENABLE,
    I2S_CMD_DISABLE_I2S,
};


#define I2S_BIT_DEBUG
#ifdef I2S_BIT_DEBUG
#define bit_dbg(fmt, ...)   rt_kprintf(fmt, ##__VA_ARGS__)
#else
#define bit_dbg(fmt, ...)
#endif

#define TX_FINISH_FLAG              (1 << 31)
#define I2S_SA
enum
{
    I2S_MODE_I2S = 0,
    I2S_MODE_LEFT_JUST = 1,
    I2S_MODE_RIGHT_JUST = 2,
    I2S_MODE_SHORT_SYNC = 4,
    I2S_MODE_LONG_SYNC = 5,
    I2S_MODE_NORMAL_2B_D = 6,
    I2S_MODE_DELAY_2B_D = 7
};
struct i2s_message
{
	UINT32 *send_buf;
	UINT32 send_len;
		
	UINT32 *recv_buf;
	UINT32 recv_len;
};

typedef struct
{
    UINT32 *p_tx_buf;
    UINT32 *p_rx_buf;
    UINT32 trans_done;
	volatile UINT32 tx_remain_data_cnt;
    volatile UINT32 rx_remain_data_cnt;
} i2s_trans_t;

typedef struct i2s_configuration
{
	UINT32 mode;
/*
		mdoe:
		bit0~2: 	mode & 0x7		000:I2S
									001:Left Justified
									010:Right Justified
									011:reserve
									100:Short Frame Sync
									101:Long Frame Sync
									110:Normal 2B+D
									111:Delay 2B+D 
		
		bit3:		mode & 0x08 	0:LRCK	no turn
									1:LRCK	turn
	
		bit4:		mode & 0x10 	0:SCK	no turn
									1:SCK	turn	
							
		bit5:		mode & 0x20 	0:MSB	first send/receive
									1:LSB	first send/receive
	
		bit8~10:	mode & 0x700	0~7:Sync length only Long Frame Sync effective
	
		bit12~14:	mode & 0x7000	0~7: 2B+D PCM ：D length
							
*/

	
	UINT32 fifo_level;
	UINT32 sample_rate;
	UINT32 bits_per_sample;
};


typedef struct
{
    UINT8 rx_level;
    UINT8 tx_level;
} i2s_level_t;

typedef struct
{
    UINT32 freq;
    UINT32 datawidth;
} i2s_rate_t;

/*******************************************************************************
* Function Declarations
*******************************************************************************/
void i2s_init(void);
void i2s_exit(void);
void i2s_isr(void);
UINT8 is_i2s_active(void);
#endif //_I2S_PUB_H_
