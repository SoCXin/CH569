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
* Description    : RGB565：取数据为 帧起始到帧接收完成 R16_DVP_ROW_NUM * R16_DVP_COL_NUM。
*                  JPEG：取数据为  帧起始 到 帧结束  一帧数据中 取  0xFF，0xD8开头； 0xFF ,0xD9结尾
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
	//JPEG:VSYNC高电平有效, HSYNC高电平有效
	R8_DVP_CR0 |= RB_DVP_D8_MOD | RB_DVP_V_POLAR | RB_DVP_JPEG;
	R8_DVP_CR1 &= ~(RB_DVP_ALL_CLR| RB_DVP_RCV_CLR);   //清 复位标志
//	R16_DVP_ROW_NUM = 320;				// rows  在JPEG模式下无意义
	R16_DVP_COL_NUM = 512;				// cols   在JPEG模式下代表 DMA长度

	//写 实际地址
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
* Description    : 初始化SCCB接口
* Input          : None
* Return         : None
*******************************************************************************/
void SCCB_Init(void)
{
	//OUT
	IIC_SCL_OUT;  //SCL 方向输出
	IIC_SDA_OUT;  //SDA 方向输出

	IIC_SCL_SET;  //SCL=1
	IIC_SDA_SET;  //SDA=1
}


/*******************************************************************************
* Function Name  : SCCB_Start
* Description    : 当时钟为高的时候,数据线的高到低,为SCCB起始信号
* 在激活状态下,SDA和SCL均为低电平
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
* Description    : 当时钟为高的时候,数据线的低到高,为SCCB停止信号
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
* Description    : 产生NA信号
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
* Description    : SCCB,写入一个字节
* Input          : None
* Return         : 返回值:0,成功;1,失败.
*******************************************************************************/
UINT8 SCCB_WR_Byte(UINT8 dat)
{
	UINT8 j,res;

	for(j=0;j<8;j++) //循环8次发送数据
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

	IIC_SDA_IN;		//设置SDA为输入

	mDelayuS(50);
	IIC_SCL_SET;			//接收第九位,以判断是否发送成功
	mDelayuS(50);
	if(SDA_IN_R)res=1;  //SDA=1发送失败，返回1
	else res=0;         //SDA=0发送成功，返回0
	IIC_SCL_CLR;

	IIC_SDA_OUT;		//设置SDA为输出
	return res;

}

/*******************************************************************************
* Function Name  : SCCB_RD_Byte
* Description    : SCCB 读取一个字节
* Input          : None
* Return         : 返回值:读到的数据
*******************************************************************************/
UINT8 SCCB_RD_Byte(void)
{
	UINT8 temp=0,j;

	IIC_SDA_IN;		//设置SDA为输入

	for(j=8;j>0;j--) 	//循环8次接收数据
	{
		mDelayuS(50);
		IIC_SCL_SET;
		temp=temp<<1;
		if(SDA_IN_R)temp++;
		mDelayuS(50);
		IIC_SCL_CLR;
	}

	IIC_SDA_OUT;		//设置SDA为输出

	return temp;
}

/*******************************************************************************
* Function Name  : SCCB_WR_Reg
* Description    : 写 摄像头 寄存器
* Input          : None
* Return         : 返回值:0,成功;1,失败
*******************************************************************************/
UINT8 SCCB_WR_Reg(UINT8 reg,UINT8 data)
{
	UINT8 res=0;

	SCCB_Start(); 					//启动SCCB传输
	if(SCCB_WR_Byte(SCCB_ID))res=1;	//写器件ID
	mDelayuS(100);
  	if(SCCB_WR_Byte(reg))res=1;		//写寄存器地址
  	mDelayuS(100);
  	if(SCCB_WR_Byte(data))res=1; 	//写数据
  	SCCB_Stop();
  	return	res;
}

/*******************************************************************************
* Function Name  : SCCB_RD_Reg
* Description    : 读 摄像头 寄存器
* Input          : None
* Return         : 返回值:读到的寄存器值
*******************************************************************************/
UINT8 SCCB_RD_Reg(UINT8 reg)
{
	UINT8 val=0;
	SCCB_Start(); 				//启动SCCB传输
	SCCB_WR_Byte(SCCB_ID);		//写器件ID
	mDelayuS(100);
  	SCCB_WR_Byte(reg);			//写寄存器地址
  	mDelayuS(100);
	SCCB_Stop();
	mDelayuS(100);
	//设置寄存器地址后，才是读
	SCCB_Start();
	SCCB_WR_Byte(SCCB_ID|0X01);	//发送读命令
	mDelayuS(100);
  	val=SCCB_RD_Byte();		 	//读取数据
  	SCCB_No_Ack();
  	SCCB_Stop();
  	return val;
}


