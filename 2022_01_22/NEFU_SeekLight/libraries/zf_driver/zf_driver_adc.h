/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            zf_driver_adc
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ790875685)
 * @version         �鿴doc��version�ļ� �汾˵��
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

#include "zf_common_debug.h"
//CH32V307ֻ��һ��ADC�����Է�ʱ�����ںܶ��ͨ������


//��ö�ٶ��岻�����û��޸�
typedef enum    // ö��ADCͨ��
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


//��ö�ٶ��岻�����û��޸�
typedef enum            // ö��ADCͨ��
{
    ADC_8BIT = 4,       //8λ�ֱ���
    ADC_10BIT = 2,      //10λ�ֱ���
    ADC_12BIT = 0,      //12λ�ֱ���
}ADC_RES_enum;




uint16_t adc_convert(ADC_CH_enum adc_ch, ADC_RES_enum resolution);
uint16_t adc_mean_filter(ADC_CH_enum adc_ch, ADC_RES_enum resolution, uint8_t count);
void adc_init(ADC_CH_enum adc_ch);


#endif
