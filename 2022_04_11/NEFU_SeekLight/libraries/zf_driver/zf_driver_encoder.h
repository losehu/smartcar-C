/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            zf_driver_encoder
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/
#ifndef _zf_driver_encoder_h
#define _zf_driver_encoder_h

#include "ch32v30x_rcc.h"
#include "ch32v30x_gpio.h"
#include "ch32v30x_tim.h"

#include "zf_common_debug.h"

typedef enum    //TIMER引脚枚举
{
    //----------定时器1----------
    //以下为TIM1编码器的一组通道，不允许通道混用。
    //例如：TIM1_CH1_ENCOEDER_A8与TIM1_CH2_ENCOEDER_E11不能同时使用。
    TIM1_CH1_ENCOEDER_A8  = 0x0000+32*0+8,     TIM1_CH2_ENCOEDER_A9    = 0x0000+32*0+9,      //默认引脚
    TIM1_CH1_ENCOEDER_E9  = 0x0100+32*4+9,     TIM1_CH2_ENCOEDER_E11   = 0x0000+32*4+11,     //重映射

    //----------定时器2----------
    TIM2_CH1_ENCOEDER_A0  = 0x1000+32*0+0,     TIM2_CH2_ENCOEDER_A1    = 0x0000+32*0+1,      //默认引脚
    TIM2_CH1_ENCOEDER_A15 = 0x1100+32*0+15,    TIM2_CH2_ENCOEDER_B3    = 0x0000+32*1+3,      //完全映射

    //----------定时器3----------
    TIM3_CH1_ENCOEDER_A6  = 0x2000+32*0+6,     TIM3_CH2_ENCOEDER_A7    = 0x0000+32*0+7,      //默认引脚
    TIM3_CH1_ENCOEDER_C6  = 0x2100+32*2+6,     TIM3_CH2_ENCOEDER_C7    = 0x0000+32*2+9,      //完全映射
    TIM3_CH1_ENCOEDER_B4  = 0x2200+32*1+4,     TIM3_CH2_ENCOEDER_B5    = 0x0000+32*1+5,      //部分映射

    //----------定时器4----------
    TIM4_CH1_ENCOEDER_B6  = 0x3000+32*1+6,     TIM4_CH2_ENCOEDER_B7    = 0x0000+32*1+9,      //默认引脚
    TIM4_CH1_ENCOEDER_D12 = 0x3100+32*3+12,    TIM4_CH2_ENCOEDER_D13   = 0x0000+32*3+13,     //重定向

    //----------定时器5----------
    TIM5_CH1_ENCOEDER_A0  = 0x4000+32*0+8,     TIM5_CH2_ENCOEDER_A1    = 0x0000+32*0+1,      //默认引脚

    //----------定时器8----------
    TIM8_CH1_ENCOEDER_C6  = 0x7000+32*2+6,     TIM8_CH2_ENCOEDER_C7    = 0x0000+32*2+7,      //默认引脚

    //重定向
    TIM8_CH1_ENCOEDER_B6  = 0x7100+32*1+6,     TIM8_CH2_ENCOEDER_B7    = 0x0000+32*1+7,      //默认引脚

    //----------定时器9----------
    TIM9_CH1_ENCOEDER_A2  = 0x8000+32*0+2,     TIM9_CH2_ENCOEDER_A3    = 0x0000+32*0+3,      //默认引脚
    TIM9_CH1_ENCOEDER_D9  = 0x8100+32*3+9,     TIM9_CH2_ENCOEDER_D11   = 0x0000+32*3+11,     //重定向

    //----------定时器10----------
    TIM10_CH1_ENCOEDER_B8 = 0x9000+32*1+8,     TIME0_CH2_ENCOEDER_B9   = 0x0000+32*1+9,     //默认引脚
    TIM10_CH1_ENCOEDER_D1 = 0x9100+32*3+1,     TIMER10_CH2_ENCOEDER_D3 = 0x0000+32*3+3,   //重定向1
    TIM10_CH1_ENCOEDER_B3 = 0x9200+32*1+3,     TIMER10_CH2_ENCOEDER_B4 = 0x0000+32*1+4,   //重定向2

}encoder_channel_enum;


typedef enum
{
    TIM1_ENCOEDER = 0,
    TIM2_ENCOEDER,
    TIM3_ENCOEDER,
    TIM4_ENCOEDER,
    TIM5_ENCOEDER,
    TIM6_ENCOEDER,
    TIM7_ENCOEDER,
    TIM8_ENCOEDER,
    TIM9_ENCOEDER,
    TIM10_ENCOEDER,
}encoder_index_enum;


//void encoder_init_dir (encoder_index_enum encoder_n, encoder_channel_enum ch1_pin, encoder_channel_enum ch2_pin);
void encoder_init_quad (encoder_index_enum encoder_n, encoder_channel_enum ch1_pin, encoder_channel_enum ch2_pin);
void encoder_init_dir(encoder_index_enum encoder_n, encoder_channel_enum ch1_pin, encoder_channel_enum ch2_pin);
int16_t encoder_get_count (encoder_index_enum encoder_n);
void encoder_clean_count (encoder_index_enum encoder_n);

extern int show_flag;

#endif
