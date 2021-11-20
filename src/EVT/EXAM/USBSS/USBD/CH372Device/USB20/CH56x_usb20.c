/********************************** (C) COPYRIGHT *******************************
* File Name          : CH56x_usb20.c
* Author             : WCH
* Version            : V1.1
* Date               : 2020/12/23
* Description        :
*******************************************************************************/
#include "CH56x_common.h"
#include "CH56x_usb20.h"
#include "CH56x_usb30.h"
/* Global define */


/* Global Variable */
UINT8 initial_end;
UINT8 set_address_status =0;
UINT8 get_config_status =0;
UINT8 device_addr;
extern UINT8V link_sta;

/* Function declaration */
void USBHS_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));           //USB2.0 interrupt service


const UINT8 hs_device_descriptor[] =
{
    0x12,   // bLength
    0x01,   // DEVICE descriptor type
    0x00,   // 2.00
    0x02,
    0x00,   // device class
    0x00,   // device sub-class
    0x00,   // vendor specific protocol
    0x40,   // max packet size 512B
    0x86,   // vendor id-0x1A86(qinheng)
    0x1A,
    0x37,   // product id 0x5537
    0x55,
    0x00,   //bcdDevice 0x0111
    0x11,
    0x01,   // manufacturer index string
    0x02,   // product index string
    0x00,   // serial number index string
    0x01    // number of configurations
};

