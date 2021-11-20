/********************************** (C) COPYRIGHT *******************************
* File Name          : usb30_callback.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 :
*******************************************************************************/
#include "CH56xUSB30_LIB.H"
#include "usb30_porp.h"
#include  <stdio.h>

void  ITP_Callback(UINT32 ITPCounter)
{

}

/***************Endpointn IN Transaction Processing*******************/
void EP1_IN_Callback()
{
	if(USB30_getTxNump(endp_1) == 0){
		USB30_setTxCtrl(endp_1,DISABLE,NRDY_TP,NUMP_0,0);
	}
	else{
		USB30_setTxCtrl(endp_1,DISABLE,ACK_TP,NUMP_0,0);
	}
}

void EP2_IN_Callback()
{

	if(USB30_getTxNump(endp_2) == 0){
	    printf("2:0\n");
		USB30_setTxCtrl(endp_2,DISABLE,NRDY_TP,NUMP_0,0);
	}
	else{
	    printf("2:1\n");
		USB30_setTxCtrl(endp_2,DISABLE,ACK_TP,NUMP_0,0);
	}
}

void EP3_IN_Callback()
{

	if(USB30_getTxNump(endp_3) == 0){
		USB30_setTxCtrl(endp_3,DISABLE,NRDY_TP,NUMP_0,0);
	}
	else{
		USB30_setTxCtrl(endp_3,DISABLE,ACK_TP,NUMP_0,0);
	}
}

void EP4_IN_Callback()
{
	if(USB30_getTxNump(endp_4) == 0){
		USB30_setTxCtrl(endp_4,DISABLE,NRDY_TP,NUMP_0,0);
	}
	else{
		USB30_setTxCtrl(endp_4,DISABLE,ACK_TP,NUMP_0,0);
	}
}

void EP5_IN_Callback()
{

	if(USB30_getTxNump(endp_5) == 0){
		USB30_setTxCtrl(endp_5,DISABLE,NRDY_TP,NUMP_0,0);
	}
	else{
		USB30_setTxCtrl(endp_5,DISABLE,ACK_TP,NUMP_0,0);
	}
}

void EP6_IN_Callback()
{
	if(USB30_getTxNump(endp_6) == 0){
		USB30_setTxCtrl(endp_6,DISABLE,NRDY_TP,NUMP_0,0);
	}
	else{
		USB30_setTxCtrl(endp_6,DISABLE,ACK_TP,NUMP_0,0);
	}
}

void EP7_IN_Callback()
{
	if(USB30_getTxNump(endp_7) == 0){
		USB30_setTxCtrl(endp_7,DISABLE,NRDY_TP,NUMP_0,0);
	}
	else{
		USB30_setTxCtrl(endp_7,DISABLE,ACK_TP,NUMP_0,0);
	}
}

/***************Endpointn OUT Transaction Processing*******************/
void EP1_OUT_Callback()
{
	UINT16 rx_len,i;
	UINT8 nump;
	UINT8 status;
	USB30_getRxCount(endp_1,&nump,&rx_len,&status);
	USB30_setEndpDMA(endp_1|endp_out,endp1RTbuff);  //突发模式下，由于自动偏移地址，需要重设
	for(i=0;i<rx_len;i++){
		endp1RTbuff[i] = ~endp1RTbuff[i];
	}
	if(nump == 0){
		USB30_setTxCtrl(endp_1,DISABLE,ACK_TP,NUMP_2,rx_len);
		USB30_setFCCTRL(endp_1|endp_in,NUMP_2);
	}
	else{
		USB30_setTxCtrl(endp_1,DISABLE,ACK_TP,NUMP_1,rx_len);
		USB30_setFCCTRL(endp_1|endp_in,NUMP_1);
	}
	USB30_setRxCtrl(endp_1,ACK_TP,NUMP_2);
	USB30_setFCCTRL(endp_1|endp_out,NUMP_2);
}

void EP2_OUT_Callback()
{
	UINT16 rx_len,i;
	UINT8 nump;
	UINT8 status;
	USB30_getRxCount(endp_2,&nump,&rx_len,&status);
	USB30_setEndpDMA(endp_2|endp_out,endp2RTbuff);//突发模式下，由于自动偏移地址，需要重设
	printf("2:%x %x %x\n",nump,rx_len,status);
	for(i=0;i<rx_len;i++){
		endp2RTbuff[i] = ~endp2RTbuff[i];
	}
	if(nump == 0){
		USB30_setTxCtrl(endp_2,DISABLE,ACK_TP,NUMP_2,rx_len);
		USB30_setFCCTRL(endp_2|endp_in,NUMP_2);
	}
	else{
		USB30_setTxCtrl(endp_2,DISABLE,ACK_TP,NUMP_1,rx_len);
		USB30_setFCCTRL(endp_2|endp_in,NUMP_1);
	}
	USB30_setRxCtrl(endp_2,ACK_TP,NUMP_2);
	USB30_setFCCTRL(endp_2|endp_out,NUMP_2);
}

