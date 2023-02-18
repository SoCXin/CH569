/********************************** (C) COPYRIGHT *******************************
* File Name          : ov.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 : OV2640 ����ͷ ���ú���
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

#include "dvp.h"
#include "ov.h"
#include "ov2640cfg.h"
#include "CH56xSFR.h"


/*********************************************************************
 * @fn      OV2640_Init
 *
 * @brief   Init OV2640
 *
 * @return  0 - Success
 *          1 - Err
 */
UINT8 OV2640_Init(void)
{
	UINT16 i=0;
	UINT16 reg;

	//PA17:RESET  PA23:PWDN
	R32_PA_DIR |= (1<<17)|(1<<23);

	OV_PWDN_CLR;        //POWER ON
	mDelaymS(10);
 	OV_RESET_CLR;				//��λOV2640
 	mDelaymS(10);
	OV_RESET_SET;				//������λ

  	SCCB_Init();        		//��ʼ��SCCB ��IO��
	SCCB_WR_Reg(OV2640_DSP_RA_DLMT, 0x01);	//����sensor�Ĵ���
 	SCCB_WR_Reg(OV2640_SENSOR_COM7, 0x80);	//��λOV2640

 	mDelaymS(50);
	reg=SCCB_RD_Reg(OV2640_SENSOR_MIDH);	//��ȡ����ID �߰�λ
	reg<<=8;
	reg|=SCCB_RD_Reg(OV2640_SENSOR_MIDL);	//��ȡ����ID �Ͱ�λ

	if(reg!=OV2640_MID)
	{
		printf("MID:%d\r\n",reg);
		return 1;
	}

	reg=SCCB_RD_Reg(OV2640_SENSOR_PIDH);	//��ȡ����ID �߰�λ
	reg<<=8;
	reg|=SCCB_RD_Reg(OV2640_SENSOR_PIDL);	//��ȡ����ID �Ͱ�λ

	if(reg!=OV2640_PID)
	{
		printf("HID:%d\r\n",reg);
		//return 2;
	}
 	//��ʼ�� OV2640,����SXGA�ֱ���(1600*1200)
	for(i=0;i<sizeof(ov2640_uxga_init_reg_tbl)/2;i++)
	{
	   	SCCB_WR_Reg(ov2640_uxga_init_reg_tbl[i][0],ov2640_uxga_init_reg_tbl[i][1]);
 	}

  	return 0x00; 	//ok
}


/*******************************************************************************
 * @fn     JPEG_Mode_Init
 *
 * @brief   �� 0xFF��0xD8��ͷ�� 0xFF ,0xD9��β
 *
 * @return   None
 */
void JPEG_Mode_Init(void)
{
	UINT8 res=0;
	UINT32 bwr;
	UINT32 i=0;
	UINT32 jpeglen=0;
	UINT8* pbuf;

	OV2640_JPEG_Mode();							//�л�ΪJPEGģʽ
  	OV2640_OutSize_Set(OV2640_JPEG_WIDTH,OV2640_JPEG_HEIGHT);

//	ov2640_speed_ctrl(30,1);  //����ٶȣ�ʱ������˭
 	ov2640_speed_ctrl(36,0);  //����ٶȣ�ʱ������˭
}

/*******************************************************************************
 * @fn     OV2640_JPEG_Mode
 * @brief   OV2640�л�ΪJPEGģʽ
 * @param   None
 * @return   None
 */
void OV2640_JPEG_Mode(void)
{
	UINT16 i=0;
	//����:YUV422��ʽ
	for(i=0;i<(sizeof(ov2640_yuv422_reg_tbl)/2);i++)
	{
		SCCB_WR_Reg(ov2640_yuv422_reg_tbl[i][0],ov2640_yuv422_reg_tbl[i][1]);
	}
	//����:���JPEG����
	for(i=0;i<(sizeof(ov2640_jpeg_reg_tbl)/2);i++)
	{
		SCCB_WR_Reg(ov2640_jpeg_reg_tbl[i][0],ov2640_jpeg_reg_tbl[i][1]);
	}
}

/* Start Camera list of initialization configuration registers */
const static UINT8 OV2640_AUTOEXPOSURE_LEVEL[5][8]=
{
	{
		0xFF,0x01,
		0x24,0x20,
		0x25,0x18,
		0x26,0x60,
	},
	{
		0xFF,0x01,
		0x24,0x34,
		0x25,0x1c,
		0x26,0x00,
	},
	{
		0xFF,0x01,
		0x24,0x3e,
		0x25,0x38,
		0x26,0x81,
	},
	{
		0xFF,0x01,
		0x24,0x48,
		0x25,0x40,
		0x26,0x81,
	},
	{
		0xFF,0x01,
		0x24,0x58,
		0x25,0x50,
		0x26,0x92,
	},
};


