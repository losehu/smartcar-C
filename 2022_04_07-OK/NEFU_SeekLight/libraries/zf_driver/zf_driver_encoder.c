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

#include "zf_driver_gpio.h"
#include "zf_driver_timer.h"
#include "zf_driver_encoder.h"

static uint8_t encoder_dir_pin[TIM10_ENCOEDER + 1] = { 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

//-------------------------------------------------------------------------------------------------------------------
//  @brief      编码器引脚初始化
//  @param      phaseA      通道A以及引脚
//  @param      phaseB      通道B以及引脚
//  @return     void
//  Sample usage:           内部使用，用户无需关心
//-------------------------------------------------------------------------------------------------------------------
void encoder_gpio_init(gpio_pin_enum pin_ch1, gpio_pin_enum pin_ch2) {
    gpio_init(pin_ch1, GPI, 0, GPI_PULL_UP);
    gpio_init(pin_ch2, GPI, 0, GPI_PULL_UP);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      定时器编码器解码取值
//  @param      timer_ch      定时器枚举体
//  @return     void
//  Sample usage:
//                  encoder_get_count(TIM2_ENCOEDER)  // 获取定时器2的采集到的编码器数据
//-------------------------------------------------------------------------------------------------------------------
int show_flag=0;

int16_t encoder_get_count(encoder_index_enum encoder_n) {
    if (encoder_dir_pin[encoder_n] == 0xFF) {
        return ((TIM_TypeDef*) tim_index[encoder_n])->CNT;
    } else {
        if (gpio_get_level(encoder_dir_pin[encoder_n]))               // 方向引脚高电平
                {
            if (encoder_n == 1&&show_flag) {
                ips114_show_int16(0, 7, 1);
                my_delay(1);

            }
            return (uint16_t) ((TIM_TypeDef*) tim_index[encoder_n])->CNT;

        }                                 // 返回正数值
        else {    // 方向引脚低电平
            if (encoder_n == 1&&show_flag) {
                ips114_show_int16(0, 7, 2);
                my_delay(1);
            }
            return -((uint16_t) ((TIM_TypeDef*) tim_index[encoder_n])->CNT); // 返回负数值

        }
    }

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      定时器的计数器清空
//  @param      timer_ch      定时器枚举体
//  @return     void
//  Sample usage:
//                  encoder_clean_count(TIM1_ENCOEDER)  //清除定时器1采集到的编码器数据
//-------------------------------------------------------------------------------------------------------------------
void encoder_clean_count(encoder_index_enum encoder_n) {
    ((TIM_TypeDef*) tim_index[encoder_n])->CNT = 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      编码器解码初始化
//  @param      timer_ch      定时器枚举体
//  @param      phaseA      通道A以及引脚
//  @param      phaseB      通道B以及引脚
//  @return     void
//  Sample usage:           推荐使用正交解码编码器。
//                          encoder_init_quad(TIM1_ENCOEDER, TIM1_CH1_ENCOEDER_E9, TIM1_CH2_ENCOEDER_E11)
//                          // 使用定时器1 做编码器正交解码， 通道1引脚号E9，通道2引脚号E11
//-------------------------------------------------------------------------------------------------------------------
void encoder_init_quad(encoder_index_enum encoder_n,
        encoder_channel_enum ch1_pin, encoder_channel_enum ch2_pin) {
    zf_assert(
            timer_funciton_check((timer_index_enum )encoder_n,
                    TIMER_FUNCTION_ENCODER));

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    timer_clock_enable(encoder_n);

    encoder_gpio_init((gpio_pin_enum) (ch1_pin & 0xFF),
            (gpio_pin_enum) (ch2_pin & 0xFF));                           //初始化引脚

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

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);        //使能AFIO复用功能模块时钟

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 0;                              //预分频器
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;                      //设定计数器自动重装值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //选择时钟分频：不分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;        //TIM向上计数
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                     //重复计数器
    TIM_TimeBaseInit((TIM_TypeDef*) tim_index[encoder_n],
            &TIM_TimeBaseStructure);       //初始化结构体
    TIM_EncoderInterfaceConfig((TIM_TypeDef*) tim_index[encoder_n],
    TIM_EncoderMode_TI2, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); //使用编码器模式

    TIM_Cmd((TIM_TypeDef*) tim_index[encoder_n], ENABLE);                //定时器使能
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      编码器解码初始化
//  @param      timer_ch      定时器枚举体
//  @param      phaseA      通道A以及引脚
//  @param      phaseB      通道B以及引脚
//  @return     void
//  Sample usage:           推荐使用正交解码编码器。
//                          encoder_init_dir(TIM1_ENCOEDER, TIM1_CH1_ENCOEDER_E9, TIM1_CH2_ENCOEDER_E11)
//                          // 使用定时器1 做编码器正交解码， 通道1方向信号引脚E9，通道2脉冲信号引脚E11
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

    encoder_dir_pin[encoder_n] = (ch1_pin & 0xFF);           //将方向引脚号存入数组中

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

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);        //使能AFIO复用功能模块时钟

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 0;                              //预分频器
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;                      //设定计数器自动重装值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //选择时钟分频：不分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;        //TIM向上计数
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                     //重复计数器
    TIM_TimeBaseInit((TIM_TypeDef*) tim_index[encoder_n],
            &TIM_TimeBaseStructure);           //初始化结构体

    TIM_ITRxExternalClockConfig((TIM_TypeDef*) tim_index[encoder_n],
    TIM_TS_TI2FP2);          //配置外部触发，否则不会计数

    TIM_Cmd((TIM_TypeDef*) tim_index[encoder_n], ENABLE);                //定时器使能
}

