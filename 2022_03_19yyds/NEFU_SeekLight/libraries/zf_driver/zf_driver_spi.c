/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            zf_driver_spi
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/


#include "zf_driver_gpio.h"
#include "zf_driver_spi.h"


const uint32_t spi_index[3] = {SPI1_BASE, SPI2_BASE, SPI3_BASE};
//-------------------------------------------------------------------------------------------------------------------
//  @brief      SPI引脚初始化
//  @param      sck_pin     SCK引脚(可选择范围由zf_spi.h内SPI_PIN_enum枚举值确定)
//  @param      mosi_pin    MOSI引脚(可选择范围由zf_spi.h内SPI_PIN_enum枚举值确定)
//  @param      miso_pin    MISO引脚(可选择范围由zf_spi.h内SPI_PIN_enum枚举值确定)
//  @param      cs_pin      片选引脚(可选择范围由common.h内PIN_enum枚举值确定)
//  @return     void
//  Sample usage:           内部使用  用户无需关心
//-------------------------------------------------------------------------------------------------------------------
void spi_gpio_init(spi_pin_enum sck_pin, spi_pin_enum mosi_pin, spi_pin_enum miso_pin, gpio_pin_enum cs_pin)
{
    if(SPI1_SCK_A5        == sck_pin)    gpio_init(A5,  GPO, 1, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(SPI1_SCK_B3   == sck_pin)    gpio_init(B3,  GPO, 1, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(SPI2_SCK_B13  == sck_pin)    gpio_init(B13, GPO, 1, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(SPI3_SCK_B3   == sck_pin)    gpio_init(B3,  GPO, 1, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(SPI3_SCK_C10  == sck_pin)    gpio_init(C10, GPO, 1, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else zf_assert(0);         //无此参数进入断言

    if(SPI1_MOSI_A7       == mosi_pin)   gpio_init(A7,  GPO, 1, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(SPI1_MOSI_B5  == mosi_pin)   gpio_init(B5,  GPO, 1, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(SPI2_MOSI_B15 == mosi_pin)   gpio_init(B15, GPO, 1, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(SPI3_MOSI_B5  == mosi_pin)   gpio_init(B5,  GPO, 1, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(SPI3_MOSI_C12 == mosi_pin)   gpio_init(C12, GPO, 1, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else zf_assert(0);         //无此参数进入断言

    if(SPI1_MISO_A6       == miso_pin)   gpio_init(A6,  GPI, 1, SPEED_50MHZ|GPI_FLOATING_IN);
    else if(SPI1_MISO_B4  == miso_pin)   gpio_init(B4,  GPI, 1, SPEED_50MHZ|GPI_FLOATING_IN);
    else if(SPI2_MISO_B14 == miso_pin)   gpio_init(B14, GPI, 1, SPEED_50MHZ|GPI_FLOATING_IN);
    else if(SPI3_MISO_C11 == miso_pin)   gpio_init(C11, GPI, 1, SPEED_50MHZ|GPI_FLOATING_IN);
    else if(SPI3_MISO_B4  == miso_pin)   gpio_init(B4,  GPI, 1, SPEED_50MHZ|GPI_FLOATING_IN);
    else if(SPI_MISO_NULL == miso_pin)   miso_pin = SPI_MISO_NULL;
    else  zf_assert(0);         //无此参数进入断言

    //初始化CS引脚
    gpio_init(cs_pin, GPO, 0, SPEED_50MHZ|GPO_PUSH_PULL);

}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI 接口初始化
// @param       spi_n           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// @param       mode            SPI 模式 参照 zf_driver_spi.h 内 spi_mode_enum 枚举体定义
// @param       baud            设置 SPI 的波特率 不超过系统时钟的一半 部分速率会被适配成相近的速率
// @param       sck_pin         选择 SCK 引脚 参照 zf_driver_spi.h 内 spi_pin_enum 枚举体定义
// @param       mosi_pin        选择 MOSI 引脚 参照 zf_driver_spi.h 内 spi_pin_enum 枚举体定义
// @param       miso_pin        选择 MISO 引脚 参照 zf_driver_spi.h 内 spi_pin_enum 枚举体定义
// @param       cs_pin          选择 CS 引脚 参照 zf_driver_gpio.h 内 gpio_pin_enum 枚举体定义
// @return      void
// Sample usage:                spi_init(SPI_1, 0, 1*1000*1000, SPI1_SCK_A5, SPI1_MOSI_A7, SPI1_MISO_A6, A4);  //硬件SPI初始化  模式0 波特率为1Mhz
//-------------------------------------------------------------------------------------------------------------------
void spi_init(spi_index_enum spi_n, spi_mode_enum mode, uint32_t baud, spi_pin_enum sck_pin, spi_pin_enum mosi_pin, spi_pin_enum miso_pin, gpio_pin_enum cs_pin)
{
    uint16_t psc = 0;
    SPI_InitTypeDef  SPI_InitStructure;


    // SCK MISO MOSI引脚必须成组传入
//    if(((mosi_pin - 2) != sck_pin))
//    {
//        assert(0);
//        return;
//    }

    SPI_I2S_DeInit((SPI_TypeDef *)(spi_index[spi_n]));

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);                                           // 复用总线使能

    if(sck_pin == SPI1_SCK_B3 && mosi_pin == SPI1_MOSI_B5 && miso_pin == SPI1_MISO_B4)
        GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);
    else if(sck_pin == SPI3_SCK_C10 && mosi_pin == SPI3_MOSI_C12 && miso_pin == SPI3_MISO_C11)
        GPIO_PinRemapConfig(GPIO_Remap_SPI3, ENABLE);


         if(SPI_1 == spi_n)   RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);                // SPI1时钟使能
    else if(SPI_2 == spi_n)   RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);                // SPI2时钟使能
    else if(SPI_3 == spi_n)   RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);                // SPI3时钟使能
    spi_gpio_init(sck_pin, mosi_pin, miso_pin, cs_pin);


    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;      // SPI设置为双线双向全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                           // 设置SPI工作模式:设置为主SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                       // 设置SPI的数据大小:SPI发送接收8位帧结构

    switch(mode)
    {
        case SPI_MODE0:
        {
            SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                      // 串行同步时钟的空闲状态为低电平
            SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                    // 串行同步时钟的第一个跳变沿（上升或下降）数据被采样
        }break;
        case SPI_MODE1:
        {
            SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                      // 串行同步时钟的空闲状态为低电平
            SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                    // 串行同步时钟的第二个跳变沿（上升或下降）数据被采样
        }break;
        case SPI_MODE2:
        {
            SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                     // 串行同步时钟的空闲状态为高电平
            SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                    // 串行同步时钟的第一个跳变沿（上升或下降）数据被采样
        }break;
        case SPI_MODE3:
        {
            SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                     // 串行同步时钟的空闲状态为高电平
            SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                    // 串行同步时钟的第二个跳变沿（上升或下降）数据被采样
        }break;
    }

    psc = system_clock / baud;                                              // 计算分频值
         if(psc >= 256) psc = SPI_BaudRatePrescaler_256;                    // 计算最佳分频值
    else if(psc >= 128) psc = SPI_BaudRatePrescaler_128;
    else if(psc >= 64)  psc = SPI_BaudRatePrescaler_64;
    else if(psc >= 32)  psc = SPI_BaudRatePrescaler_32;
    else if(psc >= 16)  psc = SPI_BaudRatePrescaler_16;
    else if(psc >= 8)   psc = SPI_BaudRatePrescaler_8;
    else if(psc >= 4)   psc = SPI_BaudRatePrescaler_4;
    else if(psc >= 2)   psc = SPI_BaudRatePrescaler_2;
    else zf_assert(0);
    SPI_InitStructure.SPI_BaudRatePrescaler = psc;                          // 定义波特率预分频的值
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                               // 主机模式，使用软件控制NSS引脚
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                      // 指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI_InitStructure.SPI_CRCPolynomial = 7;                                // CRC值计算的多项式
    SPI_Init((SPI_TypeDef *)(spi_index[spi_n]), &SPI_InitStructure);        // 根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器


    SPI_Cmd(((SPI_TypeDef *)(spi_index[spi_n])), ENABLE);                   // 使能SPI外设
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI 接口写 8bit 数据
// @param       spi_n           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// @param       data            数据
// @return      void
// Sample usage:                spi_write_8bit(SPI_1,0x11);
//-------------------------------------------------------------------------------------------------------------------
void spi_write_8bit (spi_index_enum spi_n, uint8_t dat)
{
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = dat;                                  // 发送数据
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);    // 发送为空
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI 接口写 8bit 数组
// @param       spi_n           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// @param       *data           数据存放缓冲区
// @param       len             缓冲区长度
// @return      void
// Sample usage:                spi_write_8bit_array(SPI_1,data,64);
//-------------------------------------------------------------------------------------------------------------------
void spi_write_8bit_array (spi_index_enum spi_n, uint8_t *dat, uint32_t len)
{
    while(len--)
    {
        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = *(dat++);
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI 接口写 16bit 数据
// @param       spi_n           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// @param       data            数据
// @return      void
// Sample usage:                spi_write_16bit(SPI_1,0x1101);
//-------------------------------------------------------------------------------------------------------------------
void spi_write_16bit (spi_index_enum spi_n, uint16_t dat)
{
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)((dat & 0xFF00)>>8);
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);

    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)(dat & 0x00FF);
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI 接口写 16bit 数组
// @param       spi_n           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// @param       *data           数据存放缓冲区
// @param       len             缓冲区长度
// @return      void
// Sample usage:                spi_write_16bit_array(SPI_1,data,64);
//-------------------------------------------------------------------------------------------------------------------
void spi_write_16bit_array (spi_index_enum spi_n, uint16_t *dat, uint32_t len)
{
    while(len--)
    {
        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)((*dat & 0xFF00)>>8);
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)(*dat++ & 0x00FF);
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI 接口向传感器的寄存器写 8bit 数据
// @param       spi_n           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// @param       register_name   寄存器地址
// @param       data            数据
// @return      void
// Sample usage:                spi_write_8bit_register(SPI_1,0x11,0x01);
//-------------------------------------------------------------------------------------------------------------------
void spi_write_8bit_register (spi_index_enum spi_n, uint8_t register_name, uint8_t dat)
{
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = register_name;
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY));
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;

    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = dat;
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY));
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI 接口向传感器的寄存器写 8bit 数组
// @param       spi_n           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// @param       register_name   寄存器地址
// @param       *data           数据存放缓冲区
// @param       len             缓冲区长度
// @return      void
// Sample usage:                spi_write_8bit_registers(SPI_1,0x11,data,32);
//-------------------------------------------------------------------------------------------------------------------
void spi_write_8bit_registers (spi_index_enum spi_n, uint8_t register_name, uint8_t *dat, uint32_t len)
{
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = register_name;
       while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);

    while(len--)
    {
        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = *dat++;
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI 接口向传感器的寄存器写 16bit 数据
// @param       spi_n           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// @param       register_name   寄存器地址
// @param       data            数据
// @return      void
// Sample usage:                spi_write_16bit_register(SPI_1,0x1011,0x0101);
//-------------------------------------------------------------------------------------------------------------------
void spi_write_16bit_register (spi_index_enum spi_n, uint16_t register_name, uint16_t dat)
{

    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)((register_name & 0xFF00)>>8);
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)(register_name & 0x00FF);
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);

    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)((dat & 0xFF00)>>8);
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)(dat & 0x00FF);
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);

}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI 接口向传感器的寄存器写 16bit 数组
// @param       spi_n           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// @param       register_name   寄存器地址
// @param       *data           数据存放缓冲区
// @param       len             缓冲区长度
// @return      void
// Sample usage:                spi_write_16bit_registers(SPI_1,0x1011,data,32);
//-------------------------------------------------------------------------------------------------------------------
void spi_write_16bit_registers (spi_index_enum spi_n, uint16_t register_name, uint16_t *dat, uint32_t len)
{
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)((register_name & 0xFF00)>>8);
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)(register_name & 0x00FF);
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);

    while(len--)
    {
        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)((*dat & 0xFF00)>>8);
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)(*dat++ & 0x00FF);
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
    }

}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI 接口读 8bit 数据
// @param       spi_n           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// @param       register_name   寄存器地址
// @return      uint8_t         数据
// Sample usage:                spi_read_8bit(SPI_1);
//-------------------------------------------------------------------------------------------------------------------
uint8_t spi_read_8bit (spi_index_enum spi_n)
{
    uint8_t dat = 0;

    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = 0;
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);

    //while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_RXNE) == RESET);
    dat = ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;

    return dat;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI 接口读 8bit 数组