void EP3_OUT_Callback()
{
	UINT16 rx_len,i;
	UINT8 nump;
	UINT8 status;
	USB30_getRxCount(endp_3,&nump,&rx_len,&status);
	USB30_setEndpDMA(endp_3|endp_out,endp3RTbuff);//突发模式下，由于自动偏移地址，需要重设
	for(i=0;i<rx_len;i++){
		endp3RTbuff[i] = ~endp3RTbuff[i];
	}
	if(nump == 0){
		USB30_setTxCtrl(endp_3,DISABLE,ACK_TP,NUMP_2,rx_len);
		USB30_setFCCTRL(endp_3|endp_in,NUMP_2);
	}
	else{
		USB30_setTxCtrl(endp_3,DISABLE,ACK_TP,NUMP_1,rx_len);
		USB30_setFCCTRL(endp_3|endp_in,NUMP_1);
	}
	USB30_setRxCtrl(endp_3,ACK_TP,NUMP_2);
	USB30_setFCCTRL(endp_3|endp_out,NUMP_2);
}

void EP4_OUT_Callback()
{
	UINT16 rx_len,i;
	UINT8 nump;
	UINT8 status;
	USB30_getRxCount(endp_4,&nump,&rx_len,&status);
	USB30_setEndpDMA(endp_4|endp_out,endp4RTbuff);//突发模式下，由于自动偏移地址，需要重设
	for(i=0;i<rx_len;i++){
		endp4RTbuff[i] = ~endp4RTbuff[i];
	}
	if(nump == 0){
		USB30_setTxCtrl(endp_4,DISABLE,ACK_TP,NUMP_2,rx_len);
		USB30_setFCCTRL(endp_4|endp_in,NUMP_2);
	}
	else{
		USB30_setTxCtrl(endp_4,DISABLE,ACK_TP,NUMP_1,rx_len);
		USB30_setFCCTRL(endp_4|endp_in,NUMP_1);
	}
	USB30_setRxCtrl(endp_4,ACK_TP,NUMP_2);
	USB30_setFCCTRL(endp_4|endp_out,NUMP_2);
}

void EP5_OUT_Callback()
{
	UINT16 rx_len,i;
	UINT8 nump;
	UINT8 status;

	USB30_getRxCount(endp_5,&nump,&rx_len,&status);
	USB30_setEndpDMA(endp_5|endp_out,endp5RTbuff);//突发模式下，由于自动偏移地址，需要重设

	for(i=0;i<rx_len;i++){
		endp5RTbuff[i] = ~endp5RTbuff[i];
	}
	if(nump == 0){
		USB30_setTxCtrl(endp_5,DISABLE,ACK_TP,NUMP_2,rx_len);
		USB30_setFCCTRL(endp_5|endp_in,NUMP_2);
	}
	else{
		USB30_setTxCtrl(endp_5,DISABLE,ACK_TP,NUMP_1,rx_len);
		USB30_setFCCTRL(endp_5|endp_in,NUMP_1);
	}
	USB30_setRxCtrl(endp_5,ACK_TP,NUMP_2);
	USB30_setFCCTRL(endp_5|endp_out,NUMP_2);
}

void EP6_OUT_Callback()
{
	UINT16 rx_len,i;
	UINT8 nump;
	UINT8 status;
	USB30_getRxCount(endp_6,&nump,&rx_len,&status);
	USB30_setEndpDMA(endp_6|endp_out,endp6RTbuff);//突发模式下，由于自动偏移地址，需要重设
	for(i=0;i<rx_len;i++){
		endp6RTbuff[i] = ~endp6RTbuff[i];
	}
	if(nump == 0){
		USB30_setTxCtrl(endp_6,DISABLE,ACK_TP,NUMP_2,rx_len);
		USB30_setFCCTRL(endp_6|endp_in,NUMP_2);
	}
	else{
		USB30_setTxCtrl(endp_6,DISABLE,ACK_TP,NUMP_1,rx_len);
		USB30_setFCCTRL(endp_6|endp_in,NUMP_1);
	}
	USB30_setRxCtrl(endp_6,ACK_TP,NUMP_2);
	USB30_setFCCTRL(endp_6|endp_out,NUMP_2);
}

void EP7_OUT_Callback()
{
	UINT16 rx_len,i;
	UINT8 nump;
	UINT8 status;
	USB30_getRxCount(endp_7,&nump,&rx_len,&status);
	USB30_setEndpDMA(endp_7|endp_out,endp7RTbuff);//突发模式下，由于自动偏移地址，需要重设
	for(i=0;i<rx_len;i++){
		endp7RTbuff[i] = ~endp7RTbuff[i];
	}
	if(nump == 0){
		USB30_setTxCtrl(endp_7,DISABLE,ACK_TP,NUMP_2,rx_len);
		USB30_setFCCTRL(endp_7|endp_in,NUMP_2);
	}
	else{
		USB30_setTxCtrl(endp_7,DISABLE,ACK_TP,NUMP_1,rx_len);
		USB30_setFCCTRL(endp_7|endp_in,NUMP_1);
	}
	USB30_setRxCtrl(endp_7,ACK_TP,NUMP_2);
	USB30_setFCCTRL(endp_7|endp_out,NUMP_2);
}

