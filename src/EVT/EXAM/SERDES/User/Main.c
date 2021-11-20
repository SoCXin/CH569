/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 : 
*******************************************************************************/
#define  FREQ_SYS   80000000
#include "CH56x_common.h"
#include "serdes.h"


//#define Trans 1
#define Recei 1

UINT32  Tx_DMAaddr   = 0x20020000;
UINT32  RX_DMA0_addr = 0x20033330;
UINT32  RX_DMA1_addr = 0x20034330;

volatile UINT32  i=0;
volatile UINT32  k=0;
/*******************************************************************************
* Function Name  : __attribute__((interrupt()))
* Description    : SERDES_IRQHandler hard push
* Input          : baudrate: UART1 communication baud rate.
* Return         : None
*******************************************************************************/
void SERDES_IRQHandler (void) __attribute__((interrupt()));

/*******************************************************************************
* Function Name  : DebugInit
* Description    : Initializes the UART1 peripheral.
* Input          : baudrate: UART1 communication baud rate.
* Return         : None
*******************************************************************************/
void DebugInit(UINT32 baudrate)
{
	UINT32 x;
	UINT32 t = FREQ_SYS;
	
	x = 10 * t * 2 / 16 / baudrate;
	x = ( x + 5 ) / 10;
	R8_UART1_DIV = 1;
	R16_UART1_DL = x;
	R8_UART1_FCR = RB_FCR_FIFO_TRIG | RB_FCR_TX_FIFO_CLR | RB_FCR_RX_FIFO_CLR | RB_FCR_FIFO_EN;
	R8_UART1_LCR = RB_LCR_WORD_SZ;
	R8_UART1_IER = RB_IER_TXD_EN;
	R32_PA_SMT |= (1<<8) |(1<<7);
	R32_PA_DIR |= (1<<8);
}

/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Return         : None
*******************************************************************************/
int main()
{
	UINT32 *p32_txdma = (UINT32 *)Tx_DMAaddr;
	UINT32 n=0;
	UINT32 data=0;

	SystemInit(FREQ_SYS);
	Delay_Init(FREQ_SYS);

/* 调试串口初始化 */
	DebugInit(115200);
	PRINT("Start @ChipID=%02X\r\n", R8_CHIP_ID );


#if Trans      // serdes查询发送

	Serdes_Tx_Init();

    do
    {
        *p32_txdma++ = data;
        data += 0x01010101;
        n++;
    }while(n!=512);
	mDelaymS(500);

	DMA_Tx_CFG( Tx_DMAaddr,2048,0x555555);               //512个地址空间   2048个字节

    while(1)
    {

        DMA_Tx();
        Wait_Txdone();
        ClearITFlag(SDS_TX_INT_FLG);
        mDelaymS(5000);
    }

#endif

#if Recei       // serdes中断接收
	PFIC_EnableIRQ(INT_ID_SERDES);

	DoubleDMA_Rx_CFG(RX_DMA0_addr ,RX_DMA1_addr );
	Serdes_Rx_Init();
    Wait_commadone();
    Serdes_Rx_Init();
	EnableIT(SDS_RX_INT_EN|SDS_COMMA_INT_EN);

    while(1)
    {
        if(k==2)                                                    //每完成一次双缓冲接收，打印接收到的数据
        {
            for(i=0;i<512;i++)
            {
                printf("%x\r\n",*(UINT32 *)(0x20033330+4*i));
                printf("%x\r\n",*(UINT32 *)(0x20034330+4*i));
            }
            k=0;
        }
    }
#endif

}


/*******************************************************************************
* Function Name  : SERDES_IRQHandler
* Description    : Interruption function
* Input          : None
* Return         : None
*******************************************************************************/
void SERDES_IRQHandler (void)
{
	if(ReadITFlagBit(SDS_RX_INT_FLG))
	{
		ClearITFlag(SDS_RX_INT_FLG);
		k++;
		PRINT("\n buffer received ....... !\n");
	}
	if(ReadITFlagBit(SDS_COMMA_INT_FLG))
	{
		PRINT("\n receive COMMA, init....... !\n");
		Serdes_Rx_Init();
		i = 0;
		mDelaymS(50);
		ClearITFlag(SDS_COMMA_INT_FLG);
	}
}

