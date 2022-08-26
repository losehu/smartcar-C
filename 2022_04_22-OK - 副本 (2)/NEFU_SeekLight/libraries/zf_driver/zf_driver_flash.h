/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            zf_driver_flash
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/

#ifndef _zf_flash_h
#define _zf_flash_h

#include "ch32v30x_misc.h"
#include "ch32v30x_flash.h"

#define FLASH_BASE_ADDR             (0x08000000)                // FALSH首地址
#define FLASH_PAGE_SIZE             (0x00000400)                // 1K byte
#define FLASH_SECTION_SIZE          (FLASH_PAGE_SIZE*4)         // 4K byte
//一共16个扇区，一个扇区4K。
/*
2.5.2 片上存储器及自举模式
内置 32K 或 64K 字节 SRAM 区，用于存放数据，掉电后数据丢失。
内置 128K 或 256K 字节程序闪存存储区（Code FLASH），用于用户的应用程序和常量数据存储。 其中 256K FLASH+64K SRAM 产品支持选择配置为（192K FLASH+128K SRAM）、（224K FLASH+96K SRAM）、
（256K FLASH+64K SRAM）、（288K FLASH+32K SRAM）几种组合中的一种。
内置 28K 字节系统存储区（System FLASH），用于系统引导程序存储（厂家固化自举加载程序）。
128 字节用于厂商配置字存储，128 字节用于用户选择字存储。 在启动时，通过自举引脚（BOOT0 和 BOOT1）可以选择三种自举模式中的一种：
l 从程序闪存存储器自举 l 从系统存储器自举 l 从内部 SRAM 自举 自举加载程序存放于系统存储区，可以通过 USART1 和 USB 接口对程序闪存存储区的内容重新编程。

 */


//有多少个扇区呢？？？ CH32V307？？？
// 枚举 Flash 扇区  此枚举定义不允许用户修改
typedef enum
{
    FLASH_SECTION_00,
    FLASH_SECTION_01,
    FLASH_SECTION_02,
    FLASH_SECTION_03,
    FLASH_SECTION_04,
    FLASH_SECTION_05,
    FLASH_SECTION_06,
    FLASH_SECTION_07,
    FLASH_SECTION_08,
    FLASH_SECTION_09,
    FLASH_SECTION_10,
    FLASH_SECTION_11,
    FLASH_SECTION_12,
    FLASH_SECTION_13,
    FLASH_SECTION_14,
    FLASH_SECTION_15,
}FLASH_SECTION_enum;

// 枚举 Flash 页索引 此枚举定义不允许用户修改
typedef enum
{
    FLASH_PAGE_0,
    FLASH_PAGE_1,
    FLASH_PAGE_2,
    FLASH_PAGE_3,
}FLASH_PAGE_enum;

uint8_t flash_check(FLASH_SECTION_enum sector_num, FLASH_PAGE_enum page_num);
uint8_t flash_erase_page(FLASH_SECTION_enum sector_num, FLASH_PAGE_enum page_num);
void flash_page_read (FLASH_SECTION_enum sector_num, FLASH_PAGE_enum page_num, uint32_t *buf, uint16_t len);
uint8_t flash_page_program(FLASH_SECTION_enum sector_num, FLASH_PAGE_enum page_num, const uint32_t *buf, uint16_t len);

#endif
