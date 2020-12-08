#include "CH56x_common.h"
#include "usbhs.h"

void u2_setup_packet( UINT32 tx_len )
{
	USBHS->UEP3_CTRL = tx_len; 
	USBHS->UEP2_CTRL = 0xd<<4; // setup

	while( !(USBHS->USB_STATUS & USB2_ACT_FLAG));
	USBHS->UEP2_CTRL = 0;
	USBHS->USB_STATUS = USB2_ACT_FLAG;
}

void u2_get_data( UINT32 endp_num )
{
	UINT8 tx_success;
	
	tx_success = 0;
	USBHS->UEP2_CTRL = (0x9<<4) | endp_num; // in(0x09)
	while( !tx_success )
	{
		if( USBHS->USB_STATUS & USB2_ACT_FLAG )
		{
			if( ((USBHS->USB_STATUS >>24) & 0xf) != 0xa )// !NAK(0x0A)
			{
				tx_success = 1;
				USBHS->UEP2_CTRL = 0;
			}
			USBHS->USB_STATUS = USB2_ACT_FLAG;
		}
	}
}
void u2_send_data( UINT32 endp_num, UINT32 tx_len, UINT32 toggle )
{
	UINT8 tx_success;
	UINT8 ping_status;
	
	tx_success = 0;
	ping_status = 0;

	USBHS->UEP3_CTRL = tx_len | toggle;
	USBHS->UEP2_CTRL = (0xE1<<4) | endp_num; // out(0xE1)
	while( !tx_success )
	{
		if( USBHS->USB_STATUS & USB2_ACT_FLAG )
		{
			if( (((USBHS->USB_STATUS >>24) & 0xf) != 0xa)  & !ping_status )// !NAK(0x5A)
			{
				tx_success = 1;
				USBHS->UEP2_CTRL = 0;
			}
			else if( (((USBHS->USB_STATUS >>24) & 0xf) == 0xa)  & !ping_status )
			{
				ping_status = 1;
				USBHS->UEP2_CTRL = (0xB4<<4) | endp_num;
			}
			else if( (((USBHS->USB_STATUS >>24) & 0xf) != 0xa)  & ping_status  )
			{
				ping_status = 0;
				USBHS->UEP2_CTRL = (0xE1<<4) | endp_num; // out(0xE1)
			}
			else if( (((USBHS->USB_STATUS >>24) & 0xf) == 0xa)  & ping_status  )
			{
				USBHS->USB_STATUS = USB2_ACT_FLAG;
			}

			USBHS->USB_STATUS = USB2_ACT_FLAG;
		}
	}
}

void u2_set_address( UINT32 address )
{
	USBHS->USB_CONTROL &= 0x00ffffff;
	USBHS->USB_CONTROL |= address<<24; // SET ADDRESS

}

