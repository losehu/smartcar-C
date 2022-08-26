/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            zf_driver_encoder
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ790875685)
 * @version         �鿴doc��version�ļ� �汾˵��
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

typedef enum    //TIMER����ö��
{
    //----------��ʱ��1----------
    //����ΪTIM1��������һ��ͨ����������ͨ�����á�
    //���磺TIM1_CH1_ENCOEDER_A8��TIM1_CH2_ENCOEDER_E11����ͬʱʹ�á�
    TIM1_CH1_ENCOEDER_A8  = 0x0000+32*0+8,     TIM1_CH2_ENCOEDER_A9    = 0x0000+32*0+9,      //Ĭ������
    TIM1_CH1_ENCOEDER_E9  = 0x0100+32*4+9,     TIM1_CH2_ENCOEDER_E11   = 0x0000+32*4+11,     //��ӳ��

    //----------��ʱ��2----------
    TIM2_CH1_ENCOEDER_A0  = 0x1000+32*0+0,     TIM2_CH2_ENCOEDER_A1    = 0x0000+32*0+1,      //Ĭ������
    TIM2_CH1_ENCOEDER_A15 = 0x1100+32*0+15,    TIM2_CH2_ENCOEDER_B3    = 0x0000+32*1+3,      //��ȫӳ��

    //----------��ʱ��3----------
    TIM3_CH1_ENCOEDER_A6  = 0x2000+32*0+6,     TIM3_CH2_ENCOEDER_A7    = 0x0000+32*0+7,      //Ĭ������
    TIM3_CH1_ENCOEDER_C6  = 0x2100+32*2+6,     TIM3_CH2_ENCOEDER_C7    = 0x0000+32*2+9,      //��ȫӳ��
    TIM3_CH1_ENCOEDER_B4  = 0x2200+32*1+4,     TIM3_CH2_ENCOEDER_B5    = 0x0000+32*1+5,      //����ӳ��

    //----------��ʱ��4----------
    TIM4_CH1_ENCOEDER_B6  = 0x3000+32*1+6,     TIM4_CH2_ENCOEDER_B7    = 0x0000+32*1+9,      //Ĭ������
    TIM4_CH1_ENCOEDER_D12 = 0x3100+32*3+12,    TIM4_CH2_ENCOEDER_D13   = 0x0000+32*3+13,     //�ض���

    //----------��ʱ��5----------
    TIM5_CH1_ENCOEDER_A0  = 0x4000+32*0+8,     TIM5_CH2_ENCOEDER_A1    = 0x0000+32*0+1,      //Ĭ������

    //----------��ʱ��8----------
    TIM8_CH1_ENCOEDER_C6  = 0x7000+32*2+6,     TIM8_CH2_ENCOEDER_C7    = 0x0000+32*2+7,      //Ĭ������

    //�ض���
    TIM8_CH1_ENCOEDER_B6  = 0x7100+32*1+6,     TIM8_CH2_ENCOEDER_B7    = 0x0000+32*1+7,      //Ĭ������

    //----------��ʱ��9----------
    TIM9_CH1_ENCOEDER_A2  = 0x8000+32*0+2,     TIM9_CH2_ENCOEDER_A3    = 0x0000+32*0+3,      //Ĭ������
    TIM9_CH1_ENCOEDER_D9  = 0x8100+32*3+9,     TIM9_CH2_ENCOEDER_D11   = 0x0000+32*3+11,     //�ض���

    //----------��ʱ��10----------
    TIM10_CH1_ENCOEDER_B8 = 0x9000+32*1+8,     TIME0_CH2_ENCOEDER_B9   = 0x0000+32*1+9,     //Ĭ������
    TIM10_CH1_ENCOEDER_D1 = 0x9100+32*3+1,     TIMER10_CH2_ENCOEDER_D3 = 0x0000+32*3+3,   //�ض���1
    TIM10_CH1_ENCOEDER_B3 = 0x9200+32*1+3,     TIMER10_CH2_ENCOEDER_B4 = 0x0000+32*1+4,   //�ض���2

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


#endif
