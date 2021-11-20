/*
 * dvp.c
 *
 *  Created on: Aug 6, 2020
 *      Author: OWNER
 */
#include "dvp.h"
#include "UVCLIB.H"
void DVP_IRQHandler (void) __attribute__((interrupt("WCH-Interrupt-fast")));
__attribute__ ((aligned(16))) UINT8	JPEG_DVPDMAaddr0[512] __attribute__((section(".DMADATA")));
__attribute__ ((aligned(16))) UINT8	JPEG_DVPDMAaddr1[512] __attribute__((section(".DMADATA")));

/*******************************************************************************
* Function Name  : DVP_IRQHandler
* Description    : RGB565��ȡ����Ϊ ֡��ʼ��֡������� R16_DVP_ROW_NUM * R16_DVP_COL_NUM��
*                  JPEG��ȡ����Ϊ  ֡��ʼ �� ֡����  һ֡������ ȡ  0xFF��0xD8��ͷ�� 0xFF ,0xD9��β
* Input          : None
* Return         : None
*******************************************************************************/
void DVP_IRQHandler(void)
{
	DVP_Hander();
}
/*******************************************************************************
* Function Name  : dvp_Init
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void dvp_Init(void)
{
	R8_DVP_CR0 &= ~RB_DVP_MSK_DAT_MOD;
	//JPEG:VSYNC�ߵ�ƽ��Ч, HSYNC�ߵ�ƽ��Ч
	R8_DVP_CR0 |= RB_DVP_D8_MOD | RB_DVP_V_POLAR | RB_DVP_JPEG;
	R8_DVP_CR1 &= ~(RB_DVP_ALL_CLR| RB_DVP_RCV_CLR);   //�� ��λ��־
//	R16_DVP_ROW_NUM = 320;				// rows  ��JPEGģʽ��������
	R16_DVP_COL_NUM = 512;				// cols   ��JPEGģʽ�´��� DMA����

	//д ʵ�ʵ�ַ
	R32_DVP_DMA_BUF0 = (UINT32)JPEG_DVPDMAaddr0;		//DMA addr0
	R32_DVP_DMA_BUF1 =  (UINT32)JPEG_DVPDMAaddr1;		//DMA addr1

    //Interupt Enable
	R8_DVP_INT_EN |= RB_DVP_IE_STP_FRM;
//	R8_DVP_INT_EN |= RB_DVP_IE_FIFO_OV;
	R8_DVP_INT_EN |= RB_DVP_IE_FRM_DONE;
	R8_DVP_INT_EN |= RB_DVP_IE_ROW_DONE;
	R8_DVP_INT_EN |= RB_DVP_IE_STR_FRM;

	PFIC_EnableIRQ(DVP_IRQn);		//enable DVP interrupt
	R8_DVP_CR1 |= RB_DVP_DMA_EN;  //enable DMA
	R8_DVP_CR0 |= RB_DVP_ENABLE;  //enable DVP
}
/*******************************************************************************
* Function Name  : SCCB_Init
* Description    : ��ʼ��SCCB�ӿ�
* Input          : None
* Return         : None
*******************************************************************************/
void SCCB_Init(void)
{
	//OUT
	IIC_SCL_OUT;  //SCL �������
	IIC_SDA_OUT;  //SDA �������

	IIC_SCL_SET;  //SCL=1
	IIC_SDA_SET;  //SDA=1
}


/*******************************************************************************
* Function Name  : SCCB_Start
* Description    : ��ʱ��Ϊ�ߵ�ʱ��,�����ߵĸߵ���,ΪSCCB��ʼ�ź�
* �ڼ���״̬��,SDA��SCL��Ϊ�͵�ƽ
* Input          : None
* Return         : None
*******************************************************************************/
void SCCB_Start(void)
{
	IIC_SDA_SET;  //SDA=1
	IIC_SCL_SET;  //SCL=1
	mDelayuS(50);
	IIC_SDA_CLR;  //SDA=0
	mDelayuS(50);
	IIC_SCL_CLR;  //SCL=0
}

/*******************************************************************************
* Function Name  : SCCB_Stop
* Description    : ��ʱ��Ϊ�ߵ�ʱ��,�����ߵĵ͵���,ΪSCCBֹͣ�ź�
* Input          : None
* Return         : None
*******************************************************************************/
void SCCB_Stop(void)
{
	IIC_SDA_CLR;  //SDA=0
	mDelayuS(50);
	IIC_SCL_SET;  //SCL=1
	mDelayuS(50);
	IIC_SDA_SET;  //SDA=1
	mDelayuS(50);
}

