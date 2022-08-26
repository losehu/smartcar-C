/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             zf_device_camera
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ790875685)
* @version          查看doc内version文件 版本说明
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
* @note             version:
*                   V1.1 2021.12.02 增加串口发送图像到上位机函数
********************************************************************************************************************/

#include "zf_device_mt9v03x_dvp.h"
#include "zf_device_scc8660_dvp.h"
#include "zf_device_camera.h"
#include "zf_device_type.h"
#include "zf_common_headfile.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief       摄像头串口回调函数
// @param       void
// @return      void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void camera_uart_handler (void)
{
    if(camera_type == CAMERA_BIN_UART)
    {
        // 小钻风暂不支持
        //ov7725_cof_uart_interrupt();
    }
    else if(camera_type == CAMERA_GRAYSCALE)
    {
        // 总钻风串口回调函数
        mt9v03x_uart_callback_dvp();
    }
    else if(camera_type == CAMERA_COLOR)
    {
        //凌瞳串口回调函数
        scc8660_uart_callback_dvp();
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       摄像头场中断回调函数
// @param       void
// @return      void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void camera_vsync_handler (void)
{
    //不需要
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       摄像头采集完成DMA回调函数
// @param       void
// @return      void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void camera_dma_handler (void)
{
    //不需要
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       摄像头DVP接口回调函数
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
        //小钻风不支持
    }
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       摄像头二进制图像数据解压为十六进制八位数据 小钻风用
// @param       *data1          摄像头图像数组
// @param       *data2          存放解压数据的地址
// @param       image_size      图像的大小
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
// @brief       摄像头图像发送至上位机查看图像
// @param       uartn           使用的串口号
// @param       *image_addr     需要发送的图像地址
// @param       image_size      图像的大小
// @return      void
// Sample usage:                camera_send_image(DEBUG_UART_INDEX, &mt9v03x_image[0][0], MT9V03X_IMAGE_SIZE);
//-------------------------------------------------------------------------------------------------------------------
void camera_send_image (uart_index_enum uartn, void *image_addr, uint32_t image_size)
{
    // 发送命令
    uart_write_byte(uartn,0x00);
    uart_write_byte(uartn,0xff);
    uart_write_byte(uartn,0x01);
    uart_write_byte(uartn,0x01);

    // 发送图像
    uart_write_buffer(uartn, (uint8_t*)image_addr, image_size);
}
void camera_send_image_RUBO (uart_index_enum uartn, void *image_addr, uint32_t image_size)
{
    // 发送命令
    uart_write_byte(uartn,0x00);
    uart_write_byte(uartn,0xff);
    uart_write_byte(uartn,0x01);
    uart_write_byte(uartn,0x01);

    // 发送图像
    uart_write_buffer(uartn, (uint8_t*)image_addr, image_size);
}


