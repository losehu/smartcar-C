/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            zf_driver_delay
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/
#ifndef _zf_driver_delay_h
#define _zf_driver_delay_h



#include "ch32v30x.h"

#include "zf_common_clock.h"

void system_delay(uint64_t time);

//------------------------------------以下宏定义用于SYSTICK延时--------------------------------------------------------------//
#define system_delay_ms(time)      system_delay(time * (system_clock/8000))    //设置SYSTICK延时时间  单位ms
#define system_delay_us(time)      system_delay(time * (system_clock/8000000)) //设置SYSTICK延时时间  单位us

#endif
