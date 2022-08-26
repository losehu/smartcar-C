/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             zf_common_headfile
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ790875685)
* @version          查看doc内version文件 版本说明
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

#define USE_ZF_TYPEDEF      1                                                   // 是否启用类型定义申明
#if USE_ZF_TYPEDEF
// 数据类型声明
// 尽量使用 stdint.h 定义的类型名称 避免冲突 这里可以裁剪
typedef unsigned char       uint8;                                              // 无符号  8 bits
typedef unsigned short int  uint16;                                             // 无符号 16 bits
typedef unsigned long int   uint32;                                             // 无符号 32 bits
typedef unsigned long long  uint64;                                             // 无符号 64 bits

typedef char                int8;                                               // 有符号  8 bits
typedef short int           int16;                                              // 有符号 16 bits
typedef long  int           int32;                                              // 有符号 32 bits
typedef long  long          int64;                                              // 有符号 64 bits

typedef volatile uint8      vuint8;                                             // 易变性修饰 无符号  8 bits
typedef volatile uint16     vuint16;                                            // 易变性修饰 无符号 16 bits
typedef volatile uint32     vuint32;                                            // 易变性修饰 无符号 32 bits
typedef volatile uint64     vuint64;                                            // 易变性修饰 无符号 64 bits

typedef volatile int8       vint8;                                              // 易变性修饰 有符号  8 bits
typedef volatile int16      vint16;                                             // 易变性修饰 有符号 16 bits
typedef volatile int32      vint32;                                             // 易变性修饰 有符号 32 bits
typedef volatile int64      vint64;                                             // 易变性修饰 有符号 64 bits
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
//===================================================芯片 SDK 底层===================================================
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

//===================================================芯片 SDK 底层===================================================

//====================================================开源库公共层====================================================
#include "zf_common_clock.h"
#include "zf_common_debug.h"
#include "zf_common_font.h"
#include "zf_common_function.h"
#include "zf_common_interrupt.h"
#include "zf_common_fifo.h"
//====================================================开源库公共层====================================================

//===================================================芯片外设驱动层===================================================
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

//===================================================芯片外设驱动层===================================================

//===================================================外接设备驱动层===================================================
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
//===================================================外接设备驱动层===================================================
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

