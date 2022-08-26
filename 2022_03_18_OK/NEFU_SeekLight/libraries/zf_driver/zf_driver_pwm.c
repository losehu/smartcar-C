/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            zf_driver_pwm
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/

#include "zf_driver_pwm.h"
#include "zf_driver_gpio.h"
#include "zf_driver_timer.h"



//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM引脚初始化
//  @param      pwmch       PWM通道(可选择范围由zf_pwm.h内PWMCH_enum枚举值确定)
//  @return     void
//  Sample usage:           内部使用  用户无需关心
//-------------------------------------------------------------------------------------------------------------------
void pwm_gpio_init(pwm_channel_enum pin)
{
    //-------------------------------PWM1-------------------------------//
         if(pin == TIM1_PWM_CH1_A8 )   gpio_init(A8 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM1_PWM_CH2_A9 )   gpio_init(A9 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM1_PWM_CH3_A10)   gpio_init(A10, GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM1_PWM_CH4_A11)   gpio_init(A11, GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);

    else if(pin == TIM1_PWM_CH1_E9 )   gpio_init(E9 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM1_PWM_CH2_E11)   gpio_init(E11, GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM1_PWM_CH3_E13)   gpio_init(E13, GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM1_PWM_CH4_E14)   gpio_init(E14, GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    //-------------------------------PWM2-------------------------------//
    else if(pin == TIM2_PWM_CH1_A0 ) 	gpio_init(A0 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM2_PWM_CH2_A1 ) 	gpio_init(A1 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM2_PWM_CH3_A2 ) 	gpio_init(A2 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM2_PWM_CH4_A3 ) 	gpio_init(A3 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);

    else if(pin == TIM2_PWM_CH1_A15) 	gpio_init(A15, GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM2_PWM_CH2_B3 ) 	gpio_init(B3 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM2_PWM_CH3_B10) 	gpio_init(B10, GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM2_PWM_CH4_B11) 	gpio_init(B11, GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    //-------------------------------PWM3-------------------------------//
    else if(pin == TIM3_PWM_CH1_A6 ) 	gpio_init(A6,  GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM3_PWM_CH2_A7 ) 	gpio_init(A7,  GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM3_PWM_CH3_B0 ) 	gpio_init(B0,  GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM3_PWM_CH4_B1 ) 	gpio_init(B1,  GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);

    else if(pin == TIM3_PWM_CH1_C6 ) 	gpio_init(C6,  GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM3_PWM_CH2_C7 ) 	gpio_init(C7,  GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM3_PWM_CH3_C8 ) 	gpio_init(C8,  GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM3_PWM_CH4_C9 ) 	gpio_init(C9,  GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);

    else if(pin == TIM3_PWM_CH1_B4 ) 	gpio_init(B4,  GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM3_PWM_CH2_B5 ) 	gpio_init(B5,  GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    //-------------------------------PWM4-------------------------------//
    else if(pin == TIM4_PWM_CH1_B6 ) 	gpio_init(B6,  GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM4_PWM_CH2_B7 ) 	gpio_init(B7,  GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM4_PWM_CH3_B8 ) 	gpio_init(B8,  GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM4_PWM_CH4_B9 ) 	gpio_init(B9,  GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);

    else if(pin == TIM4_PWM_CH1_D12) 	gpio_init(D12, GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM4_PWM_CH2_D13) 	gpio_init(D13, GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM4_PWM_CH3_D14) 	gpio_init(D14, GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM4_PWM_CH4_D15) 	gpio_init(D15, GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    //-------------------------------PWM5-------------------------------//
    else if(pin == TIM5_PWM_CH1_A0 ) 	gpio_init(A0 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM5_PWM_CH2_A1 ) 	gpio_init(A1 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM5_PWM_CH3_A2 ) 	gpio_init(A2 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM5_PWM_CH4_A3 ) 	gpio_init(A3 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    //-------------------------------PWM8-------------------------------//
    else if(pin == TIM8_PWM_CH1_C6 ) 	gpio_init(C6 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM8_PWM_CH2_C7 ) 	gpio_init(C7 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM8_PWM_CH3_C8 ) 	gpio_init(C8 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM8_PWM_CH4_C9 ) 	gpio_init(C9 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);

    else if(pin == TIM8_PWM_CH1_B6 ) 	gpio_init(B6 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM8_PWM_CH2_B7 ) 	gpio_init(B7 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM8_PWM_CH3_B8 ) 	gpio_init(B8 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM8_PWM_CH4_C13) 	gpio_init(C13, GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    //-------------------------------PWM9-------------------------------//
    else if(pin == TIM9_PWM_CH1_A2 ) 	gpio_init(A2 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM9_PWM_CH2_A3 ) 	gpio_init(A3 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM9_PWM_CH3_A4 ) 	gpio_init(A4 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM9_PWM_CH4_C4 ) 	gpio_init(C4 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);

    else if(pin == TIM9_PWM_CH1_D9 ) 	gpio_init(D9 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM9_PWM_CH2_D11) 	gpio_init(D11, GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM9_PWM_CH3_D13) 	gpio_init(D13, GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM9_PWM_CH4_D15) 	gpio_init(D15, GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    //-------------------------------PWM10-------------------------------//
    else if(pin == TIM10_PWM_CH1_B8 ) 	gpio_init(B8 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM10_PWM_CH2_B9 ) 	gpio_init(B9 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM10_PWM_CH3_C3 ) 	gpio_init(C3 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM10_PWM_CH4_C11) 	gpio_init(C11, GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);

    else if(pin == TIM10_PWM_CH1_D1 ) 	gpio_init(D1 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM10_PWM_CH2_D3 ) 	gpio_init(D3 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM10_PWM_CH3_D5 ) 	gpio_init(D5 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM10_PWM_CH4_D7 ) 	gpio_init(D7 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);

    else if(pin == TIM10_PWM_CH1_B3 ) 	gpio_init(B3 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM10_PWM_CH2_B4 ) 	gpio_init(B4 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM10_PWM_CH3_B5 ) 	gpio_init(B5 , GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(pin == TIM10_PWM_CH4_C15) 	gpio_init(C15, GPO, 0, SPEED_50MHZ|GPO_AF_PUSH_PULL);

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM占空比设定
//  @param      pwmch           PWM通道号及引脚
//  @param      duty            PWM占空比
//  @return     void
//  Sample usage:               pwm_set_duty(TIM10_PWM_CH4_C15, 5000);   //定时器10 通道4 使用引脚C15 占空比为百分之 5000/PWM_DUTY_MAX*100
//                              PWM_DUTY_MAX在zf_pwm.h文件中 默认为10000
//-------------------------------------------------------------------------------------------------------------------
void pwm_set_duty(pwm_channel_enum pin, uint32_t duty)
{
    uint16_t match_temp;
    uint16_t period_temp;

    //获取周期值
    //计算占空比

    period_temp = ((TIM_TypeDef *) tim_index[pin>>8])->ATRLR;    //获取定时器的周期值
    match_temp = period_temp * duty / pwm_select_maxduty(pin);;             //占空比

    (*(volatile uint32_t*)(tim_index[pin>>8] + 0x34 + (pin&0x03) * 0x04)) = match_temp;

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM频率设定
//  @param      pwmch           PWM通道号及引脚
//  @param      freq            PWM频率
//  @param      duty            PWM占空比
//  @return     void
//  Sample usage:               pwm_set_freq(PWM1_CH1_A8, 60, 5000);   //使用引脚A8  输出PWM频率60HZ   占空比为百分之 5000/PWM_DUTY_MAX*100
//                              PWM_DUTY_MAX在zf_pwm.h文件中 默认为10000
//-------------------------------------------------------------------------------------------------------------------
void pwm_set_freq(pwm_channel_enum pin, uint32_t freq, uint32_t duty)
{
    uint16_t period_temp = 0;                                     //周期值
    uint16_t freq_div = 0;                                        //分频值
    uint16_t match_temp;

    freq_div = (uint16_t)((SystemCoreClock / freq) >> 16);                //计算多少分频
    period_temp = (uint16_t)(SystemCoreClock/(freq*(freq_div + 1)));      //计算周期

    ((TIM_TypeDef *) tim_index[pin>>8])->ATRLR = period_temp - 1 ;
    ((TIM_TypeDef *) tim_index[pin>>8])->PSC = freq_div;
    match_temp = period_temp * duty / pwm_select_maxduty(pin);;             //占空比

    (*(volatile uint32_t*)(tim_index[pin>>8] + 0x34 + (pin&0x03) * 0x04)) = match_temp;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM初始化
//  @param      pwmch           PWM通道号及引脚
//  @param      freq            PWM频率
//  @param      duty            PWM占空比
//  @return     void
//  Sample usage:               pwm_init(PWM1_CH1_A8, 50, 5000);   //初始化PWM1 通道1 使用引脚A8  输出PWM频率50HZ   占空比为百分之 5000/PWM_DUTY_MAX*100
//                              PWM_DUTY_MAX在zf_pwm.h文件中 默认为10000
//-------------------------------------------------------------------------------------------------------------------
void pwm_init(pwm_channel_enum pin, uint32_t freq, uint32_t duty)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    uint16_t match_temp;                                          //占空比值
    uint16_t period_temp;                                         //周期值
    uint16_t freq_div = 0;                                        //分频值

    zf_assert(timer_funciton_check((timer_index_enum)(pin>>8), TIMER_FUNCTION_PWM));

    //开启时钟总线
    timer_clock_enable(pin >> 8);

    //PWM引脚初始化
    pwm_gpio_init(pin);

    //使能AFIO复用功能模块时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    //引脚重映射


    if((pin >> 4) == 0x01)      GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
    else if((pin >> 4) == 0x11) GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
    else if((pin >> 4) == 0x21) GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
    else if((pin >> 4) == 0x22) GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
    else if((pin >> 4) == 0x31) GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);
    else if((pin >> 4) == 0x71) AFIO->PCFR2 |= 0x01<<2;
    else if((pin >> 4) == 0x81) AFIO->PCFR2 |= 0x02<<3;
    else if((pin >> 4) == 0x91) AFIO->PCFR2 |= 0x02<<5;
    else if((pin >> 4) == 0x92) AFIO->PCFR2 |= 0x01<<5;


    freq_div = (uint16_t)((SystemCoreClock / freq) >> 16);                            //多少分频
    period_temp = (uint16_t)(SystemCoreClock/(freq*(freq_div + 1)));                  //周期
    match_temp = period_temp * duty / pwm_select_maxduty(pin);;                         //占空比

    //初始化TIM
    TIM_TimeBaseStructure.TIM_Period = period_temp - 1;                     //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = freq_div;                         //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                 //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;             //TIM向上计数模式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(((TIM_TypeDef *) tim_index[pin>>8]), &TIM_TimeBaseStructure);  //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    //初始化TIM
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;                       //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;           //比较输出使能
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_Pulse = match_temp;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;                //输出极性:TIM输出比较极性高
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_Low;               //输出极性:TIM输出比较极性高
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;


    if((pin & 0x03) == 0x00)                                          //通道选择
    {
        TIM_OC1Init(((TIM_TypeDef *) tim_index[pin>>8]), &TIM_OCInitStructure );                       //定时器通道1初始化
        TIM_OC1PreloadConfig(((TIM_TypeDef *) tim_index[pin>>8]), TIM_OCPreload_Enable);               //定时器预装载配置
        TIM_OC1FastConfig(((TIM_TypeDef *) tim_index[pin>>8]), TIM_OC1FE);                             //比较捕获通道快速使能
    }
    else if((pin & 0x03) == 0x01)
    {
        TIM_OC2Init(((TIM_TypeDef *) tim_index[pin>>8]), &TIM_OCInitStructure );
        TIM_OC2PreloadConfig(((TIM_TypeDef *) tim_index[pin>>8]), TIM_OCPreload_Enable);
        TIM_OC2FastConfig(((TIM_TypeDef *) tim_index[pin>>8]), TIM_OC2FE);                             //比较捕获通道快速使能
    }
    else if((pin & 0x03) == 0x02)
    {
        TIM_OC3Init(((TIM_TypeDef *) tim_index[pin>>8]), &TIM_OCInitStructure );
        TIM_OC3PreloadConfig(((TIM_TypeDef *) tim_index[pin>>8]), TIM_OCPreload_Enable);
        TIM_OC3FastConfig(((TIM_TypeDef *) tim_index[pin>>8]), TIM_OC3FE);                             //比较捕获通道快速使能
    }
    else if((pin & 0x03) == 0x03)
    {
        TIM_OC4Init(((TIM_TypeDef *) tim_index[pin>>8]), &TIM_OCInitStructure );
        TIM_OC4PreloadConfig(((TIM_TypeDef *) tim_index[pin>>8]), TIM_OCPreload_Enable);
        TIM_OC4FastConfig(((TIM_TypeDef *) tim_index[pin>>8]), TIM_OC4FE);                             //比较捕获通道快速使能
    }
    TIM_CtrlPWMOutputs(((TIM_TypeDef *) tim_index[pin>>8]), ENABLE );
    TIM_Cmd(((TIM_TypeDef *) tim_index[pin>>8]), ENABLE);                                              //定时器使能
    //TIM_ARRPreloadConfig( TIM1, ENABLE );
}

//=================================USER======================================//
/**
*@Name          :pwm_select_maxduty
*@Description   :pwm_select_maxduty
*@Param         :pwm_channel_enum pin
*@Return        :max duty
*@Sample        :pwm_select_maxduty(pin);
**/
uint32_t pwm_select_maxduty(pwm_channel_enum pin)
{
    switch (pin) {
        case SERVO_PWM://舵机
            return 100000;
        case ML0_PWM: //电机
        case ML1_PWM:
        case MR0_PWM:
        case MR1_PWM:
            return 1000;
        default:
            return PWM_DUTY_MAX;
    }
}
