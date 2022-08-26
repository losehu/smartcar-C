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

#ifndef _zf_usb_cdc_h
#define _zf_usb_cdc_h

#include "ch32v30x_usbotg_device.h"


void usb_cdc_init( void );
void cdc_send_pack(uint8_t *p, uint32_t length);
void camera_send_image_usb_cdc(uint8_t *image, uint32_t length);

#endif
