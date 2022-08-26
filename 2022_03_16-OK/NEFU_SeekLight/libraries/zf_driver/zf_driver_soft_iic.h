/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_driver_soft_iic
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
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
    gpio_pin_enum       scl_pin;                                                // ���ڼ�¼��Ӧ�����ű��
    gpio_pin_enum       sda_pin;                                                // ���ڼ�¼��Ӧ�����ű��
    uint8_t             addr;                                                   // ������ַ
    uint32_t            delay;                                                  // ģ�� IIC ����ʱʱ��
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

