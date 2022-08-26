/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            zf_driver_gpio
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ790875685)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        MounRiver Studio V1.5.1
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/

 
#ifndef _zf_gpio_h
#define _zf_gpio_h

#include "ch32v30x_rcc.h"
#include "ch32v30x_gpio.h"
#include "ch32v30x_exti.h"


typedef enum                                                                    // ö�� GPIO ����  ��ö�ٶ��岻�����û��޸�
{
    A0 = 32* 0, A1 ,    A2 ,    A3 ,    A4 ,    A5 ,    A6 ,    A7 ,
    A8 ,        A9 ,    A10,    A11,    A12,    A13,    A14,    A15,

    B0 = 32* 1, B1 ,    B2 ,    B3 ,    B4 ,    B5 ,    B6 ,    B7 ,
    B8 ,        B9 ,    B10,    B11,    B12,    B13,    B14,    B15,

    C0 = 32* 2, C1 ,    C2 ,    C3 ,    C4 ,    C5 ,    C6 ,    C7 ,
    C8 ,        C9 ,    C10,    C11,    C12,    C13,    C14,    C15,

    D0 = 32* 3, D1 ,    D2 ,    D3 ,    D4 ,    D5 ,    D6 ,    D7 ,
    D8 ,        D9 ,    D10,    D11,    D12,    D13,    D14,    D15,

    E0 = 32* 4, E1 ,    E2 ,    E3 ,    E4 ,    E5 ,    E6 ,    E7 ,
    E8 ,        E9 ,    E10,    E11,    E12,    E13,    E14,    E15,

    SPI_CS_NULL = 0xFF,
    PIN_NULL = 0xFF
}gpio_pin_enum;

typedef enum
{
    GPI = 0,        //  ����ܽ����뷽��
    GPO = 1,        //  ����ܽ��������
}gpio_dir_enum;

typedef enum
{
    // ����
    GPI_ANAOG_IN        = 0x0,          //  ģ������ģʽ
    GPI_FLOATING_IN     = 0x04,         //  ��������ģʽ
    GPI_PULL_DOWN       = 0x28,         //  ��������
    GPI_PULL_UP         = 0x48,         //  ��������
    // ���
    GPO_PUSH_PULL       = 0x10,         //  ͨ���������ģʽ
    GPO_OPEN_DTAIN      = 0x14,         //  ͨ�ÿ�©���ģʽ
    GPO_AF_PUSH_PULL    = 0x18,         //  ���ù����������ģʽ
    GPO_AF_OPEN_DTAIN   = 0x1C,         //  ���ù��ܿ�©���ģʽ
}gpio_mode_enum;

//  �궨��GPIO���ŵ�Ĭ�����ã����ڳ�ʼ��GPIOʱ������д�����������Ҫ���������������޸�
#define GPIO_PIN_CONFIG (SPEED_50MHZ | GPO_PUSH_PULL)
//  �궨��GPIO�жϺ��������ŵ�Ĭ�����ã����ڳ�ʼ��GPIO�ж�ʱ������д�����������Ҫ���������������޸�
#define GPIO_INT_CONFIG (GPI_PULL_UP)

typedef enum
{
    SPEED_10MHZ = (0x01 << 8),
    SPEED_2MHZ  = (0x02  << 8),
    SPEED_50MHZ = (0x03 << 8)
}gpio_speed_enum;

typedef enum
{
    GPIO_LOW  = 0,  //  �͵�ƽ
    GPIO_HIGH = 1,  //  �ߵ�ƽ
}gpio_level_enum;

extern  const uint32_t gpio_group[5];

#define gpio_odr_addr(X)        (0x4004000C + ((X&0xf0)>>5)*0x400 + ((X&0x0f)/8))

#define gpio_low(x)             ((GPIO_TypeDef*)gpio_group[(x>>5)])->BCR   = (uint16_t)(1 << (x & 0x0F))
#define gpio_high(x)            ((GPIO_TypeDef*)gpio_group[(x>>5)])->BSHR  = (uint16_t)(1 << (x & 0x0F))

void    gpio_set_level          (gpio_pin_enum gpio_pin, uint8_t dat);
uint8_t gpio_get_level          (gpio_pin_enum gpio_pin);
void    gpio_toggle_level       (gpio_pin_enum gpio_pin);
void    gpio_init               (gpio_pin_enum gpio_pin, gpio_dir_enum gpio_dir, gpio_level_enum dat, uint32_t pinconf);


#endif
