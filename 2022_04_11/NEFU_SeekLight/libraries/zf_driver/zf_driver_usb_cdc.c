/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            zf_driver_usb_cdc
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ790875685)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/


#include "zf_driver_delay.h"
#include "zf_driver_usb_cdc.h"



//-------------------------------------------------------------------------------------------------------------------
//  @brief      USB_CDC����һ�� ��
//  @param      *p          Ҫ���͵�����ָ��
//  @param      length      ���Ͷ��ٸ�����( length ��ҪС��64)
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


    //�ȴ��������
    while(!(USBOTG_FS->UEP2_TX_CTRL&USBHD_UEP_T_RES1));

    // bUEP_T_RES1 & bUEP_T_RES0: handshake response type for USB endpoint X transmittal (IN)
    // 00: DATA0 or DATA1 then expecting ACK (ready)
    // 01: DATA0 or DATA1 then expecting no response, time out from host, for non-zero endpoint isochronous transactions
    // 10: NAK (busy)
    // 11: TALL (error)

}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ͼ�����ݵ���λ��
//  @param      *image          ͼ������
//  @param      length          ���Ͷ��ٸ�����( length ��ҪС��64)
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
//  @brief      USB�豸ȫ���豸��ʼ��
//  @param      void          ��
//  @return     void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void usb_cdc_init( void )
{
    // �˵㻺������ʼ��
    pEP0_RAM_Addr = EP0_DatabufHD;
    pEP1_RAM_Addr = EP1_DatabufHD;
    pEP2_RAM_Addr = EP2_DatabufHD;
    // ʹ��usbʱ��
    USBOTG_RCC_Init();
    system_delay_ms(100);
    // usb�豸��ʼ��
    USBDeviceInit();
    EXTEN->EXTEN_CTR |= EXTEN_USBD_PU_EN;
    // ʹ��usb�ж�
    NVIC_EnableIRQ(OTG_FS_IRQn);
}