/*******************************************************************************
 * @fn       OV2640_Auto_Exposure
 *
 * @brief    �Զ��ع�ȼ�����
 *
 * @param    level - 0~4.
 *
 * @return   None
 */
void OV2640_Auto_Exposure(UINT8 level)
{
	UINT8 i;
	UINT8 *p=(UINT8*)OV2640_AUTOEXPOSURE_LEVEL[level];
	for(i=0;i<4;i++)
	{
		SCCB_WR_Reg(p[i*2],p[i*2+1]);
	}
}


/*******************************************************************************
 * @fn       OV2640_Light_Mode
 *
 * @brief    ��ƽ������
 *
 * @param    mode -
 *             0 - �Զ�
 *             1 - ̫��sunny
 *             2 - ����cloudy
 *             3 - �칫��office
 *             4 - ����home
 *
 * @return   None
 */
void OV2640_Light_Mode(UINT8 mode)
{
	UINT8 regccval=0X5E;//Sunny
	UINT8 regcdval=0X41;
	UINT8 regceval=0X54;
	switch(mode)
	{
		case 0://auto
			SCCB_WR_Reg(0XFF,0X00);
			SCCB_WR_Reg(0XC7,0X00);//AWB ON
			return;
		case 2://cloudy
			regccval=0X65;
			regcdval=0X41;
			regceval=0X4F;
			break;
		case 3://office
			regccval=0X52;
			regcdval=0X41;
			regceval=0X66;
			break;
		case 4://home
			regccval=0X42;
			regcdval=0X3F;
			regceval=0X71;
			break;
	}
	SCCB_WR_Reg(0XFF,0X00);
	SCCB_WR_Reg(0XC7,0X40);	//AWB OFF
	SCCB_WR_Reg(0XCC,regccval);
	SCCB_WR_Reg(0XCD,regcdval);
	SCCB_WR_Reg(0XCE,regceval);
}


/*******************************************************************************
 * @fn       OV2640_Color_Saturation
 *
 * @brief    ɫ������
 *
 * @param    sat-
 *             0 - -2
 *             1 - -1
 *             2 - 0
 *             3 - +1
 *             4 - +2
 *
 * @return   None
 */
void OV2640_Color_Saturation(UINT8 sat)
{
	UINT8 reg7dval=((sat+2)<<4)|0X08;
	SCCB_WR_Reg(0XFF,0X00);
	SCCB_WR_Reg(0X7C,0X00);
	SCCB_WR_Reg(0X7D,0X02);
	SCCB_WR_Reg(0X7C,0X03);
	SCCB_WR_Reg(0X7D,reg7dval);
	SCCB_WR_Reg(0X7D,reg7dval);
}


/*******************************************************************************
 * @fn       OV2640_Brightness
 *
 * @brief    ��������
 *
 * @param    bright -
 *             0 - (0X00)-2
 *             1 - (0X10)-1
 *             2 - (0X20) 0
 *             3 - (0X30)+1
 *             4 - (0X40)+2
 *
 * @return   None
 */
void OV2640_Brightness(UINT8 bright)
{
  SCCB_WR_Reg(0xff, 0x00);
  SCCB_WR_Reg(0x7c, 0x00);
  SCCB_WR_Reg(0x7d, 0x04);
  SCCB_WR_Reg(0x7c, 0x09);
  SCCB_WR_Reg(0x7d, bright<<4);
  SCCB_WR_Reg(0x7d, 0x00);
}


/*******************************************************************************
 * @fn       OV2640_Contrast
 *
 * @brief    �Աȶ�����
 *
 * @param    contrast - 
 *             0 - -2
 *             1 - -1
 *             2 - 0
 *             3 - +1
 *             4 - +2
 *
 * @return   None
 */
void OV2640_Contrast(UINT8 contrast)
{
	UINT8 reg7d0val=0X20;//Ĭ��Ϊ��ͨģʽ
	UINT8 reg7d1val=0X20;
  	switch(contrast)
	{
		case 0://-2
			reg7d0val=0X18;
			reg7d1val=0X34;
			break;
		case 1://-1
			reg7d0val=0X1C;
			reg7d1val=0X2A;
			break;
		case 3://1
			reg7d0val=0X24;
			reg7d1val=0X16;
			break;
		case 4://2
			reg7d0val=0X28;
			reg7d1val=0X0C;
			break;
	}
	SCCB_WR_Reg(0xff,0x00);
	SCCB_WR_Reg(0x7c,0x00);
	SCCB_WR_Reg(0x7d,0x04);
	SCCB_WR_Reg(0x7c,0x07);
	SCCB_WR_Reg(0x7d,0x20);
	SCCB_WR_Reg(0x7d,reg7d0val);
	SCCB_WR_Reg(0x7d,reg7d1val);
	SCCB_WR_Reg(0x7d,0x06);
}


