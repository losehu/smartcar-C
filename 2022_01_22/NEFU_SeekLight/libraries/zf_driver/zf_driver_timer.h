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

#ifndef _zf_timer_h
#define _zf_timer_h

#include "ch32v30x_rcc.h"
#include "ch32v30x_gpio.h"
#include "ch32v30x_tim.h"
#include "ch32v30x_misc.h"

#include "zf_common_debug.h"
#include "zf_common_clock.h"

//��ö�ٶ��岻�����û��޸�
typedef enum
{
    TIM_1,
    TIM_2,
    TIM_3,
    TIM_4,
    TIM_5,
    TIM_6,
    TIM_7,
    TIM_8,
    TIM_9,
    TIM_10,
}timer_index_enum;

typedef enum
{
    TIMER_SYSTEM_CLOCK,                                                         // ϵͳƵ�ʼ�ʱ ��� 0xFFFF
    TIMER_US,                                                                   // ΢���ʱ ��� 0xFFFF
    TIMER_MS,                                                                   // �����ʱ ��� 0xFFFF/2
}timer_mode_enum;

typedef enum
{
    TIMER_FUNCTION_INIT = 0,                                                    // ����δ��ʼ��
    TIMER_FUNCTION_TIMER,                                                       // ���� TIMER ��ʱ
    TIMER_FUNCTION_PIT,                                                         // ���� PIT
    TIMER_FUNCTION_PWM,                                                         // ���� PWM
    TIMER_FUNCTION_ENCODER,                                                     // ���� ENCODER
    TIMER_FUNCTION_CAMERA,                                                      // ���� CAMERA
}timer_function_enum;

extern const uint32_t tim_index[] ;


uint8_t     timer_funciton_check    (timer_index_enum index, timer_function_enum mode);
void        timer_clock_enable      (timer_index_enum index);
void        timer_start             (timer_index_enum index, timer_mode_enum mode);
uint16_t    timer_get               (timer_index_enum index);
void        timer_stop              (timer_index_enum index);





#endif
