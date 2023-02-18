/********************************** (C) COPYRIGHT *******************************
* File Name          : usb30_porp.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 : 
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/
#include "usb30_porp.h"
#include "usb30_desc.h"
#include "CH56xUSB30_LIB.H"
#include "UVCLIB.H"
__attribute__ ((aligned(16))) UINT8	endp0buff[512] __attribute__((section(".DMADATA"))); //�˵�0���ݷ��ͽ��ջ�����
__attribute__ ((aligned(16))) UINT8	endp1RTbuff[1024] __attribute__((section(".DMADATA"))); //�˵�1���ݷ��ͻ�����
__attribute__ ((aligned(16))) UINT8	endp2RTbuff[1024] __attribute__((section(".DMADATA"))); //�˵�2���ݷ��ͻ�����
__attribute__ ((aligned(16))) UINT8	endp3RTbuff[1024] __attribute__((section(".DMADATA"))); //�˵�3���ݷ��ͻ�����
__attribute__ ((aligned(16))) UINT8	endp4RTbuff[1024] __attribute__((section(".DMADATA"))); //�˵�4���ݷ��ͻ�����
__attribute__ ((aligned(16))) UINT8	endp5RTbuff[1024] __attribute__((section(".DMADATA"))); //�˵�5���ݷ��ͻ�����
__attribute__ ((aligned(16))) UINT8	endp6RTbuff[1024] __attribute__((section(".DMADATA"))); //�˵�6���ݷ��ͻ�����
__attribute__ ((aligned(16))) UINT8	endp7RTbuff[1024] __attribute__((section(".DMADATA"))); //�˵�7���ݷ��ͻ�����



/******************USB3.0 Device Initial****************************/
void USB30_init(void)
{
	USB30_initDevice();

	USB30_setEndpDMA(endp_0,endp0buff); 					// set endpoint0 dma address
	USB30_setEnableEndp(endp_0|endp_out,ENABLE);			// enable endpoint0 to send
	USB30_setEnableEndp(endp_0|endp_in,ENABLE);				// enable endpoint0 to receive

	USB30_setEndpDMA(endp_1|endp_out,endp1RTbuff);			// set endpoint1 dma sending address
	USB30_setEndpDMA(endp_1|endp_in,endp1RTbuff);			// set endpoint1 dma receiving address
	USB30_setEnableEndp(endp_1|endp_out ,ENABLE);			// enable endpoint1 to send
	USB30_setEnableEndp(endp_1|endp_in,ENABLE);				// enable endpoint1 to receive
	USB30_setISOEndp(endp_1|endp_in,ENABLE);

	USB30_setEndpDMA(endp_2|endp_out,endp2RTbuff);
	USB30_setEndpDMA(endp_2|endp_in,endp2RTbuff);
	USB30_setEnableEndp(endp_2|endp_out,ENABLE);
	USB30_setEnableEndp(endp_2|endp_in,ENABLE);

	USB30_setEndpDMA(endp_3|endp_out,endp3RTbuff);
	USB30_setEndpDMA(endp_3|endp_in,endp3RTbuff);
	USB30_setEnableEndp(endp_3|endp_out,ENABLE);
	USB30_setEnableEndp(endp_3|endp_in,ENABLE);

	USB30_setEndpDMA(endp_4|endp_out,endp4RTbuff);
	USB30_setEndpDMA(endp_4|endp_in,endp4RTbuff);
	USB30_setEnableEndp(endp_4|endp_out,ENABLE);
	USB30_setEnableEndp(endp_4|endp_in,ENABLE);

	USB30_setEndpDMA(endp_5|endp_out,endp5RTbuff);
	USB30_setEndpDMA(endp_5|endp_in,endp5RTbuff);
	USB30_setEnableEndp(endp_5|endp_out,ENABLE);
	USB30_setEnableEndp(endp_5|endp_in,ENABLE);

	USB30_setEndpDMA(endp_6|endp_out,endp6RTbuff);
	USB30_setEndpDMA(endp_6|endp_in,endp6RTbuff);
	USB30_setEnableEndp(endp_6|endp_out,ENABLE);
	USB30_setEnableEndp(endp_6|endp_in,ENABLE);

	USB30_setEndpDMA(endp_7|endp_out,endp7RTbuff);
	USB30_setEndpDMA(endp_7|endp_in,endp7RTbuff);
	USB30_setEnableEndp(endp_7|endp_out,ENABLE);
	USB30_setEnableEndp(endp_7|endp_in,ENABLE);

	USB30_setRxCtrl(endp_1,ACK_TP,NUMP_2);						// endpoint1 receive setting
	USB30_setRxCtrl(endp_2,ACK_TP,NUMP_2);
	USB30_setRxCtrl(endp_3,ACK_TP,NUMP_2);
	USB30_setRxCtrl(endp_4,ACK_TP,NUMP_2);
	USB30_setRxCtrl(endp_5,ACK_TP,NUMP_2);
	USB30_setRxCtrl(endp_6,ACK_TP,NUMP_2);
	USB30_setRxCtrl(endp_7,ACK_TP,NUMP_2);
}

UINT8   tx_lmp_port = 0;
static	UINT32		SetupLen=0;
static 	UINT8		SetupReqCode = 0;
static	PUINT8		pDescr;