/*******************************************************************************
* Function Name  : SCCB_No_Ack
* Description    : ����NA�ź�
* Input          : None
* Return         : None
*******************************************************************************/
void SCCB_No_Ack(void)
{
	mDelayuS(50);
	IIC_SDA_SET;  //SDA=1
	IIC_SCL_SET;  //SCL=1
	mDelayuS(50);
	IIC_SCL_CLR;  //SCL=0
	mDelayuS(50);
	IIC_SDA_CLR;  //SDA=0
	mDelayuS(50);
}

/*******************************************************************************
* Function Name  : SCCB_WR_Byte
* Description    : SCCB,д��һ���ֽ�
* Input          : None
* Return         : ����ֵ:0,�ɹ�;1,ʧ��.
*******************************************************************************/
UINT8 SCCB_WR_Byte(UINT8 dat)
{
	UINT8 j,res;

	for(j=0;j<8;j++) //ѭ��8�η�������
	{
		if(dat&0x80){
			IIC_SDA_SET;
		}
		else{
		    IIC_SDA_CLR;
		}

		dat<<=1;
		mDelayuS(50);
		IIC_SCL_SET;
		mDelayuS(50);
		IIC_SCL_CLR;
	}

	IIC_SDA_IN;		//����SDAΪ����

	mDelayuS(50);
	IIC_SCL_SET;			//���յھ�λ,���ж��Ƿ��ͳɹ�
	mDelayuS(50);
	if(SDA_IN_R)res=1;  //SDA=1����ʧ�ܣ�����1
	else res=0;         //SDA=0���ͳɹ�������0
	IIC_SCL_CLR;

	IIC_SDA_OUT;		//����SDAΪ���
	return res;

}

/*******************************************************************************
* Function Name  : SCCB_RD_Byte
* Description    : SCCB ��ȡһ���ֽ�
* Input          : None
* Return         : ����ֵ:����������
*******************************************************************************/
UINT8 SCCB_RD_Byte(void)
{
	UINT8 temp=0,j;

	IIC_SDA_IN;		//����SDAΪ����

	for(j=8;j>0;j--) 	//ѭ��8�ν�������
	{
		mDelayuS(50);
		IIC_SCL_SET;
		temp=temp<<1;
		if(SDA_IN_R)temp++;
		mDelayuS(50);
		IIC_SCL_CLR;
	}

	IIC_SDA_OUT;		//����SDAΪ���

	return temp;
}

/*******************************************************************************
* Function Name  : SCCB_WR_Reg
* Description    : д ����ͷ �Ĵ���
* Input          : None
* Return         : ����ֵ:0,�ɹ�;1,ʧ��
*******************************************************************************/
UINT8 SCCB_WR_Reg(UINT8 reg,UINT8 data)
{
	UINT8 res=0;

	SCCB_Start(); 					//����SCCB����
	if(SCCB_WR_Byte(SCCB_ID))res=1;	//д����ID
	mDelayuS(100);
  	if(SCCB_WR_Byte(reg))res=1;		//д�Ĵ�����ַ
  	mDelayuS(100);
  	if(SCCB_WR_Byte(data))res=1; 	//д����
  	SCCB_Stop();
  	return	res;
}

/*******************************************************************************
* Function Name  : SCCB_RD_Reg
* Description    : �� ����ͷ �Ĵ���
* Input          : None
* Return         : ����ֵ:�����ļĴ���ֵ
*******************************************************************************/
UINT8 SCCB_RD_Reg(UINT8 reg)
{
	UINT8 val=0;
	SCCB_Start(); 				//����SCCB����
	SCCB_WR_Byte(SCCB_ID);		//д����ID
	mDelayuS(100);
  	SCCB_WR_Byte(reg);			//д�Ĵ�����ַ
  	mDelayuS(100);
	SCCB_Stop();
	mDelayuS(100);
	//���üĴ�����ַ�󣬲��Ƕ�
	SCCB_Start();
	SCCB_WR_Byte(SCCB_ID|0X01);	//���Ͷ�����
	mDelayuS(100);
  	val=SCCB_RD_Byte();		 	//��ȡ����
  	SCCB_No_Ack();
  	SCCB_Stop();
  	return val;
}


