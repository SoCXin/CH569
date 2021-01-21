/********************************** (C) COPYRIGHT *******************************
* File Name          : CH56x_usb30h.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2020/12/23
* Description        : USB3.0 function
*******************************************************************************/
#include "CH56x_common.h"
#include "CH56xusb30h_LIB.h"
#include "CH56x_usb30h.h"


/* Global define */



/* Global Variable */
__attribute__ ((aligned(16))) UINT8 endpRXbuff[4096] __attribute__((section(".DMADATA"))); //数据接收缓冲区
__attribute__ ((aligned(16))) UINT8 endpTXbuff[4096] __attribute__((section(".DMADATA"))); //数据发送缓冲区
UINT8V tx_lmp_port = 0;
UINT8V device_link_status = 0;              //USBSS link标志


const uint8_t get_dev_descriptor[] =
{
    0x80,0x06,0x00,0x01,0x00,0x00,0x40,0x00
};

const uint8_t get_cfg_descriptor[] =
{
    0x80,0x06,0x00,0x02,0x00,0x00,0x09,0x00
};

const uint8_t get_cfg_descriptor_all[] =
{
    0x80,0x06,0x00,0x02,0x00,0x00,0xff,0x00
};

const uint8_t get_bos_descriptor[] =
{
    0x80,0x06,0x00,0x0f,0x00,0x00,0xff,0x00
};


const uint8_t get_string_descriptor0[] =
{
    0x80,0x06,0x00,0x03,0x00,0x00,0xff,0x00
};

const uint8_t get_string_descriptor1[] =
{
    0x80,0x06,0x01,0x03,0x09,0x04,0xff,0x00
};

const uint8_t get_string_descriptor2[] =
{
    0x80,0x06,0x02,0x03,0x09,0x04,0xff,0x00
};

const uint8_t get_string_descriptor3[] =
{
    0x80,0x06,0x03,0x03,0x09,0x04,0xff,0x00
};

const uint8_t get_interface[] =
{
    0x81,0x0a,0x00,0x00,0x00,0x00,0x09,0x00
};
const uint8_t set_configuration[] =
{
    0x00,0x09,0x01,0x00,0x00,0x00,0x00,0x00
};

const uint8_t set_address[] =
{
    0x00,0x05,0x08,0x00,0x00,0x00,0x00,0x00
};

const uint8_t set_isoch_delay[] =
{
    0x00,0x31,0x88,0x00,0x00,0x00,0x00,0x00
};

const uint8_t set_sel[] =
{
    0x00,0x30,0x00,0x00, 0x00,0x00,0x06,0x00
};

const uint8_t tx_sel_data[] =
{
    0x5f,0x0a,0x54,0x08,0xff,0x07
};

const uint8_t set_feature_U1[] =
{
    0x00,0x03,0x30,0x00,0x00,0x00,0x00,0x00
};

const uint8_t set_feature_U2[] =
{
    0x00,0x03,0x31,0x00,0x00,0x00,0x00, 0x00
};


