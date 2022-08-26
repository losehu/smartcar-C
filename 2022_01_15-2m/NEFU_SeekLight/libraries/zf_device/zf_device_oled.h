/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_device_oled
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
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

#define OLED_USE_SOFT_SPI               0                                       // Ĭ��ʹ��Ӳ�� SPI ��ʽ���� ����ʹ��Ӳ�� SPI ��ʽ����
#if OLED_USE_SOFT_SPI                                                           // ������ ��ɫ�����Ĳ�����ȷ�� ��ɫ�ҵľ���û���õ�
//====================================================��� SPI ����====================================================
#define OLED_SOFT_SPI_DELAY             1                                       // ��� SPI ��ʱ����ʱ���� ��ֵԽС SPI ͨ������Խ��
#define OLED_D0_PIN                     D4                                      // ��� SPI SCK ����
#define OLED_D1_PIN                     D6                                      // ��� SPI MOSI ����
//====================================================��� SPI ����====================================================
#else
//====================================================Ӳ�� SPI ����====================================================
#define OLED_SPI_SPEED                  system_clock/2                          // Ӳ�� SPI ����
#define OLED_SPI                        SPI_2                                   // Ӳ�� SPI ��
#define OLED_D0_PIN                     SPI2_SCK_B13                             // Ӳ�� SPI SCK ����
#define OLED_D1_PIN                     SPI2_MOSI_B15                            // Ӳ�� SPI MOSI ����
//====================================================Ӳ�� SPI ����====================================================
#endif

#define OLED_RES_PIN                    B7                                     // Һ����λ���Ŷ���
#define OLED_DC_PIN                     D7                                      // Һ������λ���Ŷ���
#define OLED_CS_PIN                     D4                                      // CS Ƭѡ����

#define OLED_RES(x)                     (x?(gpio_high(OLED_RES_PIN)):(gpio_low(OLED_RES_PIN)))
#define OLED_DC(x)                      (x?(gpio_high(OLED_DC_PIN)):(gpio_low(OLED_DC_PIN)))
#define OLED_CS(x)                      (x?(gpio_high(OLED_CS_PIN)):(gpio_low(OLED_CS_PIN)))

typedef enum
{
    OLED_CROSSWISE                      = 0,                                    // ����ģʽ
    OLED_CROSSWISE_180                  = 1,                                    // ����ģʽ  ��ת180
}oled_dir_enum;

typedef enum
{
    OLED_6x8_FONT                      = 0,                                     // 6x8 ����
    OLED_8x16_FONT                      = 1,                                    // 8x16 ����
}oled_font_size_enum;

#define OLED_BRIGHTNESS                 0x7f                                    // ����OLED���� Խ��Խ�� ��Χ0-0XFF
#define OLED_DEFAULT_DISPLAY_DIR        OLED_CROSSWISE                          // Ĭ�ϵ���ʾ����
#define OLED_DEFAULT_DISPLAY_FONT       OLED_6x8_FONT                           // Ĭ�ϵ���ʾ����
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
