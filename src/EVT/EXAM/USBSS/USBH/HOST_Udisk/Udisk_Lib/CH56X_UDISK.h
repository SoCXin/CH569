/********************************** (C) COPYRIGHT *******************************
* File Name          : CH56X_UDISK.h
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 : 
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/


#ifndef __CH56X_UDISK_H__
#define __CH56X_UDISK_H__

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
#define USB_BO_CBW_SIG			  0x43425355									/* �����CBWʶ���־'USBC' */
#define USB_BO_CSW_SIG			  0x53425355									/* ����״̬��CSWʶ���־'USBS' */

#define USB_BO_CBW_SIZE			  0x1F											/* �����CBW���ܳ��� */
#define USB_BO_CSW_SIZE			  0x0D											/* ����״̬��CSW���ܳ��� */
#define USB_BO_DATA_IN			  0x80
#define USB_BO_DATA_OUT			  0x00


typedef union _BULK_ONLY_CMD
{
	struct
	{
		UINT32 mCBW_Sig;
		UINT32 mCBW_Tag;
		UINT32 mCBW_DataLen;													/* ����: ���ݴ��䳤�� */
		UINT8  mCBW_Flag;														/* ����: ���䷽��ȱ�־ */
		UINT8  mCBW_LUN;
		UINT8  mCBW_CB_Len;														/* ����: �����ĳ���,��Чֵ��1��16 */
		UINT8  mCBW_CB_Buf[16];													/* ����: �����,�û��������Ϊ16���ֽ� */
	} mCBW;																		/* BulkOnlyЭ��������, ����CBW�ṹ */
	struct
	{
		UINT32 mCSW_Sig;
		UINT32 mCSW_Tag;
		UINT32 mCSW_Residue;													/* ����: ʣ�����ݳ��� */
		UINT8  mCSW_Status;														/* ����: ����ִ�н��״̬ */
	} mCSW;																		/* BulkOnlyЭ�������״̬��, ���CSW�ṹ */
} BULK_ONLY_CMD;

/******************************************************************************/
/* �������� */
extern UINT8  gDiskMaxLun;				    									/* ��������߼���Ԫ�� */
extern UINT8  gDiskCurLun;	    												/* ���̵�ǰ�����߼���Ԫ�� */
extern UINT32 gDiskCapability;		    										/* ���������� */
extern UINT32 gDiskPerSecSize;	    											/* ����������С */
extern UINT8  gDiskBulkInEp;													/* USB�������洢�豸��IN�˵��ַ */
extern UINT8  gDiskBulkOutEp;													/* USB�������洢�豸��OUT�˵��ַ */
extern UINT16 gDiskBulkInEpSize;  	    										/* USB�������洢�豸��IN�˵�������С */
extern UINT16 gDiskBulkOutEpSize;  												/* USB�������洢�豸��OUT�˵�������С */
extern UINT8  gDiskInterfNumber;												/* USB�������洢�豸�Ľӿں� */
extern BULK_ONLY_CMD	mBOC;													/* BulkOnly����ṹ */
extern UINT8  disk_max_lun;

#define	DEFAULT_MAX_OPERATE_SIZE      	8192									/* Ĭ�ϵ�ǰ����������С */
#define MAX_DATA_ADDR	0x20030000
extern UINT8 U30HOST_MS_CofDescrAnalyse( UINT8 *pbuf );
extern UINT8 MS_Init(  UINT8 *pbuf );
extern UINT8 MS_ReadSector( UINT32 StartLba, UINT16 SectCount, PUINT8 DataBuf );
extern UINT8 MS_WriteSector( UINT32 StartLba,UINT8 SectCount, PUINT8 DataBuf );

extern UINT8 CHRV3BulkOnlyCmd( UINT8 *DataBuf );
extern UINT8 U20HOST_Issue_BulkOut( UINT8 *pDatBuf, UINT16 *pSize );
extern UINT8 U20HOST_Issue_BulkIn( UINT8 *pDatBuf, UINT16 *pSize );
extern UINT8 U20HOST_MS_CofDescrAnalyse( UINT8 *pbuf );


#ifdef __cplusplus
}
#endif

#endif
