/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_device_ips200_parallel8
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
*                   BL                  �鿴 zf_device_ips200_parallel8.h �� IPS200_BL_PIN �궨��
*                   CS                  �鿴 zf_device_ips200_parallel8.h �� IPS200_CS_PIN �궨��
*                   RST                 �鿴 zf_device_ips200_parallel8.h �� IPS200_RST_PIN �궨��
*                   RS                  �鿴 zf_device_ips200_parallel8.h �� IPS200_RS_PIN �궨��
*                   WR                  �鿴 zf_device_ips200_parallel8.h �� IPS200_WR_PIN �궨��
*                   RD                  �鿴 zf_device_ips200_parallel8.h �� IPS200_RD_PIN �궨��
*                   D0-D7               �鿴 zf_device_ips200_parallel8.h �� ICM20602_CS_PIN �궨��
*                   ��Դ����
*                   VCC                 3.3V��Դ
*                   GND                 ��Դ��
*                   ------------------------------------
********************************************************************************************************************/
#ifndef _SEEKFREE_IPS20_PARALLEL8_H
#define _SEEKFREE_IPS20_PARALLEL8_H

#include "zf_common_clock.h"
#include "zf_common_debug.h"
#include "zf_common_font.h"
#include "zf_common_function.h"

#include "zf_driver_delay.h"
#include "zf_driver_gpio.h"

// ���Ŷ���
#define IPS200_RD_PIN           B13
#define IPS200_WR_PIN           B15
#define IPS200_RS_PIN           B7
#define IPS200_RST_PIN          D7
#define IPS200_CS_PIN           D4
#define IPS200_BL_PIN           D0

// �������
#define IPS200_RD(x)        	(x? (gpio_high(IPS200_RD_PIN)): (gpio_low(IPS200_RD_PIN)))
#define IPS200_WR(x)        	(x? (gpio_high(IPS200_WR_PIN)): (gpio_low(IPS200_WR_PIN)))
#define IPS200_RS(x)        	(x? (gpio_high(IPS200_RS_PIN)): (gpio_low(IPS200_RS_PIN)))
#define IPS200_RST(x)  			(x? (gpio_high(IPS200_RST_PIN)): (gpio_low(IPS200_RST_PIN)))
#define IPS200_CS(x)			(x? (gpio_high(IPS200_CS_PIN)): (gpio_low(IPS200_CS_PIN)))
#define IPS200_BL(x) 			(x? (gpio_high(IPS200_BL_PIN)): (gpio_low(IPS200_BL_PIN)))



//8���������ű������� ����B0-B7,B6-B13�ȵȡ�
//--------------���ݶ˿ڼĴ���--------------
#define IPS200_DATAPORT     	GPIOE

//--------------���ݶ˿���ʼ��ַƫ��--------------
#define DATA_START_NUM			0

//����D1-D8		IPS200_DATAPORT����ΪGPIOD   DATA_START_NUM����Ϊ1
//����C5-C12	    IPS200_DATAPORT����ΪGPIOC   DATA_START_NUM����Ϊ5


typedef enum
{
    IPS200_PORTAIT                      = 0,                                    // ����ģʽ
    IPS200_PORTAIT_180                  = 1,                                    // ����ģʽ  ��ת180
    IPS200_CROSSWISE                    = 2,                                    // ����ģʽ
    IPS200_CROSSWISE_180                = 3,                                    // ����ģʽ  ��ת180
}ips200_dir_enum;

#define IPS200_DEFAULT_DISPLAY_DIR      IPS200_PORTAIT                          // Ĭ�ϵ���ʾ����
#define IPS200_DEFAULT_PENCOLOR         RED                                     // Ĭ�ϵĻ�����ɫ
#define IPS200_DEFAULT_BGCOLOR          WHITE                                   // Ĭ�ϵı�����ɫ

extern  uint16_t ips200_pencolor;
extern  uint16_t ips200_bgcolor;

extern  ips200_dir_enum ips200_display_dir;
extern  uint16_t ips200_x_max;
extern  uint16_t ips200_y_max;

void    ips200_init                     (void);
void    ips200_clear                    (uint16_t color);
void    ips200_set_dir                  (ips200_dir_enum dir);
void    ips200_set_color                (uint16_t pen, uint16_t bgcolor);
void    ips200_draw_point               (uint16_t x,uint16_t y,uint16_t color);

void    ips200_show_char                (uint16_t x,uint16_t y,const char dat);
void    ips200_show_string              (uint16_t x,uint16_t y,const char dat[]);
void    ips200_show_int8                (uint16_t x,uint16_t y,int8_t dat);
void    ips200_show_uint8               (uint16_t x,uint16_t y,uint8_t dat);
void    ips200_show_int16               (uint16_t x,uint16_t y,int16_t dat);
void    ips200_show_uint16              (uint16_t x,uint16_t y,uint16_t dat);
void    ips200_show_int32               (uint16_t x,uint16_t y,int32_t dat,uint8_t num);
void    ips200_show_float               (uint16_t x,uint16_t y,double dat,uint8_t num,uint8_t pointnum);

void    ips200_show_binary_image        (uint16_t x, uint16_t y, uint8_t *p, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height);
void    ips200_show_gray_image          (uint16_t x, uint16_t y, uint8_t *p, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height, uint8_t threshold);
void    ips200_show_rgb565_image        (uint16_t x, uint16_t y, uint16_t *p, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height);

void    ips200_show_wave                (uint16_t x, uint16_t y, uint8_t *p, uint16_t width, uint16_t value_max, uint16_t dis_width, uint16_t dis_value_max);
void    ips200_show_chinese             (uint16_t x, uint16_t y, uint8_t size, const uint8_t *p, uint8_t number, uint16_t color);



#define ips200_displayimage7725(p,width,height)                                 ips200_show_binary_image(0,0,p,width,height,width,height)

#define ips200_displayimage032(p,width,height)                                  ips200_show_gray_image(0,0,p,width,height,width,height,0)
#define ips200_displayimage032_zoom(p,width,height,dis_width,dis_height)        ips200_show_gray_image(0,0,p,width,height,dis_width,dis_height,0)
#define ips200_displayimage032_zoom1(p,width,height,x,y,dis_width,dis_height)   ips200_show_gray_image(x,y,p,width,height,dis_width,dis_height,0)

#define ips200_displayimage8660(p,width,height)                                 ips200_show_rgb565_image(0,0,p,width,height,width,height)
#define ips200_displayimage8660_zoom(p,width,height,dis_width,dis_height)       ips200_show_rgb565_image(0,0,p,width,height,dis_width,dis_height)
#define ips200_displayimage8660_zoom1(p,width,height,x,y,dis_width,dis_height)  ips200_show_rgb565_image(x,y,p,width,height,dis_width,dis_height)

#endif

