/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_device_ips114
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
* @note             ���߶��壺
*                   ------------------------------------
*                   ģ��ܽ�                                       ��Ƭ���ܽ�
*                   SCL                 �鿴 zf_device_ips114.h �� IPS114_SCL_PIN �궨��
*                   SDA                 �鿴 zf_device_ips114.h �� IPS114_SDA_PIN �궨��
*                   RST                 �鿴 zf_device_ips114.h �� IPS114_RST_PIN �궨��
*                   DC                  �鿴 zf_device_ips114.h �� IPS114_DC_PIN �궨��
*                   CS                  �鿴 zf_device_ips114.h �� IPS114_CS_PIN �궨��
*                   BLK                 �鿴 zf_device_ips114.h �� IPS114_BLK_PIN �궨��
*                   ��Դ����
*                   VCC                 3.3V��Դ
*                   GND                 ��Դ��
*                   ���ֱ���135*240
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

//͸�ӱ任��ͼ��������
#define         TRFED_ROW               100
#define         TRFED_COL               114


#define IPS114_USE_SOFT_SPI             0                                       // Ĭ��ʹ��Ӳ�� SPI ��ʽ���� ����ʹ��Ӳ�� SPI ��ʽ����
#if IPS114_USE_SOFT_SPI                                                         // ������ ��ɫ�����Ĳ�����ȷ�� ��ɫ�ҵľ���û���õ�
//====================================================��� SPI ����====================================================
#define IPS114_SOFT_SPI_DELAY           1                                       // ��� SPI ��ʱ����ʱ���� ��ֵԽС SPI ͨ������Խ��
#define IPS114_SCL_PIN                  D4                                      // ��� SPI SCK ����
#define IPS114_SDA_PIN                  D6                                      // ��� SPI MOSI ����
//====================================================��� SPI ����====================================================
#else
//====================================================Ӳ�� SPI ����====================================================
#define IPS114_SPI_SPEED                system_clock/2                          // Ӳ�� SPI ����
#define IPS114_SPI                      SPI_2                                   // Ӳ�� SPI ��
#define IPS114_SCL_PIN                  SPI2_SCK_B13                            // Ӳ�� SPI SCK ����
#define IPS114_SDA_PIN                  SPI2_MOSI_B15                           // Ӳ�� SPI MOSI ����
//====================================================Ӳ�� SPI ����====================================================
#endif

#define IPS114_RST_PIN                  D12                                     // Һ����λ���Ŷ���
#define IPS114_DC_PIN                   D11                                      // Һ������λ���Ŷ���
#define IPS114_CS_PIN                   D10                                     // CS Ƭѡ����
#define IPS114_BLK_PIN                  C6                                    // Һ���������Ŷ���

#define IPS114_DC(x)                    (x? (gpio_high(IPS114_DC_PIN)): (gpio_low(IPS114_DC_PIN)))
#define IPS114_RST(x)                   (x? (gpio_high(IPS114_RST_PIN)): (gpio_low(IPS114_RST_PIN)))
#define IPS114_CS(x)                    (x? (gpio_high(IPS114_CS_PIN)): (gpio_low(IPS114_CS_PIN)))
#define IPS114_BLK(x)                   (x? (gpio_high(IPS114_BLK_PIN)): (gpio_low(IPS114_BLK_PIN)))

typedef enum
{
    IPS114_PORTAIT                      = 0,                                    // ����ģʽ
    IPS114_PORTAIT_180                  = 1,                                    // ����ģʽ  ��ת180
    IPS114_CROSSWISE                    = 2,                                    // ����ģʽ
    IPS114_CROSSWISE_180                = 3,                                    // ����ģʽ  ��ת180
}ips114_dir_enum;

#define IPS114_DEFAULT_DISPLAY_DIR      IPS114_CROSSWISE                    // Ĭ�ϵ���ʾ����
#define IPS114_DEFAULT_PENCOLOR         RED                                   // Ĭ�ϵĻ�����ɫ
#define IPS114_DEFAULT_BGCOLOR          YELLOW                                   // Ĭ�ϵı�����ɫ

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
