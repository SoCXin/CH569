/********************************** (C) COPYRIGHT *******************************
* File Name          : ov.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 : OV2640 ����ͷ ���ú���
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

#include "CH56x_common.h"
#include "ov.h"

//���� Camera ��ʼ�����üĴ��� �б�
const UINT8 OV2640_InitRegTbl[][2]=
{
	0xff, 0x00, 0x2c, 0xff,	0x2e, 0xdf,	0xff, 0x01,	0x3c, 0x32,

	0x11, 0x00,	0x09, 0x02,	0x04, 0xD8,	0x13, 0xe5,	0x14, 0x48,	0x2c, 0x0c,	0x33, 0x78,	0x3a, 0x33,	0x3b, 0xfB,

	0x3e, 0x00,	0x43, 0x11,	0x16, 0x10,

	0x39, 0x92,

	0x35, 0xda,	0x22, 0x1a,	0x37, 0xc3,	0x23, 0x00,	0x34, 0xc0,	0x36, 0x1a,	0x06, 0x88,	0x07, 0xc0,	0x0d, 0x87,	0x0e, 0x41,	0x4c, 0x00,

	0x48, 0x00,	0x5B, 0x00,	0x42, 0x03,

	0x4a, 0x81,	0x21, 0x99,

	0x24, 0x40,	0x25, 0x38,	0x26, 0x82,	0x5c, 0x00,	0x63, 0x00,	0x46, 0x00,	0x0c, 0x3c,

	0x61, 0x70,	0x62, 0x80,	0x7c, 0x05,

	0x20, 0x80,	0x28, 0x30,	0x6c, 0x00,	0x6d, 0x80,	0x6e, 0x00,	0x70, 0x02,	0x71, 0x94,	0x73, 0xc1,	0x3d, 0x34,	0x5a, 0x57,

	0x12, 0x00,

	0x17, 0x11, 0x18, 0x75,	0x19, 0x01,	0x1a, 0x97,	0x32, 0x36,	0x03, 0x0f,	0x37, 0x40,

	0x4f, 0xca,	0x50, 0xa8,	0x5a, 0x23,	0x6d, 0x00,	0x6d, 0x38,

	0xff, 0x00,	0xe5, 0x7f,	0xf9, 0xc0,	0x41, 0x24,	0xe0, 0x14,	0x76, 0xff,	0x33, 0xa0,	0x42, 0x20,	0x43, 0x18,	0x4c, 0x00,	0x87, 0xd5,	0x88, 0x3f,	0xd7, 0x03,	0xd9, 0x10,	0xd3, 0x82,

	0xc8, 0x08,	0xc9, 0x80,

	0x7c, 0x00,	0x7d, 0x00,	0x7c, 0x03,	0x7d, 0x48,	0x7d, 0x48,	0x7c, 0x08,	0x7d, 0x20,	0x7d, 0x10,	0x7d, 0x0e,

	0x90, 0x00,	0x91, 0x0e,	0x91, 0x1a,	0x91, 0x31,	0x91, 0x5a,	0x91, 0x69,	0x91, 0x75,	0x91, 0x7e,	0x91, 0x88,	0x91, 0x8f,	0x91, 0x96,	0x91, 0xa3,	0x91, 0xaf,	0x91, 0xc4,	0x91, 0xd7,	0x91, 0xe8,	0x91, 0x20,

	0x92, 0x00,	0x93, 0x06,	0x93, 0xe3,	0x93, 0x05,	0x93, 0x05,	0x93, 0x00,	0x93, 0x04,	0x93, 0x00,	0x93, 0x00,	0x93, 0x00,	0x93, 0x00,	0x93, 0x00,	0x93, 0x00,	0x93, 0x00,

	0x96, 0x00,	0x97, 0x08,	0x97, 0x19,	0x97, 0x02,	0x97, 0x0c,	0x97, 0x24,	0x97, 0x30,	0x97, 0x28,	0x97, 0x26,	0x97, 0x02,	0x97, 0x98,	0x97, 0x80,	0x97, 0x00,	0x97, 0x00,

	0xc3, 0xef,

	0xa4, 0x00,	0xa8, 0x00,	0xc5, 0x11,	0xc6, 0x51,	0xbf, 0x80,	0xc7, 0x10,	0xb6, 0x66,	0xb8, 0xA5,	0xb7, 0x64,	0xb9, 0x7C,	0xb3, 0xaf,	0xb4, 0x97,	0xb5, 0xFF,	0xb0, 0xC5,	0xb1, 0x94,	0xb2, 0x0f,	0xc4, 0x5c,

	0xc0, 0xc8,	0xc1, 0x96,	0x8c, 0x00,	0x86, 0x3d,	0x50, 0x00,	0x51, 0x90,	0x52, 0x2c,	0x53, 0x00,	0x54, 0x00,	0x55, 0x88,

	0x5a, 0x90,	0x5b, 0x2C,	0x5c, 0x05,

	0xd3, 0x02,

	0xc3, 0xed,	0x7f, 0x00,

	0xda, 0x09,

	0xe5, 0x1f,	0xe1, 0x67,	0xe0, 0x00,	0xdd, 0x7f,	0x05, 0x00,
};

//Camera YUV422 Mode ���üĴ��� �б�
const UINT8 OV2640_YUV422RegTbl[][2]=
{
	0xFF, 0x00,	0xDA, 0x10,	0xD7, 0x03,	0xDF, 0x00,	0x33, 0x80,	0x3C, 0x40,	0xe1, 0x77,	0x00, 0x00,
};

//Camera JPEG Mode ���üĴ��� �б�
const UINT8 OV2640_JPEGRegTbl[][2]=
{
	0xff, 0x01,	0xe0, 0x14,	0xe1, 0x77,	0xe5, 0x1f,	0xd7, 0x03,	0xda, 0x10,	0xe0, 0x00,
};

//Camera RGB565 Mode ���üĴ��� �б�
const UINT8 OV2640_RGB565RegTbl[][2]=
{
	0xFF, 0x00,	0xDA, 0x09,	0xD7, 0x03,	0xDF, 0x02,	0x33, 0xa0,	0x3C, 0x00,	0xe1, 0x67,

	0xff, 0x01,	0xe0, 0x00,	0xe1, 0x00,	0xe5, 0x00,	0xd7, 0x00,	0xda, 0x00,	0xe0, 0x00,
};

/***************************************************************
 * @fn       SCCB_GPIO_Init
 *
 * @brief    SCCB �ӿڳ�ʼ��
 *
 * @return   None
 */
