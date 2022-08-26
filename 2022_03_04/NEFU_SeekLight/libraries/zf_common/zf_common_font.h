/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             zf_common_font
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ790875685)
* @version          查看doc内version文件 版本说明
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
********************************************************************************************************************/

#ifndef _zf_common_font_h_
#define _zf_common_font_h_

#include "stdint.h"

//-------常用颜色----------
#define WHITE           0xFFFF  // 白色
#define BLACK           0x0000  // 黑色
#define BLUE            0x001F  // 蓝色
#define PURPLE          0XF81F  // 紫色
#define PINK            0XFE19  // 粉色
#define RED             0xF800  // 红色
#define MAGENTA         0xF81F  // 品红
#define GREEN           0x07E0  // 绿色
#define CYAN            0x07FF  // 青色
#define YELLOW          0xFFE0  // 黄色
#define BROWN           0XBC40  // 棕色
#define GRAY            0X8430  // 灰色

extern const uint8_t tft_ascii[95][16];
extern const uint8_t gImage_qq[3200];
extern const uint8_t chinese_test[8][16];
extern const uint8_t oled_6x8[][6];
extern const uint8_t oled_8x16[];
extern const uint8_t oled_16x16_chinese[][16];

#endif
