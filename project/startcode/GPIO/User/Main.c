/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 : 
*******************************************************************************/

#include "CH56x_common.h"

#define  FREQ_SYS   80000000

void GPIO_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

#define normal 1     //普通输入输出
//#define interruption 1    //中断使能


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
	SystemInit(FREQ_SYS);
	Delay_Init(FREQ_SYS);

/* 配置串口调试 */
	DebugInit(115200);
	PRINT("Start @ChipID=%02X\r\n", R8_CHIP_ID );

	#if normal                                         //通用输入输出
	while(1)
	{
		GPIOB_ModeCfg( GPIO_Pin_5, GPIO_Slowascent_PP_8mA );
		mDelaymS(1000);
		GPIOB_ModeCfg( GPIO_Pin_5, GPIO_Slowascent_PP_16mA );
		mDelaymS(2000);
	}
	#endif


	#if interruption                                     //中断配置
	GPIOB_ITModeCfg( 4, GPIO_ITMode_RiseEdge );
	R32_PB_PD = (1<<4);
	R8_GPIO_INT_FLAG = 0xff;
	PRINT("R8_GPIO_INT_MODE=%x\r\n",R8_GPIO_INT_MODE);
	PRINT("R8_GPIO_INT_POLAR=%x\r\n",R8_GPIO_INT_POLAR);
	PRINT("R8_GPIO_INT_ENABLE=%x\r\n",R8_GPIO_INT_ENABLE);
	PRINT("R8_GPIO_INT_FLAG=%x\r\n",R8_GPIO_INT_FLAG);
	PFIC_EnableIRQ(GPIO_IRQn);
	while(1)
	{
		mDelaymS(1);
	}
	#endif

	while(1);

}



/*******************************************************************************
* Function Name  : GPIO_IRQHandler
* Description    : Interruption function
* Input          : None
* Return         : None
*******************************************************************************/
void GPIO_IRQHandler(void)
{
	PRINT("in!\r\n");
	if(GPIOB_4_ReadITFlagBit(  ))
	{
		PRINT(" get a interruption!\r\n");
	}
	GPIOB_4_ClearITFlagBit(  );
}
