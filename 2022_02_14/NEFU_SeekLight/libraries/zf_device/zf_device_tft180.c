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



#include "zf_device_tft180.h"

uint16_t tft180_pencolor = TFT180_DEFAULT_PENCOLOR;
uint16_t tft180_bgcolor = TFT180_DEFAULT_BGCOLOR;

tft180_dir_enum tft180_display_dir = TFT180_DEFAULT_DISPLAY_DIR;
uint8_t tft180_x_max = 240;
uint8_t tft180_y_max = 135;

#if TFT180_USE_SOFT_SPI
static soft_spi_info_struct             tft180_spi;
#define tft180_write_8bit_data(data)    soft_spi_write_8bit(&TFT180_spi, data)
#define tft180_write_16bit_data(data)   soft_spi_write_16bit(&TFT180_spi, data)
#else
#define tft180_write_8bit_data(data)    spi_write_8bit(TFT180_SPI, data)
#define tft180_write_16bit_data(data)   spi_write_16bit(TFT180_SPI, data)
#endif

//-------------------------------------------------------------------------------------------------------------------
// @brief       д���� �ڲ�����
// @note        �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void tft180_write_index (uint8_t dat)
{
    TFT180_CS(1);
    TFT180_CS(0);
    TFT180_DC(0);
    tft180_write_8bit_data(dat);
    TFT180_CS(1);
    TFT180_DC(1);
    TFT180_CS(0);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ������ʾ���� �ڲ�����
// @param       x1              ��ʼx������
// @param       y1              ��ʼy������
// @param       x2              ����x������
// @param       y2              ����y������
// @return      void
// @note        �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void tft180_set_region (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    zf_assert(x1 < tft180_x_max);
    zf_assert(y1 < tft180_y_max);
    zf_assert(x2 < tft180_x_max);
    zf_assert(y2 < tft180_y_max);

    if(tft180_display_dir==TFT180_PORTAIT || tft180_display_dir==TFT180_PORTAIT_180)
    {
        tft180_write_index(0x2a);
        tft180_write_8bit_data(0x00);
        tft180_write_8bit_data(x1+2);
        tft180_write_8bit_data(0x00);
        tft180_write_8bit_data(x2+2);

        tft180_write_index(0x2b);
        tft180_write_8bit_data(0x00);
        tft180_write_8bit_data(y1+1);
        tft180_write_8bit_data(0x00);
        tft180_write_8bit_data(y2+1);
    }
    else
    {
        tft180_write_index(0x2a);
        tft180_write_8bit_data(0x00);
        tft180_write_8bit_data(x1+1);
        tft180_write_8bit_data(0x0);
        tft180_write_8bit_data(x2+1);

        tft180_write_index(0x2b);
        tft180_write_8bit_data(0x00);
        tft180_write_8bit_data(y1+2);
        tft180_write_8bit_data(0x00);
        tft180_write_8bit_data(y2+2);
    }
    tft180_write_index(0x2c);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       1.14�� IPSҺ����ʼ��
// @return      void
// Sample usage:                tft180_init();
//-------------------------------------------------------------------------------------------------------------------
void tft180_init (void)
{
#if TFT180_USE_SOFT_SPI
    soft_spi_init(&tft180_spi, 0, TFT180_SOFT_SPI_DELAY, TFT180_SCL_PIN, TFT180_SDA_PIN, SOFT_SPI_PIN_NULL, SOFT_SPI_PIN_NULL);
#else
    spi_init(TFT180_SPI, SPI_MODE0, TFT180_SPI_SPEED, TFT180_SCL_PIN, TFT180_SDA_PIN, SPI_MISO_NULL, SPI_CS_NULL);
#endif

    gpio_init(TFT180_DC_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(TFT180_RES_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(TFT180_CS_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(TFT180_BL_PIN, GPO, GPIO_HIGH, GPO_PUSH_PULL);

    tft180_set_dir(tft180_display_dir);
    tft180_set_color(tft180_pencolor, tft180_bgcolor);

    TFT180_RES(0);
    system_delay_ms(10);

    TFT180_RES(1);
    system_delay_ms(120);

    tft180_write_index(0x11);
    system_delay_ms(120);

    tft180_write_index(0xB1);
    tft180_write_8bit_data(0x01);
    tft180_write_8bit_data(0x2C);
    tft180_write_8bit_data(0x2D);

    tft180_write_index(0xB2);
    tft180_write_8bit_data(0x01);
    tft180_write_8bit_data(0x2C);
    tft180_write_8bit_data(0x2D);

    tft180_write_index(0xB3);
    tft180_write_8bit_data(0x01);
    tft180_write_8bit_data(0x2C);
    tft180_write_8bit_data(0x2D);
    tft180_write_8bit_data(0x01);
    tft180_write_8bit_data(0x2C);
    tft180_write_8bit_data(0x2D);

    tft180_write_index(0xB4);
    tft180_write_8bit_data(0x07);

    tft180_write_index(0xC0);
    tft180_write_8bit_data(0xA2);
    tft180_write_8bit_data(0x02);
    tft180_write_8bit_data(0x84);
    tft180_write_index(0xC1);
    tft180_write_8bit_data(0xC5);

    tft180_write_index(0xC2);
    tft180_write_8bit_data(0x0A);
    tft180_write_8bit_data(0x00);

    tft180_write_index(0xC3);
    tft180_write_8bit_data(0x8A);
    tft180_write_8bit_data(0x2A);
    tft180_write_index(0xC4);
    tft180_write_8bit_data(0x8A);
    tft180_write_8bit_data(0xEE);

    tft180_write_index(0xC5);
    tft180_write_8bit_data(0x0E);

    tft180_write_index(0x36);
    switch(tft180_display_dir)                                                         // y x v
    {
        case 0: tft180_write_8bit_data(1<<7 | 1<<6 | 0<<5);  break;                          // ����ģʽ
        case 1: tft180_write_8bit_data(0<<7 | 0<<6 | 0<<5);  break;                          // ����ģʽ  ��ת180
        case 2: tft180_write_8bit_data(1<<7 | 0<<6 | 1<<5);  break;                          // ����ģʽ
        case 3: tft180_write_8bit_data(0<<7 | 1<<6 | 1<<5);  break;                          // ����ģʽ  ��ת180
    }

    tft180_write_index(0xe0);
    tft180_write_8bit_data(0x0f);
    tft180_write_8bit_data(0x1a);
    tft180_write_8bit_data(0x0f);
    tft180_write_8bit_data(0x18);
    tft180_write_8bit_data(0x2f);
    tft180_write_8bit_data(0x28);
    tft180_write_8bit_data(0x20);
    tft180_write_8bit_data(0x22);
    tft180_write_8bit_data(0x1f);
    tft180_write_8bit_data(0x1b);
    tft180_write_8bit_data(0x23);
    tft180_write_8bit_data(0x37);
    tft180_write_8bit_data(0x00);
    tft180_write_8bit_data(0x07);
    tft180_write_8bit_data(0x02);
    tft180_write_8bit_data(0x10);

    tft180_write_index(0xe1);
    tft180_write_8bit_data(0x0f);
    tft180_write_8bit_data(0x1b);
    tft180_write_8bit_data(0x0f);
    tft180_write_8bit_data(0x17);
    tft180_write_8bit_data(0x33);
    tft180_write_8bit_data(0x2c);
    tft180_write_8bit_data(0x29);
    tft180_write_8bit_data(0x2e);
    tft180_write_8bit_data(0x30);
    tft180_write_8bit_data(0x30);
    tft180_write_8bit_data(0x39);
    tft180_write_8bit_data(0x3f);
    tft180_write_8bit_data(0x00);
    tft180_write_8bit_data(0x07);
    tft180_write_8bit_data(0x03);
    tft180_write_8bit_data(0x10);

    tft180_write_index(0x2a);
    tft180_write_8bit_data(0x00);
    tft180_write_8bit_data(0x00+2);
    tft180_write_8bit_data(0x00);
    tft180_write_8bit_data(0x80+2);

    tft180_write_index(0x2b);
    tft180_write_8bit_data(0x00);
    tft180_write_8bit_data(0x00+3);
    tft180_write_8bit_data(0x00);
    tft180_write_8bit_data(0x80+3);

    tft180_write_index(0xF0);
    tft180_write_8bit_data(0x01);
    tft180_write_index(0xF6);
    tft180_write_8bit_data(0x00);

    tft180_write_index(0x3A);
    tft180_write_8bit_data(0x05);
    tft180_write_index(0x29);

    tft180_clear(tft180_bgcolor);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����������
// @param       color           ��ɫ��ʽ RGB565 ���߿���ʹ�� zf_common_font.h �ڳ�����ɫ�궨��
// @return      void
// Sample usage:                tft180_clear(YELLOW);
//-------------------------------------------------------------------------------------------------------------------
void tft180_clear (uint16_t color)
{
    uint32_t i = tft180_x_max*tft180_y_max;

    tft180_set_region(0,0,tft180_x_max-1,tft180_y_max-1);
    for( ; i>0; i--)
    {
        tft180_write_16bit_data(color);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ������ʾ���� �������ֻ���ڳ�ʼ����Ļ֮ǰ���ò���Ч
// @param       dir             ��ʾ����  ���� zf_device_ips114.h �� tft180_dir_enum ö���嶨��
// @return      void
// Sample usage:                tft180_set_dir(TFT180_CROSSWISE);
//-------------------------------------------------------------------------------------------------------------------
void tft180_set_dir (tft180_dir_enum dir)
{
    tft180_display_dir = dir;
    if(dir < 2)
    {
        tft180_x_max = 128;
        tft180_y_max = 160;
    }
    else
    {
        tft180_x_max = 160;
        tft180_y_max = 128;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ������ʾ��ɫ
// @param       pen             ��ɫ��ʽ RGB565 ���߿���ʹ�� zf_common_font.h �ڳ�����ɫ�궨��
// @param       bgcolor         ��ɫ��ʽ RGB565 ���߿���ʹ�� zf_common_font.h �ڳ�����ɫ�궨��
// @return      void
// Sample usage:                tft180_set_color(WHITE,BLACK);
//-------------------------------------------------------------------------------------------------------------------
void tft180_set_color (uint16_t pen, uint16_t bgcolor)
{
    tft180_pencolor = pen;
    tft180_bgcolor = bgcolor;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ������
// @param       x               ����x�������� ������Χ [0, tft180_x_max-1]
// @param       y               ����y�������� ������Χ [0, tft180_y_max-1]
// @param       dat             ��Ҫ��ʾ����ɫ
// @return      void
// Sample usage:                tft180_draw_point(0,0,RED);                     // ���� 0,0 ��һ����ɫ�ĵ�
//-------------------------------------------------------------------------------------------------------------------
void tft180_draw_point (uint16_t x,uint16_t y,uint16_t color)
{
    zf_assert(x < tft180_x_max);
    zf_assert(y < tft180_y_max);

    tft180_set_region(x,y,x,y);
    tft180_write_16bit_data(color);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ�ַ�
// @param       x               ����x�������� ������Χ [0, tft180_x_max-1]
// @param       y               ����y�������� ������Χ [0, tft180_y_max-1]
// @param       dat             ��Ҫ��ʾ���ַ�
// @return      void
// Sample usage:                tft180_show_char(0,0,'x');                      // ���� 0,0 дһ���ַ� x
//-------------------------------------------------------------------------------------------------------------------
void tft180_show_char (uint16_t x,uint16_t y,const char dat)
{
    zf_assert(x < tft180_x_max);
    zf_assert(y < tft180_y_max);

    uint8_t i,j;
    uint8_t temp;

    for(i=0; i<16; i++)
    {
        tft180_set_region(x,y+i,x+7,y+i);
        temp = tft_ascii[dat-32][i];                                            //�� 32 ��Ϊ��ȡģ�Ǵӿո�ʼȡ�� �ո��� ascii ������� 32
        for(j=0; j<8; j++)
        {
            if(temp&0x01)   
                tft180_write_16bit_data(tft180_pencolor);
            else
                tft180_write_16bit_data(tft180_bgcolor);
            temp>>=1;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ�ַ���
// @param       x               ����x�������� ������Χ [0, tft180_x_max-1]
// @param       y               ����y�������� ������Χ [0, tft180_y_max-1]
// @param       dat             ��Ҫ��ʾ���ַ���
// @return      void
// Sample usage:                tft180_show_string(0,0,"seekfree");
//-------------------------------------------------------------------------------------------------------------------
void tft180_show_string (uint16_t x,uint16_t y,const char dat[])
{
    zf_assert(x < tft180_x_max);
    zf_assert(y < tft180_y_max);
    
    uint16_t j = 0;
    while(dat[j] != '\0')
    {
        tft180_show_char(x+8*j,y,dat[j]);
        j++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ8λ�з���
// @param       x               ����x�������� ������Χ [0, tft180_x_max-1]
// @param       y               ����y�������� ������Χ [0, tft180_y_max-1]
//  @param      dat             ��Ҫ��ʾ�ı�������������int8_t
//  @return     void
//  Sample usage:               tft180_show_int8(0,0,x);                        // xΪ int8 ����
//-------------------------------------------------------------------------------------------------------------------
void tft180_show_int8(uint16_t x,uint16_t y,int8_t dat)
{
    zf_assert(x < tft180_x_max);
    zf_assert(y < tft180_y_max);

    uint8_t a[3];
    uint8_t i;
    if(dat<0)
    {
        tft180_show_char(x,y,'-');
        dat = -dat;
    }
    else
        tft180_show_char(x,y,' ');
    
    a[0] = dat/100;
    a[1] = dat/10%10;
    a[2] = dat%10;
    i = 0;
    while(i<3)
    {
        tft180_show_char(x+(8*(i+1)),y,'0' + a[i]);
        i++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ8λ�޷���
// @param       x               ����x�������� ������Χ [0, tft180_x_max-1]
// @param       y               ����y�������� ������Χ [0, tft180_y_max-1]
// @param       dat             ��Ҫ��ʾ�ı��� ��������uint8_t
// @return      void
// Sample usage:                tft180_show_uint8(0,0,x);                       // x Ϊ uint8_t ����
//-------------------------------------------------------------------------------------------------------------------
void tft180_show_uint8(uint16_t x,uint16_t y,uint8_t dat)
{
    zf_assert(x < tft180_x_max);
    zf_assert(y < tft180_y_max);

    uint8_t a[3];
    uint8_t i;
    
    a[0] = dat/100;
    a[1] = dat/10%10;
    a[2] = dat%10;
    i = 0;
    while(i<3)
    {
        tft180_show_char(x+(8*i),y,'0' + a[i]);
        i++;
    }
    
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ16λ�з���
// @param       x               ����x�������� ������Χ [0, tft180_x_max-1]
// @param       y               ����y�������� ������Χ [0, tft180_y_max-1]
// @param       dat             ��Ҫ��ʾ�ı��� ��������int16_t
// @return      void
// Sample usage:                tft180_show_int16(0,0,x);                       // x Ϊ int16_t ����
//-------------------------------------------------------------------------------------------------------------------
void tft180_show_int16(uint16_t x,uint16_t y,int16_t dat)
{
    zf_assert(x < tft180_x_max);
    zf_assert(y < tft180_y_max);

    uint8_t a[5];
    uint8_t i;

    if(dat<0)
    {
        tft180_show_char(x,y,'-');
        dat = -dat;
    }
    else    tft180_show_char(x,y,' ');

    a[0] = dat/10000;
    a[1] = dat/1000%10;
    a[2] = dat/100%10;
    a[3] = dat/10%10;
    a[4] = dat%10;
    
    i = 0;
    while(i<5)
    {
        tft180_show_char(x+(8*(i+1)),y,'0' + a[i]);
        i++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ16λ�޷���
// @param       x               ����x�������� ������Χ [0, tft180_x_max-1]
// @param       y               ����y�������� ������Χ [0, tft180_y_max-1]
// @param       dat             ��Ҫ��ʾ�ı�������������uint16_t
// @return      void
// Sample usage:                tft180_show_uint16(0,0,x);                      // x Ϊ uint16_t ����
//-------------------------------------------------------------------------------------------------------------------
void tft180_show_uint16(uint16_t x,uint16_t y,uint16_t dat)
{
    zf_assert(x < tft180_x_max);
    zf_assert(y < tft180_y_max);

    uint8_t a[5];
    uint8_t i;

    a[0] = dat/10000;
    a[1] = dat/1000%10;
    a[2] = dat/100%10;
    a[3] = dat/10%10;
    a[4] = dat%10;
    
    i = 0;
    while(i<5)
    {
        tft180_show_char(x+(8*i),y,'0' + a[i]);
        i++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ32λ�з���(ȥ������������Ч��0)
// @param       x               ����x�������� ������Χ [0, tft180_x_max-1]
// @param       y               ����y�������� ������Χ [0, tft180_y_max-1]
// @param       dat             ��Ҫ��ʾ�ı�������������uint32_t
// @param       num             ��Ҫ��ʾ��λ�� ���10λ  ������������
// @return      void
// Sample usage:                tft180_showuint32(0,0,x,3);                     // x ����Ϊ int32_t uint16_t int16_t uint8_t int8_t ����
// note:                        ��������ʾһ�� ��-����   ������ʾһ���ո�
//-------------------------------------------------------------------------------------------------------------------
void tft180_show_int32(uint16_t x,uint16_t y,int32_t dat,uint8_t num)
{
    zf_assert(x < tft180_x_max);
    zf_assert(y < tft180_y_max);

    int8_t    buff[34];
    uint8_t   length;
    
    if(10<num)      num = 10;
    
    num++;
    if(0>dat)   length = zf_sprintf( &buff[0],(const int8_t *)"%d",dat);        // ����
    else
    {
        buff[0] = ' ';
        length = zf_sprintf( &buff[1],(const int8_t *)"%d",dat);
        length++;
    }
    while(length < num)
    {
        buff[length] = ' ';
        length++;
    }
    buff[num] = '\0';

    tft180_show_string(x, y, (const char *)buff);                               // ��ʾ����
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ������(ȥ������������Ч��0)
// @param       x               ����x�������� ������Χ [0, tft180_x_max-1]
// @param       y               ����y�������� ������Χ [0, tft180_y_max-1]
// @param       dat             ��Ҫ��ʾ�ı�������������float��double
// @param       num             ����λ��ʾ����   ���10λ
// @param       pointnum        С��λ��ʾ����   ���6λ
// @return      void
// Sample usage:                tft180_showfloat(0,0,x,2,3);                    // ��ʾ������   ������ʾ2λ   С����ʾ��λ
// @note                        �ر�ע�⵱����С��������ʾ��ֵ����д���ֵ��һ����ʱ��
//                              ���������ڸ��������ȶ�ʧ���⵼�µģ��Ⲣ������ʾ���������⣬
//                              �й���������飬�����аٶ�ѧϰ   ���������ȶ�ʧ���⡣
//                              ��������ʾһ�� ��-����   ������ʾһ���ո�
//-------------------------------------------------------------------------------------------------------------------
void tft180_show_float(uint16_t x,uint16_t y,double dat,uint8_t num,uint8_t pointnum)
{
    zf_assert(x < tft180_x_max);
    zf_assert(y < tft180_y_max);

    uint8_t   length;
    int8_t    buff[34];
    int8_t    start,end,point;

    if(6<pointnum)  pointnum = 6;
    if(10<num)      num = 10;
        
    if(0>dat)   length = zf_sprintf( &buff[0],(const int8_t *)"%f",dat);        // ����
    else
    {
        length = zf_sprintf( &buff[1],(const int8_t *)"%f",dat);
        length++;
    }
    point = length - 7;                                                         // ����С����λ��
    start = point - num - 1;                                                    // ������ʼλ
    end = point + pointnum + 1;                                                 // �������λ
    while(0>start)                                                              // ����λ����  ĩβӦ�����ո�
    {
        buff[end] = ' ';
        end++;
        start++;
    }
    
    if(0>dat)   buff[start] = '-';
    else        buff[start] = ' ';
    
    buff[end] = '\0';

    tft180_show_string(x, y, (const char *)buff);                               // ��ʾ����
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       TFT180 ��ʾ��ֵͼ�� ����ÿ�˸������һ���ֽ�����
// @param       x               ����x�������� ������Χ [0, tft180_x_max-1]
// @param       y               ����y�������� ������Χ [0, tft180_y_max-1]
// @param       *p              ͼ������ָ��
// @param       width           ͼ��ʵ�ʿ��
// @param       height          ͼ��ʵ�ʸ߶�
// @param       dis_width       ͼ����ʾ��� ������Χ [0, tft180_x_max]
// @param       dis_height      ͼ����ʾ�߶� ������Χ [0, tft180_y_max]
// @return      void
// Sample usage:                tft180_show_binary_image(0,0,ov7725_image_binary[0],OV7725_W,OV7725_H,OV7725_W,OV7725_H);
//-------------------------------------------------------------------------------------------------------------------
void tft180_show_binary_image (uint16_t x, uint16_t y, uint8_t *p, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height)
{
    zf_assert(x < tft180_x_max);
    zf_assert(y < tft180_y_max);

    uint32_t i = 0, j = 0;
    uint8_t temp = 0;
    uint32_t width_index = 0, height_index = 0;

    tft180_set_region(x, y, x+dis_width-1, y+dis_height-1);                     // ������ʾ����

    for(j=0;j<dis_height;j++)
    {
        height_index = j*height/dis_height;
        for(i=0;i<dis_width;i++)
        {
            width_index = i*width/dis_width;
            temp = *(p+height_index*width/8+width_index/8);                     // ��ȡ���ص�
            if(0x80 & (temp << (width_index%8)))
                tft180_write_16bit_data(WHITE);
            else
                tft180_write_16bit_data(BLACK);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       TFT180 ��ʾ 8bit �Ҷ�ͼ�� ����ֵ����ֵ
// @param       x               ����x�������� ������Χ [0, tft180_x_max-1]
// @param       y               ����y�������� ������Χ [0, tft180_y_max-1]
// @param       *p              ͼ������ָ��
// @param       width           ͼ��ʵ�ʿ��
// @param       height          ͼ��ʵ�ʸ߶�
// @param       dis_width       ͼ����ʾ��� ������Χ [0, tft180_x_max]
// @param       dis_height      ͼ����ʾ�߶� ������Χ [0, tft180_y_max]
// @param       threshold       ��ֵ����ʾ��ֵ 0-��������ֵ��
// @return      void
// Sample usage:                tft180_show_gray_image(0,0,camera_buffer_addr,MT9V03X_W,MT9V03X_H,MT9V03X_W,MT9V03X_H,0);
//-------------------------------------------------------------------------------------------------------------------
void tft180_show_gray_image (uint16_t x, uint16_t y, uint8_t *p, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height, uint8_t threshold)
{
    zf_assert(x < tft180_x_max);
    zf_assert(y < tft180_y_max);

    uint32_t i = 0, j = 0;
    uint16_t color = 0,temp = 0;
    uint32_t width_index = 0, height_index = 0;

    tft180_set_region(x, y, x+dis_width-1, y+dis_height-1);                     // ������ʾ����

    for(j=0;j<dis_height;j++)
    {
        height_index = j*height/dis_height;
        for(i=0;i<dis_width;i++)
        {
            width_index = i*width/dis_width;
            temp = *(p+height_index*width+width_index);                         // ��ȡ���ص�
            if(threshold == 0)
            {
                color=(0x001f&((temp)>>3))<<11;
                color=color|(((0x003f)&((temp)>>2))<<5);
                color=color|(0x001f&((temp)>>3));
                tft180_write_16bit_data(color);
            }
            else if(temp < threshold)
                tft180_write_16bit_data(BLACK);
            else
                tft180_write_16bit_data(WHITE);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       TFT180 ��ʾ RGB565 ��ɫͼ��
// @param       x               ����x�������� ������Χ [0, tft180_x_max-1]
// @param       y               ����y�������� ������Χ [0, tft180_y_max-1]
// @param       *p              ͼ������ָ��
// @param       width           ͼ��ʵ�ʿ��
// @param       height          ͼ��ʵ�ʸ߶�
// @param       dis_width       ͼ����ʾ��� ������Χ [0, tft180_x_max]
// @param       dis_height      ͼ����ʾ�߶� ������Χ [0, tft180_y_max]
// @param       threshold       ��ֵ����ʾ��ֵ 0-��������ֵ��
// @return      void
// Sample usage:                tft180_show_gray_image(0,0,camera_buffer_addr,SCCB8660_W,SCCB8660_H,SCCB8660_W,SCCB8660_H);
//-------------------------------------------------------------------------------------------------------------------
void tft180_show_rgb565_image (uint16_t x, uint16_t y, uint16_t *p, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height)
{
    zf_assert(x < tft180_x_max);
    zf_assert(y < tft180_y_max);

    uint32_t i = 0, j = 0;
    uint16_t color = 0;
    uint32_t width_index = 0, height_index = 0;

    tft180_set_region(x, y, x+dis_width-1, y+dis_height-1);                     // ������ʾ����

    for(j=0;j<dis_height;j++)
    {
        height_index = j*height/dis_height;
        for(i=0;i<dis_width;i++)
        {
            width_index = i*width/dis_width;
            color = *(p+height_index*width+width_index);                        // ��ȡ���ص�
            color = ((color&0xff)<<8) | (color>>8);
            tft180_write_16bit_data(color);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       TFT180 ��ʾ����2
// @param       x               ����x�������� ������Χ [0, tft180_x_max-1]
// @param       y               ����y�������� ������Χ [0, tft180_y_max-1]
// @param       *p              ��������ָ��
// @param       width           ����ʵ�ʿ��
// @param       value_max       ����ʵ�����ֵ
// @param       dis_width       ������ʾ��� ������Χ [0, tft180_x_max]
// @param       dis_value_max   ������ʾ���ֵ ������Χ [0, tft180_y_max]
// @return      void
// Sample usage:                tft180_show_gray_image(0,0,camera_buffer_addr,MT9V03X_W,MT9V03X_H,240,135,128);
//-------------------------------------------------------------------------------------------------------------------
void tft180_show_wave(uint16_t x, uint16_t y, uint8_t *p, uint16_t width, uint16_t value_max, uint16_t dis_width, uint16_t dis_value_max)
{
    zf_assert(x < tft180_x_max);
    zf_assert(y < tft180_y_max);

    uint32_t i = 0, j = 0;
    uint32_t width_index = 0, value_max_index = 0;

    tft180_set_region(x, y, x+dis_width-1, y+dis_value_max-1);                  // ������ʾ����
    for(i=0;i<dis_value_max;i++)
    {
        for(j=0;j<dis_width;j++)
        {
            tft180_write_16bit_data(tft180_bgcolor); 
        }
    }

    for(i=0;i<dis_width;i++)
    {
        width_index = i*width/dis_width;
        value_max_index = *(p+width_index)*(dis_value_max-1)/value_max;
        tft180_draw_point(i+x, (dis_value_max-1)-value_max_index+y, tft180_pencolor);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ������ʾ
// @param       x               ����x�������� ������Χ [0, tft180_x_max-1]
// @param       y               ����y�������� ������Χ [0, tft180_y_max-1]
// @param       size            ȡģ��ʱ�����õĺ��������С Ҳ����һ������ռ�õĵ��󳤿�Ϊ���ٸ��� ȡģ��ʱ����Ҫ������һ����
// @param       *p              ��Ҫ��ʾ�ĺ�������
// @param       number          ��Ҫ��ʾ����λ
// @param       color           ��ʾ��ɫ
// @return      void
// Sample usage:                tft180_show_chinese(0,0,16,chinese_test[0],4,RED);//��ʾfont�ļ������ ʾ��
// @Note                        ʹ��PCtoLCD2002���ȡģ           ���롢����ʽ��˳��   16*16
//-------------------------------------------------------------------------------------------------------------------
void tft180_show_chinese (uint16_t x, uint16_t y, uint8_t size, const uint8_t *p, uint8_t number, uint16_t color)
{
    zf_assert(x < tft180_x_max);
    zf_assert(y < tft180_y_max);

    int i, j, k; 
    uint8_t temp, temp1, temp2;
    const uint8_t *p_data;
    
    temp2 = size/8;
    
    tft180_set_region(x,y,number*size-1+x,y+size-1);
    
    for(i=0;i<size;i++)
    {
        temp1 = number;
        p_data = p+i*temp2;
        while(temp1--)
        {
            for(k=0;k<temp2;k++)
            {
                for(j=8;j>0;j--)
                {
                    temp = (*p_data>>(j-1)) & 0x01;
                    if(temp)    tft180_write_16bit_data(color);
                    else        tft180_write_16bit_data(tft180_bgcolor);
                }
                p_data++;
            }
            p_data = p_data - temp2 + temp2*size;
        }   
    }
}