/*******************************************************************************
* Function Name  : USB30_host_enum
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void USB30_host_enum(void)
{
    UINT32 i=0;
    UINT16 len,t;
    UINT8 req_nump = 1;
//================= set address ===================
		printf("****** set address : 0x08 \n");
		memcpy( endpTXbuff , set_address , 8);
		USB30H_Send_Setup( 8 ); // send setup packet
		USB30H_Send_Status();
		USB30H_Set_Address(set_address[2]);// set device address is 0x01

//================= get descriptor ===================
		printf("****** get dev descriptor : \n");
		memcpy( endpTXbuff , get_dev_descriptor , 8);
		USB30H_Send_Setup( 8 );
		req_nump = 1;
		len = USB30H_IN_Data( 0, &req_nump, 0 ); // nump =1, endp=0
        switch( len & 0x7000 ){
            case 0x1000:    //ACK
                break;
            default:        //NRDY
                i=0;
                while( !USB30H_Erdy_Status( NULL, NULL  ) ){             //等待ERDY标志
                    i++;
                    if(i==2000000){
                        i=0;
                        len = USB30H_IN_Data(0, &req_nump, 0 );    //重试
                        if( (len & 0x7000) == 0x1000 )    break;
                    }
                }
                if( (len & 0x7000) == 0x2000){
                    len = USB30H_IN_Data(0,&req_nump, 0 );
                }
                break;
        }
        len &= 0x0fff;   //提取包数量
		for(t=0;t<len;t++)
		    printf("%02x ",endpRXbuff[t]);
        printf("\n");
        USB30H_Send_Status();
//================= get cfg descriptor ===================
		printf("****** get cfg descriptor : \n");
		memcpy( endpTXbuff , get_cfg_descriptor , 8);
		USB30H_Send_Setup( 8 );
        req_nump = 1;
        len = USB30H_IN_Data( 0, &req_nump, 0 ); // nump =1, endp=0
        switch( len & 0x7000 ){
            case 0x1000:    //ACK
                break;
            default:        //NRDY
                i=0;
                while( !USB30H_Erdy_Status( NULL, NULL  ) ){             //等待ERDY标志
                    i++;
                    if(i==2000000){
                        i=0;
                        len = USB30H_IN_Data(0, &req_nump, 0 );    //重试
                        if( (len & 0x7000) == 0x1000 )    break;
                    }
                }
                if( (len & 0x7000) == 0x2000){
                    len = USB30H_IN_Data(0,&req_nump, 0 );
                }
                break;
        }
        len &= 0x0fff;   //提取包数量
        for(t=0;t<len;t++)
            printf("%02x ",endpRXbuff[t]);
        printf("\n");
        USB30H_Send_Status();
//================= get cfg descriptor all ===================
		printf("****** get cfg descriptor all : \n");
        memcpy( endpTXbuff , get_cfg_descriptor_all , 8);
        USB30H_Send_Setup( 8 );
        req_nump = 1;
        len = USB30H_IN_Data( 0, &req_nump, 0 ); // nump =1, endp=0
        switch( len & 0x7000 ){
            case 0x1000:    //ACK
                break;
            default:        //NRDY
                i=0;
                while( !USB30H_Erdy_Status( NULL, NULL  ) ){             //等待ERDY标志
                    i++;
                    if(i==2000000){
                        i=0;
                        len = USB30H_IN_Data(0, &req_nump, 0 );    //重试
                        if( (len & 0x7000) == 0x1000 )    break;
                    }
                }
                if( (len & 0x7000) == 0x2000){
                    len = USB30H_IN_Data(0,&req_nump, 0 );
                }
                break;
        }
        len &= 0x0fff;   //提取包数量
        for(t=0;t<len;t++)
            printf("%02x ",endpRXbuff[t]);
        printf("\n");
        USB30H_Send_Status();

//========================= set config =============================
		printf("****** set config : \n");
		//----- setup stage -----
		memcpy( endpTXbuff , set_configuration , 8);
		USB30H_Send_Setup( 8 );
		//----- status stage -----
		USB30H_Send_Status();
		printf("set config is complete !\n\n");


}

/*******************************************************************************
* Function Name  : USB30H_init
* Description    :
* Input          : sta：ENABLE使能    DISABLE不使能、关闭
* Return         : None
*******************************************************************************/
void USB30H_init (FunctionalState sta) // USBSS host initial
{
    if(sta){
        USBSSH->UH_TX_CTRL = 0;
        USBSSH->UH_RX_CTRL = 0;
        USBSSH->LINK_CFG = HP_PENDING | CFG_EQ_EN | DEEMPH_CFG | DOWN_FLAG;// downstream
        USBSSH->LINK_CTRL = POWER_MODE_2 | GO_DISABLED;     // change U3-PHY enter P2

        while( USBSSH->LINK_STATUS & LINK_BUSY );           // wait link_busy=0
        USBSSH->USB_CONTROL = HOST_MODE | ITP_EN | INT_BUSY_EN | DMA_EN ;//| USB_ERDY_IE;   // host_mode, release link reset
        USBSSH->UH_TX_CTRL = 0x0;
        USBSSH->USB_STATUS = USB_ACT_FLAG | USB_LMP_TX_FLAG | USB_LMP_RX_FLAG | USB_OV_FLAG;
        USBSSH->UH_TX_DMA = (UINT32)(UINT8 *)endpTXbuff;                     // set tx dma address
        USBSSH->UH_RX_DMA = (UINT32)(UINT8 *)endpRXbuff;                     // set rx dma address
        USBSSH->UEP_CFG = UH_T_EN | UH_R_EN;                // set HOST rx/tx enable
        USBSS->LINK_CFG |= TERM_EN; // term enable
        USBSS->LINK_INT_CTRL = LINK_DISABLE_IE | LINK_INACT_IE | LINK_RX_DET_IE | TERM_PRESENT_IE | LINK_TXEQ_IE | LINK_RDY_IE;
        USBSS->LINK_CTRL = POWER_MODE_2; // GO RX DETECT
    }
    else{
        USBSSH->LINK_INT_CTRL = 0;  // disable all link int enable
        USBSSH->LINK_CTRL = POWER_MODE_3 | GO_DISABLED;     // change U3-PHY enter P3 & link disabled
        USBSSH->LINK_CFG = PIPE_RESET | LFPS_RX_PD | LPM_EN | DEEMPH_CFG;// reset U3-PHY
        USBSSH->USB_CONTROL = USB_ALL_CLR | USB_FORCE_RST; // reset link layer
        USBSSH->UH_TX_CTRL = 0;
        USBSSH->UH_RX_CTRL = 0;
    }
}
void USB30_link_status(UINT8 s){
    if(s){    //成功连接到设备
        printf("Linked!\n");
        device_link_status = 1;
    }
    else{     //同设备断开连接
        device_link_status = 0;
        printf("link is disconnect !\n\n");
    }
}

