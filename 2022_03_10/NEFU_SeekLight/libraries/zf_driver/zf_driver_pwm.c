/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            zf_driver_pwm
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ790875685)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/

#include "zf_driver_pwm.h"
#include "zf_driver_gpio.h"
#include "zf_driver_timer.h"



//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM���ų�ʼ��
//  @param      pwmch       PWMͨ��(��ѡ��Χ��zf_pwm.h��PWMCH_enumö��ֵȷ��)
//  @return     void
//  Sample usage:           �ڲ�ʹ��  �û��������
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
//  @brief      PWMռ�ձ��趨
//  @param      pwmch           PWMͨ���ż�����
//  @param      duty            PWMռ�ձ�
//  @return     void
//  Sample usage:               pwm_set_duty(TIM10_PWM_CH4_C15, 5000);   //��ʱ��10 ͨ��4 ʹ������C15 ռ�ձ�Ϊ�ٷ�֮ 5000/PWM_DUTY_MAX*100
//                              PWM_DUTY_MAX��zf_pwm.h�ļ��� Ĭ��Ϊ10000
//-------------------------------------------------------------------------------------------------------------------
void pwm_set_duty(pwm_channel_enum pin, uint32_t duty)
{
    uint16_t match_temp;
    uint16_t period_temp;

    //��ȡ����ֵ
    //����ռ�ձ�

    period_temp = ((TIM_TypeDef *) tim_index[pin>>8])->ATRLR;    //��ȡ��ʱ��������ֵ
    match_temp = period_temp * duty / pwm_select_maxduty(pin);;             //ռ�ձ�

    (*(volatile uint32_t*)(tim_index[pin>>8] + 0x34 + (pin&0x03) * 0x04)) = match_temp;

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWMƵ���趨
//  @param      pwmch           PWMͨ���ż�����
//  @param      freq            PWMƵ��
//  @param      duty            PWMռ�ձ�
//  @return     void
//  Sample usage:               pwm_set_freq(PWM1_CH1_A8, 60, 5000);   //ʹ������A8  ���PWMƵ��60HZ   ռ�ձ�Ϊ�ٷ�֮ 5000/PWM_DUTY_MAX*100
//                              PWM_DUTY_MAX��zf_pwm.h�ļ��� Ĭ��Ϊ10000
//-------------------------------------------------------------------------------------------------------------------
void pwm_set_freq(pwm_channel_enum pin, uint32_t freq, uint32_t duty)
{
    uint16_t period_temp = 0;                                     //����ֵ
    uint16_t freq_div = 0;                                        //��Ƶֵ
    uint16_t match_temp;

    freq_div = (uint16_t)((SystemCoreClock / freq) >> 16);                //������ٷ�Ƶ
    period_temp = (uint16_t)(SystemCoreClock/(freq*(freq_div + 1)));      //��������

    ((TIM_TypeDef *) tim_index[pin>>8])->ATRLR = period_temp - 1 ;
    ((TIM_TypeDef *) tim_index[pin>>8])->PSC = freq_div;
    match_temp = period_temp * duty / pwm_select_maxduty(pin);;             //ռ�ձ�

    (*(volatile uint32_t*)(tim_index[pin>>8] + 0x34 + (pin&0x03) * 0x04)) = match_temp;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM��ʼ��
//  @param      pwmch           PWMͨ���ż�����
//  @param      freq            PWMƵ��
//  @param      duty            PWMռ�ձ�
//  @return     void
//  Sample usage:               pwm_init(PWM1_CH1_A8, 50, 5000);   //��ʼ��PWM1 ͨ��1 ʹ������A8  ���PWMƵ��50HZ   ռ�ձ�Ϊ�ٷ�֮ 5000/PWM_DUTY_MAX*100
//                              PWM_DUTY_MAX��zf_pwm.h�ļ��� Ĭ��Ϊ10000
//-------------------------------------------------------------------------------------------------------------------
void pwm_init(pwm_channel_enum pin, uint32_t freq, uint32_t duty)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    uint16_t match_temp;                                          //ռ�ձ�ֵ
    uint16_t period_temp;                                         //����ֵ
    uint16_t freq_div = 0;                                        //��Ƶֵ

    zf_assert(timer_funciton_check((timer_index_enum)(pin>>8), TIMER_FUNCTION_PWM));

    //����ʱ������
    timer_clock_enable(pin >> 8);

    //PWM���ų�ʼ��
    pwm_gpio_init(pin);

    //ʹ��AFIO���ù���ģ��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    //������ӳ��


    if((pin >> 4) == 0x01)      GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
    else if((pin >> 4) == 0x11) GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
    else if((pin >> 4) == 0x21) GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
    else if((pin >> 4) == 0x22) GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
    else if((pin >> 4) == 0x31) GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);
    else if((pin >> 4) == 0x71) AFIO->PCFR2 |= 0x01<<2;
    else if((pin >> 4) == 0x81) AFIO->PCFR2 |= 0x02<<3;
    else if((pin >> 4) == 0x91) AFIO->PCFR2 |= 0x02<<5;
    else if((pin >> 4) == 0x92) AFIO->PCFR2 |= 0x01<<5;


    freq_div = (uint16_t)((SystemCoreClock / freq) >> 16);                            //���ٷ�Ƶ
    period_temp = (uint16_t)(SystemCoreClock/(freq*(freq_div + 1)));                  //����
    match_temp = period_temp * duty / pwm_select_maxduty(pin);;                         //ռ�ձ�

    //��ʼ��TIM
    TIM_TimeBaseStructure.TIM_Period = period_temp - 1;                     //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = freq_div;                         //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                 //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;             //TIM���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(((TIM_TypeDef *) tim_index[pin>>8]), &TIM_TimeBaseStructure);  //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    //��ʼ��TIM
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;                       //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;           //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_Pulse = match_temp;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;                //�������:TIM����Ƚϼ��Ը�
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_Low;               //�������:TIM����Ƚϼ��Ը�
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;


    if((pin & 0x03) == 0x00)                                          //ͨ��ѡ��
    {
        TIM_OC1Init(((TIM_TypeDef *) tim_index[pin>>8]), &TIM_OCInitStructure );                       //��ʱ��ͨ��1��ʼ��
        TIM_OC1PreloadConfig(((TIM_TypeDef *) tim_index[pin>>8]), TIM_OCPreload_Enable);               //��ʱ��Ԥװ������
        TIM_OC1FastConfig(((TIM_TypeDef *) tim_index[pin>>8]), TIM_OC1FE);                             //�Ƚϲ���ͨ������ʹ��
    }
    else if((pin & 0x03) == 0x01)
    {
        TIM_OC2Init(((TIM_TypeDef *) tim_index[pin>>8]), &TIM_OCInitStructure );
        TIM_OC2PreloadConfig(((TIM_TypeDef *) tim_index[pin>>8]), TIM_OCPreload_Enable);
        TIM_OC2FastConfig(((TIM_TypeDef *) tim_index[pin>>8]), TIM_OC2FE);                             //�Ƚϲ���ͨ������ʹ��
    }
    else if((pin & 0x03) == 0x02)
    {
        TIM_OC3Init(((TIM_TypeDef *) tim_index[pin>>8]), &TIM_OCInitStructure );
        TIM_OC3PreloadConfig(((TIM_TypeDef *) tim_index[pin>>8]), TIM_OCPreload_Enable);
        TIM_OC3FastConfig(((TIM_TypeDef *) tim_index[pin>>8]), TIM_OC3FE);                             //�Ƚϲ���ͨ������ʹ��
    }
    else if((pin & 0x03) == 0x03)
    {
        TIM_OC4Init(((TIM_TypeDef *) tim_index[pin>>8]), &TIM_OCInitStructure );
        TIM_OC4PreloadConfig(((TIM_TypeDef *) tim_index[pin>>8]), TIM_OCPreload_Enable);
        TIM_OC4FastConfig(((TIM_TypeDef *) tim_index[pin>>8]), TIM_OC4FE);                             //�Ƚϲ���ͨ������ʹ��
    }
    TIM_CtrlPWMOutputs(((TIM_TypeDef *) tim_index[pin>>8]), ENABLE );
    TIM_Cmd(((TIM_TypeDef *) tim_index[pin>>8]), ENABLE);                                              //��ʱ��ʹ��
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
        case SERVO_PWM://���
            return 100000;
        case ML0_PWM: //���
        case ML1_PWM:
        case MR0_PWM:
        case MR1_PWM:
            return 1000;
        default:
            return PWM_DUTY_MAX;
    }
}
