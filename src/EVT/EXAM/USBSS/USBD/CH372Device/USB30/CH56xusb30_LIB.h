/********************************** (C) COPYRIGHT *******************************
* File Name          : CH56x_usb30_lib.h
* Author             : WCH
* Version            : V1.1
* Date               : 2020/12/23
* Description        :
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
*******************************************************************************/
#ifndef USB30_CH56X_USB30_LIB_H_
#define USB30_CH56X_USB30_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "CH56x_common.h"

// link CFG
#define TERM_EN                 (1<<1)
#define PIPE_RESET              (1<<3)
#define LFPS_RX_PD              (1<<5)
#define CFG_EQ_EN               (1<<6)
#define DEEMPH_CFG              (1<<8)

#define POWER_MODE_0            ((UINT32)0x00000000)
#define POWER_MODE_1            ((UINT32)0x00000001)
#define POWER_MODE_2            ((UINT32)0x00000002)
#define POWER_MODE_3            ((UINT32)0x00000003)

#define LINK_PRESENT            (1<<0)
#define RX_WARM_RESET           ((UINT32)1<<1)

#define LINK_TXEQ               (1<<6)
#define GO_DISABLED             (1<<4)
#define POLLING_EN              (1<<12)

#define TX_HOT_RESET            ((UINT32)1<<16)
#define RX_HOT_RESET            ((UINT32)1<<24)

#define TX_WARM_RESET           ((UINT32)1<<8)
#define TX_Ux_EXIT              ((UINT32)1<<9)
// link int flag
#define LINK_RDY_FLAG           (1<<0)
#define LINK_RECOV_FLAG         (1<<1)
#define LINK_INACT_FLAG         (1<<2)
#define LINK_DISABLE_FLAG       (1<<3)
#define LINK_GO_U3_FLAG         (1<<4)
#define LINK_GO_U2_FLAG         (1<<5)
#define LINK_GO_U1_FLAG         (1<<6)
#define LINK_GO_U0_FLAG         (1<<7)
#define LINK_U3_WAKE_FLAG       (1<<8)
#define LINK_Ux_REJECT_FLAG     (1<<9)
#define TERM_PRESENT_FLAG       (1<<10)
#define LINK_TXEQ_FLAG          (1<<11)
#define LINK_Ux_EXIT_FLAG       (1<<12)
#define WARM_RESET_FLAG         (1<<13)
#define U3_WAKEUP_FLAG          (1<<14)
#define HOT_RESET_FLAG          (1<<15)
#define LINK_RX_DET_FLAG        (1<<20)

#define EP0_R_EN                (1<<0)
#define EP1_R_EN                (1<<1)
#define EP2_R_EN                (1<<2)
#define EP3_R_EN                (1<<3)
#define EP4_R_EN                (1<<4)
#define EP5_R_EN                (1<<5)
#define EP6_R_EN                (1<<6)
#define EP7_R_EN                (1<<7)

#define EP0_T_EN                (1<<8)
#define EP1_T_EN                (1<<9)
#define EP2_T_EN                (1<<10)
#define EP3_T_EN                (1<<11)
#define EP4_T_EN                (1<<12)
#define EP5_T_EN                (1<<13)
#define EP6_T_EN                (1<<14)
#define EP7_T_EN                (1<<15)

#define USB_FORCE_RST           (1<<2)
#define USB_ALL_CLR             (1<<1)
// LMP
#define LMP_HP                  0
#define LMP_SUBTYPE_MASK        (0xf<<5)
#define SET_LINK_FUNC           (0x1<<5)
#define U2_INACT_TOUT           (0x2<<5)
#define VENDOR_TEST             (0x3<<5)
#define PORT_CAP                (0x4<<5)
#define PORT_CFG                (0x5<<5)
#define PORT_CFG_RES            (0x6<<5)

#define LINK_SPEED              (1<<9)

#define NUM_HP_BUF              (4<<0)
#define DOWN_STREAM             (1<<16)
#define UP_STREAM               (2<<16)
#define TIE_BRK                 (1<<20)

/**********device status***********/
typedef enum _DEVICE_STATE
{
  UNCONNECTED,
  ATTACHED,
  POWERED,
  SUSPENDED,
  ADDRESSED,
  CONFIGURED
} DEVICE_STATE;
/*********************/
typedef union
{
  UINT16 w;
  struct BW
  {
    UINT8 bb1; //���ֽ�
    UINT8 bb0;
  }
  bw;
} UINT16_UINT8;
/**********standard request command***********/
typedef struct __PACKED
{
    UINT8       bRequestType;
    UINT8       bRequest;
    UINT16_UINT8 wValue;
    UINT16_UINT8 wIndex;
    UINT16       wLength;
} *PUSB_SETUP;

#define UsbSetupBuf     ((PUSB_SETUP)endp0RTbuff)//�˵�0
#define ENDP0_MAXPACK       512

// status response
#define NRDY                0
#define ACK                 0x01
#define STALL               0x02
#define INVALID             0x03

