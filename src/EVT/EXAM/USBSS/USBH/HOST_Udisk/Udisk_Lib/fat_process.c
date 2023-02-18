/********************************** (C) COPYRIGHT *******************************
* File Name          : fat_process.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 :
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

#include "string.h"
#include "CH56xSFR.h"
#include "core_riscv.h"
#include "CHRV3UFI.h"
#include "fat_process.h"
#include "stdio.h"
#include "type.h"


#define CREAT              1
#define READ               0
#define WRITE              0
#define BYTEWRITE_READ     1
#define MODIFY             1
#define ERASE              0

__attribute__ ((aligned(16))) UINT8 buffer[512] __attribute__((section(".DMADATA"))); //Data sending/receiving buffer of endpoint1

/*******************************************************************************
 * @fn       Fat_Init
 *
 * @return   None
 */
 UINT8  Fat_Init( void )
 {
	UINT8  i,s,c,count;
	UINT8  *pd;
	UINT8  *pd1;
	UINT32 temp32;
	pd = AA;
	pd1 = BB;
	i = CHRV3LibInit();

	printf("LIB_Init=%02x\n",i);
	CHRV3DiskStatus = DISK_MOUNTED;	/* �����Ѿ���ʼ���ɹ�,������δ�����ļ�ϵͳ�����ļ�ϵͳ��֧�� */
	CHRV3vSectorSize =  512;
	CHRV3vSectorSizeB = 0;
	temp32 = CHRV3vSectorSize;
	for( i = 0; i <= 12; i++ )
	{
		temp32 = temp32 >> 1;
		CHRV3vSectorSizeB++;
		if( ( temp32 & 0x01 )== 0x01 )
		{
			break;
		}
	}

#if CREAT

    strcpy( (char *)&mCmdParam.Create.mPathName,( const char * )"\\NEWFILE.TXT");  //����236.txt�ļ�
    i = CHRV3FileOpen();
    if((i == ERR_MISS_DIR)||(i == ERR_MISS_FILE))
    {
        printf( "Find No File And Create\r\n" );
        i = CHRV3FileCreate();
    }
    if( i!= ERR_SUCCESS )  return i;

    mCmdParam.Close.mUpdateLen=1;
    i = CHRV3FileClose();                                //�޸��ļ���Ϣ���
    if( i != ERR_SUCCESS )  return i;
#endif


#if WRITE
    printf("write\n");
    strcpy( (char *)&mCmdParam.Open.mPathName,( const char * )"\\NEWFILE.TXT");
    i = CHRV3FileOpen();
    if( i!= ERR_SUCCESS )return i;
    memset(buffer,0x42,512);

    mCmdParam.Write.mDataBuffer = buffer;
    mCmdParam.Write.mSectorCount = 1;
    i=CHRV3FileWrite( );                               //��NEWFILE.txt��д������

    if( i != ERR_SUCCESS )  printf("write_error\n");
    for(s=0;s!=5;s++)  printf("%02x ",buffer[s]);
    printf("\n");

    mCmdParam.Close.mUpdateLen = 1;
   i= CHRV3FileClose();                                 //��NEWFILE.txt��д�����
#endif

#if READ
   printf("read\n");
    strcpy( (char *)&mCmdParam.Open.mPathName,( const char * )"\\NEWFILE.TXT");    //���ļ�
    i = CHRV3FileOpen();
    if( i != ERR_SUCCESS ) return i;

    while(1){                                   //��ȡ�ļ�

      mCmdParam.Read.mDataBuffer = pd;
      mCmdParam.Read.mSectorCount = 16;
      i=CHRV3FileRead( );
      if( i != ERR_SUCCESS ) break;

      if( mCmdParam.Read.mSectorCount < 16 )break;

      mCmdParam.Read.mDataBuffer = pd1;
      mCmdParam.Read.mSectorCount = 16;
      i=CHRV3FileRead( );
      if( i != ERR_SUCCESS )break;

      if( mCmdParam.Read.mSectorCount < 16 )break;
      if( i != ERR_SUCCESS ) return i;
    }
    mCmdParam.Close.mUpdateLen = 1;
    i= CHRV3FileClose();
#endif

#if BYTEWRITE_READ
    printf( "ByteWrite\r\n" );
    strcpy( (char *)&mCmdParam.Open.mPathName,( const char * )"\\NEWFILE.TXT");    //���ļ�
    i = CHRV3FileOpen();
    if( i != ERR_SUCCESS ) return i;

  //ʵ��Ӧ���ж�д���ݳ��ȺͶ��建���������Ƿ���� ������ڻ�������������Ҫ���д��

    i = sprintf( (PCHAR)buffer,"Note: \xd\xa������������ֽ�Ϊ��λ����U���ļ���д,����ʾ���ܡ�\xd\xa");  /*��ʾ */
    for(c=0; c<10; c++)
    {
        mCmdParam.ByteWrite.mByteCount = i;                          /* ָ������д����ֽ��� */
        mCmdParam.ByteWrite.mByteBuffer = buffer;                       /* ָ�򻺳��� */
        s = CHRV3ByteWrite( );                                       /* ���ֽ�Ϊ��λ���ļ�д������ */
        if(s != ERR_SUCCESS )
        {
            printf("write error:%02x\n",s);
            return s;
        }
        printf("�ɹ�д�� %02X��\r\n",(UINT16)c);
    }
    ///////////������ȡ�ļ�ǰN�ֽ�/////////////////////////////////////////
    count = 10;                                                               //����׼����ȡ�ܳ���100�ֽ�
    printf( "������ǰ%d���ַ���:\r\n",count );
    while ( count ) {                                                        //����ļ��Ƚϴ�,һ�ζ�����,�����ٵ���CHRV3ByteRead������ȡ,�ļ�ָ���Զ�����ƶ�
        if ( count > (MAX_PATH_LEN-1) ) c = MAX_PATH_LEN-1;                          /* ʣ�����ݽ϶�,���Ƶ��ζ�д�ĳ��Ȳ��ܳ��� sizeof( mCmdParam.Other.mBuffer ) */
        else c = count;                                                              /* ���ʣ����ֽ��� */
        mCmdParam.ByteRead.mByteCount = c;                                           /* ���������ʮ�ֽ����� */
        mCmdParam.ByteRead.mByteBuffer= &buffer[0];
        s = CHRV3ByteRead( );                                                                                    /* ���ֽ�Ϊ��λ��ȡ���ݿ�,���ζ�д�ĳ��Ȳ��ܳ���MAX_BYTE_IO,�ڶ��ε���ʱ���Ÿղŵ����� */
        count -= mCmdParam.ByteRead.mByteCount;                                                                 /* ����,��ȥ��ǰʵ���Ѿ��������ַ��� */
        for ( i=0; i!=mCmdParam.ByteRead.mByteCount; i++ ) printf( "%c ", mCmdParam.ByteRead.mByteBuffer[i] );   /* ��ʾ�������ַ� */
        if ( mCmdParam.ByteRead.mByteCount < c ) {                                                              /* ʵ�ʶ������ַ�������Ҫ��������ַ���,˵���Ѿ����ļ��Ľ�β */
            printf( "\r\n" );
            printf( "�ļ��Ѿ�����\r\n" );
            break;
        }
    }
    printf( "Close\r\n" );
    i = CHRV3FileClose( );
#endif

#if MODIFY

   strcpy( (char *)&mCmdParam.Open.mPathName,( const char * )"\\NEWFILE.TXT");
   i = CHRV3FileOpen();
   if( i!= ERR_SUCCESS )         return i;

   mCmdParam.Modify.mFileAttr = 0xff;
   mCmdParam.Modify.mFileTime = 0xffff;
   mCmdParam.Modify.mFileDate = MAKE_FILE_DATE(2021, 12, 20);
   mCmdParam.Modify.mFileSize = 0xffffffff;
   i = CHRV3FileModify();                                  //�޸��ļ���Ϣ
   if( i!= ERR_SUCCESS ) return i;
   printf("modify=%02x\n",i);

   mCmdParam.Close.mUpdateLen = 1 ;
   i= CHRV3FileClose();                                //�޸��ļ���Ϣ���
   if( i!= ERR_SUCCESS )   return i;

#endif

#if ERASE

     strcpy( (char *)&mCmdParam.Erase.mPathName,( const char * )"\\NEWFILE.TXT");
     i = CHRV3FileErase();                                  //�޸��ļ���Ϣ
     if( i!= ERR_SUCCESS )return i;
     printf("erase=%02x\n",i);
#endif
     return i;
 }




