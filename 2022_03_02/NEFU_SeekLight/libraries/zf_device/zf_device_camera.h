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

#ifndef _zf_device_camera_h_
#define _zf_device_camera_h_

#include "zf_common_debug.h"
#include "zf_common_interrupt.h"

#include "zf_driver_timer.h"
#include "zf_driver_dvp.h"
#include "zf_driver_uart.h"

#include "zf_device_type.h"


extern uint8_t *camera_buffer_addr;

void camera_init (camera_type_enum camera_install, uint32_t image_size);

void camera_uart_handler (void);
//void camera_vsync_handler (void);
//void camera_dma_handler (void);
void camera_dvp_handler(void);

void camera_binary_image_decompression (uint8_t *data1, uint8_t *data2, uint32_t image_size);
void camera_send_image (uart_index_enum uartn, void *image_addr, uint32_t image_size);

#endif
