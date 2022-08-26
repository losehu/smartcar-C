/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             zf_device_oled
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ790875685)
* @version          查看doc内version文件 版本说明
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
* @note
********************************************************************************************************************/

#ifndef _zf_device_oled_h_
#define _zf_device_oled_h_

#include "zf_common_clock.h"
#include "zf_common_debug.h"
#include "zf_common_font.h"
#include "zf_common_function.h"

#include "zf_driver_delay.h"
//#include "zf_driver_soft_spi.h"
#include "zf_driver_spi.h"

#define OLED_USE_SOFT_SPI               0                                       // 默认使用硬件 SPI 方式驱动 建议使用硬件 SPI 方式驱动
#if OLED_USE_SOFT_SPI                                                           // 这两段 颜色正常的才是正确的 颜色灰的就是没有用的
//====================================================软件 SPI 驱动====================================================
#define OLED_SOFT_SPI_DELAY             1                                       // 软件 SPI 的时钟延时周期 数值越小 SPI 通信速率越快
#define OLED_D0_PIN                     D4                                      // 软件 SPI SCK 引脚
#define OLED_D1_PIN                     D6                                      // 软件 SPI MOSI 引脚
//====================================================软件 SPI 驱动====================================================
#else
//====================================================硬件 SPI 驱动====================================================
#define OLED_SPI_SPEED                  system_clock/2                          // 硬件 SPI 速率
#define OLED_SPI                        SPI_2                                   // 硬件 SPI 号
#define OLED_D0_PIN                     SPI2_SCK_B13                             // 硬件 SPI SCK 引脚
#define OLED_D1_PIN                     SPI2_MOSI_B15                            // 硬件 SPI MOSI 引脚
//====================================================硬件 SPI 驱动====================================================
#endif

#define OLED_RES_PIN                    B7                                     // 液晶复位引脚定义
#define OLED_DC_PIN                     D7                                      // 液晶命令位引脚定义
#define OLED_CS_PIN                     D4                                      // CS 片选引脚

#define OLED_RES(x)                     (x?(gpio_high(OLED_RES_PIN)):(gpio_low(OLED_RES_PIN)))
#define OLED_DC(x)                      (x?(gpio_high(OLED_DC_PIN)):(gpio_low(OLED_DC_PIN)))
#define OLED_CS(x)                      (x?(gpio_high(OLED_CS_PIN)):(gpio_low(OLED_CS_PIN)))

typedef enum
{
    OLED_CROSSWISE                      = 0,                                    // 横屏模式
    OLED_CROSSWISE_180                  = 1,                                    // 横屏模式  旋转180
}oled_dir_enum;

typedef enum
{
    OLED_6x8_FONT                      = 0,                                     // 6x8 字体
    OLED_8x16_FONT                      = 1,                                    // 8x16 字体
}oled_font_size_enum;

#define OLED_BRIGHTNESS                 0x7f                                    // 设置OLED亮度 越大越亮 范围0-0XFF
#define OLED_DEFAULT_DISPLAY_DIR        OLED_CROSSWISE                          // 默认的显示方向
#define OLED_DEFAULT_DISPLAY_FONT       OLED_6x8_FONT                           // 默认的显示方向
#define OLED_X_MAX                      128
#define OLED_Y_MAX                      64

void    oled_init                       (void);
void    oled_clear                      (uint8_t color);
void    oled_set_dir                    (oled_dir_enum dir);
void    oled_set_font                   (oled_font_size_enum font);
void    oled_draw_point                 (uint16_t x,uint16_t y,uint8_t color);

void    oled_show_string                (uint8_t x,uint8_t y,char ch[]);
void    oled_show_int8                  (uint16_t x,uint16_t y,int8_t dat);
void    oled_show_uint8                 (uint16_t x,uint16_t y,uint8_t dat);
void    oled_show_int16                 (uint16_t x,uint16_t y,int16_t dat);
void    oled_show_uint16                (uint16_t x,uint16_t y,uint16_t dat);
void    oled_show_int32                 (uint16_t x,uint16_t y,int32_t dat,uint8_t num);
void    oled_show_float                 (uint16_t x,uint16_t y,double dat,uint8_t num,uint8_t pointnum);

void    oled_show_binary_image          (uint16_t x, uint16_t y, uint8_t *p, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height);
void    oled_show_gray_image            (uint16_t x, uint16_t y, uint8_t *p, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height, uint8_t threshold);

void    oled_show_wave                  (uint16_t x, uint16_t y, uint8_t *p, uint16_t width, uint16_t value_max, uint16_t dis_width, uint16_t dis_value_max);
void    oled_show_chinese               (uint16_t x, uint16_t y, uint8_t size, const uint8_t *p, uint8_t number);

#define oled_displayimage7725(p,width,height)                                   oled_show_binary_image(0,0,p,width,height,128,64)

#define oled_displayimage032(p,width,height,x)                                  oled_show_gray_image(0,0,p,width,height,128,64,x)
#define oled_displayimage032_zoom(p,width,height,dis_width,dis_height,x)        oled_show_gray_image(0,0,p,width,height,dis_width,dis_height,x)

#endif