const UINT8 hs_config_descriptor[] =
{
    0x09,   // length of this descriptor
    0x02,   // CONFIGURATION (2)
    0x74,   // total length includes endpoint descriptors (should be 1 more than last address)
    0x00,   // total length high byte
    0x01,   // number of interfaces
    0x01,   // configuration value for this one
    0x00,   // configuration - string is here, 0 means no string
    0x80,   // attributes - bus powered, no wakeup
    0x64,   // max power - 800 ma is 100 (64 hex)

    0x09,   // length of the interface descriptor
    0x04,   // INTERFACE (4)
    0x00,   // Zero based index 0f this interface
    0x00,   // Alternate setting value (?)
    0x0e,   // Number of endpoints (not counting 0)
    0xFF,   // Interface class, ff is vendor specific
    0xFF,   // Interface sub-class
    0xFF,   // Interface protocol
    0x00,   // Index to string descriptor for this interface

    0x07,   // length of this endpoint descriptor
    0x05,   // ENDPOINT (5)
    0x81,   // endpoint direction (80 is in) and address
    0x02,   // transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
    0x00,   // max packet size - 512 bytes
    0x02,   // max packet size - high
    0x00,   // polling interval in milliseconds (1 for iso)

    0x07,   // length of this endpoint descriptor
    0x05,   // ENDPOINT (5)
    0x01,   // endpoint direction (00 is out) and address
    0x02,   // transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
    0x00,   // max packet size - 512 bytes
    0x02,   // max packet size - high
    0x00,    // polling interval in milliseconds (1 for iso)

    0x07,   // length of this endpoint descriptor
    0x05,   // ENDPOINT (5)
    0x82,   // endpoint direction (80 is in) and address
    0x02,   // transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
    0x00,   // max packet size - 512 bytes
    0x02,   // max packet size - high
    0x00,   // polling interval in milliseconds (1 for iso)

    0x07,   // length of this endpoint descriptor
    0x05,   // ENDPOINT (5)
    0x02,   // endpoint direction (80 is in) and address
    0x02,   // transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
    0x00,   // max packet size - 512 bytes
    0x02,   // max packet size - high
    0x00,   // polling interval in milliseconds (1 for iso)

    0x07,   // length of this endpoint descriptor
    0x05,   // ENDPOINT (5)
    0x83,   // endpoint direction (80 is in) and address
    0x02,   // transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
    0x00,   // max packet size - 512 bytes
    0x02,   // max packet size - high
    0x00,   // polling interval in milliseconds (1 for iso)

    0x07,   // length of this endpoint descriptor
    0x05,   // ENDPOINT (5)
    0x03,   // endpoint direction (80 is in) and address
    0x02,   // transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
    0x00,   // max packet size - 512 bytes
    0x02,   // max packet size - high
    0x00,   // polling interval in milliseconds (1 for iso)

    0x07,   // length of this endpoint descriptor
    0x05,   // ENDPOINT (5)
    0x84,   // endpoint direction (80 is in) and address
    0x02,   // transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
    0x00,   // max packet size - 512 bytes
    0x02,   // max packet size - high
    0x00,   // polling interval in milliseconds (1 for iso)

    0x07,   // length of this endpoint descriptor
    0x05,   // ENDPOINT (5)
    0x04,   // endpoint direction (80 is in) and address
    0x02,   // transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
    0x00,   // max packet size - 512 bytes
    0x02,   // max packet size - high
    0x00,   // polling interval in milliseconds (1 for iso)

    0x07,   // length of this endpoint descriptor
    0x05,   // ENDPOINT (5)
    0x85,   // endpoint direction (80 is in) and address
    0x02,   // transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
    0x00,   // max packet size - 512 bytes
    0x02,   // max packet size - high
    0x00,   // polling interval in milliseconds (1 for iso)

    0x07,   // length of this endpoint descriptor
    0x05,   // ENDPOINT (5)
    0x05,   // endpoint direction (80 is in) and address
    0x02,   // transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
    0x00,   // max packet size - 512 bytes
    0x02,   // max packet size - high
    0x00,   // polling interval in milliseconds (1 for iso)

    0x07,   // length of this endpoint descriptor
    0x05,   // ENDPOINT (5)
    0x86,   // endpoint direction (80 is in) and address
    0x02,   // transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
    0x00,   // max packet size - 512 bytes
    0x02,   // max packet size - high
    0x00,   // polling interval in milliseconds (1 for iso)

    0x07,   // length of this endpoint descriptor
    0x05,   // ENDPOINT (5)
    0x06,   // endpoint direction (80 is in) and address
    0x02,   // transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
    0x00,   // max packet size - 512 bytes
    0x02,   // max packet size - high
    0x00,   // polling interval in milliseconds (1 for iso)

    0x07,   // length of this endpoint descriptor
    0x05,   // ENDPOINT (5)
    0x87,   // endpoint direction (80 is in) and address
    0x02,   // transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
    0x00,   // max packet size - 512 bytes
    0x02,   // max packet size - high
    0x00,   // polling interval in milliseconds (1 for iso)

    0x07,   // length of this endpoint descriptor
    0x05,   // ENDPOINT (5)
    0x07,   // endpoint direction (80 is in) and address
    0x02,   // transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
    0x00,   // max packet size - 512 bytes
    0x02,   // max packet size - high
    0x00    // polling interval in milliseconds (1 for iso)
};
// ====================================
// ===== String Descriptor Lang ID=====
// ====================================
const UINT8 hs_string_descriptor0[] =
{
    0x04,   // this descriptor length
    0x03,   // descriptor type
    0x09,   // Language ID 0 low byte
    0x04    // Language ID 0 high byte
};
// ====================================
// =====   String Descriptor 1    =====
// ====================================
const UINT8 hs_string_descriptor1[] =
{
    0x08,   // length of this descriptor
    0x03,
    'W',
    0x00,
    'C',
    0x00,
    'H',
    0x00
};
// ====================================
// =====   String Descriptor 2    =====
// ====================================
const UINT8 hs_string_descriptor2[]=
{
    38,         //该描述符的长度为38字节
    0x03,       //字符串描述符的类型编码为0x03
    0x57, 0x00, //W
    0x43, 0x00, //C
    0x48, 0x00, //H
    0x20, 0x00, //
    0x55, 0x00, //U
    0x53, 0x00, //S
    0x42, 0x00, //B
    0x32, 0x00, //2
    0x2e, 0x00, //.
    0x30, 0x00, //0
    0x20, 0x00, //
    0x44, 0x00, //D
    0x45, 0x00, //E
    0x56, 0x00, //V
    0x49, 0x00, //I
    0x43, 0x00, //C
    0x45, 0x00, //E
    0x20, 0x00  //
};
const UINT8 hs_bos_descriptor[] =
{
    0x05,   // length of this descriptor
    0x0f,   // CONFIGURATION (2)
    0x16,   // total length includes endpoint descriptors (should be 1 more than last address)
    0x00,   // total length high byte
    0x02,       // number of device cap

    0x07,
    0x10,   // DEVICE CAPABILITY type
    0x02,   // USB2.0 EXTENSION
    0x02,
    0x00,
    0x00,
    0x00,

    0x0a,   // length of this descriptor
    0x10,   // DEVICE CAPABILITY type
    0x03,   // superspeed usb device capability
    0x00,   //
    0x0e,   // ss/hs/fs
    0x00,
    0x01,   // the lowest speed is full speed
    0x0a,   // u1 exit latency is 10us
    0xff,   // u1 exit latency is 8us
    0x07
};



/*******************************************************************************
* Function Name  : USB20_Endp_Init
* Description    : USB2.0端点初始化
* Input          : None
* Return         : None
*******************************************************************************/
void USB20_Endp_Init ()	// USBHS device endpoint initial
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

	USBHS->UEP0_DMA    = (UINT32)(UINT8 *)endp0RTbuff;
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
/*******************************************************************************
* Function Name  : USB20_Device_Init
* Description    : USB2.0设备初始化
* Input          : ENABLE：使能   DISABLE：关闭
* Return         : None
*******************************************************************************/
void USB20_Device_Init ( FunctionalState sta )  // USBSS device initial
{
    if(sta){
        USB20_Endp_Init();
        USBHS->USB_CONTROL = 0;
        USBHS->USB_CONTROL =  HIGH_SPEED | INT_BUSY_EN | DMA_EN | DEV_PU_EN | USB2_SETUP_EN | USB2_ACT_EN | USB2_DETECT_EN | USB2_SUSP_EN;
    }
    else{
        USBHS->USB_CONTROL = USB_ALL_CLR | USB_FORCE_RST;
    }
}