// number of NUMP
#define NUMP_0              0x00
#define NUMP_1              0x01
#define NUMP_2              0x02
#define NUMP_3              0x03
#define NUMP_4              0x04
#define NUMP_5              0x05
#define NUMP_6              0x06


/* USB endpoint direction */
#define OUT                 0x00
#define IN                  0x80
/* USB endpoint serial number */
#define ENDP_0              0x00
#define ENDP_1              0x01
#define ENDP_2              0x02
#define ENDP_3              0x03
#define ENDP_4              0x04
#define ENDP_5              0x05
#define ENDP_6              0x06
#define ENDP_7              0x07

#define USB_DESCR_TYP_BOS       0x0f
#define USB_DESCR_UNSUPPORTED   0xffff
#define INVALID_REQ_CODE 0xFF

/* string descriptor type */
#ifndef USB_DESCR_STRING
    #define USB_DESCR_LANGID_STRING    0x00
    #define USB_DESCR_VENDOR_STRING    0x01
    #define USB_DESCR_PRODUCT_STRING   0x02
    #define USB_DESCR_SERIAL_STRING    0x03
    #define USB_DESCR_OS_STRING        0xee
#endif

/*******************************************************************************
 * @fn      USB30_Device_Init
 *
 * @brief   USB3.0 Device��ʼ��
 *
 * @return   None
 */
extern UINT8 USB30_Device_Init(void);

/*******************************************************************************
 * @fn      USB30_Lib_Getversion
 *
 * @brief   USB3.0 Device�ӳ�����ʼ��
 *
 * @return   None
 */
extern UINT8 USB30_Lib_Getversion(void);

/*******************************************************************************
 * @fn      USB30_ISO_Setendp
 *
 * @brief   ����ͬ���˵�
 *
 * @return   None
 */
extern void USB30_ISO_Setendp(UINT8 num,FunctionalState Status );

/*******************************************************************************
 * @fn       USB30_ISO_Setdelay( UINT32 dly )
 *
 * @brief   ����ͬ���ӳ�ʱ��
 *
 * @param dly - �ӳ�ʱ��
 *
 * @return   None
 */
extern void USB30_ISO_Setdelay( UINT32 dly );

/*******************************************************************************
 * @fn       USB30_ITP_Enable
 *
 * @brief   USB ITPʹ��
 *
 * @param Status - enable/disable
 *
 * @return   None
 */
extern void USB30_ITP_Enable(FunctionalState Status);

/*******************************************************************************
 * @fn     USB30_OUT_Status
 *
 * @brief  ��ȡ�˵�������ݳ���
 *
 * @param  endp - �˵��
 *         nump - �˵�ʣ��ɽ��յİ�����
 *         len -  �˵���յ����ݳ��ȣ�����ͻ�������ʾ�˵���յ������һ���İ�����
 *         status -  ��ʾ�����Ƿ������ݰ����·��� 1-�˴�ͻ������ �յ������� 0-�����������ݰ��·�
 *
 * @return   None
 */
extern void USB30_OUT_Status(UINT8 endp,UINT8 *nump,UINT16 *len,UINT8 *status);

/*******************************************************************************
 * @fn     USB30_OUT_Set
 *
 * @brief  �˵��������
 *
 * @param  endp - ��Ҫ���õĶ˵��
 *         nump - �˵�ʣ��ɽ��յİ�����
 *         status -  �˵�״̬    0-NRDY,1-ACK,2-STALL
 *
 * @return   None
 */
extern void USB30_OUT_Set(UINT8 endp,UINT8 status,UINT8 nump);

/*******************************************************************************
 * @fn     USB30_OUT_ClearIT
 *
 * @brief  ���OUT��������ж�,�����������к�
 *
 * @param  endp - ��Ҫ���õĶ˵��
 *
 * @return   None
 */
extern void USB30_OUT_ClearIT(UINT8 endp);

/*******************************************************************************
 * @fn     USB30_OUT_ClearPendingIT
 *
 * @brief  ���OUT��������жϣ������˵���������
 *
 * @param  endp - ��Ҫ���õĶ˵��
 *
 * @return   None
 */
extern void USB30_OUT_ClearPendingIT(UINT8 endp);

/*******************************************************************************
 * @fn     USB30_OUT_ITflag
 *
 * @brief  ��ȡOUT��������жϱ�־
 *
 * @param  endp - ��Ҫ���õĶ˵��
 *
 * @return   1 - ���ж� 0 - ���ж�
 */
extern UINT8 USB30_OUT_ITflag(UINT8 endp);

/*******************************************************************************
 * @fn      USB30_IN_Set
 *
 * @brief  �˵㷢������
 *
 * @param  endp - �˵��
 *         lpf -  end of burst��־   1-enable 0-disable
 *         nump -  �˵��ܷ��͵����ݰ��ĸ���
 *         status -  �˵�״̬   0-NRDY,1-ACK,2-STALL
 *         TxLen - �˵㷢�����һ�������ݳ���
 *
 * @return   None
 */
extern void USB30_IN_Set(UINT8 endp,FunctionalState lpf,UINT8 status,UINT8 nump,UINT16 TxLen);

