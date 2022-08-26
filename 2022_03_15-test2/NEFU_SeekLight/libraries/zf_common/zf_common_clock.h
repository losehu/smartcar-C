/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_common_clock
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
********************************************************************************************************************/

#ifndef _zf_common_clock_h_
#define _zf_common_clock_h_

#include "ch32v30x.h"

#define BOARD_XTAL_FREQ         8000000                                         // ����Ƶ�� ����Լ��õĲ������Ƶ�ʾ��޸����� UM ���巶ΧΪ 4-24Mhz
//#define XTAL_STARTUP_TIMEOUT    0x0800                                          // ��������ȴ���ʱʱ��

typedef enum
{
    SYSTEM_CLOCK_XTAL       = BOARD_XTAL_FREQ,                                  // ʹ�þ���Ƶ����Ϊʱ��Ƶ��
    SYSTEM_CLOCK_24M        = 24000000,                                         // 24Mhz
    SYSTEM_CLOCK_48M        = 48000000,                                         // 48Mhz
    SYSTEM_CLOCK_72M        = 72000000,                                         // 72Mhz
    SYSTEM_CLOCK_96M        = 96000000,                                         // 96Mhz
    SYSTEM_CLOCK_120M       = 120000000,                                        // 120Mhz
    SYSTEM_CLOCK_144M       = 144000000,                                        // 144Mhz
}system_clock_enum;

extern uint32_t system_clock;                                                   // ȫ�ֱ��� ϵͳʱ����Ϣ

void clock_init (uint32_t clock);                                               // ����ʱ�ӳ�ʼ��

#endif
