/********************************** (C) COPYRIGHT *******************************
* File Name          : CH56x_usb30h.h
* Author             : WCH
* Version            : V1.0.0
* Date               : 2020/12/23
* Description        : USB3.0
*******************************************************************************/
#ifndef HOST_CH56X_USB30H_H_
#define HOST_CH56X_USB30H_H_

#ifdef __cplusplus
extern "C" {
#endif

extern UINT8V device_link_status;

extern const UINT8 get_descriptor[];
extern const UINT8 get_cfg_descriptor[];
extern const UINT8 get_cfg_descriptor_all[];
extern const UINT8 get_bos_descriptor[];
extern const UINT8 get_string_descriptor0[];
extern const UINT8 get_string_descriptor1[];
extern const UINT8 get_string_descriptor2[];
extern const UINT8 get_string_descriptor3[];
extern const UINT8 get_interface[];
extern const UINT8 set_configuration[];
extern const UINT8 set_address[];
extern const UINT8 set_isoch_delay[];
extern const UINT8 set_sel[];
extern const UINT8 tx_sel_data[];
extern const UINT8 set_feature_U1[];
extern const UINT8 set_feature_U2[];

extern __attribute__ ((aligned(16))) UINT8 endpRXbuff[4096] __attribute__((section(".DMADATA"))); //数据接收缓冲区
extern __attribute__ ((aligned(16))) UINT8 endpTXbuff[4096] __attribute__((section(".DMADATA"))); //数据发送缓冲区

extern void USB30_host_enum(void);
extern void USB30H_init (FunctionalState s);
extern void USB30_link_status(UINT8 s);
void LINK_IRQHandler (void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USBSS_IRQHandler (void) __attribute__((interrupt("WCH-Interrupt-fast")));

#ifdef __cplusplus
}
#endif

#endif /* USER_USB30H_PROCESS_H_ */