/*******************************************************************************
 * @fn       OV2640_Special_Effects
 *
 * @brief    ��Ч����
 *
 * @param    eft -
 *             0 - ��ͨģʽ
 *             1 - ��Ƭ
 *             2 - �ڰ�
 *             3 - ƫ��ɫ
 *             4 - ƫ��ɫ
 *             5 - ƫ��ɫ
 *             6 - ����
 *
 * @return   None
 */
void OV2640_Special_Effects(UINT8 eft)
{
	UINT8 reg7d0val=0X00;//Ĭ��Ϊ��ͨģʽ
	UINT8 reg7d1val=0X80;
	UINT8 reg7d2val=0X80;
	switch(eft)
	{
		case 1://��Ƭ
			reg7d0val=0X40;
			break;
		case 2://�ڰ�
			reg7d0val=0X18;
			break;
		case 3://ƫ��ɫ
			reg7d0val=0X18;
			reg7d1val=0X40;
			reg7d2val=0XC0;
			break;
		case 4://ƫ��ɫ
			reg7d0val=0X18;
			reg7d1val=0X40;
			reg7d2val=0X40;
			break;
		case 5://ƫ��ɫ
			reg7d0val=0X18;
			reg7d1val=0XA0;
			reg7d2val=0X40;
			break;
		case 6://����
			reg7d0val=0X18;
			reg7d1val=0X40;
			reg7d2val=0XA6;
			break;
	}
	SCCB_WR_Reg(0xff,0x00);
	SCCB_WR_Reg(0x7c,0x00);
	SCCB_WR_Reg(0x7d,reg7d0val);
	SCCB_WR_Reg(0x7c,0x05);
	SCCB_WR_Reg(0x7d,reg7d1val);
	SCCB_WR_Reg(0x7d,reg7d2val);
}


/*******************************************************************************
 * @fn       OV2640_Color_Bar
 *
 * @brief    ��������
 *
 * @param    sw -
 *            0 - �رղ���
 *            1 - ��������
 *
 * @return   None
 */
void OV2640_Color_Bar(UINT8 sw)
{
	UINT8 reg;
	SCCB_WR_Reg(0XFF,0X01);
	reg=SCCB_RD_Reg(0X12);
	reg&=~(1<<1);
	if(sw)reg|=1<<1;
	SCCB_WR_Reg(0X12,reg);
}

/*******************************************************************************
 * @fn       OV2640_Window_Set
 *
 * @brief    ���ô������������
 *
 * @param    sx,sy - ��ʼ��ַ
 *           width,height - ���(��Ӧ:horizontal)�͸߶�(��Ӧ:vertical)
 *
 * @return   None
 */
void OV2640_Window_Set(UINT16 sx,UINT16 sy,UINT16 width,UINT16 height)
{
	UINT16 endx;
	UINT16 endy;
	UINT8 temp;
	endx=sx+width/2;	//V*2
 	endy=sy+height/2;

	SCCB_WR_Reg(0XFF,0X01);
	temp=SCCB_RD_Reg(0X03);				//��ȡVref֮ǰ��ֵ
	temp&=0XF0;
	temp|=((endy&0X03)<<2)|(sy&0X03);
	SCCB_WR_Reg(0X03,temp);				//����Vref��start��end�����2λ
	SCCB_WR_Reg(0X19,sy>>2);			//����Vref��start��8λ
	SCCB_WR_Reg(0X1A,endy>>2);			//����Vref��end�ĸ�8λ

	temp=SCCB_RD_Reg(0X32);				//��ȡHref֮ǰ��ֵ
	temp&=0XC0;
	temp|=((endx&0X07)<<3)|(sx&0X07);
	SCCB_WR_Reg(0X32,temp);				//����Href��start��end�����3λ
	SCCB_WR_Reg(0X17,sx>>3);			//����Href��start��8λ
	SCCB_WR_Reg(0X18,endx>>3);			//����Href��end�ĸ�8λ
}

