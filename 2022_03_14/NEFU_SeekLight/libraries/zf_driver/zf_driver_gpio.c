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
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/



#include "zf_driver_gpio.h"


//  GPIO基地址定义
const uint32_t gpio_group[5] = {GPIOA_BASE, GPIOB_BASE, GPIOC_BASE, GPIOD_BASE, GPIOE_BASE};

//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO输出设置
//  @param      pin         引脚号选择的引脚 (可选择范围由 common.h 内GPIO_PIN_enum枚举值确定)
//  @param      dat         引脚的电平状态，输出时有效 0：低电平 1：高电平
//  @return     void
//  Sample usage:           gpio_set_level(D0, 0);//D0输出低电平
//-------------------------------------------------------------------------------------------------------------------
void gpio_set_level(gpio_pin_enum pin, uint8_t dat)
{
    if(dat)
        gpio_high(pin);                                                         // 输出高电平
    else
        gpio_low(pin);                                                          // 输出低电平
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO状态获取
//  @param      pin         选择的引脚 (可选择范围由 common.h 内GPIO_PIN_enum枚举值确定)
//  @return     uint8_t       0：低电平 1：高电平
//  Sample usage:           uint8_t status = gpio_get_level(D0);//获取D0引脚电平
//-------------------------------------------------------------------------------------------------------------------
uint8_t gpio_get_level(gpio_pin_enum pin)
{
    if(((GPIO_TypeDef*)gpio_group[(pin&0xE0)>>5])->INDR & (((uint16_t)0x0001) << (pin&0x1F)))     // 读取引脚值
        return 1;
    return 0;

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO 翻转
//  @param      pin         选择的引脚 (可选择范围由 common.h 内GPIO_PIN_enum枚举值确定)
//  @return     void        
//  Sample usage:           gpio_toggle_level(D5);//D5引脚电平翻转
//-------------------------------------------------------------------------------------------------------------------
void gpio_toggle_level(gpio_pin_enum pin)
{
    uint8_t io_group = (pin & 0xF0) >> 5;
    uint8_t io_pin = pin & 0x0F;

    ((GPIO_TypeDef*)(gpio_group[io_group]))->OUTDR ^= (uint16_t)(1 << io_pin);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO初始化
//  @param      pin         选择的引脚 (可选择范围由 common.h 内GPIO_PIN_enum枚举值确定)
//  @param      dir         引脚的方向   输出：GPO   输入：GPI
//  @param      dat         引脚初始化时设置的电平状态，输出时有效 0：低电平 1：高电平
//  @param      pinconf     引脚配置（可设置参数由zf_gpio.h文件内GPIOSPEED_enum与GPIOMODE_enum枚举值确定，多个条件使用 | 相或）
//  @return     void
//  Sample usage:           gpio_init(D0, GPO, 1, GPIO_PIN_CONFIG);//D0初始化为GPIO功能、输出模式、输出高电平、速度100MHZ 推挽输出
//-------------------------------------------------------------------------------------------------------------------
void gpio_init(gpio_pin_enum gpio_pin, gpio_dir_enum gpio_dir, gpio_level_enum dat, uint32_t pinconf)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    uint8_t io_group = (gpio_pin & 0xF0) >> 5;
    uint8_t io_pin = gpio_pin & 0x0F;

    //GPIO_DeInit((GPIO_TypeDef *)(GPION[io_group]));
    //  空引脚则返回
    if(PIN_NULL == gpio_pin) return;
    if(SPI_CS_NULL == gpio_pin) return;

    //  GPIO总线使能
    if(0 == io_group)      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    else if(1 == io_group) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    else if(2 == io_group) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    else if(3 == io_group) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    else if(4 == io_group) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    else if(5 == io_group) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    else if(6 == io_group) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);

    //  设置引脚号
    GPIO_InitStructure.GPIO_Pin = (uint16_t)(1 << io_pin);

    //  只有输出需要设置速度
    if(GPO == gpio_dir)
    {
        if((uint16_t)pinconf >> 8 == 0) GPIO_InitStructure.GPIO_Speed = (GPIOSpeed_TypeDef)(GPIO_Speed_50MHz);            //防止没有填写这个参数,默认设置50Mhz
        else GPIO_InitStructure.GPIO_Speed = (gpio_speed_enum)((uint16_t)pinconf >> 8);         //已经填写了该参数。根据值来设置
    }

    //  设置模式
    GPIO_InitStructure.GPIO_Mode = (GPIOMode_TypeDef)((uint16_t)pinconf & 0xFF);
    GPIO_Init((GPIO_TypeDef*)gpio_group[io_group], &GPIO_InitStructure);

    //  只有输出需要设置引脚状态
    if(GPO == gpio_dir)
    {
        GPIO_WriteBit((GPIO_TypeDef*)gpio_group[io_group], (uint16_t)(1 << io_pin), dat);
    }
}

