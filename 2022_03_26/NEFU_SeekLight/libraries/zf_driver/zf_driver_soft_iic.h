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

#ifndef _zf_driver_soft_iic_h_
#define _zf_driver_soft_iic_h_

#include "zf_common_debug.h"
#include "zf_driver_gpio.h"

typedef struct
{
    gpio_pin_enum       scl_pin;                                                // 用于记录对应的引脚编号
    gpio_pin_enum       sda_pin;                                                // 用于记录对应的引脚编号
    uint8_t             addr;                                                   // 器件地址
    uint32_t            delay;                                                  // 模拟 IIC 软延时时长
}soft_iic_info_struct;

void        soft_iic_init                   (soft_iic_info_struct *soft_iic_obj, uint8_t addr, uint32_t delay, gpio_pin_enum scl_pin, gpio_pin_enum sda_pin);

void        soft_iic_write_8bit             (soft_iic_info_struct *soft_iic_obj, uint8_t data);
void        soft_iic_write_8bit_arrray      (soft_iic_info_struct *soft_iic_obj, uint8_t *data, uint32_t len);

void        soft_iic_write_16bit            (soft_iic_info_struct *soft_iic_obj, uint16_t data);
void        soft_iic_write_16bit_arrray     (soft_iic_info_struct *soft_iic_obj, uint16_t *data, uint32_t len);

void        soft_iic_write_8bit_register    (soft_iic_info_struct *soft_iic_obj, uint8_t register_name, uint8_t data);
void        soft_iic_write_8bit_registers   (soft_iic_info_struct *soft_iic_obj, uint8_t register_name, uint8_t *data, uint32_t len);

void        soft_iic_write_16bit_register   (soft_iic_info_struct *soft_iic_obj, uint16_t register_name, uint16_t data);
void        soft_iic_write_16bit_registers  (soft_iic_info_struct *soft_iic_obj, uint16_t register_name, uint16_t *data, uint32_t len);

uint8_t     soft_iic_read_8bit              (soft_iic_info_struct *soft_iic_obj);
void        soft_iic_read_8bit_array        (soft_iic_info_struct *soft_iic_obj, uint8_t *data, uint32_t len);

uint16_t    soft_iic_read_16bit             (soft_iic_info_struct *soft_iic_obj);
void        soft_iic_read_16bit_array       (soft_iic_info_struct *soft_iic_obj, uint16_t *data, uint32_t len);

uint8_t     soft_iic_read_8bit_register     (soft_iic_info_struct *soft_iic_obj, uint8_t register_name);
void        soft_iic_read_8bit_registers    (soft_iic_info_struct *soft_iic_obj, uint8_t register_name, uint8_t *data, uint32_t len);

uint16_t    soft_iic_read_16bit_register    (soft_iic_info_struct *soft_iic_obj, uint16_t register_name);
void        soft_iic_read_16bit_registers   (soft_iic_info_struct *soft_iic_obj, uint16_t register_name, uint16_t *data, uint32_t len);

void        soft_iic_sccb_write_register    (soft_iic_info_struct *soft_iic_obj, uint8_t register_name, uint8_t data);
uint8_t     soft_iic_sccb_read_register     (soft_iic_info_struct *soft_iic_obj, uint8_t register_name);

#endif