/*******************************************************************************
 * @fn       ����ͼ�������С
 *
 * @brief    OV2640���ͼ��Ĵ�С(�ֱ���)
 *
 * @param    width,height - ���(horizontal)�͸߶�(vertical),width��height������4�ı���         
 *
 * @return   0 - Success
 *           1 - Error
 */
UINT8 OV2640_OutSize_Set(UINT16 width,UINT16 height)
{
	UINT16 outh;
	UINT16 outw;
	UINT8 temp;
	if(width%4)return 1;
	if(height%4)return 2;
	outw=width/4;
	outh=height/4;
	SCCB_WR_Reg(0XFF,0X00);
	SCCB_WR_Reg(0XE0,0X04);
	SCCB_WR_Reg(0X5A,outw&0XFF);		//����OUTW�ĵͰ�λ
	SCCB_WR_Reg(0X5B,outh&0XFF);		//����OUTH�ĵͰ�λ
	temp=(outw>>8)&0X03;
	temp|=(outh>>6)&0X04;
	SCCB_WR_Reg(0X5C,temp);				//����OUTH/OUTW�ĸ�λ
	SCCB_WR_Reg(0XE0,0X00);
	return 0;
}

/*******************************************************************************
 * @fn       OV2640_ImageWin_Set
 *
 * @brief    ����ͼ�񿪴���С
 *
 * @param    offx,offy - ��ʼ��ַ
 *           width,height - ���(��Ӧ:horizontal)�͸߶�(��Ӧ:vertical)
 *
 * @return   None
 */
UINT8 OV2640_ImageWin_Set(UINT16 offx,UINT16 offy,UINT16 width,UINT16 height)
{
	UINT16 hsize;
	UINT16 vsize;
	UINT8 temp;
	if(width%4)return 1;
	if(height%4)return 2;
	hsize=width/4;
	vsize=height/4;
	SCCB_WR_Reg(0XFF,0X00);
	SCCB_WR_Reg(0XE0,0X04);
	SCCB_WR_Reg(0X51,hsize&0XFF);		//����H_SIZE�ĵͰ�λ
	SCCB_WR_Reg(0X52,vsize&0XFF);		//����V_SIZE�ĵͰ�λ
	SCCB_WR_Reg(0X53,offx&0XFF);		//����offx�ĵͰ�λ
	SCCB_WR_Reg(0X54,offy&0XFF);		//����offy�ĵͰ�λ
	temp=(vsize>>1)&0X80;
	temp|=(offy>>4)&0X70;
	temp|=(hsize>>5)&0X08;
	temp|=(offx>>8)&0X07;
	SCCB_WR_Reg(0X55,temp);				//����H_SIZE/V_SIZE/OFFX,OFFY�ĸ�λ
	SCCB_WR_Reg(0X57,(hsize>>2)&0X80);	//����H_SIZE/V_SIZE/OFFX,OFFY�ĸ�λ
	SCCB_WR_Reg(0XE0,0X00);
	return 0;
}

/*******************************************************************************
 * @fn       OV2640_ImageSize_Set
 *
 * @brief    Image Resolution
 *
 * @param    Image_width - ͼ���� (4�ı���)
 *           Image_height - ͼ��߶�
 *
 * @return   0 - Success
 *           1 - Error
 */
UINT8 OV2640_ImageSize_Set(UINT16 width,UINT16 height)
{
	UINT8 temp;
	SCCB_WR_Reg(0XFF,0X00);
	SCCB_WR_Reg(0XE0,0X04);
	SCCB_WR_Reg(0XC0,(width)>>3&0XFF);		//����HSIZE��10:3λ
	SCCB_WR_Reg(0XC1,(height)>>3&0XFF);		//����VSIZE��10:3λ
	temp=(width&0X07)<<3;
	temp|=height&0X07;
	temp|=(width>>4)&0X80;
	SCCB_WR_Reg(0X8C,temp);
	SCCB_WR_Reg(0XE0,0X00);
	return 0;
}

/*******************************************************************************
 * @fn        ov2640_speed_ctrl
 *
 * @brief     Set DVP PCLK
 *
 * @param     Pclk_Div: DVP output speed ctrl
 *            Xclk_Div: ���������Ƶ
 *
 * @return    none
 */

void ov2640_speed_ctrl(UINT8 pclkdiv, UINT8 clkdiv)
{
	SCCB_WR_Reg(0XFF,0X00);
	SCCB_WR_Reg(0XD3,pclkdiv);	//����PCLK��Ƶ
	SCCB_WR_Reg(0XFF,0X01);
	SCCB_WR_Reg(0X11,clkdiv);	//����CLK��Ƶ
}

