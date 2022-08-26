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

#include "zf_driver_gpio.h"
#include "zf_driver_timer.h"
#include "zf_driver_encoder.h"

static uint8_t encoder_dir_pin[TIM10_ENCOEDER + 1] = { 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���������ų�ʼ��
//  @param      phaseA      ͨ��A�Լ�����
//  @param      phaseB      ͨ��B�Լ�����
//  @return     void
//  Sample usage:           �ڲ�ʹ�ã��û��������
//-------------------------------------------------------------------------------------------------------------------
void encoder_gpio_init(gpio_pin_enum pin_ch1, gpio_pin_enum pin_ch2) {
    gpio_init(pin_ch1, GPI, 0, GPI_PULL_UP);
    gpio_init(pin_ch2, GPI, 0, GPI_PULL_UP);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ʱ������������ȡֵ
//  @param      timer_ch      ��ʱ��ö����
//  @return     void
//  Sample usage:
//                  encoder_get_count(TIM2_ENCOEDER)  // ��ȡ��ʱ��2�Ĳɼ����ı���������
//-------------------------------------------------------------------------------------------------------------------
int show_flag=0;

int16_t encoder_get_count(encoder_index_enum encoder_n) {
    if (encoder_dir_pin[encoder_n] == 0xFF) {
        return ((TIM_TypeDef*) tim_index[encoder_n])->CNT;
    } else {
        if (gpio_get_level(encoder_dir_pin[encoder_n]))               // �������Ÿߵ�ƽ
                {
            if (encoder_n == 1&&show_flag) {
                ips114_show_int16(0, 7, 1);
                my_delay(1);

            }
            return (uint16_t) ((TIM_TypeDef*) tim_index[encoder_n])->CNT;

        }                                 // ��������ֵ
        else {    // �������ŵ͵�ƽ
            if (encoder_n == 1&&show_flag) {
                ips114_show_int16(0, 7, 2);
                my_delay(1);
            }
            return -((uint16_t) ((TIM_TypeDef*) tim_index[encoder_n])->CNT); // ���ظ���ֵ

        }
    }

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ʱ���ļ��������
//  @param      timer_ch      ��ʱ��ö����
//  @return     void
//  Sample usage:
//                  encoder_clean_count(TIM1_ENCOEDER)  //�����ʱ��1�ɼ����ı���������
//-------------------------------------------------------------------------------------------------------------------
void encoder_clean_count(encoder_index_enum encoder_n) {
    ((TIM_TypeDef*) tim_index[encoder_n])->CNT = 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �����������ʼ��
//  @param      timer_ch      ��ʱ��ö����
//  @param      phaseA      ͨ��A�Լ�����
//  @param      phaseB      ͨ��B�Լ�����
//  @return     void
//  Sample usage:           �Ƽ�ʹ�����������������
//                          encoder_init_quad(TIM1_ENCOEDER, TIM1_CH1_ENCOEDER_E9, TIM1_CH2_ENCOEDER_E11)
//                          // ʹ�ö�ʱ��1 ���������������룬 ͨ��1���ź�E9��ͨ��2���ź�E11
//-------------------------------------------------------------------------------------------------------------------
void encoder_init_quad(encoder_index_enum encoder_n,
        encoder_channel_enum ch1_pin, encoder_channel_enum ch2_pin) {
    zf_assert(
            timer_funciton_check((timer_index_enum )encoder_n,
                    TIMER_FUNCTION_ENCODER));

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    timer_clock_enable(encoder_n);

    encoder_gpio_init((gpio_pin_enum) (ch1_pin & 0xFF),
            (gpio_pin_enum) (ch2_pin & 0xFF));                           //��ʼ������

    if ((ch1_pin >> 8) == 0x01)
        GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
    else if ((ch1_pin >> 8) == 0x11)
        GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
    else if ((ch1_pin >> 8) == 0x21)
        GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
    else if ((ch1_pin >> 8) == 0x22)
        GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
    else if ((ch1_pin >> 8) == 0x31)
        GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);
    else if ((ch1_pin >> 8) == 0x71)
        AFIO->PCFR2 |= 0x01 << 2;
    else if ((ch1_pin >> 8) == 0x81)
        AFIO->PCFR2 |= 0x02 << 3;
    else if ((ch1_pin >> 8) == 0x91)
        AFIO->PCFR2 |= 0x02 << 5;
    else if ((ch1_pin >> 8) == 0x92)
        AFIO->PCFR2 |= 0x01 << 5;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);        //ʹ��AFIO���ù���ģ��ʱ��

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 0;                              //Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;                      //�趨�������Զ���װֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //ѡ��ʱ�ӷ�Ƶ������Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;        //TIM���ϼ���
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                     //�ظ�������
    TIM_TimeBaseInit((TIM_TypeDef*) tim_index[encoder_n],
            &TIM_TimeBaseStructure);       //��ʼ���ṹ��
    TIM_EncoderInterfaceConfig((TIM_TypeDef*) tim_index[encoder_n],
    TIM_EncoderMode_TI2, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); //ʹ�ñ�����ģʽ

    TIM_Cmd((TIM_TypeDef*) tim_index[encoder_n], ENABLE);                //��ʱ��ʹ��
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �����������ʼ��
//  @param      timer_ch      ��ʱ��ö����
//  @param      phaseA      ͨ��A�Լ�����
//  @param      phaseB      ͨ��B�Լ�����
//  @return     void
//  Sample usage:           �Ƽ�ʹ�����������������
//                          encoder_init_dir(TIM1_ENCOEDER, TIM1_CH1_ENCOEDER_E9, TIM1_CH2_ENCOEDER_E11)
//                          // ʹ�ö�ʱ��1 ���������������룬 ͨ��1�����ź�����E9��ͨ��2�����ź�����E11
//-------------------------------------------------------------------------------------------------------------------
void encoder_init_dir(encoder_index_enum encoder_n,
        encoder_channel_enum ch1_pin, encoder_channel_enum ch2_pin) {
    zf_assert(
            timer_funciton_check((timer_index_enum )encoder_n,
                    TIMER_FUNCTION_ENCODER));

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    timer_clock_enable(encoder_n);

    encoder_gpio_init((gpio_pin_enum) (ch1_pin & 0xFF),
            (gpio_pin_enum) (ch2_pin & 0xFF));

    encoder_dir_pin[encoder_n] = (ch1_pin & 0xFF);           //���������źŴ���������

    if ((ch1_pin >> 8) == 0x01)
        GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
    else if ((ch1_pin >> 8) == 0x11)
        GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
    else if ((ch1_pin >> 8) == 0x21)
        GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
    else if ((ch1_pin >> 8) == 0x22)
        GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
    else if ((ch1_pin >> 8) == 0x31)
        GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);
    else if ((ch1_pin >> 8) == 0x71)
        AFIO->PCFR2 |= 0x01 << 2;
    else if ((ch1_pin >> 8) == 0x81)
        AFIO->PCFR2 |= 0x02 << 3;
    else if ((ch1_pin >> 8) == 0x91)
        AFIO->PCFR2 |= 0x02 << 5;
    else if ((ch1_pin >> 8) == 0x92)
        AFIO->PCFR2 |= 0x01 << 5;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);        //ʹ��AFIO���ù���ģ��ʱ��

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 0;                              //Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;                      //�趨�������Զ���װֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //ѡ��ʱ�ӷ�Ƶ������Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;        //TIM���ϼ���
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                     //�ظ�������
    TIM_TimeBaseInit((TIM_TypeDef*) tim_index[encoder_n],
            &TIM_TimeBaseStructure);           //��ʼ���ṹ��

    TIM_ITRxExternalClockConfig((TIM_TypeDef*) tim_index[encoder_n],
    TIM_TS_TI2FP2);          //�����ⲿ���������򲻻����

    TIM_Cmd((TIM_TypeDef*) tim_index[encoder_n], ENABLE);                //��ʱ��ʹ��
}

