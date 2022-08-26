/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            zf_driver_pit
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ790875685)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/

#include "zf_driver_timer.h"
#include "zf_driver_pit.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ʱ�������ж�
//  @param      timer_ch      ��ʱ��ͨ��
//  @param      us          ��ʱ����(1-65535)
//  @return     void
//  Sample usage:           pit_init(TIMER_1, 5); ʹ�ö�ʱ��1��Ϊ5msһ�ε������ж�
//-------------------------------------------------------------------------------------------------------------------
void pit_init (pit_index_enum pit_n, uint32_t period)
{
    const uint32_t  irq_index[10] =
    {
        TIM1_UP_IRQn, TIM2_IRQn, TIM3_IRQn, TIM4_IRQn,
        TIM5_IRQn, TIM6_IRQn, TIM7_IRQn, TIM8_UP_IRQn,
        TIM9_UP_IRQn, TIM10_UP_IRQn
    };
    zf_assert(timer_funciton_check((timer_index_enum)pit_n, TIMER_FUNCTION_TIMER));
    zf_assert(period!=0);

    uint16_t freq_div = (period >> 15);                                 // ����Ԥ��Ƶ
    uint16_t period_temp = (period / (freq_div+1));                     // �����Զ���װ��ֵ

    timer_clock_enable(pit_n);

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = period_temp;
    TIM_TimeBaseStructure.TIM_Prescaler = freq_div;                     // ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;             // ����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;         // TIM���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                    // �ظ�����������Ϊ0
    TIM_TimeBaseInit((TIM_TypeDef*)tim_index[pit_n], &TIM_TimeBaseStructure);         // ����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
    TIM_ITConfig((TIM_TypeDef*)tim_index[pit_n],TIM_IT_Update,ENABLE );               // ʹ��ָ����TIM�ж�,��������ж�
    TIM_ClearITPendingBit((TIM_TypeDef*)tim_index[pit_n], TIM_IT_Update);

    interrupt_set_priority((IRQn_Type)irq_index[(uint8_t)pit_n], 0x03);                    // �����ж����ȼ�
    interrupt_enable((IRQn_Type)irq_index[pit_n]);                                // ʹ���ж�

    TIM_Cmd((TIM_TypeDef*)tim_index[pit_n], ENABLE);                                 // ʹ�ܶ�ʱ��

}

//-------------------------------------------------------------------------------------------------------------------
// @brief       PIT �ж�ʹ��
// @param       pit_n           PIT ����ģ���
// @return      void
// Sample usage:                pit_enable(TIM1_PIT);
//-------------------------------------------------------------------------------------------------------------------
void pit_enable (pit_index_enum pit_n)
{
    const uint32_t irq_index[] =
    {
        TIM1_UP_IRQn, TIM2_IRQn, TIM3_IRQn, TIM4_IRQn,
        TIM5_IRQn, TIM6_IRQn, TIM7_IRQn, TIM8_UP_IRQn,
        TIM9_UP_IRQn, TIM10_UP_IRQn
    };
    interrupt_enable(irq_index[pit_n]);                                // ʹ���ж�
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       PIT �жϽ�ֹ
// @param       pit_n           PIT ����ģ���
// @return      void
// Sample usage:                pit_disable(TIM1_PIT);
//-------------------------------------------------------------------------------------------------------------------
void pit_disable (pit_index_enum pit_n)
{
    const uint32_t irq_index[] =
    {
        TIM1_UP_IRQn, TIM2_IRQn, TIM3_IRQn, TIM4_IRQn,
        TIM5_IRQn, TIM6_IRQn, TIM7_IRQn, TIM8_UP_IRQn,
        TIM9_UP_IRQn, TIM10_UP_IRQn
    };

    interrupt_disable(irq_index[pit_n]);                                // ʹ���ж�

}

