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
#ifndef _zf_spi_h
#define _zf_spi_h

#include "ch32v30x_spi.h"
#include "ch32v30x_rcc.h"
#include "ch32v30x_gpio.h"

#include "zf_common_debug.h"
#include "zf_common_clock.h"

#include "zf_driver_gpio.h"

#define SPI_SPEED_PRIORITY                                                      // SPI 通信速度优先

//此枚举定义不允许用户修改
typedef enum //枚举串口号
{
    SPI_1 = 0x00,
    SPI_2 = 0x01,
    SPI_3 = 0x02
}spi_index_enum;


typedef enum //枚举串口号
{
    //----------SPI1----------
    SPI1_SCK_A5 = 0x00,
    SPI1_MISO_A6,
    SPI1_MOSI_A7,

    // 重映射
    SPI1_SCK_B3 = 0x10,
    SPI1_MISO_B4,
    SPI1_MOSI_B5,

    //----------SPI2----------
    SPI2_SCK_B13 = 0x20,
    SPI2_MISO_B14,
    SPI2_MOSI_B15,

    //----------SPI3----------
    SPI3_SCK_B3 = 0x30,
    SPI3_MISO_B4,
    SPI3_MOSI_B5,

    // 重映射
    SPI3_SCK_C10 = 0x40,
    SPI3_MISO_C11,
    SPI3_MOSI_C12,

    // 不需要MISO引脚就用此定义
    SPI_MISO_NULL = 0xFF
}spi_pin_enum;


typedef enum                                                                    // 枚举 SPI 模式 此枚举定义不允许用户修改
{
    SPI_MODE0,
    SPI_MODE1,
    SPI_MODE2,
    SPI_MODE3,
}spi_mode_enum;


//typedef enum //枚举串口号
//{
//    FPCLK_DIV_2 = 0x00,
//    FPCLK_DIV_4 = 0x08,
//    FPCLK_DIV_8 = 0x10,
//    FPCLK_DIV_16 = 0x18,
//    FPCLK_DIV_32 = 0x20,
//    FPCLK_DIV_64 = 0x28,
//    FPCLK_DIV_128 = 0x30,
//    FPCLK_DIV_256 = 0x38,
//}SPI_SPEED_enum;


void        spi_init                        (spi_index_enum spi_n, spi_mode_enum mode, uint32_t baud, spi_pin_enum sck_pin, spi_pin_enum mosi_pin, spi_pin_enum miso_pin, gpio_pin_enum cs_pin);

void        spi_write_8bit                  (spi_index_enum spi_n, uint8_t data);
void        spi_write_8bit_array            (spi_index_enum spi_n, uint8_t *data, uint32_t len);

void        spi_write_16bit                 (spi_index_enum spi_n, uint16_t data);
void        spi_write_16bit_array           (spi_index_enum spi_n, uint16_t *data, uint32_t len);

void        spi_write_8bit_register         (spi_index_enum spi_n, uint8_t register_name, uint8_t data);
void        spi_write_8bit_registers        (spi_index_enum spi_n, uint8_t register_name, uint8_t *data, uint32_t len);

void        spi_write_16bit_register        (spi_index_enum spi_n, uint16_t register_name, uint16_t data);
void        spi_write_16bit_registers       (spi_index_enum spi_n, uint16_t register_name, uint16_t *data, uint32_t len);

uint8_t     spi_read_8bit                   (spi_index_enum spi_n);
void        spi_read_8bit_array             (spi_index_enum spi_n, uint8_t *data, uint32_t len);

uint16_t    spi_read_16bit                  (spi_index_enum spi_n);
void        spi_read_16bit_array            (spi_index_enum spi_n, uint16_t *data, uint32_t len);

uint8_t     spi_read_8bit_register          (spi_index_enum spi_n, uint8_t register_name);
void        spi_read_8bit_registers         (spi_index_enum spi_n, uint8_t register_name, uint8_t *data, uint32_t len);

uint16_t    spi_read_16bit_register         (spi_index_enum spi_n, uint16_t register_name);
void        spi_read_16bit_registers        (spi_index_enum spi_n, uint16_t register_name, uint16_t *data, uint32_t len);

void        spi_transfer_8bit               (spi_index_enum spi_n, uint8_t *write_buffer, uint8_t *read_buffer, uint32_t len);
void        spi_transfer_16bit              (spi_index_enum spi_n, uint16_t *write_buffer, uint16_t *read_buffer, uint32_t len);



#endif
