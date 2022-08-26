/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            zf_driver_gpio
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
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


typedef enum                                                                    // 枚举 GPIO 引脚  此枚举定义不允许用户修改
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
    GPI = 0,        //  定义管脚输入方向
    GPO = 1,        //  定义管脚输出方向
}gpio_dir_enum;

typedef enum
{
    // 输入
    GPI_ANAOG_IN        = 0x0,          //  模拟输入模式
    GPI_FLOATING_IN     = 0x04,         //  浮空输入模式
    GPI_PULL_DOWN       = 0x28,         //  下拉输入
    GPI_PULL_UP         = 0x48,         //  上拉输入
    // 输出
    GPO_PUSH_PULL       = 0x10,         //  通用推挽输出模式
    GPO_OPEN_DTAIN      = 0x14,         //  通用开漏输出模式
    GPO_AF_PUSH_PULL    = 0x18,         //  复用功能推挽输出模式
    GPO_AF_OPEN_DTAIN   = 0x1C,         //  复用功能开漏输出模式
}gpio_mode_enum;

//  宏定义GPIO引脚的默认配置，便于初始化GPIO时快速填写参数，如果需要其他参数可自行修改
#define GPIO_PIN_CONFIG (SPEED_50MHZ | GPO_PUSH_PULL)
//  宏定义GPIO中断和输入引脚的默认配置，便于初始化GPIO中断时快速填写参数，如果需要其他参数可自行修改
#define GPIO_INT_CONFIG (GPI_PULL_UP)

typedef enum
{
    SPEED_10MHZ = (0x01 << 8),
    SPEED_2MHZ  = (0x02  << 8),
    SPEED_50MHZ = (0x03 << 8)
}gpio_speed_enum;

typedef enum
{
    GPIO_LOW  = 0,  //  低电平
    GPIO_HIGH = 1,  //  高电平
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
