#include "core_riscv.h"
#include "CH56x_common.h"
#include "usbhs.h"
#include "usb20_des.h"
#include "stdio.h"
#include "usb30_porp.h"

UINT8 initial_end;
UINT8 set_address_status =0;
UINT8 get_config_status =0;
UINT8 device_addr;

void usbhsdev_deinit ( void )
{
	USBHS->USB_CONTROL = USB_ALL_CLR | USB_FORCE_RST;
}

void endp_init ()	// USBHS device endpoint initial
{
	USBHS->USB_BUF_MODE = 0xccccccc;
	USBHS->UEP0_MAX_LEN = 64;
	USBHS->UEP1_MAX_LEN = 512;
	USBHS->UEP2_MAX_LEN = 512;
	USBHS->UEP3_MAX_LEN = 512;
	USBHS->UEP4_MAX_LEN = 512;
	USBHS->UEP5_MAX_LEN = 512;
	USBHS->UEP6_MAX_LEN = 512;
	USBHS->UEP7_MAX_LEN = 512;

	USBHS->UEP0_DMA    = (UINT32)(UINT8 *)endp0buff;
	USBHS->UEP1_TX_DMA = (UINT32)(UINT8 *)endp1RTbuff;
	USBHS->UEP1_RX_DMA = (UINT32)(UINT8 *)endp1RTbuff;
	USBHS->UEP2_TX_DMA = (UINT32)(UINT8 *)endp2RTbuff;
	USBHS->UEP2_RX_DMA = (UINT32)(UINT8 *)endp2RTbuff;
	USBHS->UEP3_TX_DMA = (UINT32)(UINT8 *)endp3RTbuff;
	USBHS->UEP3_RX_DMA = (UINT32)(UINT8 *)endp3RTbuff;
	USBHS->UEP4_TX_DMA = (UINT32)(UINT8 *)endp4RTbuff;
	USBHS->UEP4_RX_DMA = (UINT32)(UINT8 *)endp4RTbuff;
	USBHS->UEP5_TX_DMA = (UINT32)(UINT8 *)endp5RTbuff;
	USBHS->UEP5_RX_DMA = (UINT32)(UINT8 *)endp5RTbuff;
	USBHS->UEP6_TX_DMA = (UINT32)(UINT8 *)endp6RTbuff;
	USBHS->UEP6_RX_DMA = (UINT32)(UINT8 *)endp6RTbuff;
	USBHS->UEP7_TX_DMA = (UINT32)(UINT8 *)endp7RTbuff;
	USBHS->UEP7_RX_DMA = (UINT32)(UINT8 *)endp7RTbuff;

	USBHS->UEP0_CTRL = 0;
	USBHS->UEP1_CTRL = 0x20200000;
	USBHS->UEP2_CTRL = 0x20200000;
	USBHS->UEP3_CTRL = 0x20200000;
	USBHS->UEP4_CTRL = 0x20200000;
	USBHS->UEP5_CTRL = 0x20200000;
	USBHS->UEP6_CTRL = 0x20200000;
	USBHS->UEP7_CTRL = 0x20200000;

}
void usbhsdev_init (void )  // USBSS device initial
{
    endp_init();
    USBHS->USB_CONTROL = 0;
    USBHS->USB_CONTROL =  HIGH_SPEED | INT_BUSY_EN | DMA_EN | DEV_PU_EN | USB2_SETUP_EN | USB2_ACT_EN | USB2_DETECT_EN | USB2_SUSP_EN;
}
void USB20_IRQ (void)			//USBHS interrupt severice
{
	UINT32 end_num;
	UINT32 rx_token;
	UINT32 rx_len;
	UINT8  *p8;

	UINT32 i,temp,temp1;
	UINT8  send_data_len =0;
	UINT8  bmRequestType;
	UINT8  bRequest;
	UINT8  wValue_l;
	UINT8  wValue_h;
	UINT8  wLength;
	UINT8  hs_config_len;
	if( USBHS->USB_STATUS & USB2_SETUP_FLAG )
	{
		temp = *(UINT32 *)0x20020000;
		temp1 = *(UINT32 *)0x20020004;
		bmRequestType = *(UINT8 *)0x20020000;
		bRequest = *(UINT8 *)0x20020001;
		wValue_l = *(UINT8 *)0x20020002;
		wValue_h = *(UINT8 *)0x20020003;
		wLength = (temp1>>16) & 0xffff;
		 printf("SETUP :");
		 p8 = (UINT8 *)0x20020000;
		 for(i=0; i<8; i++)  { printf("%02x ", *p8++); }
		 printf("\n");
		if( bmRequestType == 0 ) // without data stage
		{
			if( bRequest == 0x5 ) // set address
			{
				set_address_status = 1; 
				device_addr = wValue_l;
			}
			else if( bRequest == 0x9 ) // set configuration
			{
				
			}
			USBHS->UEP0_CTRL = EP_T_RES_ACK | EP_T_TOG_1; // IN -DATA1-ACK( len =0 )
		}
		else if( bmRequestType == 0x80 ) // data stage is IN
		{ 
			if( bRequest == 0x06 && wValue_h == 0x01 ) // dev desc
			{
			    memcpy(endp0buff,(UINT8 *)hs_device_descriptor,(UINT16)hs_device_descriptor[0] );
				send_data_len = hs_device_descriptor[0];
			}
			else if( bRequest == 0x06 && wValue_h == 0x02 ) // config desc
			{ 

				if( wLength > 64)
				{
				    memcpy(endp0buff,(UINT8 *)hs_config_descriptor,64 );
					send_data_len = 64;
					get_config_status = 1;
				}
				else
				{
				    memcpy(endp0buff,(UINT8 *)hs_config_descriptor,wLength );
					send_data_len = wLength;
				}
			}
			else if( bRequest == 0x06 && wValue_h == 0x0f )
			{
				if( wLength >= hs_bos_descriptor[2])
				{
				    memcpy(endp0buff,(UINT8 *)hs_bos_descriptor, hs_bos_descriptor[2] );
					send_data_len = hs_bos_descriptor[2];
				}
				else
				{
				    memcpy(endp0buff,(UINT8 *)hs_bos_descriptor, wLength );
					send_data_len = wLength;
				}
			}
			else if( bRequest == 0x06 && wValue_h == 0x03 && wValue_l == 0x01  )// string1
			{ 
			    memcpy(endp0buff,(UINT8 *)hs_string_descriptor1, hs_string_descriptor1[0] );
				send_data_len = hs_string_descriptor1[0];
			}
			else if( bRequest == 0x06 && wValue_h == 0x03 && wValue_l == 0x02  )// string2
			{ 
			    memcpy(endp0buff,(UINT8 *)hs_string_descriptor2, hs_string_descriptor2[0] );
				send_data_len = hs_string_descriptor2[0];
			}
			else if( bRequest == 0x06 && wValue_h == 0x03  )// string0
			{ 
			    memcpy(endp0buff,(UINT8 *)hs_string_descriptor0, hs_string_descriptor0[0] );
				send_data_len = hs_string_descriptor0[0];
			}
			USBHS->UEP0_CTRL = EP_T_RES_ACK | EP_T_TOG_1 | send_data_len; // DATA stage (IN -DATA1-ACK)
		}
		else if( (temp & 0xffff) == 0xfea1 ) //
		{
			*( UINT32*) 0x20020000 = 0;
			USBHS->UEP0_CTRL = EP_T_RES_ACK | EP_T_TOG_1 | 1;
		}
		else if( (temp & 0xffff) == 0x0102 ) // clear feature
		{
			USBHS->UEP0_CTRL = EP_T_RES_ACK | EP_T_TOG_1; // IN -DATA1-ACK( len =0 )
		}
		else 
		{
			USBHS->UEP0_CTRL = EP_T_RES_STALL | EP_R_RES_STALL; // IN - STALL / OUT - DATA - STALL
			// printf(" stall \n");
		}
		USBHS->USB_STATUS = USB2_SETUP_FLAG; // clear int flag
	}
	else if( USBHS->USB_STATUS & USB2_ACT_FLAG ) 
	{
		end_num =  ((USBHS->USB_STATUS)>>24) & 0xf;
		rx_token = ((USBHS->USB_STATUS)>>28) & 0x3; // 00: OUT, 01:SOF, 10:IN, 11:SETUP

		if( !(USBHS->USB_STATUS & TOG_MATCH) )
		{
			printf(" TOG MATCH FAIL : ENDP %x token %x \n", end_num, rx_token);
		}

		if( end_num == 0 )
		{
			if( rx_token == PID_IN ) // IN
			{
				if( set_address_status )
				{
					u2_set_address( device_addr );// SET ADDRESS
					set_address_status = 0;
					USBHS->UEP0_CTRL = 0;
				}
				else if( get_config_status )
				{
					hs_config_len = hs_config_descriptor[2]-64;
					memcpy(endp0buff,(UINT8 *)(hs_config_descriptor + 64), (UINT16)hs_config_len );
					USBHS->UEP0_CTRL = EP_T_RES_ACK | EP_T_TOG_0 | hs_config_len; // DATA stage (IN -DATA1-ACK)
					get_config_status = 0;
				}
				else
				{
					USBHS->UEP0_CTRL = EP_R_RES_ACK | EP_R_TOG_1;
				}
			}
			else if( rx_token == PID_OUT ) //OUT
			{
				USBHS->UEP0_CTRL = EP_T_RES_ACK | EP_T_TOG_1;
			}
		}
		else if( end_num == 1 )
		{
			if( rx_token == PID_IN ) // IN - DATA - ACK
			{

			}
			else if( rx_token == PID_OUT ) // OUT - DATA - ACK
			{
				rx_len = USBHS->USB_RX_LEN;
				for(i=0;i<rx_len;i++){
                    endp1RTbuff[i] = ~endp1RTbuff[i];   //数据取反上传
                }
				USBHS->UEP1_CTRL &= 0xffff0000;
				USBHS->UEP1_CTRL |= rx_len; // IN -DATAx-ACK( len =rx_len )
			}
		}
		else if( end_num == 2 )
		{
			if( rx_token == PID_IN ) // IN - DATA - ACK
			{

			}
			else if( rx_token == PID_OUT ) // OUT - DATA - ACK
			{
				rx_len = USBHS->USB_RX_LEN;
				for(i=0;i<rx_len;i++){
				    endp2RTbuff[i] = ~endp2RTbuff[i];   //数据取反上传
				}
				USBHS->UEP2_CTRL &= 0xffff0000;
				USBHS->UEP2_CTRL |= rx_len; // IN -DATAx-ACK( len =rx_len )
			}
		}
		else if( end_num == 3 )
		{
			if( rx_token == PID_IN ) // IN - DATA - ACK
			{

			}
			else if( rx_token == PID_OUT ) // OUT - DATA - ACK
			{
				rx_len = USBHS->USB_RX_LEN;
				for(i=0;i<rx_len;i++){
                    endp3RTbuff[i] = ~endp3RTbuff[i];   //数据取反上传
                }
				USBHS->UEP3_CTRL &= 0xffff0000;
				USBHS->UEP3_CTRL |= rx_len; // IN -DATAx-ACK( len =rx_len )
			}
		}
		else if( end_num == 4 )
		{
			if( rx_token == PID_IN ) // IN - DATA - ACK
			{

			}
			else if( rx_token == PID_OUT ) // OUT - DATA - ACK
			{
				rx_len = USBHS->USB_RX_LEN;
				for(i=0;i<rx_len;i++){
                    endp4RTbuff[i] = ~endp4RTbuff[i];   //数据取反上传
                }
				USBHS->UEP4_CTRL &= 0xffff0000;
				USBHS->UEP4_CTRL |= rx_len; // IN -DATAx-ACK( len =rx_len )
			}
		}
		else if( end_num == 5 )
		{
			if( rx_token == PID_IN ) // IN - DATA - ACK
			{

			}
			else if( rx_token == PID_OUT ) // OUT - DATA - ACK
			{
				rx_len = USBHS->USB_RX_LEN;
				for(i=0;i<rx_len;i++){
                    endp5RTbuff[i] = ~endp5RTbuff[i];   //数据取反上传
                }
				USBHS->UEP5_CTRL &= 0xffff0000;
				USBHS->UEP5_CTRL |= rx_len; // IN -DATAx-ACK( len =rx_len )
			}
		}
		else if( end_num == 6 )
		{
			if( rx_token == PID_IN ) // IN - DATA - ACK
			{

			}
			else if( rx_token == PID_OUT ) // OUT - DATA - ACK
			{
				rx_len = USBHS->USB_RX_LEN;
				for(i=0;i<rx_len;i++){
                    endp6RTbuff[i] = ~endp6RTbuff[i];   //数据取反上传
                }
				USBHS->UEP6_CTRL &= 0xffff0000;
				USBHS->UEP6_CTRL |= rx_len; // IN -DATAx-ACK( len =rx_len )
			}
		}
		else if( end_num == 7 )
		{
			if( rx_token == PID_IN ) // IN - DATA - ACK
			{
				
			}
			else if( rx_token == PID_OUT ) // OUT - DATA - ACK
			{
				rx_len = USBHS->USB_RX_LEN;
				for(i=0;i<rx_len;i++){
                    endp7RTbuff[i] = ~endp7RTbuff[i];   //数据取反上传
                }
				USBHS->UEP7_CTRL &= 0xffff0000;
				USBHS->UEP7_CTRL |= rx_len; // IN -DATAx-ACK( len =rx_len )
			}
		}
		USBHS->USB_STATUS = USB2_ACT_FLAG;
	}
	else if( USBHS->USB_STATUS & USB2_DETECT_FLAG )
	{
	    USB30_init();
		device_addr = 0;
		endp_init ();
		u2_set_address( device_addr );
		printf(" bus reset! \n");
		USBHS->USB_STATUS = USB2_DETECT_FLAG;
	}
	else if(USBHS->USB_STATUS & USB2_SUSP_FLAG )
	{
		printf(" bus suspend! \n");
		USBHS->USB_STATUS = USB2_SUSP_FLAG;
	}
}
