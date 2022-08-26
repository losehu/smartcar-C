/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            zf_driver_timer
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


// �������ֹ�޸ģ��ڲ�ʹ���û��������
static timer_function_enum timer_function_state[10] =
{
    TIMER_FUNCTION_INIT, TIMER_FUNCTION_INIT,
    TIMER_FUNCTION_INIT, TIMER_FUNCTION_INIT,
    TIMER_FUNCTION_INIT, TIMER_FUNCTION_INIT,
    TIMER_FUNCTION_INIT, TIMER_FUNCTION_INIT,
    TIMER_FUNCTION_INIT, TIMER_FUNCTION_INIT
};
// �������ֹ�޸ģ��ڲ�ʹ���û��������
const uint32_t tim_index[] =
{
    TIM1_BASE, TIM2_BASE, TIM3_BASE, TIM4_BASE,
    TIM5_BASE, TIM6_BASE, TIM7_BASE, TIM8_BASE,
    TIM9_BASE, TIM10_BASE
};

//-------------------------------------------------------------------------------------------------------------------
// @brief       TIMER ����ȷ�Ϲ���״̬ ���ڲ�����
// @param       index           TIMER ����ģ���
// @param       mode            ��Ҫȷ�Ĺ���ģ��
// @return      uint8_t         1-����ʹ�� 0-������ʹ��
// Sample usage:                zf_assert(timer_funciton_check(TIM_1, TIMER_FUNCTION_PWM);
//-------------------------------------------------------------------------------------------------------------------
uint8_t timer_funciton_check (timer_index_enum index, timer_function_enum mode)
{
    if(timer_function_state[index] == TIMER_FUNCTION_INIT)
        timer_function_state[index] = mode;
    else if(timer_function_state[index] == mode)
        return 1;
    else
        return 0;
    return 1;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ʱ��ʱ�����߳�ʼ��
//  @param      timer_ch      ��ʱ��ͨ��
//  @return     void
//  Sample usage:          �ڲ�ʹ�ã��û��������
//-------------------------------------------------------------------------------------------------------------------
void timer_clock_enable(timer_index_enum index)
{
    if(TIM_1 == index)        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);//ʹ�ܶ�ʱ��1��ʱ��
    else  if(TIM_2 == index)  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//ʹ�ܶ�ʱ��2��ʱ��
    else  if(TIM_3 == index)  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//ʹ�ܶ�ʱ��3��ʱ��
    else  if(TIM_4 == index)  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//ʹ�ܶ�ʱ��4��ʱ��
    else  if(TIM_5 == index)  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);//ʹ�ܶ�ʱ��5��ʱ��
    else  if(TIM_6 == index)  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);//ʹ�ܶ�ʱ��6��ʱ��
    else  if(TIM_7 == index)  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//ʹ�ܶ�ʱ��7��ʱ��
    else  if(TIM_8 == index)  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);//ʹ�ܶ�ʱ��8��ʱ��
    else  if(TIM_9 == index)  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);//ʹ�ܶ�ʱ��9��ʱ��
    else  if(TIM_10== index)  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);//ʹ�ܶ�ʱ��10��ʱ��
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ʱ����ʼ
//  @param      timer_ch      ѡ��ģ���ͨ�� (ѡ��Χ ��TIMERN_enumö��ֵ������ȷ��)
//  @return     void
//  Sample usage:           timer_start(TIM_1, TIMER_US);  //��ʱ��1 ��ʼ������ÿһus����һ��
//-------------------------------------------------------------------------------------------------------------------
void timer_start(timer_index_enum index, timer_mode_enum mode)
{

    zf_assert(timer_funciton_check(index, TIMER_FUNCTION_TIMER));

    //����ʱ������
    timer_clock_enable(index);

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    if(mode == TIMER_US)
    {
        TIM_TimeBaseStructure.TIM_Period = 0xFFFF;                          // װ���Զ���װ��ֵ
        TIM_TimeBaseStructure.TIM_Prescaler = system_clock/1000000-1;           // װ��Ԥ��Ƶ
    }
    else if(mode == TIMER_MS)
    {
        TIM_TimeBaseStructure.TIM_Period = 0xFFFF;                          // װ���Զ���װ��ֵ
        TIM_TimeBaseStructure.TIM_Prescaler = (system_clock/1000)/2-1;          // װ��Ԥ��Ƶ
    }
    else
    {
        TIM_TimeBaseStructure.TIM_Period = 0xFFFF;                          // װ���Զ���װ��ֵ
        TIM_TimeBaseStructure.TIM_Prescaler = 0;                                // װ��Ԥ��Ƶ
    }
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                     // ����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                 // TIM���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                            // �ظ�����������Ϊ0
    TIM_TimeBaseInit((TIM_TypeDef*)tim_index[index], &TIM_TimeBaseStructure);                 // ����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    TIM_Cmd((TIM_TypeDef*)tim_index[index], ENABLE);                                          // ʹ��TIMx
}




//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ��ʱ��ֵ
//  @param      timer_ch      ѡ��ģ���ͨ�� (ѡ��Χ ��TIMERN_enumö��ֵ������ȷ��)
//  @return     uint32_t      ����ֵ
//  Sample usage:             timer_get(TIM_1)  //��ȡ��ʱ��1�ļ�ʱʱ��
//-------------------------------------------------------------------------------------------------------------------
uint16_t timer_get(timer_index_enum index)
{
    return ((TIM_TypeDef*)tim_index[index])->CNT;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �رռ�ʱ��
//  @param      timer_ch      ѡ��ģ���ͨ�� (ѡ��Χ ��TIMERN_enumö��ֵ������ȷ��)
//  @return     void
//  Sample usage:             �رն�ʱ���������־λ
//-------------------------------------------------------------------------------------------------------------------
void timer_stop(timer_index_enum index)
{
    TIM_Cmd((TIM_TypeDef*)tim_index[index], DISABLE);  //ʧ��TIM
}



