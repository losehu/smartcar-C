/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             zf_device_ips114
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ790875685)
* @version          查看doc内version文件 版本说明
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
* @note             接线定义：
*                   ------------------------------------
*                   模块管脚                                       单片机管脚
*                   SCL                 查看 zf_device_ips114.h 中 IPS114_SCL_PIN 宏定义
*                   SDA                 查看 zf_device_ips114.h 中 IPS114_SDA_PIN 宏定义
*                   RST                 查看 zf_device_ips114.h 中 IPS114_RST_PIN 宏定义
*                   DC                  查看 zf_device_ips114.h 中 IPS114_DC_PIN 宏定义
*                   CS                  查看 zf_device_ips114.h 中 IPS114_CS_PIN 宏定义
*                   BLK                 查看 zf_device_ips114.h 中 IPS114_BLK_PIN 宏定义
*                   电源引脚
*                   VCC                 3.3V电源
*                   GND                 电源地
*                   最大分辨率135*240
*                   ------------------------------------
********************************************************************************************************************/

#ifndef _zf_device_ips114_h_
#define _zf_device_ips114_h_

#include "zf_driver_spi.h"
#include "zf_common_clock.h"
#include "zf_common_debug.h"
#include "zf_common_font.h"
#include "zf_common_function.h"

#include "zf_driver_delay.h"

//透视变换后图像行列数
#define         TRFED_ROW               100
#define         TRFED_COL               114


#define IPS114_USE_SOFT_SPI             0                                       // 默认使用硬件 SPI 方式驱动 建议使用硬件 SPI 方式驱动
#if IPS114_USE_SOFT_SPI                                                         // 这两段 颜色正常的才是正确的 颜色灰的就是没有用的
//====================================================软件 SPI 驱动====================================================
#define IPS114_SOFT_SPI_DELAY           1                                       // 软件 SPI 的时钟延时周期 数值越小 SPI 通信速率越快
#define IPS114_SCL_PIN                  D4                                      // 软件 SPI SCK 引脚
#define IPS114_SDA_PIN                  D6                                      // 软件 SPI MOSI 引脚
//====================================================软件 SPI 驱动====================================================
#else
//====================================================硬件 SPI 驱动====================================================
#define IPS114_SPI_SPEED                system_clock/2                          // 硬件 SPI 速率
#define IPS114_SPI                      SPI_2                                   // 硬件 SPI 号
#define IPS114_SCL_PIN                  SPI2_SCK_B13                            // 硬件 SPI SCK 引脚
#define IPS114_SDA_PIN                  SPI2_MOSI_B15                           // 硬件 SPI MOSI 引脚
//====================================================硬件 SPI 驱动====================================================
#endif
#define IPS114_RST_PIN                  D12//B7                                      // Һ����λ���Ŷ���
#define IPS114_DC_PIN                   D11//D7                                      // Һ������λ���Ŷ���
#define IPS114_CS_PIN                   D10//D4                                      // CS Ƭѡ����
#define IPS114_BLK_PIN                  C6//D0
#define IPS114_DC(x)                    (x? (gpio_high(IPS114_DC_PIN)): (gpio_low(IPS114_DC_PIN)))
#define IPS114_RST(x)                   (x? (gpio_high(IPS114_RST_PIN)): (gpio_low(IPS114_RST_PIN)))
#define IPS114_CS(x)                    (x? (gpio_high(IPS114_CS_PIN)): (gpio_low(IPS114_CS_PIN)))
#define IPS114_BLK(x)                   (x? (gpio_high(IPS114_BLK_PIN)): (gpio_low(IPS114_BLK_PIN)))

typedef enum
{
    IPS114_PORTAIT                      = 0,                                    // 竖屏模式
    IPS114_PORTAIT_180                  = 1,                                    // 竖屏模式  旋转180
    IPS114_CROSSWISE                    = 2,                                    // 横屏模式
    IPS114_CROSSWISE_180                = 3,                                    // 横屏模式  旋转180
}ips114_dir_enum;