/*******************************************************************************
 * @fn     USB30_IN_ClearPendingIT
 *
 * @brief  ���IN��������жϣ������˵���������
 *
 * @param  endp - �˵��
 *
 * @return   None
 */
extern void USB30_IN_ClearPendingIT(UINT8 endp);

/*******************************************************************************
 * @fn      USB30_IN_ClearIT
 *
 * @brief  ���IN�����ж���˵�����״̬�������������к�
 *
 * @param  endp - �˵��
 *
 * @return   None
 */
extern void USB30_IN_ClearIT(UINT8 endp);

/*******************************************************************************
 * @fn       USB30_IN_ITflagT
 *
 * @brief  ��ȡIN��������жϱ�־
 *
 * @param  endp - ��Ҫ���õĶ˵��
 *
 * @return   None
 */
extern UINT8 USB30_IN_ITflag(UINT8 endp);

/*******************************************************************************
 * @fn       USB30_IN_Nump
 *
 * @brief  ��ȡ�˵�ʣ������͵İ�����
 *
 * @param  endp - �˵��
 *
 * @return   ʣ������Ͱ�����
 */
extern UINT8 USB30_IN_Nump(UINT8 endp);

/*******************************************************************************
 * @fn      USB30_Send_ERDY
 *
 * @brief  �˵���������  ����ERDY��
 *
 * @param  endp - �˵��   ���λ���򣬵���λΪ�˵��
 *         nump -  �˵���ջ��͵����ݰ�����
 *
 * @return   None
 */
extern void USB30_Send_ERDY(UINT8 endp,UINT8 nump);

/*******************************************************************************
 * @fn       USB30_Device_Setaddress
 *
 * @brief  �����豸��ַ
 *
 * @param  address - ��Ҫ���õĵ�ַ
 *
 * @return   None
 */
extern void USB30_Device_Setaddress( UINT32 address );

/*******************************************************************************
 * @fn      USB30_IN_Nump
 *
 * @brief  ��ȡ�˵�ʣ������͵İ�����
 *
 * @return   ���ƴ������ݽ׶�ΪOUTʱ�������·������ݳ���
 */
extern UINT16 USB30_Setup_OutData(void);

/*******************************************************************************
 * @fn      USB30_IRQHandler
 *
 * @brief  USB3.0�жϴ���
 *
 * @return   None
 */
extern void USB30_IRQHandler();

/*******************************************************************************
 * @fn      USB30_StandardReq
 *
 * @brief  USB�豸ģʽ��׼���������
 *
 * @return   ���������豸���͵����ݳ���
 */
extern UINT16 USB30_StandardReq();

/*******************************************************************************
 * @fn      USB30_NonStandardReq
 *
 * @brief  USB�豸ģʽ�Ǳ�׼���������
 *
 * @return   ���������豸���͵����ݳ���
 */
extern UINT16 USB30_NonStandardReq();

/*******************************************************************************
 * @fn      EP0_IN_Callback
 *
 * @brief  �˵�0 IN������ɻص�����
 *
 * @return   һ��IN������Ӧ���͵����ݳ���
 */
extern UINT16 EP0_IN_Callback();

/*******************************************************************************
 * @fn      EP0_OUT_Callback
 *
 * @brief  �˵�0 OUT������ɻص�����
 *
 * @return   None
 */
extern UINT16 EP0_OUT_Callback();

/*******************************************************************************
 * @fn      USB30_Setup_Status
 *
 * @brief   ���ƴ���״̬�׶�
 *
 * @return   None
 */
extern void USB30_Setup_Status();

/*******************************************************************************
 * @fn      USB30_ITP_Callback
 *
 * @brief   ITP�ص�����
 *
 * @return   None
 */
extern void USB30_ITP_Callback(UINT32 ITPCounter);

/*******************************************************************************
 * @fn      USB30_switch_pwr_mode
 *
 * @brief   �л�USB3.0 ��Դģʽ
 *
 * @return   None
 */
extern void USB30_Switch_Powermode( UINT8 pwr_mode );

/*******************************************************************************
 * @fn      EPn_IN_Callback()
 *
 * @brief   �˵�n IN������ص�����
 *
 * @return   None
 */
extern void  EP1_IN_Callback(void);
extern void  EP2_IN_Callback(void);
extern void  EP3_IN_Callback(void);
extern void  EP4_IN_Callback(void);
extern void  EP5_IN_Callback(void);
extern void  EP6_IN_Callback(void);
extern void  EP7_IN_Callback(void);

/*******************************************************************************
 * @fn      EPn_IN_Callback()
 *
 * @brief   �˵�n OUT������ص�����
 *
 * @return   None
 */
extern void  EP1_OUT_Callback(void);
extern void  EP2_OUT_Callback(void);
extern void  EP3_OUT_Callback(void);
extern void  EP4_OUT_Callback(void);
extern void  EP5_OUT_Callback(void);
extern void  EP6_OUT_Callback(void);
extern void  EP7_OUT_Callback(void);

#ifdef __cplusplus
}
#endif

#endif /* USB30_CH56X_USB30_LIB_H_ */
