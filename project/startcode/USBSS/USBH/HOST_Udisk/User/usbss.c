/********************************** (C) COPYRIGHT *******************************
* File Name          : usbss.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 :
*******************************************************************************/

#include "CH56xSFR.h"
#include "core_riscv.h"
#include "usbss.h"

/*******************************************************************************
* Function Name  : usbssh_lmp_init
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void usbssh_lmp_init( )
{
	UINT8 	rx_lmp_complete;

	// LMP, TX PORT_CAP & RX PORT_CAP
	USBSS->LMP_TX_DATA0 = LINK_SPEED | PORT_CAP | LMP_HP;
	USBSS->LMP_TX_DATA1 = DOWN_STREAM | NUM_HP_BUF;
	USBSS->LMP_TX_DATA2 = 0x0;

	rx_lmp_complete = 0;
	while( !rx_lmp_complete )
	{
		if( USBSSH->USB_STATUS & USB_LMP_RX_FLAG )
		{
			USBSSH->USB_STATUS = USB_LMP_RX_FLAG; // clear int flag
			rx_lmp_complete = 1;
		}
	}
	// LMP, TX PORT_CFG & RX PORT_CFG_RES
	USBSS->LMP_TX_DATA0 = LINK_SPEED | PORT_CFG | LMP_HP;
	USBSS->LMP_TX_DATA1 = 0x0;
	USBSS->LMP_TX_DATA2 = 0x0;

	rx_lmp_complete = 0;
	while( !rx_lmp_complete )
	{
		if( USBSSH->USB_STATUS & USB_LMP_RX_FLAG )
		{
			USBSSH->USB_STATUS = USB_LMP_RX_FLAG; // clear int flag
			rx_lmp_complete = 1;
		}
	}
	// TX PING-TP, for test
	USBSS->LMP_TX_DATA0 = TP_HP;
	USBSS->LMP_TX_DATA1 = PING_ENDP | PING_TP;
	USBSS->LMP_TX_DATA2 = 0x0;
}


/*******************************************************************************
* Function Name  : set_device_address
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void set_device_address( UINT32 address )
{
	USBSS->USB_CONTROL &= 0x00ffffff;
	USBSS->USB_CONTROL |= address<<24; // SET ADDRESS
}

/*******************************************************************************
* Function Name  : switch_pwr_mode
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void switch_pwr_mode( UINT8 pwr_mode )
{
	UINT32 temp;
	temp = USBSS->LINK_CTRL;
	temp &= ~PM_MASK;
	temp |= pwr_mode;
	USBSS->LINK_CTRL = temp;
	while( USBSS->LINK_STATUS & LINK_BUSY );			// wait power mode switch done
}