/*******************************************************************************
* Function Name  : LINK_IRQHandler
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void LINK_IRQHandler (void)         //USBSSH interrupt severice
{
    UINT32 temp = 0;
    temp = USBSS->LINK_ERR_STATUS;
    if( USBSSH->LINK_INT_FLAG & LINK_RECOV_FLAG )
    {
        USBSSH->LINK_INT_FLAG = LINK_RECOV_FLAG;
        printf("link recovery, error status = %0x\n", temp);
    }
    if( USBSSH->LINK_INT_FLAG & LINK_INACT_FLAG )
    {
        USBSSH->LINK_INT_FLAG = LINK_INACT_FLAG;
        USB30H_Switch_Powermode(POWER_MODE_2);
        printf("link inactive, error status = %0x\n", temp>>16);
    }
    else if( USBSS->LINK_INT_FLAG & LINK_DISABLE_FLAG ) // GO DISABLED
    {
        USBSSH->LINK_INT_FLAG = LINK_DISABLE_FLAG;
        USBSS->LINK_CTRL = POWER_MODE_2;// GO RX DETECT
    }
    else if( USBSSH->LINK_INT_FLAG & LINK_RX_DET_FLAG )
    {
        USBSSH->LINK_INT_FLAG = LINK_RX_DET_FLAG;
        USB30H_Switch_Powermode(POWER_MODE_2);
    }
    else if( USBSSH->LINK_INT_FLAG & TERM_PRESENT_FLAG ) // term present , begin POLLING
    {
        USBSSH->LINK_INT_FLAG = TERM_PRESENT_FLAG;
        if( USBSS->LINK_STATUS & LINK_PRESENT )
        {
            USB30H_Switch_Powermode(POWER_MODE_2);
            USBSSH->LINK_CTRL |= POLLING_EN;
        }
        else
        {
            USB30H_init (DISABLE); // USBSS host initial
            USB30_link_status(0);       //断开link
        }

    }
    else if( USBSSH->LINK_INT_FLAG & LINK_TXEQ_FLAG ) // POLLING SHAKE DONE
    {
        tx_lmp_port = 1;
        USBSSH->LINK_INT_FLAG = LINK_TXEQ_FLAG;
        if( USBSS->LINK_STATUS & LINK_PRESENT )
        {
            while(USBSS->LINK_STATUS & LINK_RX_DETECT);
            USB30H_Switch_Powermode(POWER_MODE_0);
        }
    }
    else if( USBSSH->LINK_INT_FLAG & LINK_RDY_FLAG ) // POLLING SHAKE DONE
    {
        USBSSH->LINK_INT_FLAG = LINK_RDY_FLAG;
        if( tx_lmp_port ) // LMP, TX PORT_CAP & RX PORT_CAP
        {
            tx_lmp_port = 0;
            USB30H_Lmp_Init();
            USB30_link_status(1);   //link成功
        }
    }
}
/*******************************************************************************
* Function Name  : USBSS_IRQHandler
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void USBSS_IRQHandler (void)            //USBSSH interrupt severice
{
    ;
}