// @param       spi_n           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// @param       *data           数据存放缓冲区
// @param       len             发送缓冲区长度
// @return      void
// Sample usage:                spi_read_8bit_array(SPI_1,data,64);
//-------------------------------------------------------------------------------------------------------------------
void spi_read_8bit_array (spi_index_enum spi_n, uint8_t *dat, uint32_t len)
{
    while(len--)
    {
        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = 0;
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
        //while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_RXNE) == RESET);
        *dat++ = ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI 接口读 16bit 数据
// @param       spi_n           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// @param       register_name   寄存器地址
// @return      uint16_t        数据
// Sample usage:                spi_read_16bit(SPI_1);
//-------------------------------------------------------------------------------------------------------------------
uint16_t spi_read_16bit (spi_index_enum spi_n)
{
    uint16_t dat = 0;

    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = 0;
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
    dat = ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;

    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = 0;
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
    dat = ((dat << 8)| ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR);

    return dat;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI 接口读 16bit 数组
// @param       spi_n           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// @param       *data           数据存放缓冲区
// @param       len             发送缓冲区长度
// @return      void
// Sample usage:                spi_read_16bit_array(SPI_1,data,64);
//-------------------------------------------------------------------------------------------------------------------
void spi_read_16bit_array (spi_index_enum spi_n, uint16_t *dat, uint32_t len)
{
    while(len--)
    {
        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = 0;
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
        //while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_RXNE) == RESET);
        *dat = ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;

        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = 0;
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
        //while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_RXNE) == RESET);
        *dat = ((*dat << 8)| ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR);
        dat++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI 接口从传感器的寄存器读 8bit 数据
// @param       spi_n           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// @param       register_name   寄存器地址
// @return      uint8_t         数据
// Sample usage:                spi_read_8bit_register(SPI_1,0x11);
 //-------------------------------------------------------------------------------------------------------------------
uint8_t spi_read_8bit_register (spi_index_enum spi_n, uint8_t register_name)
{
    uint8_t dat;

    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = register_name;
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY));
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;


    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = 0;
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY));
    dat = ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;

    return dat;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI 接口从传感器的寄存器读 8bit 数组
// @param       spi_n           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// @param       register_name   寄存器地址
// @param       *data           数据存放缓冲区
// @param       len             发送缓冲区长度
// @return      void
// Sample usage:                spi_read_8bit_registers(SPI_1,0x11,data,32);
//-------------------------------------------------------------------------------------------------------------------
void spi_read_8bit_registers (spi_index_enum spi_n, uint8_t register_name, uint8_t *dat, uint32_t len)
{
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = register_name;
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY));
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;

    while(len--)
    {
        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = 0;
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY));
        *dat++ = ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI 接口从传感器的寄存器读 16bit 数据
