/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_device_virtual_oscilloscope
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ3184284598)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         IAR 8.32.4 or MDK 5.28
* @Target core      MM32F3277
* @Taobao           https://seekfree.taobao.com/
* @date             2021-02-22
* @note
********************************************************************************************************************/

#ifndef _zf_device_virtual_oscilloscope_h_
#define _zf_device_virtual_oscilloscope_h_

#include "zf_common_headfile.h"

extern uint8_t virtual_oscilloscope_data[10];

void    virtual_oscilloscope_data_conversion    (const int16 data1, const int16 data2, const int16 data3, const int16 data4);

#endif
