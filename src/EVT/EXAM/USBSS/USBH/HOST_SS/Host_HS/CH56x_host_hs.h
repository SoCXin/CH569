/********************************** (C) COPYRIGHT *******************************
* File Name          : CH56x_usb20.h
* Author             : WCH
* Version            : V1.1
* Date               : 2020/12/23
* Description        :
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/
#ifndef HOST_HS_CH56X_HOST_HS_H_
#define HOST_HS_CH56X_HOST_HS_H_

#ifdef __cplusplus
extern "C" {
#endif

//#include "core_riscv.h"
#define U20_MAXPACKET_LEN       512
#define WAIT_TIME               10000000

#ifndef  ENDPOINT_NUM
#define  ENDP_0                 0
#define  ENDP_1                 1
#define  ENDP_2                 2
#define  ENDP_3                 3
#define  ENDP_4                 4
#define  ENDP_5                 5
#define  ENDP_6                 6
#define  ENDP_7                 7
#endif
/*******************TRANSFER STATUS***********************/
#ifndef ERR_SUCCESS
#define ERR_SUCCESS           (0x00)
#define ERR_USB_CONNECT       (0x15)
#define ERR_USB_DISCON        (0x16)
#define ERR_USB_BUF_OVER      (0x17)
#define ERR_USB_DISK_ERR      (0x1F)
#define ERR_USB_TRANSFER      (0x20)
#define ERR_USB_UNSUPPORT     (0xFB)
#define ERR_USB_UNKNOWN       (0xFE)
#define ERR_AOA_PROTOCOL      (0x41)
#define ERR_TIME_OUT          (0xFF)
#endif

typedef struct
{
    UINT16  OutEndpMaxSize;
    UINT16  InEndpMaxSize;          // IN  �˵�������С
    UINT8   InEndpNum;              // IN  �˵��
    UINT8   InEndpCount;              // IN  �˵��
    UINT8   InTog;                  // IN ͬ����־
    UINT8   OutEndpNum;             // OUT �˵��
    UINT8   OutTog;
    UINT8   OutEndpCount;              // IN  �˵��
}DEVENDP;

typedef struct  __attribute__((packed))
{
     DEVENDP DevEndp;
     UINT8   DeviceStatus;           // �豸״̬,0-���豸,1-���豸����δ��ʼ��,2-���豸����ʼ��ö��ʧ��,3-���豸�ҳ�ʼ��ö�ٳɹ�
     UINT8   DeviceAddress;          // �豸�������USB��ַ
     UINT8   DeviceSpeed;            // 0Ϊȫ��,1Ϊ���٣�2Ϊ����
     UINT8   DeviceType;             // �豸���ͣ�08-����   0x30-HID  0x31-KEYBOARD  0x32-MOUSE
     UINT8   DeviceEndp0Size;        // USB0�豸�Ķ˵�0�������ߴ�
     UINT8   DeviceCongValue;        // �豸����ֵ

 }DEV_INFO_Typedef,*pDEV_INFO_Typedef;

 void  CopySetupReqPkg( const UINT8 *pReqPkt );
 void  SetBusReset(void);
 void  USBHS_CurrentAddr( UINT8 addr );
 void  USBHS_Host_Init(FunctionalState sta);
 void  USBHS_Analysis_Descr(pDEV_INFO_Typedef pusbdev,PUINT8 pdesc, UINT16 l);

 UINT8 USBHS_Transact( UINT8 endp_pid, UINT8 toggle,UINT32 timeout);
 UINT8 USBHS_HostCtrlTransfer(UINT8 *databuf,PUINT16 len);
 UINT8 USBHS_Host_Enum(UINT8 *Databuf);
 UINT8 CtrlGetDevDescr( UINT8 *databuf );
 UINT8 CtrlGetConfigDescr( UINT8 *databuf );
 UINT8 CtrlSetAddress( UINT8 addr );
 UINT8 CtrlSetConfig( UINT8 cfg_val );

#ifdef __cplusplus
}
#endif

#endif /* HOST_HS_CH56X_HOST_HS_H_ */

