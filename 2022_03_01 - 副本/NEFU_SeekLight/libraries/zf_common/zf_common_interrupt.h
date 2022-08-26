/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             zf_common_interrupt
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ790875685)
* @version          查看doc内version文件 版本说明
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
********************************************************************************************************************/

#ifndef _zf_nvic_h
#define _zf_nvic_h

#include "stdint.h"
#include "ch32v30x.h"

#include "ch32v30x_misc.h"

void        interrupt_init              (void);
void        interrupt_enable_all        (uint8_t state);
uint32_t    interrupt_disable_all       (void);
void        interrupt_enable            (IRQn_Type irqn);
void        interrupt_disable           (IRQn_Type irqn);
void        interrupt_set_priority      (IRQn_Type irqn, uint8_t priority);

#endif
