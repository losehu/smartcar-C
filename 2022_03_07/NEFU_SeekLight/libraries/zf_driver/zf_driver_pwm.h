/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            zf_driver_pwm
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/

#ifndef _zf_pwm_h
#define _zf_pwm_h

#include "ch32v30x_rcc.h"
#include "ch32v30x_gpio.h"
#include "ch32v30x_tim.h"
#include "zf_common_debug.h"

#include "Device.h"

typedef enum
{
    //同一个PWMP模块不同通道只能输出相同频率的PWM，占空比可设置不同
    //例如PWM1_PWM_CH1_PA8与PWM1_PWM_CH3_PA10属于同一个子模块，频率只能一样，但是占空比可以不一样

    //----------------PWM1----------------
    //以下为TIM1的PWM的一组通道，不允许通道混用。
    //例如：PWM1_PWM_CH2_PA9与PWM1_PWM_CH1N_PB13不能同时使用。
    TIM1_PWM_CH1_A8 = 0x0000,
    TIM1_PWM_CH2_A9,
    TIM1_PWM_CH3_A10,
    TIM1_PWM_CH4_A11,

    //重映射
    TIM1_PWM_CH1_E9 = 0x0010,
    TIM1_PWM_CH2_E11,
    TIM1_PWM_CH3_E13,
    TIM1_PWM_CH4_E14,

    //----------------PWM2----------------
    //以下为TIM2的PWM的一组通道，不允许通道混用。
    //例如：PWM2_PWM_CH1_A15与PWM2_PWM_CH3_A2不能同时使用。
    TIM2_PWM_CH1_A0 = 0x0100,
    TIM2_PWM_CH2_A1,
    TIM2_PWM_CH3_A2,
    TIM2_PWM_CH4_A3,

    //以下为TIM2的PWM的一组通道，不允许通道混用。
    TIM2_PWM_CH1_A15 = 0x0110,     //完全映射
    TIM2_PWM_CH2_B3,               //完全映射
    TIM2_PWM_CH3_B10,              //完全映射,下载器串口
    TIM2_PWM_CH4_B11,              //完全映射,下载器串口

    //----------------PWM3----------------
    ///以下为TIM3的PWM的一组通道，不允许通道混用。
    //例如：PWM3_PWM_CH1_PA6与PWM3_PWM_CH4_C9不能同时使用。
    TIM3_PWM_CH1_A6 = 0x0200,
    TIM3_PWM_CH2_A7,
    TIM3_PWM_CH3_B0,
    TIM3_PWM_CH4_B1,

    //以下为TIM3的PWM的一组通道，不允许通道混用。
    TIM3_PWM_CH1_C6 = 0x0210,      //完全映射
    TIM3_PWM_CH2_C7,               //完全映射
    TIM3_PWM_CH3_C8,               //完全映射
    TIM3_PWM_CH4_C9,               //完全映射

    //以下为TIM3的PWM的一组通道，不允许通道混用。
    TIM3_PWM_CH1_B4 = 0x0220,      //部分映射
    TIM3_PWM_CH2_B5,               //部分映射

    //----------------PWM4----------------
    //以下为TIM4的PWM的一组通道。
    TIM4_PWM_CH1_B6 = 0x0300,
    TIM4_PWM_CH2_B7,
    TIM4_PWM_CH3_B8,
    TIM4_PWM_CH4_B9,

    //重映射
    TIM4_PWM_CH1_D12 = 0x0310,
    TIM4_PWM_CH2_D13,
    TIM4_PWM_CH3_D14,
    TIM4_PWM_CH4_D15,


    //定时器5
    TIM5_PWM_CH1_A0 = 0x0400,
    TIM5_PWM_CH2_A1,
    TIM5_PWM_CH3_A2,
    TIM5_PWM_CH4_A3,

    //定时器8
    TIM8_PWM_CH1_C6 = 0x0700,
    TIM8_PWM_CH2_C7,
    TIM8_PWM_CH3_C8,
    TIM8_PWM_CH4_C9,

    //重映射
    TIM8_PWM_CH1_B6 = 0x0710,
    TIM8_PWM_CH2_B7,
    TIM8_PWM_CH3_B8,
    TIM8_PWM_CH4_C13,

    //定时器9
    TIM9_PWM_CH1_A2 = 0x0800,
    TIM9_PWM_CH2_A3,
    TIM9_PWM_CH3_A4,
    TIM9_PWM_CH4_C4,

    //重映射
    TIM9_PWM_CH1_D9 = 0x0810,
    TIM9_PWM_CH2_D11,
    TIM9_PWM_CH3_D13,
    TIM9_PWM_CH4_D15,

    //定时器10
    TIM10_PWM_CH1_B8 = 0x0900,
    TIM10_PWM_CH2_B9,
    TIM10_PWM_CH3_C3,
    TIM10_PWM_CH4_C11,

    //重定向一
    TIM10_PWM_CH1_D1 = 0x0910,
    TIM10_PWM_CH2_D3,
    TIM10_PWM_CH3_D5,
    TIM10_PWM_CH4_D7,

    //重定向二
    TIM10_PWM_CH1_B3 = 0x0920,
    TIM10_PWM_CH2_B4,
    TIM10_PWM_CH3_B5,
    TIM10_PWM_CH4_C15,

}pwm_channel_enum;

#define PWM_DUTY_MAX 10000

void    pwm_set_duty    (pwm_channel_enum pin, uint32_t duty);
void    pwm_set_freq    (pwm_channel_enum pin, uint32_t freq, uint32_t duty);
void    pwm_init        (pwm_channel_enum pin, uint32_t freq, uint32_t duty);
//user
uint32_t pwm_select_maxduty(pwm_channel_enum pin);
#endif
