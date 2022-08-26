/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_common_headfile
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
********************************************************************************************************************/

#ifndef __HEADFILE_H
#define __HEADFILE_H


#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"

#define USE_ZF_TYPEDEF      1                                                   // �Ƿ��������Ͷ�������
#if USE_ZF_TYPEDEF
// ������������
// ����ʹ�� stdint.h ������������� �����ͻ ������Բü�
typedef unsigned char       uint8;                                              // �޷���  8 bits
typedef unsigned short int  uint16;                                             // �޷��� 16 bits
typedef unsigned long int   uint32;                                             // �޷��� 32 bits
typedef unsigned long long  uint64;                                             // �޷��� 64 bits

typedef char                int8;                                               // �з���  8 bits
typedef short int           int16;                                              // �з��� 16 bits
typedef long  int           int32;                                              // �з��� 32 bits
typedef long  long          int64;                                              // �з��� 64 bits

typedef volatile uint8      vuint8;                                             // �ױ������� �޷���  8 bits
typedef volatile uint16     vuint16;                                            // �ױ������� �޷��� 16 bits
typedef volatile uint32     vuint32;                                            // �ױ������� �޷��� 32 bits
typedef volatile uint64     vuint64;                                            // �ױ������� �޷��� 64 bits

typedef volatile int8       vint8;                                              // �ױ������� �з���  8 bits
typedef volatile int16      vint16;                                             // �ױ������� �з��� 16 bits
typedef volatile int32      vint32;                                             // �ױ������� �з��� 32 bits
typedef volatile int64      vint64;                                             // �ױ������� �з��� 64 bits
#endif



#define MAX max
#define MIN min
#define max(x, y)  ({\
int _x = x;\
int _y = y;\
_x > _y ? _x : _y;\
})
#define min(x, y)  ({\
int _x = x;\
int _y = y;\
_x < _y ? _x : _y;\
})
//===================================================оƬ SDK �ײ�===================================================
#include "ch32v30x_adc.h"
#include "ch32v30x_bkp.h"
#include "ch32v30x_can.h"
#include "ch32v30x_crc.h"
#include "ch32v30x_dac.h"
#include "ch32v30x_dbgmcu.h"
#include "ch32v30x_dma.h"
#include "ch32v30x_exti.h"
#include "ch32v30x_flash.h"
#include "ch32v30x_fsmc.h"
#include "ch32v30x_gpio.h"
#include "ch32v30x_i2c.h"
#include "ch32v30x_iwdg.h"
#include "ch32v30x_pwr.h"
#include "ch32v30x_rcc.h"
#include "ch32v30x_rtc.h"
#include "ch32v30x_sdio.h"
#include "ch32v30x_spi.h"
#include "ch32v30x_tim.h"
#include "ch32v30x_usart.h"
#include "ch32v30x_wwdg.h"
#include "ch32v30x_misc.h"

//===================================================оƬ SDK �ײ�===================================================

//====================================================��Դ�⹫����====================================================
#include "zf_common_clock.h"
#include "zf_common_debug.h"
#include "zf_common_font.h"
#include "zf_common_function.h"
#include "zf_common_interrupt.h"
#include "zf_common_fifo.h"
//====================================================��Դ�⹫����====================================================

//===================================================оƬ����������===================================================
#include "zf_driver_adc.h"
#include "zf_driver_delay.h"
#include "zf_driver_dvp.h"
#include "zf_driver_encoder.h"
//#include "zf_driver_exti.h"
#include "zf_driver_flash.h"
#include "zf_driver_gpio.h"
//#include "zf_driver_iic.h"
#include "zf_driver_pit.h"
#include "zf_driver_pwm.h"
//#include "zf_driver_soft_iic.h"
//#include "zf_driver_soft_spi.h"
#include "zf_driver_spi.h"
#include "zf_driver_timer.h"
#include "zf_driver_uart.h"
#include "zf_driver_usb_cdc.h"

//===================================================оƬ����������===================================================

//===================================================����豸������===================================================
#include "zf_device_camera.h"
#include "zf_device_icm20602.h"
#include "zf_device_ips114.h"
#include "zf_device_tft180.h"
#include "zf_device_ips200_parallel8.h"
#include "zf_device_mt9v03x_dvp.h"
#include "zf_device_mpu6050.h"
#include "zf_device_type.h"
#include "zf_device_wireless_uart.h"
#include "zf_device_oled.h"
#include "zf_device_scc8660_dvp.h"
#include "zf_device_bluetooth_ch9141.h"
#include "zf_device_bluetooth_ch9141_ch2.h"
#include "zf_device_virtual_oscilloscope.h"
//===================================================����豸������===================================================
#include "swj.h"

#include "All_Init.h"
#include "ANO_DT.h"
#include "Device.h"
#include "Filter.h"
#include "image_deal.h"
#include "cam.h"
#include "control.h"
#include "display.h"
#include "elc.h"
#include "pid.h"
#include "My_Flash.h"
#include "element.h"
#include "game.h"

#endif

