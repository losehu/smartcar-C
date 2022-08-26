/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            zf_driver_usb_cdc
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/


#include "zf_driver_delay.h"
#include "zf_driver_usb_cdc.h"



//-------------------------------------------------------------------------------------------------------------------
//  @brief      USB_CDC发送一个 包
//  @param      *p          要发送的数据指针
//  @param      length      发送多少个数据( length 需要小于64)
//  @return     void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void cdc_send_pack(uint8_t *p, uint32_t length)
{
    uint32_t i = 0;

    for(i=0; i<length; i++)
    {
       pEP2_IN_DataBuf[i] = p[i];
    }
    DevEP2_IN_Deal(length);


    //等待发送完成
    while(!(USBOTG_FS->UEP2_TX_CTRL&USBHD_UEP_T_RES1));

    // bUEP_T_RES1 & bUEP_T_RES0: handshake response type for USB endpoint X transmittal (IN)
    // 00: DATA0 or DATA1 then expecting ACK (ready)
    // 01: DATA0 or DATA1 then expecting no response, time out from host, for non-zero endpoint isochronous transactions
    // 10: NAK (busy)
    // 11: TALL (error)

}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      发送图像数据到上位机
//  @param      *image          图像数组
//  @param      length          发送多少个数据( length 需要小于64)
//  @return     void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void camera_send_image_usb_cdc(uint8_t *image, uint32_t length)
{
    uint8_t send_buffer[4] = {0x00,0xff,0x01,0x01};
    cdc_send_pack(send_buffer, 4);

    while(length)
    {
        if(length > 63)
        {
            cdc_send_pack(image, 63);
            image += 63;
            length -= 63;
        }
        else
        {
            cdc_send_pack(image, length);
            length = 0;
        }
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      USB设备全速设备初始化
//  @param      void          空
//  @return     void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void usb_cdc_init( void )
{
    // 端点缓冲区初始化
    pEP0_RAM_Addr = EP0_DatabufHD;
    pEP1_RAM_Addr = EP1_DatabufHD;
    pEP2_RAM_Addr = EP2_DatabufHD;
    // 使能usb时钟
    USBOTG_RCC_Init();
    system_delay_ms(100);
    // usb设备初始化
    USBDeviceInit();
    EXTEN->EXTEN_CTR |= EXTEN_USBD_PU_EN;
    // 使能usb中断
    NVIC_EnableIRQ(OTG_FS_IRQn);
}