void SCCB_GPIO_Init(void)
{
	IIC_SCL_OUT;
	IIC_SDA_OUT;
	IIC_SCL_SET;
	IIC_SDA_SET;
}

/*******************************************************************************
 * @fn       SCCB_Start
 *
 * @brief    Start Signal
 *
 * @return   None
 */
void SCCB_Start(void)
{
	IIC_SDA_SET;
	IIC_SCL_SET;
	mDelayuS(50);
	IIC_SDA_CLR;
	mDelayuS(50);
	IIC_SCL_CLR;
}

/*******************************************************************************
 * @fn        SCCB_Stop
 *
 * @brief     Stop Signal
 *
 * @return    None
 */
void SCCB_Stop(void)
{
	IIC_SDA_CLR;
	mDelayuS(50);
	IIC_SCL_SET;
	mDelayuS(50);
	IIC_SDA_SET;
	mDelayuS(50);
}

/*******************************************************************************
 * @fn      SCCB_No_Ack
 *
 * @brief   NAK Signal
 *
 * @return  None
 */
void SCCB_No_Ack(void)
{
	mDelayuS(50);
	IIC_SDA_SET;
	IIC_SCL_SET;
	mDelayuS(50);
	IIC_SCL_CLR;
	mDelayuS(50);
	IIC_SDA_CLR;
	mDelayuS(50);
}

/*******************************************************************************
 * @fn        SCCB_WR_Byte
 * @brief     Write One Byte
 * @param     data
 * @return    0:�ɹ�
 *                                 ����:ʧ��
 */
