/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_device_camera
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
* @note             version:
*                   V1.1 2021.12.02 ���Ӵ��ڷ���ͼ����λ������
********************************************************************************************************************/

#include "zf_device_mt9v03x_dvp.h"
#include "zf_device_scc8660_dvp.h"
#include "zf_device_camera.h"
#include "zf_device_type.h"
#include "zf_common_headfile.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ͷ���ڻص�����
// @param       void
// @return      void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void camera_uart_handler (void)
{
    if(camera_type == CAMERA_BIN_UART)
    {
        // С����ݲ�֧��
        //ov7725_cof_uart_interrupt();
    }
    else if(camera_type == CAMERA_GRAYSCALE)
    {
        // ����紮�ڻص�����
        mt9v03x_uart_callback_dvp();
    }
    else if(camera_type == CAMERA_COLOR)
    {
        //��ͫ���ڻص�����
        scc8660_uart_callback_dvp();
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ͷ���жϻص�����
// @param       void
// @return      void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void camera_vsync_handler (void)
{
    //����Ҫ
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ͷ�ɼ����DMA�ص�����
// @param       void
// @return      void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void camera_dma_handler (void)
{
    //����Ҫ
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ͷDVP�ӿڻص�����
// @param       void
// @return      void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void camera_dvp_handler(void)
{
    if(camera_type == CAMERA_COLOR)
    {
        scc8660_vsync_dvp();
    }
    else if(camera_type == CAMERA_GRAYSCALE)
    {
        mt9v03x_handler_dvp();
    }
    else if(camera_type == CAMERA_BIN_UART)
    {
        //С��粻֧��
    }
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ͷ������ͼ�����ݽ�ѹΪʮ�����ư�λ���� С�����
// @param       *data1          ����ͷͼ������
// @param       *data2          ��Ž�ѹ���ݵĵ�ַ
// @param       image_size      ͼ��Ĵ�С
// @return      void
// Sample usage:                camera_binary_image_decompression(&ov7725_image_binary[0][0], &data_buffer[0][0], OV7725_SIZE);
//-------------------------------------------------------------------------------------------------------------------
void camera_binary_image_decompression (uint8_t *data1, uint8_t *data2, uint32_t image_size)
{
    uint8_t  i = 8;

    while(image_size--)
    {
        i = 8;
        while(i--)
        {
            *data2++ = ((*data1 >> i)&0x01 ? 255:0);
        }
        data1++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ͷͼ��������λ���鿴ͼ��
// @param       uartn           ʹ�õĴ��ں�
// @param       *image_addr     ��Ҫ���͵�ͼ���ַ
// @param       image_size      ͼ��Ĵ�С
// @return      void
// Sample usage:                camera_send_image(DEBUG_UART_INDEX, &mt9v03x_image[0][0], MT9V03X_IMAGE_SIZE);
//-------------------------------------------------------------------------------------------------------------------
void camera_send_image (uart_index_enum uartn, void *image_addr, uint32_t image_size)
{
    // ��������
    uart_write_byte(uartn,0x00);
    uart_write_byte(uartn,0xff);
    uart_write_byte(uartn,0x01);
    uart_write_byte(uartn,0x01);

    // ����ͼ��
    uart_write_buffer(uartn, (uint8_t*)image_addr, image_size);
}
void camera_send_image_RUBO (uart_index_enum uartn, void *image_addr, uint32_t image_size)
{
    // ��������
    uart_write_byte(uartn,0x00);
    uart_write_byte(uartn,0xff);
    uart_write_byte(uartn,0x01);
    uart_write_byte(uartn,0x01);

    // ����ͼ��
    uart_write_buffer(uartn, (uint8_t*)image_addr, image_size);
}


