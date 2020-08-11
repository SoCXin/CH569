/********************************** (C) COPYRIGHT *******************************
* File Name          : fat_process.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 :
*******************************************************************************/

#include "string.h"
#include "CH56xSFR.h"
#include "core_riscv.h"
#include "type.h"
#include "CHRV3UFI.h"
#include "fat_process.h"

/*******************************************************************************
* Function Name  : Fat_Init
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
 UINT8  Fat_Init( void )
 {
	UINT8 i,s;
	UINT8 *pd;
	UINT8 *pd1;
	UINT32 temp32;
	pd = AA;
	pd1 = BB;
	i = CHRV3LibInit();

	printf("LIB_Init=%02x\n",i);
	CHRV3DiskStatus = DISK_MOUNTED;	/* 磁盘已经初始化成功,但是尚未分析文件系统或者文件系统不支持 */
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
	strcpy( (char *)&mCmdParam.Open.mPathName,( const char * )"\\123.C" );
	i = CHRV3FileOpen();
	printf("Open=%02x\n",i);
	if( i!=0 )return i;
	printf("CHRV3vFileSize=%d\n",CHRV3vFileSize);
	while(1){
		R32_PB_OUT |= 1<<11;
	    mCmdParam.Read.mDataBuffer=pd;
		mCmdParam.Read.mSectorCount=16;
		i=CHRV3FileRead( );
		if( i != 0 )break;
		if( mCmdParam.Read.mSectorCount<16 )break;

		for(s=0;s!=5;s++)printf("%02x ",*(pd+s));
		printf("\n");
		if( i!=0 )printf("error=%02x\n",i);
		R32_PB_CLR |= 1<<11;
	    mCmdParam.Read.mDataBuffer=pd1;
		mCmdParam.Read.mSectorCount=16;
		i=CHRV3FileRead( );
		if( i != 0 )break;
		for(s=0;s!=5;s++)printf("%02x ",*(pd1+s));
		printf("\n");

		if( mCmdParam.Read.mSectorCount<16 )break;
		if( i!=0 )printf("error1=%02x\n",i);
	}
	printf("read_end\n");
 }




