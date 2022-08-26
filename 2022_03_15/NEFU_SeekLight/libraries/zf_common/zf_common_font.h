/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_common_font
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
********************************************************************************************************************/

#ifndef _zf_common_font_h_
#define _zf_common_font_h_

#include "stdint.h"

//-------������ɫ----------
#define WHITE           0xFFFF  // ��ɫ
#define BLACK           0x0000  // ��ɫ
#define BLUE            0x001F  // ��ɫ
#define PURPLE          0XF81F  // ��ɫ
#define PINK            0XFE19  // ��ɫ
#define RED             0xF800  // ��ɫ
#define MAGENTA         0xF81F  // Ʒ��
#define GREEN           0x07E0  // ��ɫ
#define CYAN            0x07FF  // ��ɫ
#define YELLOW          0xFFE0  // ��ɫ
#define BROWN           0XBC40  // ��ɫ
#define GRAY            0X8430  // ��ɫ

extern const uint8_t tft_ascii[95][16];
extern const uint8_t gImage_qq[3200];
extern const uint8_t chinese_test[8][16];
extern const uint8_t oled_6x8[][6];
extern const uint8_t oled_8x16[];
extern const uint8_t oled_16x16_chinese[][16];

#endif
