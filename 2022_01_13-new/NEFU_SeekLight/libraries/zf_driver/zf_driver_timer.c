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

#include "zf_driver_gpio.h"
#include "zf_driver_timer.h"


// 该数组禁止修改，内部使用用户无需关心
static timer_function_enum timer_function_state[10] =
{
    TIMER_FUNCTION_INIT, TIMER_FUNCTION_INIT,
    TIMER_FUNCTION_INIT, TIMER_FUNCTION_INIT,
    TIMER_FUNCTION_INIT, TIMER_FUNCTION_INIT,
    TIMER_FUNCTION_INIT, TIMER_FUNCTION_INIT,
    TIMER_FUNCTION_INIT, TIMER_FUNCTION_INIT
};
// 该数组禁止修改，内部使用用户无需关心
const uint32_t tim_index[] =
{
    TIM1_BASE, TIM2_BASE, TIM3_BASE, TIM4_BASE,
    TIM5_BASE, TIM6_BASE, TIM7_BASE, TIM8_BASE,
    TIM9_BASE, TIM10_BASE
};

//-------------------------------------------------------------------------------------------------------------------
// @brief       TIMER 外设确认功能状态 库内部调用
// @param       index           TIMER 外设模块号
// @param       mode            需要确的功能模块
// @return      uint8_t         1-可以使用 0-不可以使用
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
//  @brief      定时器时钟总线初始化
//  @param      timer_ch      定时器通道
//  @return     void
//  Sample usage:          内部使用，用户无需关心
//-------------------------------------------------------------------------------------------------------------------
void timer_clock_enable(timer_index_enum index)
{
    if(TIM_1 == index)        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);//使能定时器1的时钟
    else  if(TIM_2 == index)  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//使能定时器2的时钟
    else  if(TIM_3 == index)  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//使能定时器3的时钟
    else  if(TIM_4 == index)  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//使能定时器4的时钟
    else  if(TIM_5 == index)  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);//使能定时器5的时钟
    else  if(TIM_6 == index)  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);//使能定时器6的时钟
    else  if(TIM_7 == index)  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//使能定时器7的时钟
    else  if(TIM_8 == index)  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);//使能定时器8的时钟
    else  if(TIM_9 == index)  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);//使能定时器9的时钟
    else  if(TIM_10== index)  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);//使能定时器10的时钟
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      计时器开始
//  @param      timer_ch      选择模块的通道 (选择范围 由TIMERN_enum枚举值的内容确定)
//  @return     void
//  Sample usage:           timer_start(TIM_1, TIMER_US);  //定时器1 开始计数，每一us计数一次
//-------------------------------------------------------------------------------------------------------------------
void timer_start(timer_index_enum index, timer_mode_enum mode)
{

    zf_assert(timer_funciton_check(index, TIMER_FUNCTION_TIMER));

    //开启时钟总线
    timer_clock_enable(index);

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    if(mode == TIMER_US)
    {
        TIM_TimeBaseStructure.TIM_Period = 0xFFFF;                          // 装载自动重装载值
        TIM_TimeBaseStructure.TIM_Prescaler = system_clock/1000000-1;           // 装载预分频
    }
    else if(mode == TIMER_MS)
    {
        TIM_TimeBaseStructure.TIM_Period = 0xFFFF;                          // 装载自动重装载值
        TIM_TimeBaseStructure.TIM_Prescaler = (system_clock/1000)/2-1;          // 装载预分频
    }
    else
    {
        TIM_TimeBaseStructure.TIM_Period = 0xFFFF;                          // 装载自动重装载值
        TIM_TimeBaseStructure.TIM_Prescaler = 0;                                // 装载预分频
    }
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                     // 设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                 // TIM向上计数模式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                            // 重复计数器设置为0
    TIM_TimeBaseInit((TIM_TypeDef*)tim_index[index], &TIM_TimeBaseStructure);                 // 根据指定的参数初始化TIMx的时间基数单位

    TIM_Cmd((TIM_TypeDef*)tim_index[index], ENABLE);                                          // 使能TIMx
}




//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取计时器值
//  @param      timer_ch      选择模块的通道 (选择范围 由TIMERN_enum枚举值的内容确定)
//  @return     uint32_t      计数值
//  Sample usage:             timer_get(TIM_1)  //获取定时器1的计时时间
//-------------------------------------------------------------------------------------------------------------------
uint16_t timer_get(timer_index_enum index)
{
    return ((TIM_TypeDef*)tim_index[index])->CNT;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      关闭计时器
//  @param      timer_ch      选择模块的通道 (选择范围 由TIMERN_enum枚举值的内容确定)
//  @return     void
//  Sample usage:             关闭定时器并清除标志位
//-------------------------------------------------------------------------------------------------------------------
void timer_stop(timer_index_enum index)
{
    TIM_Cmd((TIM_TypeDef*)tim_index[index], DISABLE);  //失能TIM
}



