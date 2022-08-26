/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            zf_driver_delay
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ790875685)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/
#ifndef _zf_driver_delay_h
#define _zf_driver_delay_h



#include "ch32v30x.h"

#include "zf_common_clock.h"

void system_delay(uint64_t time);

//------------------------------------���º궨������SYSTICK��ʱ--------------------------------------------------------------//
#define system_delay_ms(time)      system_delay(time * (system_clock/8000))    //����SYSTICK��ʱʱ��  ��λms
#define system_delay_us(time)      system_delay(time * (system_clock/8000000)) //����SYSTICK��ʱʱ��  ��λus

#endif
