/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            zf_driver_adc
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/
#ifndef _zf_adc_h
#define _zf_adc_h

#include "ch32v30x.h"
#include "ch32v30x_adc.h"
#include "ch32v30x_rcc.h"
#include "zf_common_headfile.h"
#include "zf_common_debug.h"
//CH32V307只有一个ADC，可以分时复用在很多个通道上面


//此枚举定义不允许用户修改
typedef enum    // 枚举ADC通道
{
    ADC_IN0_A0 = 0,
    ADC_IN1_A1,
    ADC_IN2_A2,
    ADC_IN3_A3,
    ADC_IN4_A4,
    ADC_IN5_A5,
    ADC_IN6_A6,
    ADC_IN7_A7,
    ADC_IN8_B0,
    ADC_IN9_B1,
    ADC_IN10_C0,
    ADC_IN11_C1,
    ADC_IN12_C2,
    ADC_IN13_C3,
    ADC_IN14_C4,
    ADC_IN15_C5,
}ADC_CH_enum;


//此枚举定义不允许用户修改
typedef enum            // 枚举ADC通道
{
    ADC_8BIT = 4,       //8位分辨率
    ADC_10BIT = 2,      //10位分辨率
    ADC_12BIT = 0,      //12位分辨率
}ADC_RES_enum;




uint16_t adc_convert(ADC_CH_enum adc_ch, ADC_RES_enum resolution);
uint16_t adc_mean_filter(ADC_CH_enum adc_ch, ADC_RES_enum resolution, uint8_t count);
void adc_init(ADC_CH_enum adc_ch);


#endif