UINT8 SCCB_WR_Byte(UINT8 data)
{
	UINT8 i,t;

	for(i=0; i<8; i++){
		if(data & 0x80){
			IIC_SDA_SET;
		}
		else{
		    IIC_SDA_CLR;
		}

		data <<= 1;
		mDelayuS(50);
		IIC_SCL_SET;
		mDelayuS(50);
		IIC_SCL_CLR;
	}

	IIC_SDA_IN;
	mDelayuS(50);
	IIC_SCL_SET;
	mDelayuS(50);

	if(SDA_IN_R)t=1;
	else t=0;

	IIC_SCL_CLR;
	IIC_SDA_OUT;

	return t;
}

/*******************************************************************************
 * @fn        SCCB_RD_Byte
 *
 * @brief     Read One Byte
 *
 * @return    Read one byte data
 */
UINT8 SCCB_RD_Byte(void)
{
	UINT8 t=0,i;

	IIC_SDA_IN;

	for(i=8; i>0; i--){
		mDelayuS(50);
		IIC_SCL_SET;
		t=t<<1;

		if(SDA_IN_R)t++;

		mDelayuS(50);
		IIC_SCL_CLR;
	}

	IIC_SDA_OUT;

	return t;
}

/*******************************************************************************
 * @fn        SCCB_WR_Reg
 *
 * @brief     Write camera Register
 *
 * @param     Reg_Adr - Register address
 *            Reg_Val - Register value
 *
 * @return    0 - �ɹ�
 *            ���� - ʧ��
 */
UINT8 SCCB_WR_Reg(UINT8 Reg_Adr, UINT8 Reg_Val)
{
	UINT8 res=0;

	SCCB_Start();
	if(SCCB_WR_Byte(SCCB_ID))res=1;
	mDelayuS(100);
  	if(SCCB_WR_Byte(Reg_Adr))res=1;
  	mDelayuS(100);
  	if(SCCB_WR_Byte(Reg_Val))res=1;
  	SCCB_Stop();

  	return	res;
}

/*******************************************************************************
 * @fn         SCCB_RD_Reg
 *
 * @brief      Read camera Register
 *
 * @return     Camera Register value
 */
UINT8 SCCB_RD_Reg(UINT8 Reg_Adr)
{
	UINT8 val=0;

	SCCB_Start();
	SCCB_WR_Byte(SCCB_ID);
	mDelayuS(100);
  	SCCB_WR_Byte(Reg_Adr);
  	mDelayuS(100);
	SCCB_Stop();
	mDelayuS(100);

	SCCB_Start();
	SCCB_WR_Byte(SCCB_ID|0X01);
	mDelayuS(100);
  	val=SCCB_RD_Byte();
  	SCCB_No_Ack();
  	SCCB_Stop();

  	return val;
}


/*******************************************************************************
 * @fn         OV2640_Init
 *
 * @brief      OV2640 ��ʼ��
 *
 * @return     0 - ��ʼ���ɹ�
 *             1 - ��ʼ��ʧ��
 */
UINT8 OV2640_Init(void)
{
	UINT16 i=0;
	UINT16 reg;

	//PA17:RESET  PA23:PWDN
	R32_PA_DIR |= (1<<17)|(1<<23);

	OV_PWDN_CLR;                //POWER ON
 	mDelaymS(10);
 	OV_RESET_CLR;				//Reset OV2640
	mDelaymS(10);
	OV_RESET_SET;				//Reset End

	SCCB_GPIO_Init();
	SCCB_WR_Reg(0xFF, 0x01);	//Register Bank Select Sensor address
 	SCCB_WR_Reg(0x12, 0x80);	//Reset All Register

 	mDelaymS(50);

	//Read MID
	reg=SCCB_RD_Reg(0x1C);	//MID_H
	reg<<=8;
	reg|=SCCB_RD_Reg(0x1D);	//MID_L

	if(reg!=OV2640_MID)
	{
		PRINT("MID:%08x\r\n",reg);
		return 1;
	}

	//Read PID
	reg=SCCB_RD_Reg(0x0A);	//PID_H
	reg<<=8;
	reg|=SCCB_RD_Reg(0x0B);	//PID_L

	if(reg!=OV2640_PID)
	{
		PRINT("PID:%08x\r\n",reg);
		return 2;
	}

	for(i=0;i<sizeof(OV2640_InitRegTbl)/2;i++)
	{
	   	SCCB_WR_Reg(OV2640_InitRegTbl[i][0],OV2640_InitRegTbl[i][1]);
 	}

  	return 0;
}

