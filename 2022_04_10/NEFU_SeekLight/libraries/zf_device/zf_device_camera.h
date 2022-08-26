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
