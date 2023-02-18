/********************************** (C) COPYRIGHT *******************************
* File Name          : fat_process.h
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 :
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

#ifndef UDISK_LIB_FAT_PROCESS_H_
#define UDISK_LIB_FAT_PROCESS_H_

#ifdef __cplusplus
extern "C" {
#endif

#define     MAX_LEN_MCU         32

typedef struct              //����λ����
{
    UINT8   Name[ 13 ];             // ���ļ��� 8+3��ʽ(�Ѿ�ת����)
    UINT8   attrib;                 // ���ļ������ļ���0--�ļ���1--�ļ���
    UINT32  FileLen;                // �ļ����ȣ��ļ���Ϊ0
    UINT16  UpdateDate;             // �޸�����
    UINT16  UpdateTime;             // �޸�ʱ��
    UINT16  CreateDate;             // ��������
    UINT16  CreateTime;             // ����ʱ��
    UINT16  LongNameLen;            // ��ų��ļ��� ��Ч���ȵ���: RecCmd.len - 25
} disk_2;


//#pragma pack(1)
typedef  union
    {
    unsigned char   buf[ MAX_LEN_MCU];  // �������
    disk_2  FileDef;
} DiskData_1;
DiskData_1  DiskData;

extern  UINT8  Fat_Init( void );
extern  unsigned char Enum_Dir( void );
extern  unsigned char Manual_Send_Dir( void );
extern  UINT8 path_name[255],back_path_name[255];

#ifdef __cplusplus
}
#endif

#endif /* UDISK_LIB_FAT_PROCESS_H_ */
