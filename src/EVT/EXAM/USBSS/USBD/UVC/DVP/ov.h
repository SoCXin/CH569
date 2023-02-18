/********************************** (C) COPYRIGHT *******************************
* File Name          : ov.h
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 : OV2640 ����ͷ ���ú���
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/


#ifndef DVP_OV_H_
#define DVP_OV_H_


////PA17:RESET  PA23:PWDN
#define OV_RESET_SET    {R32_PA_OUT |= 1<<17;}  //�����
#define OV_RESET_CLR    {R32_PA_CLR |= 1<<17;}  //�����
#define OV_PWDN_SET     {R32_PA_OUT |= 1<<23;}  //�����
#define OV_PWDN_CLR     {R32_PA_CLR |= 1<<23;}  //�����


//////////////////////////////////////////////////////////////////////////////////
#define OV2640_MID				0X7FA2
#define OV2640_PID				0X2642


//��ѡ��DSP��ַ(0XFF=0X00)ʱ,OV2640��DSP�Ĵ�����ַӳ���
#define OV2640_DSP_R_BYPASS     0x05
#define OV2640_DSP_Qs           0x44
#define OV2640_DSP_CTRL         0x50
#define OV2640_DSP_HSIZE1       0x51
#define OV2640_DSP_VSIZE1       0x52
#define OV2640_DSP_XOFFL        0x53
#define OV2640_DSP_YOFFL        0x54
#define OV2640_DSP_VHYX         0x55
#define OV2640_DSP_DPRP         0x56
#define OV2640_DSP_TEST         0x57
#define OV2640_DSP_ZMOW         0x5A
#define OV2640_DSP_ZMOH         0x5B
#define OV2640_DSP_ZMHH         0x5C
#define OV2640_DSP_BPADDR       0x7C
#define OV2640_DSP_BPDATA       0x7D
#define OV2640_DSP_CTRL2        0x86
#define OV2640_DSP_CTRL3        0x87
#define OV2640_DSP_SIZEL        0x8C
#define OV2640_DSP_HSIZE2       0xC0
#define OV2640_DSP_VSIZE2       0xC1
#define OV2640_DSP_CTRL0        0xC2
#define OV2640_DSP_CTRL1        0xC3
#define OV2640_DSP_R_DVP_SP     0xD3
#define OV2640_DSP_IMAGE_MODE   0xDA
#define OV2640_DSP_RESET        0xE0
#define OV2640_DSP_MS_SP        0xF0
#define OV2640_DSP_SS_ID        0x7F
#define OV2640_DSP_SS_CTRL      0xF8
#define OV2640_DSP_MC_BIST      0xF9
#define OV2640_DSP_MC_AL        0xFA
#define OV2640_DSP_MC_AH        0xFB
#define OV2640_DSP_MC_D         0xFC
#define OV2640_DSP_P_STATUS     0xFE
#define OV2640_DSP_RA_DLMT      0xFF

//��ѡ�񴫸�����ַ(0XFF=0X01)ʱ,OV2640��DSP�Ĵ�����ַӳ���
#define OV2640_SENSOR_GAIN       0x00
#define OV2640_SENSOR_COM1       0x03
#define OV2640_SENSOR_REG04      0x04
#define OV2640_SENSOR_REG08      0x08
#define OV2640_SENSOR_COM2       0x09
#define OV2640_SENSOR_PIDH       0x0A
#define OV2640_SENSOR_PIDL       0x0B
#define OV2640_SENSOR_COM3       0x0C
#define OV2640_SENSOR_COM4       0x0D
#define OV2640_SENSOR_AEC        0x10
#define OV2640_SENSOR_CLKRC      0x11
#define OV2640_SENSOR_COM7       0x12
#define OV2640_SENSOR_COM8       0x13
#define OV2640_SENSOR_COM9       0x14
#define OV2640_SENSOR_COM10      0x15
#define OV2640_SENSOR_HREFST     0x17
#define OV2640_SENSOR_HREFEND    0x18
#define OV2640_SENSOR_VSTART     0x19
#define OV2640_SENSOR_VEND       0x1A
#define OV2640_SENSOR_MIDH       0x1C
#define OV2640_SENSOR_MIDL       0x1D
#define OV2640_SENSOR_AEW        0x24
#define OV2640_SENSOR_AEB        0x25
#define OV2640_SENSOR_W          0x26
#define OV2640_SENSOR_REG2A      0x2A
#define OV2640_SENSOR_FRARL      0x2B
#define OV2640_SENSOR_ADDVSL     0x2D
#define OV2640_SENSOR_ADDVHS     0x2E
#define OV2640_SENSOR_YAVG       0x2F
#define OV2640_SENSOR_REG32      0x32
#define OV2640_SENSOR_ARCOM2     0x34
#define OV2640_SENSOR_REG45      0x45
#define OV2640_SENSOR_FLL        0x46
#define OV2640_SENSOR_FLH        0x47
#define OV2640_SENSOR_COM19      0x48
#define OV2640_SENSOR_ZOOMS      0x49
#define OV2640_SENSOR_COM22      0x4B
#define OV2640_SENSOR_COM25      0x4E
#define OV2640_SENSOR_BD50       0x4F
#define OV2640_SENSOR_BD60       0x50
#define OV2640_SENSOR_REG5D      0x5D
#define OV2640_SENSOR_REG5E      0x5E
#define OV2640_SENSOR_REG5F      0x5F
#define OV2640_SENSOR_REG60      0x60
#define OV2640_SENSOR_HISTO_LOW  0x61
#define OV2640_SENSOR_HISTO_HIGH 0x62



UINT8 OV2640_Init(void);
void OV2640_JPEG_Mode(void);
void OV2640_RGB565_Mode(void);
void OV2640_Auto_Exposure(UINT8 level);
void OV2640_Light_Mode(UINT8 mode);
void OV2640_Color_Saturation(UINT8 sat);
void OV2640_Brightness(UINT8 bright);
void OV2640_Contrast(UINT8 contrast);
void OV2640_Special_Effects(UINT8 eft);
void OV2640_Color_Bar(UINT8 sw);
void OV2640_Window_Set(UINT16 sx,UINT16 sy,UINT16 width,UINT16 height);
UINT8 OV2640_OutSize_Set(UINT16 width,UINT16 height);
UINT8 OV2640_ImageWin_Set(UINT16 offx,UINT16 offy,UINT16 width,UINT16 height);
UINT8 OV2640_ImageSize_Set(UINT16 width,UINT16 height);
void ov2640_speed_ctrl(UINT8 pclkdiv, UINT8 clkdiv);

//Mode Init
void JPEG_Mode_Init(void);

#endif /* DVP_OV_C_ */
