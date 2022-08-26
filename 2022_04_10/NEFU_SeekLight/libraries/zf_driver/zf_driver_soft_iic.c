/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             zf_driver_soft_iic
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ790875685)
* @version          查看doc内version文件 版本说明
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
********************************************************************************************************************/


#include "zf_driver_soft_iic.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief       软件 IIC 延时 内部调用
// @param       void
// @return      void
//-------------------------------------------------------------------------------------------------------------------
static void soft_iic_delay (uint32_t delay)
{
    volatile uint32_t count = delay;
    while(count--);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       软件 IIC 接口初始化 默认 MASTER 模式 不提供 SLAVE 模式
// @param       *soft_iic_obj   软件 IIC 指定信息存放结构体的指针
// @param       addr            软件 IIC 地址 这里需要注意 标准七位地址 最高位忽略 写入时请务必确认无误
// @param       delay           软件 IIC 延时 就是时钟高电平时间 越短 IIC 速率越高
// @param       scl_pin         软件 IIC 时钟引脚 参照 zf_driver_gpio.h 内 gpio_pin_enum 枚举体定义
// @param       sda_pin         软件 IIC 数据引脚 参照 zf_driver_gpio.h 内 gpio_pin_enum 枚举体定义
// @return      void            
// Sample usage:                soft_iic_init(&soft_iic_obj, addr, 100, B6, B7);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_init (soft_iic_info_struct *soft_iic_obj, uint8_t addr, uint32_t delay, gpio_pin_enum scl_pin, gpio_pin_enum sda_pin)
{
    soft_iic_obj->scl_pin = scl_pin;
    soft_iic_obj->sda_pin = sda_pin;
    soft_iic_obj->addr = addr;
    soft_iic_obj->delay = delay;
    gpio_init(scl_pin, GPO, GPIO_HIGH, GPO_PUSH_PULL);                          // 提取对应IO索引 AF功能编码
    gpio_init(sda_pin, GPO, GPIO_HIGH, GPO_OPEN_DTAIN);                         // 提取对应IO索引 AF功能编码
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       软件 IIC START 信号 内部调用
// @param       *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// @return      void
//-------------------------------------------------------------------------------------------------------------------
static void soft_iic_start (soft_iic_info_struct *soft_iic_obj)
{
    gpio_high(soft_iic_obj->scl_pin);                                           // SCL 高电平
    gpio_high(soft_iic_obj->sda_pin);                                           // SDA 高电平

    soft_iic_delay(soft_iic_obj->delay);
    gpio_low(soft_iic_obj->sda_pin);                                            // SDA 先拉低
    soft_iic_delay(soft_iic_obj->delay);
    gpio_low(soft_iic_obj->scl_pin);                                            // SCL 再拉低
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       软件 IIC STOP 信号 内部调用
// @param       *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// @return      void
//-------------------------------------------------------------------------------------------------------------------
static void soft_iic_stop (soft_iic_info_struct *soft_iic_obj)
{
    gpio_low(soft_iic_obj->sda_pin);                                            // SDA 低电平
    gpio_low(soft_iic_obj->scl_pin);                                            // SCL 低电平

    soft_iic_delay(soft_iic_obj->delay);
    gpio_high(soft_iic_obj->scl_pin);                                           // SCL 先拉高
    soft_iic_delay(soft_iic_obj->delay);
    gpio_high(soft_iic_obj->sda_pin);                                           // SDA 再拉高
    soft_iic_delay(soft_iic_obj->delay);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       软件 IIC 发送 ACK/NAKC 信号 内部调用
// @param       *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// @return      void
//-------------------------------------------------------------------------------------------------------------------
static void soft_iic_send_ack (soft_iic_info_struct *soft_iic_obj, uint8_t ack)
{
    gpio_low(soft_iic_obj->scl_pin);                                            // SCL 低电平

    soft_iic_delay(soft_iic_obj->delay);
    if(ack)
        gpio_high(soft_iic_obj->sda_pin);                                       // SDA 拉高
    else
        gpio_low(soft_iic_obj->sda_pin);                                        // SDA 拉低

    gpio_high(soft_iic_obj->scl_pin);                                           // SCL 拉高
    soft_iic_delay(soft_iic_obj->delay);
    gpio_low(soft_iic_obj->scl_pin);                                            // SCL 拉低
    soft_iic_delay(soft_iic_obj->delay);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       软件 IIC 获取 ACK/NAKC 信号 内部调用
// @param       *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// @return      uint8_t         ACK 状态
//-------------------------------------------------------------------------------------------------------------------
static uint8_t soft_iic_wait_ack (soft_iic_info_struct *soft_iic_obj)
{
    uint8_t temp = 0;
    gpio_low(soft_iic_obj->scl_pin);                                            // SCL 低电平
    soft_iic_delay(soft_iic_obj->delay);

    gpio_high(soft_iic_obj->scl_pin);                                           // SCL 高电平
    soft_iic_delay(soft_iic_obj->delay);

    if(gpio_get_level(soft_iic_obj->sda_pin))
        temp = 1;
    gpio_low(soft_iic_obj->scl_pin);                                            // SCL 低电平
    soft_iic_delay(soft_iic_obj->delay);

    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       软件 IIC 发送 8bit 数据 内部调用
// @param       *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// @param       data            数据
// @return      uint8_t         ACK 状态
//-------------------------------------------------------------------------------------------------------------------
static uint8_t soft_iic_send_data (soft_iic_info_struct *soft_iic_obj, uint8_t data)
{
    uint8_t temp = 8;

    while(temp--)
    {
        if(data & 0x80)
            gpio_high(soft_iic_obj->sda_pin);                                   // SDA 高电平
        else
            gpio_low(soft_iic_obj->sda_pin);                                    // SDA 低电平
        data = data << 1;

        soft_iic_delay(soft_iic_obj->delay);
        gpio_high(soft_iic_obj->scl_pin);                                       // SCL 拉高
        soft_iic_delay(soft_iic_obj->delay);
        gpio_low(soft_iic_obj->scl_pin);                                        // SCL 拉低
    }
    if(soft_iic_wait_ack(soft_iic_obj) == 1)
        return 0;
    return 1;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       软件 IIC 读取 8bit 数据 内部调用
// @param       *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// @param       ack             ACK 或 NACK
// @return      uint8_t         数据
//-------------------------------------------------------------------------------------------------------------------
static uint8_t soft_iic_read_data (soft_iic_info_struct *soft_iic_obj, uint8_t ack)
{
    uint8_t data = 0x00;
    uint8_t temp = 8;
    gpio_low(soft_iic_obj->scl_pin);                                            // SCL 低电平
    soft_iic_delay(soft_iic_obj->delay);
    gpio_high(soft_iic_obj->sda_pin);                                           // SDA 高电平 释放 SDA

    while(temp--)
    {
        soft_iic_delay(soft_iic_obj->delay);
        gpio_low(soft_iic_obj->scl_pin);                                        // SCL 拉低
        soft_iic_delay(soft_iic_obj->delay);
        gpio_high(soft_iic_obj->scl_pin);                                       // SCL 拉高
        soft_iic_delay(soft_iic_obj->delay);
        data = data << 1;
        if(gpio_get_level(soft_iic_obj->sda_pin))
            data |= 1;
    }
    gpio_low(soft_iic_obj->scl_pin);                                            // SCL 低电平
    soft_iic_delay(soft_iic_obj->delay);
    soft_iic_send_ack(soft_iic_obj, ack);
    return data;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       软件 IIC 接口写 8bit 数据
// @param       *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// @param       data            要写入的数据
// @return      void            
// Sample usage:                soft_iic_write_8bit_register(soft_iic_obj,0x01);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_8bit (soft_iic_info_struct *soft_iic_obj, uint8_t data)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, data);
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       软件 IIC 接口写 8bit 数组
// @param       *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// @param       *data           数据存放缓冲区
// @param       len             缓冲区长度
// @return      void            
// Sample usage:                soft_iic_write_8bit_arrray(soft_iic_obj,data,6);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_8bit_arrray (soft_iic_info_struct *soft_iic_obj, uint8_t *data, uint32_t len)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    while(len--)
    {
        soft_iic_send_data(soft_iic_obj, *data++);
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       软件 IIC 接口器写 16bit 数据
// @param       *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// @param       data            要写入的数据
// @return      void            
// Sample usage:                soft_iic_write_16bit(soft_iic_obj,0x0101);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_16bit (soft_iic_info_struct *soft_iic_obj, uint16_t data)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, (uint8_t)((data & 0xFF00)>>8));
    soft_iic_send_data(soft_iic_obj, (uint8_t)(data & 0x00FF));
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       软件 IIC 接口写 16bit 数组
// @param       *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// @param       *data           数据存放缓冲区
// @param       len             缓冲区长度
// @return      void            
// Sample usage:                soft_iic_write_16bit_array(soft_iic_obj,data,6);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_16bit_array (soft_iic_info_struct *soft_iic_obj, uint16_t *data, uint32_t len)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    while(len--)
    {
        soft_iic_send_data(soft_iic_obj, (uint8_t)((*data & 0xFF00)>>8));
        soft_iic_send_data(soft_iic_obj, (uint8_t)(*data++ & 0x00FF));
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       软件 IIC 接口向传感器寄存器写 8bit 数据
// @param       *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// @param       register_name   传感器的寄存器地址
// @param       data            要写入的数据
// @return      void            
// Sample usage:                soft_iic_write_8bit_register(soft_iic_obj,0x01,0x01);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_8bit_register (soft_iic_info_struct *soft_iic_obj, uint8_t register_name, uint8_t data)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, register_name);
    soft_iic_send_data(soft_iic_obj, data);
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       软件 IIC 接口向传感器寄存器写 8bit 数组
// @param       *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// @param       register_name   传感器的寄存器地址
// @param       *data           数据存放缓冲区
// @param       len             缓冲区长度
// @return      void            
// Sample usage:                soft_iic_write_8bit_registers(soft_iic_obj,0x01,data,6);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_8bit_registers (soft_iic_info_struct *soft_iic_obj, uint8_t register_name, uint8_t *data, uint32_t len)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, register_name);
    while(len--)
    {
        soft_iic_send_data(soft_iic_obj, *data++);
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       软件 IIC 接口向传感器寄存器写 16bit 数据
// @param       *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// @param       register_name   传感器的寄存器地址
// @param       data            要写入的数据
// @return      void            
// Sample usage:                soft_iic_write_16bit_register(soft_iic_obj,0x0101,0x0101);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_16bit_register (soft_iic_info_struct *soft_iic_obj, uint16_t register_name, uint16_t data)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, (uint8_t)((register_name & 0xFF00)>>8));
    soft_iic_send_data(soft_iic_obj, (uint8_t)(register_name & 0x00FF));
    soft_iic_send_data(soft_iic_obj, (uint8_t)((data & 0xFF00)>>8));
    soft_iic_send_data(soft_iic_obj, (uint8_t)(data & 0x00FF));
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       软件 IIC 接口向传感器寄存器写 16bit 数组
// @param       *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// @param       register_name   传感器的寄存器地址
// @param       *data           数据存放缓冲区
// @param       len             缓冲区长度
// @return      void            
// Sample usage:                soft_iic_write_16bit_registers(soft_iic_obj,0x0101,data,6);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_16bit_registers (soft_iic_info_struct *soft_iic_obj, uint16_t register_name, uint16_t *data, uint32_t len)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, (uint8_t)((register_name & 0xFF00)>>8));
    soft_iic_send_data(soft_iic_obj, (uint8_t)(register_name & 0x00FF));
    while(len--)
    {
        soft_iic_send_data(soft_iic_obj, (uint8_t)((*data & 0xFF00)>>8));
        soft_iic_send_data(soft_iic_obj, (uint8_t)(*data++ & 0x00FF));
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       软件 IIC 接口读取 8bit 数据
// @param       *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// @return      uint8_t         返回读取的 8bit 数据
// Sample usage:                soft_iic_read_8bit(soft_iic_obj);
//-------------------------------------------------------------------------------------------------------------------
uint8_t soft_iic_read_8bit (soft_iic_info_struct *soft_iic_obj)
{
    uint8_t temp = 0;
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
    temp = soft_iic_read_data(soft_iic_obj, 1);
    soft_iic_stop(soft_iic_obj);
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       软件 IIC 接口从传感器寄存器读取 8bit 数组
// @param       *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// @param       register_name   传感器的寄存器地址
// @param       *data           要读取的数据的缓冲区指针
// @param       len             要读取的数据长度
// @return      void            
// Sample usage:                soft_iic_read_8bit_array(soft_iic_obj,data,8);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_read_8bit_array (soft_iic_info_struct *soft_iic_obj, uint8_t *data, uint32_t len)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
    while(len--)
    {
        *data++ = soft_iic_read_data(soft_iic_obj, len==1);
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       软件 IIC 接口读取 16bit 数据
// @param       *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// @param       register_name   传感器的寄存器地址
// @return      uint16_t        返回读取的 16bit 数据
// Sample usage:                soft_iic_read_16bit(soft_iic_obj);
//-------------------------------------------------------------------------------------------------------------------
uint16_t soft_iic_read_16bit (soft_iic_info_struct *soft_iic_obj)
{
    uint16_t temp = 0;
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
    temp = soft_iic_read_data(soft_iic_obj, 0);
    temp = ((temp << 8)| soft_iic_read_data(soft_iic_obj, 1));
    soft_iic_stop(soft_iic_obj);
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       软件 IIC 接口读取 16bit 数组
// @param       *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// @param       *data           要读取的数据的缓冲区指针
// @param       len             要读取的数据长度
// @return      void            
// Sample usage:                soft_iic_read_16bit_array(soft_iic_obj,data,8);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_read_16bit_array (soft_iic_info_struct *soft_iic_obj, uint16_t *data, uint32_t len)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
    while(len--)
    {
        *data = soft_iic_read_data(soft_iic_obj, 0);
        *data = ((*data << 8)| soft_iic_read_data(soft_iic_obj, len==1));
        data++;
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       软件 IIC 接口从传感器寄存器读取 8bit 数据
// @param       *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// @param       register_name   传感器的寄存器地址
// @return      uint8_t         返回读取的 8bit 数据
// Sample usage:                soft_iic_read_8bit_register(soft_iic_obj,0x01);
//-------------------------------------------------------------------------------------------------------------------
uint8_t soft_iic_read_8bit_register (soft_iic_info_struct *soft_iic_obj, uint8_t register_name)
{
    uint8_t temp = 0;
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, register_name);
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
    temp = soft_iic_read_data(soft_iic_obj, 1);
    soft_iic_stop(soft_iic_obj);
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       软件 IIC 接口从传感器寄存器读取 8bit 数组
// @param       *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// @param       register_name   传感器的寄存器地址
// @param       *data           要读取的数据的缓冲区指针
// @param       len             要读取的数据长度
// @return      void            
// Sample usage:                soft_iic_read_8bit_registers(soft_iic_obj,0x01,data,8);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_read_8bit_registers (soft_iic_info_struct *soft_iic_obj, uint8_t register_name, uint8_t *data, uint32_t len)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, register_name);
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
    while(len--)
    {
        *data++ = soft_iic_read_data(soft_iic_obj, len==1);
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       软件 IIC 接口从传感器寄存器读取 16bit 数据
// @param       *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// @param       register_name   传感器的寄存器地址
// @return      uint16_t        返回读取的 16bit 数据
// Sample usage:                soft_iic_read_16bit_register(soft_iic_obj,0x0101);
//-------------------------------------------------------------------------------------------------------------------
uint16_t soft_iic_read_16bit_register (soft_iic_info_struct *soft_iic_obj, uint16_t register_name)
{
    uint16_t temp = 0;
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, (uint8_t)((register_name & 0xFF00)>>8));
    soft_iic_send_data(soft_iic_obj, (uint8_t)(register_name & 0x00FF));
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
    temp = soft_iic_read_data(soft_iic_obj, 0);
    temp = ((temp << 8)| soft_iic_read_data(soft_iic_obj, 1));
    soft_iic_stop(soft_iic_obj);
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       软件 IIC 接口从传感器寄存器读取 16bit 数组
// @param       *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// @param       register_name   传感器的寄存器地址
// @param       *data           要读取的数据的缓冲区指针
// @param       len             要读取的数据长度
// @return      void            
// Sample usage:                soft_iic_read_16bit_registers(soft_iic_obj,0x0101,data,8);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_read_16bit_registers (soft_iic_info_struct *soft_iic_obj, uint16_t register_name, uint16_t *data, uint32_t len)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, (uint8_t)((register_name & 0xFF00)>>8));
    soft_iic_send_data(soft_iic_obj, (uint8_t)(register_name & 0x00FF));
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
    while(len--)
    {
        *data = soft_iic_read_data(soft_iic_obj, 0);
        *data = ((*data << 8)| soft_iic_read_data(soft_iic_obj, len==1));
        data++;
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       软件 IIC 接口 SCCB 模式向传感器寄存器写 8bit 数据
// @param       *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// @param       register_name   传感器的寄存器地址
// @param       data            要写入的数据
// @return      void            
// Sample usage:                soft_iic_sccb_write_register(soft_iic_obj,0x01,0x01);
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_sccb_write_register (soft_iic_info_struct *soft_iic_obj, uint8_t register_name, uint8_t data)
{
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, register_name);
    soft_iic_send_data(soft_iic_obj, data);
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       软件 IIC 接口 SCCB 模式从传感器寄存器读取 8bit 数据
// @param       *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// @param       register_name   传感器的寄存器地址
// @return      uint8_t         返回读取的 8bit 数据
// Sample usage:                soft_iic_sccb_read_register(soft_iic_obj,0x01);
//-------------------------------------------------------------------------------------------------------------------
uint8_t soft_iic_sccb_read_register (soft_iic_info_struct *soft_iic_obj, uint8_t register_name)
{
    uint8_t temp = 0;
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, register_name);
    soft_iic_stop(soft_iic_obj);

    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
    temp = soft_iic_read_data(soft_iic_obj, 1);
    soft_iic_stop(soft_iic_obj);
    return temp;
}

