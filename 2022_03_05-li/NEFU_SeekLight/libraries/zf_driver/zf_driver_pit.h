/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            zf_driver_pit
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/

#ifndef _zf_pit_h
#define _zf_pit_h

#include "ch32v30x_rcc.h"
#include "ch32v30x_gpio.h"
#include "ch32v30x_tim.h"
#include "ch32v30x_misc.h"

#include "zf_common_debug.h"
#include "zf_common_interrupt.h"


typedef enum                                                                    // 枚举 TIM通道
{
    TIM1_PIT,
    TIM2_PIT,
    TIM3_PIT,
    TIM4_PIT,
    TIM5_PIT,
    TIM6_PIT,
    TIM7_PIT,
    TIM8_PIT,
    TIM9_PIT,
    TIM10_PIT
}pit_index_enum;



void pit_init    (pit_index_enum pit_n, uint32_t period);
void pit_enable  (pit_index_enum pit_n);
void pit_disable (pit_index_enum pit_n);

// pit中断调用下面函数即可
#define pit_init_ms(pit_n, ms)        pit_init(pit_n, ms*(system_clock/1000))
#define pit_init_us(pit_n, us)        pit_init(pit_n, us*(system_clock/1000000))


#endif
