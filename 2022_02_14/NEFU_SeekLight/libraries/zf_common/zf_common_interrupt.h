/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_common_interrupt
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
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