/*******************************************************************************
 * @fn       RGB565_Mode_Init
 *
 * @brief    RGB565 ģʽ��ʼ��
 *
 * @return   None
 */
void RGB565_Mode_Init(void)
{
	OV2640_RGB565_Mode();
	OV2640_OutSize_Set(OV2640_RGB565_WIDTH,OV2640_RGB565_HEIGHT);
	OV2640_Speed_Set(28,1);
}

/*******************************************************************************
 * @fn       JPEG_Mode_Init
 *
 * @brief    �� 0xFF��0xD8��ͷ�� 0xFF��0xD9��β ����1֡ͼƬ
 *
 * @return    None
 */
void JPEG_Mode_Init(void)
{
	OV2640_JPEG_Mode();
  	OV2640_OutSize_Set(OV2640_JPEG_WIDTH,OV2640_JPEG_HEIGHT);
  	OV2640_Speed_Set(30,1);
}

/*******************************************************************************
 * @fn       OV2640_JPEG_Mode
 *
 * @brief    JPEG Mode
 *
 * @return   None
 */
void OV2640_JPEG_Mode(void)
{
	UINT16 i=0;

	//YUV422
	for(i=0;i<(sizeof(OV2640_YUV422RegTbl)/2);i++)
	{
		SCCB_WR_Reg(OV2640_YUV422RegTbl[i][0],OV2640_YUV422RegTbl[i][1]);
	}
	//JPEG
	for(i=0;i<(sizeof(OV2640_JPEGRegTbl)/2);i++)
	{
		SCCB_WR_Reg(OV2640_JPEGRegTbl[i][0],OV2640_JPEGRegTbl[i][1]);
	}
}

/*******************************************************************************
 * @fn       OV2640_RGB565_Mode
 *
 * @brief    RGB565 Mode
 *
 * @return   None
 */
void OV2640_RGB565_Mode(void)
{
	UINT16 i=0;

	//RGB565
	for(i=0;i<(sizeof(OV2640_RGB565RegTbl)/2);i++)
	{
		SCCB_WR_Reg(OV2640_RGB565RegTbl[i][0],OV2640_RGB565RegTbl[i][1]);
	}
}

/*******************************************************************************
 * @fn           OV2640_OutSize_Set
 *
 * @briefSet     Image Resolution
 *
 * @param        Image_width -  ͼ���� (4�ı���)
 *               Image_height - ͼ��߶�
 *
 * @return       0 -�ɹ�
 *               ���� -ʧ��
 */
UINT8 OV2640_OutSize_Set(UINT16 Image_width, UINT16 Image_height)
{
	UINT16 Out_Size_Width;
	UINT16 Out_Size_Height;
	UINT8 t;

	if((Image_width%4) || (Image_height%4))return 0x01;

	Out_Size_Width=Image_width/4;
	Out_Size_Height=Image_height/4;

	SCCB_WR_Reg(0XFF,0X00);
	SCCB_WR_Reg(0XE0,0X04);
	SCCB_WR_Reg(0X5A,Out_Size_Width&0XFF);
	SCCB_WR_Reg(0X5B,Out_Size_Height&0XFF);
	t=(Out_Size_Width>>8)&0X03;
	t|=(Out_Size_Height>>6)&0X04;
	SCCB_WR_Reg(0X5C,t);
	SCCB_WR_Reg(0XE0,0X00);

	return 0;
}

/*******************************************************************************
 * @fn        OV2640_Speed_Set
 *
 * @brief     Set DVP PCLK
 *
 * @param     Pclk_Div - DVP output speed ctrl
 *            Xclk_Div - ���������Ƶ
 *
 * @return    0 -�ɹ�
 *            ���� - ʧ��
 */
void OV2640_Speed_Set(UINT8 Pclk_Div, UINT8 Xclk_Div)
{
	SCCB_WR_Reg(0XFF,0X00);
	SCCB_WR_Reg(0XD3,Pclk_Div);

	SCCB_WR_Reg(0XFF,0X01);
	SCCB_WR_Reg(0X11,Xclk_Div);
}



