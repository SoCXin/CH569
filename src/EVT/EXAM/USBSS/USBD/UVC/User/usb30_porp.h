/*
 * usb30_porp.h
 *
 *  Created on: Aug 17, 2020
 *      Author: OWNER
 */

#ifndef USER_USB30_PORP_H_
#define USER_USB30_PORP_H_

#include "CH56xUSB30_LIB.H"
#include "UVCLIB.H"
void USB30_init(void);
extern __attribute__ ((aligned(16))) UINT8	endp0buff[512] __attribute__((section(".DMADATA"))); //数据发送/接收缓冲区
extern __attribute__ ((aligned(16))) UINT8	endp1RTbuff[1024] __attribute__((section(".DMADATA"))); //数据发送/接收缓冲区
extern __attribute__ ((aligned(16))) UINT8	endp2RTbuff[1024] __attribute__((section(".DMADATA"))); //数据发送/接收缓冲区
extern __attribute__ ((aligned(16))) UINT8	endp3RTbuff[1024] __attribute__((section(".DMADATA"))); //数据发送/接收缓冲区
extern __attribute__ ((aligned(16))) UINT8	endp4RTbuff[1024] __attribute__((section(".DMADATA"))); //数据发送/接收缓冲区
extern __attribute__ ((aligned(16))) UINT8	endp5RTbuff[1024] __attribute__((section(".DMADATA"))); //数据发送/接收缓冲区
extern __attribute__ ((aligned(16))) UINT8	endp6RTbuff[1024] __attribute__((section(".DMADATA"))); //数据发送/接收缓冲区
extern __attribute__ ((aligned(16))) UINT8	endp7RTbuff[1024] __attribute__((section(".DMADATA"))); //数据发送/接收缓冲区

#define UsbSetupBuf     ((PUSB_SETUP)endp0buff)//端点0

#endif /* USER_USB30_PORP_H_ */
