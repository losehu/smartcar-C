/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_device_tft180
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
* @note             ���߶��壺
*                   ------------------------------------
*                   ģ��ܽ�                                        ��Ƭ���ܽ�
*                   SCL                 �鿴 zf_device_tft180.h �� TFT180_SCL_PIN �궨��
*                   SDA                 �鿴 zf_device_tft180.h �� TFT180_SDA_PIN �궨��
*                   RES                 �鿴 zf_device_tft180.h �� TFT180_RES_PIN �궨��
*                   DC                  �鿴 zf_device_tft180.h �� TFT180_DC_PIN �궨��
*                   CS                  �鿴 zf_device_tft180.h �� TFT180_CS_PIN �궨��
*                   BL                  �鿴 zf_device_tft180.h �� TFT180_BL_PIN �궨��
*                   ��Դ����
*                   VCC                 3.3V��Դ
*                   GND                 ��Դ��
*                   ���ֱ���160*128
*                   ------------------------------------
********************************************************************************************************************/

#ifndef _zf_device_tft180_h_
#define _zf_device_tft180_h_


#include "zf_common_clock.h"
#include "zf_common_debug.h"
#include "zf_common_font.h"
#include "zf_common_function.h"

#include "zf_driver_delay.h"
#include "zf_driver_spi.h"
//#include "zf_driver_soft_spi.h"

#define TFT180_USE_SOFT_SPI             0                                       // Ĭ��ʹ��Ӳ�� SPI ��ʽ���� ����ʹ��Ӳ�� SPI ��ʽ����
#if TFT180_USE_SOFT_SPI                                                         // ������ ��ɫ�����Ĳ�����ȷ�� ��ɫ�ҵľ���û���õ�
//====================================================��� SPI ����====================================================
#define TFT180_SOFT_SPI_DELAY           1                                       // ��� SPI ��ʱ����ʱ���� ��ֵԽС SPI ͨ������Խ��
#define TFT180_SCL_PIN                  D4                                      // ��� SPI SCK ����
#define TFT180_SDA_PIN                  D6                                      // ��� SPI MOSI ����
//====================================================��� SPI ����====================================================
#else
//====================================================Ӳ�� SPI ����====================================================
#define TFT180_SPI_SPEED                system_clock/4                          // Ӳ�� SPI ����
#define TFT180_SPI                      SPI_2                                   // Ӳ�� SPI ��
#define TFT180_SCL_PIN                  SPI2_SCK_B13                            // Ӳ�� SPI SCK ����
#define TFT180_SDA_PIN                  SPI2_MOSI_B15                           // Ӳ�� SPI MOSI ����
//====================================================Ӳ�� SPI ����====================================================
#endif

#define TFT180_RES_PIN                  B7                                      // Һ����λ���Ŷ���
#define TFT180_DC_PIN                   D7                                      // Һ������λ���Ŷ���
#define TFT180_CS_PIN                   D4                                      // CS Ƭѡ����
#define TFT180_BL_PIN                   D0                                      // Һ���������Ŷ���

#define TFT180_DC(x)                    (x? (gpio_high(TFT180_DC_PIN)): (gpio_low(TFT180_DC_PIN)))
#define TFT180_RES(x)                   (x? (gpio_high(TFT180_RES_PIN)): (gpio_low(TFT180_RES_PIN)))
#define TFT180_CS(x)                    (x? (gpio_high(TFT180_CS_PIN)): (gpio_low(TFT180_CS_PIN)))
#define TFT180_BL(x)                    (x? (gpio_high(TFT180_BL_PIN)): (gpio_low(TFT180_BL_PIN)))

typedef enum
{
    TFT180_PORTAIT                      = 0,                                    // ����ģʽ
    TFT180_PORTAIT_180                  = 1,                                    // ����ģʽ  ��ת180
    TFT180_CROSSWISE                    = 2,                                    // ����ģʽ
    TFT180_CROSSWISE_180                = 3,                                    // ����ģʽ  ��ת180
}tft180_dir_enum;

#define TFT180_DEFAULT_DISPLAY_DIR      TFT180_PORTAIT                          // Ĭ�ϵ���ʾ����
#define TFT180_DEFAULT_PENCOLOR         RED                                     // Ĭ�ϵĻ�����ɫ
#define TFT180_DEFAULT_BGCOLOR          WHITE                                   // Ĭ�ϵı�����ɫ

extern uint16_t tft180_pencolor;
extern uint16_t tft180_bgcolor;

extern tft180_dir_enum tft180_display_dir;
extern uint8_t tft180_x_max;
extern uint8_t tft180_y_max;

void    tft180_init                     (void);
void    tft180_clear                    (uint16_t color);
void    tft180_set_dir                  (tft180_dir_enum dir);
void    tft180_set_color                (uint16_t pen, uint16_t bgcolor);
void    tft180_draw_point               (uint16_t x,uint16_t y,uint16_t color);

void    tft180_show_char                (uint16_t x,uint16_t y,const char dat);
void    tft180_show_string              (uint16_t x,uint16_t y,const char dat[]);
void    tft180_show_int8                (uint16_t x,uint16_t y,int8_t dat);
void    tft180_show_uint8               (uint16_t x,uint16_t y,uint8_t dat);
void    tft180_show_int16               (uint16_t x,uint16_t y,int16_t dat);
void    tft180_show_uint16              (uint16_t x,uint16_t y,uint16_t dat);
void    tft180_show_int32               (uint16_t x,uint16_t y,int32_t dat,uint8_t num);
void    tft180_show_float               (uint16_t x,uint16_t y,double dat,uint8_t num,uint8_t pointnum);

void    tft180_show_binary_image        (uint16_t x, uint16_t y, uint8_t *p, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height);
void    tft180_show_gray_image          (uint16_t x, uint16_t y, uint8_t *p, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height, uint8_t threshold);
void    tft180_show_rgb565_image        (uint16_t x, uint16_t y, uint16_t *p, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height);

void    tft180_show_wave                (uint16_t x, uint16_t y, uint8_t *p, uint16_t width, uint16_t value_max, uint16_t dis_width, uint16_t dis_value_max);
void    tft180_show_chinese             (uint16_t x, uint16_t y, uint8_t size, const uint8_t *p, uint8_t number, uint16_t color);

#define tft180_displayimage7725(p,width,height)                                 tft180_show_binary_image(0,0,p,width,height,width,height)

#define tft180_displayimage032(p,width,height)                                  tft180_show_gray_image(0,0,p,width,height,width,height,0)
#define tft180_displayimage032_zoom(p,width,height,dis_width,dis_height)        tft180_show_gray_image(0,0,p,width,height,dis_width,dis_height,0)
#define tft180_displayimage032_zoom1(p,width,height,x,y,dis_width,dis_height)   tft180_show_gray_image(x,y,p,width,height,dis_width,dis_height,0)

#define tft180_displayimage8660(p,width,height)                                 tft180_show_rgb565_image(0,0,p,width,height,width,height)
#define tft180_displayimage8660_zoom(p,width,height,dis_width,dis_height)       tft180_show_rgb565_image(0,0,p,width,height,dis_width,dis_height)
#define tft180_displayimage8660_zoom1(p,width,height,x,y,dis_width,dis_height)  tft180_show_rgb565_image(x,y,p,width,height,dis_width,dis_height)

#endif

