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

#include "zf_driver_timer.h"
#include "zf_driver_pit.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      定时器周期中断
//  @param      timer_ch      定时器通道
//  @param      us          定时周期(1-65535)
//  @return     void
//  Sample usage:           pit_init(TIMER_1, 5); 使用定时器1作为5ms一次的周期中断
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

    uint16_t freq_div = (period >> 15);                                 // 计算预分频
    uint16_t period_temp = (period / (freq_div+1));                     // 计算自动重装载值

    timer_clock_enable(pit_n);

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = period_temp;
    TIM_TimeBaseStructure.TIM_Prescaler = freq_div;                     // 分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;             // 设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;         // TIM向上计数模式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                    // 重复计数器设置为0
    TIM_TimeBaseInit((TIM_TypeDef*)tim_index[pit_n], &TIM_TimeBaseStructure);         // 根据指定的参数初始化TIMx的时间基数单位
    TIM_ITConfig((TIM_TypeDef*)tim_index[pit_n],TIM_IT_Update,ENABLE );               // 使能指定的TIM中断,允许更新中断
    TIM_ClearITPendingBit((TIM_TypeDef*)tim_index[pit_n], TIM_IT_Update);

    interrupt_set_priority((IRQn_Type)irq_index[(uint8_t)pit_n], 0x03);                    // 设置中断优先级
    interrupt_enable((IRQn_Type)irq_index[pit_n]);                                // 使能中断

    TIM_Cmd((TIM_TypeDef*)tim_index[pit_n], ENABLE);                                 // 使能定时器

}

//-------------------------------------------------------------------------------------------------------------------
// @brief       PIT 中断使能
// @param       pit_n           PIT 外设模块号
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
    interrupt_enable(irq_index[pit_n]);                                // 使能中断
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       PIT 中断禁止
// @param       pit_n           PIT 外设模块号
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

    interrupt_disable(irq_index[pit_n]);                                // 使能中断

}

