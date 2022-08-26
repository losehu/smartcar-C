/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             zf_common_clock
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ790875685)
* @version          查看doc内version文件 版本说明
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
********************************************************************************************************************/

#ifndef _zf_common_clock_h_
#define _zf_common_clock_h_

#include "ch32v30x.h"

#define BOARD_XTAL_FREQ         8000000                                         // 晶振频率 如果自己用的不是这个频率就修改这里 UM 定义范围为 4-24Mhz
//#define XTAL_STARTUP_TIMEOUT    0x0800                                          // 晶振就绪等待超时时长

typedef enum
{
    SYSTEM_CLOCK_XTAL       = BOARD_XTAL_FREQ,                                  // 使用晶振频率作为时钟频率
    SYSTEM_CLOCK_24M        = 24000000,                                         // 24Mhz
    SYSTEM_CLOCK_48M        = 48000000,                                         // 48Mhz
    SYSTEM_CLOCK_72M        = 72000000,                                         // 72Mhz
    SYSTEM_CLOCK_96M        = 96000000,                                         // 96Mhz
    SYSTEM_CLOCK_120M       = 120000000,                                        // 120Mhz
    SYSTEM_CLOCK_144M       = 144000000,                                        // 144Mhz
}system_clock_enum;

extern uint32_t system_clock;                                                   // 全局变量 系统时钟信息

void clock_init (uint32_t clock);                                               // 核心时钟初始化

#endif
