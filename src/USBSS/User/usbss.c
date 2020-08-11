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
* Function Name  : send_setup_packet
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void send_setup_packet( UINT32 tx_len )
{
	UINT8 tx_success;
	UINT8 wait_erdy;
	tx_success =0 ;
	wait_erdy =0;
	USBSS->HOST_STATUS = 0;// set ENDP NUM = 0

	USBSSH->UH_TX_CTRL = UH_TX_SETUP | UH_RTX_VALID | USB_NUMP_1 | tx_len; // tx SETUP
	tx_success = 0;
	while(!tx_success)
	{
		if(USBSSH->USB_STATUS & USB_ACT_FLAG)
		{
			if( (USBSSH->USB_STATUS & USB_INT_RES_MASK) == USB_RES_ACK )
			{
				tx_success = 1;
				USBSSH->UH_TX_CTRL = 0x0;
				USBSSH->USB_STATUS = USB_ACT_FLAG;
				 printf("setup is complete !\n");
			}
		}
	}
}

/*******************************************************************************
* Function Name  : send_status_packet
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void send_status_packet()
{
	UINT8 tx_success;
	tx_success = 0;
	USBSS->HOST_STATUS = 0;// set ENDP NUM = 0

	USBSSH->UH_TX_CTRL = UH_TX_STATUS | UH_RTX_VALID | USB_NUMP_1;// tx status-TP
	while( !tx_success ) // wait STATUS-TP complete
	{
		if( USBSSH->USB_STATUS & USB_ACT_FLAG ) //
		{
			if( (USBSSH->USB_STATUS & USB_INT_RES_MASK) == USB_RES_ACK )
			{
				tx_success = 1;
				USBSSH->UH_TX_CTRL = 0x0;
				USBSSH->USB_STATUS = USB_ACT_FLAG;
				 printf("status is complete !\n");
			}
			else if( (USBSSH->USB_STATUS & USB_INT_RES_MASK) == USB_RES_NRDY )
			{
				USBSSH->UH_TX_CTRL = 0x0;
				USBSSH->USB_STATUS = USB_ACT_FLAG;
				 printf("status stage: status is Nrdy !\n");
			}
			else if( (USBSSH->USB_STATUS & USB_INT_RES_MASK) == USB_RES_STALL )
			{
				tx_success = 1;
				USBSSH->UH_TX_CTRL = 0x0;
				USBSSH->USB_STATUS = USB_ACT_FLAG;
				 printf("status stage: status is stall !\n");
			}
			else
			{
				USBSSH->USB_STATUS = USB_ACT_FLAG;
			}
		}
		else if(USBSSH->USB_STATUS & USB_ERDY_FLAG)
		{
			USBSSH->UH_TX_CTRL = UH_TX_STATUS | UH_RTX_VALID | USB_NUMP_1;// tx status-TP
			USBSSH->USB_STATUS = USB_ERDY_FLAG;
			 printf("status stage: device is Erdy !\n");
		}
	}
}

/*******************************************************************************
* Function Name  : get_data
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void get_data( UINT32 seq_num, UINT32 packet_num, UINT32 endp_num )
{
	UINT32 temp, i,s;
	UINT8 wait_erdy;
	UINT16 timeout;
	UINT8  *p8;
	UINT8 tx_success;
	tx_success = 0;
	wait_erdy = 0;
	timeout=0;
	USBSS->HOST_STATUS = endp_num;// set ENDP NUM  0
	while((USBSS->HOST_STATUS & (3<<18)));
	USBSSH->UH_RX_CTRL = seq_num <<21 | UH_RTX_VALID | packet_num<<16; // ACK-TP(IN)

	while(!tx_success)
	{
		s = USBSSH->USB_STATUS;

		if(s & USB_ACT_FLAG)
		{
			if( (USBSSH->USB_STATUS & USB_INT_RES_MASK) == USB_RES_ACK )
			{
				tx_success = 1;

				USBSSH->UH_RX_CTRL = 0x0;
				USBSSH->USB_STATUS = USB_ACT_FLAG;

				temp = USBSS->USB_RX_LEN;

				printf(" get data is ok !\n");
			}
			else if( (USBSSH->USB_STATUS & USB_INT_RES_MASK) == USB_RES_NRDY )
			{
				printf("nrdy.....\n");
				USBSSH->UH_RX_CTRL = 0x0;
				USBSSH->USB_STATUS = USB_ACT_FLAG;
				wait_erdy = 1;
			}
			else if( (USBSSH->USB_STATUS & USB_INT_RES_MASK) == USB_RES_STALL )//?????清除状态是否有问题？？？
			{
				USBSSH->USB_STATUS = USB_ACT_FLAG;
			}
			else
			{
				USBSSH->USB_STATUS = USB_ACT_FLAG;
			}
		}
		else if( ((USBSSH->USB_STATUS & USB_ERDY_FLAG)  ))
		{
			USBSSH->UH_RX_CTRL = seq_num <<21 | UH_RTX_VALID | packet_num<<16; // ACK-TP(IN)
			USBSSH->USB_STATUS = USB_ERDY_FLAG;
			if( wait_erdy  ){
				printf("11\n");
			}
			else{
				printf("erdy !\n");
			}
		}
	}
}

/*******************************************************************************
* Function Name  : send_data
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void send_data( UINT32 seq_num, UINT32 packet_num, UINT32 endp_num, UINT32 tx_len )
{
	UINT8 tx_success;
	UINT8 wait_erdy;
	UINT16 timeout;
	tx_success = 0;
	wait_erdy =0;
	timeout=0;
	USBSS->HOST_STATUS = endp_num;// set ENDP NUM

	USBSSH->UH_TX_CTRL =  seq_num <<21 | UH_RTX_VALID | packet_num<<16 | tx_len;

	while(!tx_success )
	{
		if( USBSSH->USB_STATUS & USB_ACT_FLAG ) //
		{
			if( (USBSSH->USB_STATUS & USB_INT_RES_MASK) == USB_RES_ACK )
			{
				tx_success = 1;
				USBSSH->UH_TX_CTRL = 0x0;
				USBSSH->USB_STATUS = USB_ACT_FLAG;
			}
			else if( (USBSSH->USB_STATUS & USB_INT_RES_MASK) == USB_RES_NRDY )
			{
				USBSSH->UH_TX_CTRL = 0x0;
				USBSSH->USB_STATUS = USB_ACT_FLAG;

			}
			else if( (USBSSH->USB_STATUS & USB_INT_RES_MASK) == USB_RES_STALL )
			{
				USBSSH->USB_STATUS = USB_ACT_FLAG;
			}
			else
			{
				USBSSH->USB_STATUS = USB_ACT_FLAG;
			}

		}
		else if( ((USBSSH->USB_STATUS & USB_ERDY_FLAG) || ( timeout == 200 ) ) & wait_erdy)
		{
			USBSSH->UH_TX_CTRL =  seq_num <<21 | UH_RTX_VALID | packet_num<<16 | tx_len;
			USBSSH->USB_STATUS = USB_ERDY_FLAG;
		}
		else if( timeout == 200 & !wait_erdy)
		{
			wait_erdy = 1;
			timeout=0;
		}
		else
		{
			timeout++;
		}
	}
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



