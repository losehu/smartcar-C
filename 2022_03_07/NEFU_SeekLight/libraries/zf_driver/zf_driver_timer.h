/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            zf_driver_timer
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/

#ifndef _zf_timer_h
#define _zf_timer_h

#include "ch32v30x_rcc.h"
#include "ch32v30x_gpio.h"
#include "ch32v30x_tim.h"
#include "ch32v30x_misc.h"

#include "zf_common_debug.h"
#include "zf_common_clock.h"

//此枚举定义不允许用户修改
typedef enum
{
    TIM_1,
    TIM_2,
    TIM_3,
    TIM_4,
    TIM_5,
    TIM_6,
    TIM_7,
    TIM_8,
    TIM_9,
    TIM_10,
}timer_index_enum;

typedef enum
{
    TIMER_SYSTEM_CLOCK,                                                         // 系统频率计时 最大 0xFFFF
    TIMER_US,                                                                   // 微秒计时 最大 0xFFFF
    TIMER_MS,                                                                   // 毫秒计时 最大 0xFFFF/2
}timer_mode_enum;

typedef enum
{
    TIMER_FUNCTION_INIT = 0,                                                    // 功能未初始化
    TIMER_FUNCTION_TIMER,                                                       // 用作 TIMER 计时
    TIMER_FUNCTION_PIT,                                                         // 用作 PIT
    TIMER_FUNCTION_PWM,                                                         // 用作 PWM
    TIMER_FUNCTION_ENCODER,                                                     // 用作 ENCODER
    TIMER_FUNCTION_CAMERA,                                                      // 用作 CAMERA
}timer_function_enum;

extern const uint32_t tim_index[] ;


uint8_t     timer_funciton_check    (timer_index_enum index, timer_function_enum mode);
void        timer_clock_enable      (timer_index_enum index);
void        timer_start             (timer_index_enum index, timer_mode_enum mode);
uint16_t    timer_get               (timer_index_enum index);
void        timer_stop              (timer_index_enum index);





#endif