/*******************************************************************************
* Function Name  : USB20_Device_setaddress
* Description    : USB2.0设置设备地址
* Input          : 地址
* Return         : None
*******************************************************************************/
void USB20_Device_Setaddress( UINT32 address )
{
    USBHS->USB_CONTROL &= 0x00ffffff;
    USBHS->USB_CONTROL |= address<<24; // SET ADDRESS

}

/*******************************************************************************
* Function Name  : USBHS_IRQHandler
* Description    : USB2.0 Interrupt Handler.
* Input          : None
* Return         : None
*******************************************************************************/
void USBHS_IRQHandler(void)			//USBHS interrupt severice
{
	UINT32 end_num;
	UINT32 rx_token;
	UINT16 rx_len;
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
		temp = *(UINT32 *)endp0RTbuff;
		temp1 = *(UINT32 *)(endp0RTbuff+4);
		bmRequestType = *(UINT8 *)endp0RTbuff;
		bRequest = *(UINT8 *)(endp0RTbuff+1);
		wValue_l = *(UINT8 *)(endp0RTbuff+2);
		wValue_h = *(UINT8 *)(endp0RTbuff+3);
		wLength = (temp1>>16) & 0xffff;
		 printf("SETUP :");
		 p8 = (UINT8 *)endp0RTbuff;
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
			    memcpy(endp0RTbuff,(UINT8 *)hs_device_descriptor,(UINT16)hs_device_descriptor[0] );
				send_data_len = hs_device_descriptor[0];
			}
			else if( bRequest == 0x06 && wValue_h == 0x02 ) // config desc
			{ 

				if( wLength > 64)
				{
				    memcpy(endp0RTbuff,(UINT8 *)hs_config_descriptor,64 );
					send_data_len = 64;
					get_config_status = 1;
				}
				else
				{
				    memcpy(endp0RTbuff,(UINT8 *)hs_config_descriptor,wLength );
					send_data_len = wLength;
				}
			}
			else if( bRequest == 0x06 && wValue_h == 0x0f )
			{
				if( wLength >= hs_bos_descriptor[2])
				{
				    memcpy(endp0RTbuff,(UINT8 *)hs_bos_descriptor, hs_bos_descriptor[2] );
					send_data_len = hs_bos_descriptor[2];
				}
				else
				{
				    memcpy(endp0RTbuff,(UINT8 *)hs_bos_descriptor, wLength );
					send_data_len = wLength;
				}
			}
			else if( bRequest == 0x06 && wValue_h == 0x03 && wValue_l == 0x01  )// string1
			{ 
			    memcpy(endp0RTbuff,(UINT8 *)hs_string_descriptor1, hs_string_descriptor1[0] );
				send_data_len = hs_string_descriptor1[0];
			}
			else if( bRequest == 0x06 && wValue_h == 0x03 && wValue_l == 0x02  )// string2
			{ 
			    memcpy(endp0RTbuff,(UINT8 *)hs_string_descriptor2, hs_string_descriptor2[0] );
				send_data_len = hs_string_descriptor2[0];
			}
			else if( bRequest == 0x06 && wValue_h == 0x03  )// string0
			{ 
			    memcpy(endp0RTbuff,(UINT8 *)hs_string_descriptor0, hs_string_descriptor0[0] );
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
				    USB20_Device_Setaddress( device_addr );// SET ADDRESS
					set_address_status = 0;
					USBHS->UEP0_CTRL = 0;
				}
				else if( get_config_status )
				{
					hs_config_len = hs_config_descriptor[2]-64;
					memcpy(endp0RTbuff,(UINT8 *)(hs_config_descriptor + 64), (UINT16)hs_config_len );
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
		device_addr = 0;
		USB20_Endp_Init();
		USB20_Device_Setaddress( device_addr );
		printf(" bus reset! \n");
		USBHS->USB_STATUS = USB2_DETECT_FLAG;
        {
            if( link_sta == 1 ){
                PFIC_EnableIRQ(USBSS_IRQn);
                PFIC_EnableIRQ(LINK_IRQn);
                PFIC_EnableIRQ(TMR0_IRQn);
                R8_TMR0_INTER_EN = 1;
                TMR0_TimerInit( 67000000 );   //约0.5秒
                USB30D_init(ENABLE);
            }
        }
	}
	else if(USBHS->USB_STATUS & USB2_SUSP_FLAG )
	{
		printf(" bus suspend! \n");
		USBHS->USB_STATUS = USB2_SUSP_FLAG;
	}
}