// @param       spi_n           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// @param       register_name   寄存器地址
// @return      uint16_t        数据
// Sample usage:                spi_read_16bit_register(SPI_1,0x1011);
//-------------------------------------------------------------------------------------------------------------------
uint16_t spi_read_16bit_register (spi_index_enum spi_n, uint16_t register_name)
{

    uint16_t dat = 0;

    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)((register_name & 0xFF00)>>8);
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);

    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)(register_name & 0x00FF);
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);

    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = 0;
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
    //while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_RXNE) == RESET);
    dat = ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;

    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = 0;
     while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
     //while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_RXNE) == RESET);
     dat = ((dat << 8)|  ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR);

     return dat;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI 接口从传感器的寄存器读 16bit 数组
// @param       spi_n           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// @param       register_name   寄存器地址
// @param       *data           数据存放缓冲区
// @param       len             发送缓冲区长度
// @return      void
// Sample usage:                spi_read_16bit_registers(SPI_1,0x1101,data,32);
//-------------------------------------------------------------------------------------------------------------------
void spi_read_16bit_registers (spi_index_enum spi_n, uint16_t register_name, uint16_t *dat, uint32_t len)
{
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)((register_name & 0xFF00)>>8);
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);

    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)(register_name & 0x00FF);
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);

    while(len--)
    {
        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = 0;
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
        //while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_RXNE) == RESET);
        *dat = ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;

        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = 0;
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
        //while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_RXNE) == RESET);
        *dat = ((*dat << 8)|  ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR);
        dat++;
    }

}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI 8bit 数据传输 发送与接收数据是同时进行的
// @param       spi_n           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// @param       write_buffer    发送的数据缓冲区地址
// @param       read_buffer     发送数据时接收到的数据的存储地址(不需要接收则传 NULL)
// @param       len             缓冲区长度
// @return      void
// Sample usage:                spi_transfer_8bit(SPI_1,buf,buf,1);
//-------------------------------------------------------------------------------------------------------------------
void spi_transfer_8bit (spi_index_enum spi_n, uint8_t *write_buffer, uint8_t *read_buffer, uint32_t len)
{
    while(len--)
    {
        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = *(write_buffer++);
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
        if(read_buffer != NULL)
        {
            //while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_RXNE) == RESET);
            *read_buffer++ = ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI 16bit 数据传输 发送与接收数据是同时进行的
// @param       spi_n           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// @param       write_buffer    发送的数据缓冲区地址
// @param       read_buffer     发送数据时接收到的数据的存储地址(不需要接收则传 NULL)
// @param       len             缓冲区长度
// @return      void
// Sample usage:                spi_transfer_16bit(SPI_1,buf,buf,1);
//-------------------------------------------------------------------------------------------------------------------
void spi_transfer_16bit (spi_index_enum spi_n, uint16_t *write_buffer, uint16_t *read_buffer, uint32_t len)
{
    while(len--)
    {
        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (*write_buffer&0xFF00)>>8;
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
        if(read_buffer != NULL)
        {
            //while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_RXNE) == RESET);
            *read_buffer++ = ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;
        }

        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (*write_buffer&0x00FF);
         while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
         if(read_buffer != NULL)
         {
             //while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_RXNE) == RESET);
             *read_buffer = ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;
         }
         write_buffer++;
         read_buffer++;
    }
}

