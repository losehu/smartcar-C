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
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/



#include "zf_driver_gpio.h"


//  GPIO����ַ����
const uint32_t gpio_group[5] = {GPIOA_BASE, GPIOB_BASE, GPIOC_BASE, GPIOD_BASE, GPIOE_BASE};

//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO�������
//  @param      pin         ���ź�ѡ������� (��ѡ��Χ�� common.h ��GPIO_PIN_enumö��ֵȷ��)
//  @param      dat         ���ŵĵ�ƽ״̬�����ʱ��Ч 0���͵�ƽ 1���ߵ�ƽ
//  @return     void
//  Sample usage:           gpio_set_level(D0, 0);//D0����͵�ƽ
//-------------------------------------------------------------------------------------------------------------------
void gpio_set_level(gpio_pin_enum pin, uint8_t dat)
{
    if(dat)
        gpio_high(pin);                                                         // ����ߵ�ƽ
    else
        gpio_low(pin);                                                          // ����͵�ƽ
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO״̬��ȡ
//  @param      pin         ѡ������� (��ѡ��Χ�� common.h ��GPIO_PIN_enumö��ֵȷ��)
//  @return     uint8_t       0���͵�ƽ 1���ߵ�ƽ
//  Sample usage:           uint8_t status = gpio_get_level(D0);//��ȡD0���ŵ�ƽ
//-------------------------------------------------------------------------------------------------------------------
uint8_t gpio_get_level(gpio_pin_enum pin)
{
    if(((GPIO_TypeDef*)gpio_group[(pin&0xE0)>>5])->INDR & (((uint16_t)0x0001) << (pin&0x1F)))     // ��ȡ����ֵ
        return 1;
    return 0;

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO ��ת
//  @param      pin         ѡ������� (��ѡ��Χ�� common.h ��GPIO_PIN_enumö��ֵȷ��)
//  @return     void        
//  Sample usage:           gpio_toggle_level(D5);//D5���ŵ�ƽ��ת
//-------------------------------------------------------------------------------------------------------------------
void gpio_toggle_level(gpio_pin_enum pin)
{
    uint8_t io_group = (pin & 0xF0) >> 5;
    uint8_t io_pin = pin & 0x0F;

    ((GPIO_TypeDef*)(gpio_group[io_group]))->OUTDR ^= (uint16_t)(1 << io_pin);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO��ʼ��
//  @param      pin         ѡ������� (��ѡ��Χ�� common.h ��GPIO_PIN_enumö��ֵȷ��)
//  @param      dir         ���ŵķ���   �����GPO   ���룺GPI
//  @param      dat         ���ų�ʼ��ʱ���õĵ�ƽ״̬�����ʱ��Ч 0���͵�ƽ 1���ߵ�ƽ
//  @param      pinconf     �������ã������ò�����zf_gpio.h�ļ���GPIOSPEED_enum��GPIOMODE_enumö��ֵȷ�����������ʹ�� | ���
//  @return     void
//  Sample usage:           gpio_init(D0, GPO, 1, GPIO_PIN_CONFIG);//D0��ʼ��ΪGPIO���ܡ����ģʽ������ߵ�ƽ���ٶ�100MHZ �������
//-------------------------------------------------------------------------------------------------------------------
void gpio_init(gpio_pin_enum gpio_pin, gpio_dir_enum gpio_dir, gpio_level_enum dat, uint32_t pinconf)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    uint8_t io_group = (gpio_pin & 0xF0) >> 5;
    uint8_t io_pin = gpio_pin & 0x0F;

    //GPIO_DeInit((GPIO_TypeDef *)(GPION[io_group]));
    //  �������򷵻�
    if(PIN_NULL == gpio_pin) return;
    if(SPI_CS_NULL == gpio_pin) return;

    //  GPIO����ʹ��
    if(0 == io_group)      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    else if(1 == io_group) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    else if(2 == io_group) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    else if(3 == io_group) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    else if(4 == io_group) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    else if(5 == io_group) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    else if(6 == io_group) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);

    //  �������ź�
    GPIO_InitStructure.GPIO_Pin = (uint16_t)(1 << io_pin);

    //  ֻ�������Ҫ�����ٶ�
    if(GPO == gpio_dir)
    {
        if((uint16_t)pinconf >> 8 == 0) GPIO_InitStructure.GPIO_Speed = (GPIOSpeed_TypeDef)(GPIO_Speed_50MHz);            //��ֹû����д�������,Ĭ������50Mhz
        else GPIO_InitStructure.GPIO_Speed = (gpio_speed_enum)((uint16_t)pinconf >> 8);         //�Ѿ���д�˸ò���������ֵ������
    }

    //  ����ģʽ
    GPIO_InitStructure.GPIO_Mode = (GPIOMode_TypeDef)((uint16_t)pinconf & 0xFF);
    GPIO_Init((GPIO_TypeDef*)gpio_group[io_group], &GPIO_InitStructure);

    //  ֻ�������Ҫ��������״̬
    if(GPO == gpio_dir)
    {
        GPIO_WriteBit((GPIO_TypeDef*)gpio_group[io_group], (uint16_t)(1 << io_pin), dat);
    }
}

