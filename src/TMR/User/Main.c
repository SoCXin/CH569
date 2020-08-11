/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 : 
*******************************************************************************/
#include "CH56x_common.h"

#define  FREQ_SYS   80000000


__attribute__ ((aligned(4))) UINT32 CapBuf[100];
volatile UINT8 capFlag = 0;


#define count 1    //开启计数功能
//#define pwm   1    //开启脉冲调制波输出功能
//#define capture 1    //开启捕获功能

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

	UINT8 i;


/************************************************************
 * TMR1定时器PWM输出
 *****/
#if count
	GPIOA_SetBits(GPIO_Pin_9);
	GPIOA_ModeCfg( GPIO_Pin_9 , GPIO_Slowascent_PP_8mA );
	TMR0_TimerInit(40000000);

	TMR0_ITCfg( ENABLE ,RB_TMR_IE_CYC_END);
	PFIC_EnableIRQ( TMR0_IRQn );

#endif

/************************************************************
 * TMR1定时器PWM输出
 *****/
#if pwm
	 //GPIOPinRemap(ENABLE,RB_PIN_TMR1);  //使能TMR1重映射功能
	 GPIOA_ResetBits(GPIO_Pin_4);
	 GPIOA_ModeCfg(GPIO_Pin_4,GPIO_Slowascent_PP_8mA);

	 TMR2_PWMInit( high_on_low , PWM_Times_1 );
	 TMR2_PWMCycleCfg(40000);
	 TMR2_PWMActDataWidth(20000);
#endif

/************************************************************
 * TMR2定时器DMA捕获
 *****/
#if capture
	 GPIOA_ResetBits(GPIO_Pin_4);
	 GPIOA_ModeCfg( GPIO_Pin_4, GPIO_ModeIN_PU_NSMT );

	 TMR2_CapInit( Edge_To_Edge );
	 TMR2_CAPTimeoutCfg( 67108863 );
	 TMR2_DMACfg( ENABLE, &CapBuf[0], &CapBuf[99], Mode_Single );
	 TMR2_ITCfg( ENABLE , RB_TMR_IE_DMA_END);
	 PFIC_EnableIRQ(TMR2_IRQn);

	 while( capFlag == 0 );
	 capFlag = 0;
	 for( i=0; i<100; i++ )
	 {
		PRINT("%08ld ", CapBuf[i]&0x1ffffff);      // bit26 最高位表示 高电平还是低电平
	 }PRINT("\n");
#endif

	while(1);
}


/*******************************************************************************
* Function Name  : TMR0_IRQHandler
* Description    : Interruption function
* Input          : None
* Return         : None
*******************************************************************************/
void TMR0_IRQHandler(void)
{
	 if( TMR0_GetITFlag( RB_TMR_IE_CYC_END ) )
	 {
		PRINT("counting done!\r\n");
	    TMR0_ClearITFlag( RB_TMR_IE_CYC_END );      // 清除中断标志
	    GPIOA_InverseBits( GPIO_Pin_9 );
	 }
}

/*******************************************************************************
* Function Name  : TMR2_IRQHandler
* Description    : Interruption function
* Input          : None
* Return         : None
*******************************************************************************/
void TMR2_IRQHandler(void)
{
	PRINT("in Capture!\r\n");
	if( R8_TMR2_INT_FLAG & RB_TMR_IF_DMA_END  )
	{
		R8_TMR2_INTER_EN &= ~RB_TMR_IE_DMA_END;       // 使用单次DMA功能+中断，注意完成后关闭此中断使能，否则会一直上报中断。
		R8_TMR2_INT_FLAG &= ~RB_TMR_IF_DMA_END;      // 清除中断标志
		capFlag = 1;
		PRINT("*");
	}
}
