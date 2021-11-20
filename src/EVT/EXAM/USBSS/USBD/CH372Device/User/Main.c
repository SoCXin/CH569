/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.c
* Author             : WCH
* Version            : V1.1
* Date               : 2020/12/23
* Description 		 : 
*******************************************************************************/
#include "CH56x_common.h"
#include "CH56xusb30_lib.h"
#include "CH56x_usb30.h"

/* Global define */
#define UART1_BAUD  921600

/* Global Variable */



/* Function declaration */



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
    UINT32 i = 0,j ;
    UINT8 val = 1;

    SystemInit(FREQ_SYS);
	Delay_Init(FREQ_SYS);

/* 配置串口调试 */
	DebugInit(UART1_BAUD);
	PRINT("CH56x USB3.0 & USB2.0 device test(120MHz) !\n");

//USB初始化
    R32_USB_CONTROL = 0;
    PFIC_EnableIRQ(USBSS_IRQn);
    PFIC_EnableIRQ(LINK_IRQn);
    PFIC_EnableIRQ(TMR0_IRQn);
    R8_TMR0_INTER_EN = 1;
    TMR0_TimerInit( 67000000 );   //约0.5秒
	USB30D_init(ENABLE);          //USB3.0初始化，初始化之前确保USB3.0两个中断使能

	while(1){
	    ;
	}
}