#define IPS114_DEFAULT_DISPLAY_DIR      IPS114_CROSSWISE                    // 默认的显示方向
#define IPS114_DEFAULT_PENCOLOR         RED                                   // 默认的画笔颜色
#define IPS114_DEFAULT_BGCOLOR          YELLOW                                   // 默认的背景颜色

extern uint16_t ips114_pencolor;
extern uint16_t ips114_bgcolor;

extern ips114_dir_enum ips114_display_dir;
extern uint8_t ips114_x_max;
extern uint8_t ips114_y_max;

void    ips114_init                     (void);
void    ips114_clear                    (uint16_t color);
void    ips114_set_dir                  (ips114_dir_enum dir);
void    ips114_set_color                (uint16_t pen, uint16_t bgcolor);
void    ips114_draw_point               (uint16_t x,uint16_t y,uint16_t color);

void    ips114_show_char                (uint16_t x,uint16_t y,const char dat);
void    ips114_show_string              (uint16_t x,uint16_t y,const char dat[]);
void    ips114_show_int8                (uint16_t x,uint16_t y,int8_t dat);
void    ips114_show_uint8               (uint16_t x,uint16_t y,uint8_t dat);
void    ips114_show_int16               (uint16_t x,uint16_t y,int16_t dat);
void    ips114_show_uint16              (uint16_t x,uint16_t y,uint16_t dat);
void    ips114_show_int32               (uint16_t x,uint16_t y,int32_t dat,uint8_t num);
void    ips114_show_float               (uint16_t x,uint16_t y,double dat,uint8_t num,uint8_t pointnum);

void    ips114_show_binary_image        (uint16_t x, uint16_t y, uint8_t *p, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height);
void    ips114_show_gray_image          (uint16_t x, uint16_t y, uint8_t *p, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height, uint8_t threshold);
void    ips114_show_rgb565_image        (uint16_t x, uint16_t y, uint16_t *p, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height);

void    ips114_show_wave                (uint16_t x, uint16_t y, uint8_t *p, uint16_t width, uint16_t value_max, uint16_t dis_width, uint16_t dis_value_max);
void    ips114_show_chinese             (uint16_t x, uint16_t y, uint8_t size, const uint8_t *p, uint8_t number, uint16_t color);

#define ips114_displayimage7725(p,width,height)                                 ips114_show_binary_image(0,0,p,width,height,width,height)

#define ips114_displayimage032(p,width,height)                                  ips114_show_gray_image(0,0,p,width,height,width,height,0)
#define ips114_displayimage032_zoom(p,width,height,dis_width,dis_height)        ips114_show_gray_image(0,0,p,width,height,dis_width,dis_height,0)
#define ips114_displayimage032_zoom1(p,width,height,x,y,dis_width,dis_height)   ips114_show_gray_image(x,y,p,width,height,dis_width,dis_height,0)

#define ips114_displayimage8660(p,width,height)                                 ips114_show_rgb565_image(0,0,p,width,height,width,height)
#define ips114_displayimage8660_zoom(p,width,height,dis_width,dis_height)       ips114_show_rgb565_image(0,0,p,width,height,dis_width,dis_height)
#define ips114_displayimage8660_zoom1(p,width,height,x,y,dis_width,dis_height)  ips114_show_rgb565_image(x,y,p,width,height,dis_width,dis_height)
//==============================USER======================================//
void ips114_display_myimage(uint16_t *p,uint16_t posi_x,uint16_t posi_y, \
        uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height);
void ips114_DrawXLine(uint16_t x,uint16_t color,uint16_t size);
void ips114_DrawYLine(uint16_t x,uint16_t color,uint16_t size);

void ips114_show_gray_image_vec (uint16_t x, uint16_t y, uint8_t *p[][TRFED_COL], uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height, uint8_t threshold);

#endif