/******************NonStandard Request Command Process****************************/
UINT16 USB30_NonStandardReq()
{
	SetupReqCode = UsbSetupBuf->bRequest;
	SetupLen = UsbSetupBuf->wLength;
	UINT16 len = 0xFFFF;

	if(UsbSetupBuf->bRequestType & 0x80){	//�ϴ�����
		len = Res_NonStandardReq(&pDescr);
		if(len != 0xFFFF){ //�ж��Ƿ�ظ�stall
					len = SetupLen >= ENDP0_MAXPACK ? ENDP0_MAXPACK : SetupLen;// ���δ��䳤��
					memcpy(endp0buff, pDescr,len );		// device  /* �����ϴ����� */
					SetupLen -= len;
					pDescr += len;
				}
	}else{	//�´�����
		if(UsbSetupBuf->bRequestType==0x21 && UsbSetupBuf->bRequest == 0x01){//set cur   //������ͷ����
			len = 0;
		}
	}
	return len;
}
/******************Standard Request Command Process****************************/
UINT16 USB30_StandardReq()
{
	SetupReqCode = UsbSetupBuf->bRequest;
	SetupLen = UsbSetupBuf->wLength;
	UINT16 len = 0;
	switch(SetupReqCode)
	{
		case USB_GET_DESCRIPTOR:
			switch(UsbSetupBuf->wValue.bw.bb0)
			{
					case USB_DESCR_TYP_DEVICE:
							if(SetupLen>SIZE_DEVICE_DESC) SetupLen  = SIZE_DEVICE_DESC;
							pDescr = (PUINT8)DeviceDescriptor;
							break;
					case	USB_DESCR_TYP_CONFIG:
							if(SetupLen > SIZE_CONFIG_DESC) SetupLen = SIZE_CONFIG_DESC;
							pDescr = (PUINT8)ConfigDescriptor;
							break;
					case USB_DESCR_TYP_BOS:
							if(SetupLen > SIZE_BOS_DESC) SetupLen = SIZE_BOS_DESC;
							pDescr = (PUINT8)BOSDescriptor;
							break;
					case 	USB_DESCR_TYP_STRING:
							switch(UsbSetupBuf->wValue.bw.bb1)
							{
							case USB_DESCR_LANGID_STRING:
								if(SetupLen > SIZE_STRING_LANGID) SetupLen = SIZE_STRING_LANGID;
								pDescr = (PUINT8)StringLangID;
								break;
							case USB_DESCR_VENDOR_STRING:
								if(SetupLen > SIZE_STRING_VENDOR) SetupLen = SIZE_STRING_VENDOR;
								pDescr = (PUINT8)StringVendor;
								break;
							case USB_DESCR_PRODUCT_STRING:
								if(SetupLen > SIZE_STRING_PRODUCT) SetupLen = SIZE_STRING_PRODUCT;
								pDescr = (PUINT8)StringProduct;
								break;
							case USB_DESCR_SERIAL_STRING:
								if(SetupLen > SIZE_STRING_SERIAL) SetupLen = SIZE_STRING_SERIAL;
								pDescr = (PUINT8)StringSerial;
								break;
							case USB_DESCR_OS_STRING:
								if(SetupLen >SIZE_STRING_OS) SetupLen = SIZE_STRING_OS;
								pDescr = (PUINT8)OSStringDescriptor;
								break;
							default:
								len = USB_DESCR_UNSUPPORTED;								//��֧�ֵ�������
								SetupReqCode = INVALID_REQ_CODE;							//��Ч��������
								break;
							}
							break;
					default:
						len = USB_DESCR_UNSUPPORTED;//��֧�ֵ�������
						SetupReqCode = INVALID_REQ_CODE;
						break;
			}
			if(len != USB_DESCR_UNSUPPORTED){
				len = SetupLen >= ENDP0_MAXPACK ? ENDP0_MAXPACK : SetupLen;					 // ���δ��䳤��
				memcpy(endp0buff, pDescr,len );												 // device  /* �����ϴ����� */
				SetupLen -= len;
				pDescr += len;
			}
			break;
		case USB_SET_ADDRESS:
					SetupLen = UsbSetupBuf->wValue.bw.bb1;  							// �ݴ�USB�豸��ַ
					break;
		case 0x31:																		//
					SetupLen = UsbSetupBuf->wValue.w;
					break;
		case 0x30:																		//
					break;
		case USB_SET_CONFIGURATION:
					break;
		case USB_GET_STATUS:
					len=2;
					endp0buff[0]=0x00;
					endp0buff[1]=0x00;
					SetupLen = 0;
					break;
		case USB_CLEAR_FEATURE:
					break;
		case USB_SET_FEATURE:
					break;
		case USB_SET_INTERFACE:
			ctrlCamera();
					break;
		default:
				len =USB_DESCR_UNSUPPORTED;										//����stall����֧�ֵ�����
				SetupReqCode = INVALID_REQ_CODE;
				printf(" stall \n");
					break;
	}
	return len;
}
/******************IN Transaction Response for Endpoint0 ****************************/
UINT16 EP0_IN_Callback(void)
{

	UINT16 len = 0;
	switch(SetupReqCode)
	{
		case USB_GET_DESCRIPTOR:
			 len = SetupLen >= ENDP0_MAXPACK ? ENDP0_MAXPACK : SetupLen;
			 memcpy(  endp0buff, pDescr,len );
			 SetupLen -= len;
			 pDescr += len;
			 break;
	}
	return len;
}
/******************OUT Transaction  Response for Endpoint0 ****************************/
UINT16 EP0_OUT_Callback()
{

	return 0;
}
/******************Status Stage ****************************/
void USB30_SetupStatus( void)
{
	switch(SetupReqCode)
	{
		case USB_SET_ADDRESS:
			 set_device_address(SetupLen );// SET ADDRESS
			 break;
		case 0x81:
			clearError();
			break;
		case 0x31:
		    set_ISO_delay(SetupLen);
		    break;
	}
}
