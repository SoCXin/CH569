/********************************** (C) COPYRIGHT *******************************
* File Name          : Main.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 : 
*******************************************************************************/

#include "CH56x_common.h"
#include "usb30_porp.H"
#define	UART1_BAUD	921600

void USBSS_IRQHandler (void) __attribute__((interrupt("WCH-Interrupt-fast")));			//TMR0 interrupt service
void LINK_IRQHandler (void) __attribute__((interrupt("WCH-Interrupt-fast")));		//TMR1 interrupt service
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

/* ÅäÖÃ´®¿Úµ÷ÊÔ */
	DebugInit(UART1_BAUD);
	PRINT("UVC Program(80MHz) !\n");

	PRINT("go here\n");
	R32_USB_CONTROL = 0;
	USB30_init();

	PFIC_EnableIRQ(USBSS_IRQn);
	PFIC_EnableIRQ(LINK_IRQn);
	while(1);
}

/*******************************************************************************
* Function Name  : LINK_IRQHandler
* Description    : USB3.0 Link Interrupt Handler.
* Input          : None
* Return         : None
*******************************************************************************/
void LINK_IRQHandler (void)	          //USBSS link interrupt service
{
	USB30_linkIRQHandler();
}

/*******************************************************************************
* Function Name  : USBSS_IRQHandler
* Description    : USB3.0 Interrupt Handler.
* Input          : None
* Return         : None
*******************************************************************************/
void USBSS_IRQHandler (void)			//USBSS interrupt service
{
	USB30_usbssIRQHandler();
}
