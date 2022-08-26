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


#include "zf_device_ips114.h"

uint16_t ips114_pencolor = IPS114_DEFAULT_PENCOLOR;
uint16_t ips114_bgcolor = IPS114_DEFAULT_BGCOLOR;

ips114_dir_enum ips114_display_dir = IPS114_DEFAULT_DISPLAY_DIR;
uint8_t ips114_x_max = 240;
uint8_t ips114_y_max = 135;

#if IPS114_USE_SOFT_SPI
static soft_spi_info_struct             ips114_spi;
#define ips114_write_8bit_data(data)    soft_spi_write_8bit(&ips114_spi, data)
#define ips114_write_16bit_data(data)   soft_spi_write_16bit(&ips114_spi, data)
#else
#define ips114_write_8bit_data(data)    spi_write_8bit(IPS114_SPI, data)
#define ips114_write_16bit_data(data)   spi_write_16bit(IPS114_SPI, data)


//#define ips114_write_8bit_data(data)    (IPS114_CS(0),IPS114_DC(1),spi_write_8bit(IPS114_SPI, data),IPS114_CS(1))
//#define ips114_write_16bit_data(data)   (IPS114_CS(0),IPS114_DC(1),spi_write_16bit(IPS114_SPI, data),IPS114_CS(1))
#endif

//-------------------------------------------------------------------------------------------------------------------
// @brief       д���� �ڲ�����
// @note        �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void ips114_write_index (uint8_t dat)
{
    IPS114_CS(1);
    IPS114_CS(0);
    IPS114_DC(0);
    ips114_write_8bit_data(dat);
    IPS114_DC(1);
    IPS114_CS(1);
    IPS114_CS(0);
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
static void ips114_set_region (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    zf_assert(x1 < ips114_x_max);
    zf_assert(y1 < ips114_y_max);
    zf_assert(x2 < ips114_x_max);
    zf_assert(y2 < ips114_y_max);

    if(ips114_display_dir==IPS114_PORTAIT)
    {
        ips114_write_index(0x2a);                                               // �е�ַ����
        ips114_write_16bit_data(x1+52);
        ips114_write_16bit_data(x2+52);
        ips114_write_index(0x2b);                                               // �е�ַ����
        ips114_write_16bit_data(y1+40);
        ips114_write_16bit_data(y2+40);
        ips114_write_index(0x2c);                                               // ������д
    }
    else if(ips114_display_dir==IPS114_PORTAIT_180)
    {
        ips114_write_index(0x2a);                                               // �е�ַ����
        ips114_write_16bit_data(x1+53);
        ips114_write_16bit_data(x2+53);
        ips114_write_index(0x2b);                                               // �е�ַ����
        ips114_write_16bit_data(y1+40);
        ips114_write_16bit_data(y2+40);
        ips114_write_index(0x2c);                                               // ������д
    }
    else if(ips114_display_dir==IPS114_CROSSWISE)
    {
        ips114_write_index(0x2a);                                               // �е�ַ����
        ips114_write_16bit_data(x1+40);
        ips114_write_16bit_data(x2+40);
        ips114_write_index(0x2b);                                               // �е�ַ����
        ips114_write_16bit_data(y1+53);
        ips114_write_16bit_data(y2+53);
        ips114_write_index(0x2c);                                               // ������д
    }
    else
    {
        ips114_write_index(0x2a);                                               // �е�ַ����
        ips114_write_16bit_data(x1+40);
        ips114_write_16bit_data(x2+40);
        ips114_write_index(0x2b);                                               // �е�ַ����
        ips114_write_16bit_data(y1+52);
        ips114_write_16bit_data(y2+52);
        ips114_write_index(0x2c);                                               // ������д
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       1.14�� IPSҺ����ʼ��
// @return      void
// Sample usage:                ips114_init();
//-------------------------------------------------------------------------------------------------------------------
void ips114_init (void)
{
#if IPS114_USE_SOFT_SPI
    soft_spi_init(&ips114_spi, 0, IPS114_SOFT_SPI_DELAY, IPS114_SCL_PIN, IPS114_SDA_PIN, SOFT_SPI_PIN_NULL, SOFT_SPI_PIN_NULL);
#else
    zf_assert(IPS114_SPI != (IPS114_SCL_PIN&0xF000>>12));
    zf_assert(IPS114_SPI != (IPS114_SDA_PIN&0xF000>>12));
    spi_init(IPS114_SPI, SPI_MODE0, IPS114_SPI_SPEED, IPS114_SCL_PIN, IPS114_SDA_PIN, SPI_MISO_NULL, SPI_CS_NULL);
#endif

    gpio_init(IPS114_DC_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(IPS114_RST_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(IPS114_CS_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(IPS114_BLK_PIN, GPO, GPIO_HIGH, GPO_PUSH_PULL);

    ips114_set_dir(ips114_display_dir);
    ips114_set_color(ips114_pencolor, ips114_bgcolor);

    IPS114_RST(0);
    system_delay_ms(200);

    IPS114_RST(1);
    system_delay_ms(100);

    ips114_write_index(0x36);
    system_delay_ms(100);
    if      (ips114_display_dir==0) ips114_write_8bit_data(0x00);
    else if (ips114_display_dir==1) ips114_write_8bit_data(0xC0);
    else if (ips114_display_dir==2) ips114_write_8bit_data(0x70);
    else                            ips114_write_8bit_data(0xA0);

    ips114_write_index(0x3A);
    ips114_write_8bit_data(0x05);

    ips114_write_index(0xB2);
    ips114_write_8bit_data(0x0C);
    ips114_write_8bit_data(0x0C);
    ips114_write_8bit_data(0x00);
    ips114_write_8bit_data(0x33);
    ips114_write_8bit_data(0x33);

    ips114_write_index(0xB7);
    ips114_write_8bit_data(0x35);

    ips114_write_index(0xBB);
    ips114_write_8bit_data(0x37);

    ips114_write_index(0xC0);
    ips114_write_8bit_data(0x2C);

    ips114_write_index(0xC2);
    ips114_write_8bit_data(0x01);

    ips114_write_index(0xC3);
    ips114_write_8bit_data(0x12);

    ips114_write_index(0xC4);
    ips114_write_8bit_data(0x20);

    ips114_write_index(0xC6);
    ips114_write_8bit_data(0x0F);

    ips114_write_index(0xD0);
    ips114_write_8bit_data(0xA4);
    ips114_write_8bit_data(0xA1);

    ips114_write_index(0xE0);
    ips114_write_8bit_data(0xD0);
    ips114_write_8bit_data(0x04);
    ips114_write_8bit_data(0x0D);
    ips114_write_8bit_data(0x11);
    ips114_write_8bit_data(0x13);
    ips114_write_8bit_data(0x2B);
    ips114_write_8bit_data(0x3F);
    ips114_write_8bit_data(0x54);
    ips114_write_8bit_data(0x4C);
    ips114_write_8bit_data(0x18);
    ips114_write_8bit_data(0x0D);
    ips114_write_8bit_data(0x0B);
    ips114_write_8bit_data(0x1F);
    ips114_write_8bit_data(0x23);

    ips114_write_index(0xE1);
    ips114_write_8bit_data(0xD0);
    ips114_write_8bit_data(0x04);
    ips114_write_8bit_data(0x0C);
    ips114_write_8bit_data(0x11);
    ips114_write_8bit_data(0x13);
    ips114_write_8bit_data(0x2C);
    ips114_write_8bit_data(0x3F);
    ips114_write_8bit_data(0x44);
    ips114_write_8bit_data(0x51);
    ips114_write_8bit_data(0x2F);
    ips114_write_8bit_data(0x1F);
    ips114_write_8bit_data(0x1F);
    ips114_write_8bit_data(0x20);
    ips114_write_8bit_data(0x23);

    ips114_write_index(0x21);

    ips114_write_index(0x11);
    system_delay_ms(120);

    ips114_write_index(0x29);

    ips114_clear(ips114_bgcolor);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����������
// @param       color           ��ɫ��ʽ RGB565 ���߿���ʹ�� zf_common_font.h �ڳ�����ɫ�궨��
// @return      void
// Sample usage:                ips114_clear(YELLOW);
//-------------------------------------------------------------------------------------------------------------------
void ips114_clear (uint16_t color)
{
    uint32_t i = ips114_x_max*ips114_y_max;

    ips114_set_region(0,0,ips114_x_max-1,ips114_y_max-1);
    for( ; i>0; i--)
    {
        ips114_write_16bit_data(color);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ������ʾ���� �������ֻ���ڳ�ʼ����Ļ֮ǰ���ò���Ч
// @param       dir             ��ʾ����  ���� zf_device_ips114.h �� ips114_dir_enum ö���嶨��
// @return      void
// Sample usage:                ips114_set_dir(IPS114_CROSSWISE);
//-------------------------------------------------------------------------------------------------------------------
void ips114_set_dir (ips114_dir_enum dir)
{
    ips114_display_dir = dir;
    if(dir < 2)
    {
        ips114_x_max = 135;
        ips114_y_max = 240;
    }
    else
    {
        ips114_x_max = 240;
        ips114_y_max = 135;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ������ʾ��ɫ
// @param       pen             ��ɫ��ʽ RGB565 ���߿���ʹ�� zf_common_font.h �ڳ�����ɫ�궨��
// @param       bgcolor         ��ɫ��ʽ RGB565 ���߿���ʹ�� zf_common_font.h �ڳ�����ɫ�궨��
// @return      void
// Sample usage:                ips114_set_color(WHITE,BLACK);
//-------------------------------------------------------------------------------------------------------------------
void ips114_set_color (uint16_t pen, uint16_t bgcolor)
{
    ips114_pencolor = pen;
    ips114_bgcolor = bgcolor;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ������
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       dat             ��Ҫ��ʾ����ɫ
// @return      void
// Sample usage:                ips114_draw_point(0,0,RED);                     // ���� 0,0 ��һ����ɫ�ĵ�
//-------------------------------------------------------------------------------------------------------------------
void ips114_draw_point (uint16_t x,uint16_t y,uint16_t color)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);

    ips114_set_region(x,y,x,y);
    ips114_write_16bit_data(color);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ�ַ�
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       dat             ��Ҫ��ʾ���ַ�
// @return      void
// Sample usage:                ips114_show_char(0,0,'x');                      // ���� 0,0 дһ���ַ� x
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_char (uint16_t x,uint16_t y,const char dat)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);
    y = y*16;
    uint8_t i,j;
    uint8_t temp;

    for(i=0; i<16; i++)
    {
        ips114_set_region(x,y+i,x+7,y+i);
        temp = tft_ascii[dat-32][i];                                            //�� 32 ��Ϊ��ȡģ�Ǵӿո�ʼȡ�� �ո��� ascii ������� 32
        for(j=0; j<8; j++)
        {
            if(temp&0x01)   
                ips114_write_16bit_data(ips114_pencolor);
            else
                ips114_write_16bit_data(ips114_bgcolor);
            temp>>=1;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ�ַ���
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       dat             ��Ҫ��ʾ���ַ���
// @return      void
// Sample usage:                ips114_show_string(0,0,"seekfree");
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_string (uint16_t x,uint16_t y,const char dat[])
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);
    
    uint16_t j = 0;
    while(dat[j] != '\0')
    {
        ips114_show_char(x+8*j,y,dat[j]);
        j++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ8λ�з���
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
//  @param      dat             ��Ҫ��ʾ�ı�������������int8_t
//  @return     void
//  Sample usage:               ips114_show_int8(0,0,x);                        // xΪ int8 ����
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_int8(uint16_t x,uint16_t y,int8_t dat)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);

    uint8_t a[3];
    uint8_t i;
    if(dat<0)
    {
        ips114_show_char(x,y,'-');
        dat = -dat;
    }
    else
        ips114_show_char(x,y,' ');
    
    a[0] = dat/100;
    a[1] = dat/10%10;
    a[2] = dat%10;
    i = 0;
    while(i<3)
    {
        ips114_show_char(x+(8*(i+1)),y,'0' + a[i]);
        i++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ8λ�޷���
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       dat             ��Ҫ��ʾ�ı��� ��������uint8_t
// @return      void
// Sample usage:                ips114_show_uint8(0,0,x);                       // x Ϊ uint8_t ����
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_uint8(uint16_t x,uint16_t y,uint8_t dat)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);

    uint8_t a[3];
    uint8_t i;
    
    a[0] = dat/100;
    a[1] = dat/10%10;
    a[2] = dat%10;
    i = 0;
    while(i<3)
    {
        ips114_show_char(x+(8*i),y,'0' + a[i]);
        i++;
    }
    
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ16λ�з���
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       dat             ��Ҫ��ʾ�ı��� ��������int16_t
// @return      void
// Sample usage:                ips114_show_int16(0,0,x);                       // x Ϊ int16_t ����
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_int16(uint16_t x,uint16_t y,int16_t dat)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);

    uint8_t a[5];
    uint8_t i;

    if(dat<0)
    {
        ips114_show_char(x,y,'-');
        dat = -dat;
    }
    else    ips114_show_char(x,y,' ');

    a[0] = dat/10000;
    a[1] = dat/1000%10;
    a[2] = dat/100%10;
    a[3] = dat/10%10;
    a[4] = dat%10;
    
    i = 0;
    while(i<5)
    {
        ips114_show_char(x+(8*(i+1)),y,'0' + a[i]);
        i++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ16λ�޷���
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       dat             ��Ҫ��ʾ�ı�������������uint16_t
// @return      void
// Sample usage:                ips114_show_uint16(0,0,x);                      // x Ϊ uint16_t ����
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_uint16(uint16_t x,uint16_t y,uint16_t dat)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);

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
        ips114_show_char(x+(8*i),y,'0' + a[i]);
        i++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ32λ�з���(ȥ������������Ч��0)
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       dat             ��Ҫ��ʾ�ı�������������uint32_t
// @param       num             ��Ҫ��ʾ��λ�� ���10λ  ������������
// @return      void
// Sample usage:                ips114_showuint32(0,0,x,3);                     // x ����Ϊ int32_t uint16_t int16_t uint8_t int8_t ����
// note:                        ��������ʾһ�� ��-����   ������ʾһ���ո�
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_int32(uint16_t x,uint16_t y,int32_t dat,uint8_t num)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);

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

    ips114_show_string(x, y, (const char *)buff);                               // ��ʾ����
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ������(ȥ������������Ч��0)
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       dat             ��Ҫ��ʾ�ı�������������float��double
// @param       num             ����λ��ʾ����   ���10λ
// @param       pointnum        С��λ��ʾ����   ���6λ
// @return      void
// Sample usage:                ips114_showfloat(0,0,x,2,3);                    // ��ʾ������   ������ʾ2λ   С����ʾ��λ
// @note                        �ر�ע�⵱����С��������ʾ��ֵ����д���ֵ��һ����ʱ��
//                              ���������ڸ��������ȶ�ʧ���⵼�µģ��Ⲣ������ʾ���������⣬
//                              �й���������飬�����аٶ�ѧϰ   ���������ȶ�ʧ���⡣
//                              ��������ʾһ�� ��-����   ������ʾһ���ո�
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_float(uint16_t x,uint16_t y,double dat,uint8_t num,uint8_t pointnum)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);

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

    ips114_show_string(x, y, (const char *)buff);                               // ��ʾ����
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS114 ��ʾ��ֵͼ�� ����ÿ�˸������һ���ֽ�����
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       *p              ͼ������ָ��
// @param       width           ͼ��ʵ�ʿ��
// @param       height          ͼ��ʵ�ʸ߶�
// @param       dis_width       ͼ����ʾ��� ������Χ [0, ips114_x_max]
// @param       dis_height      ͼ����ʾ�߶� ������Χ [0, ips114_y_max]
// @return      void
// Sample usage:                ips114_show_binary_image(0,0,ov7725_image_binary[0],OV7725_W,OV7725_H,OV7725_W,OV7725_H);
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_binary_image (uint16_t x, uint16_t y, uint8_t *p, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);

    uint32_t i = 0, j = 0;
    uint8_t temp = 0;
    uint32_t width_index = 0, height_index = 0;

    ips114_set_region(x, y, x+dis_width-1, y+dis_height-1);                     // ������ʾ����

    for(j=0;j<dis_height;j++)
    {
        height_index = j*height/dis_height;
        for(i=0;i<dis_width;i++)
        {
            width_index = i*width/dis_width;
            temp = *(p+height_index*width/8+width_index/8);                     // ��ȡ���ص�
            if(0x80 & (temp << (width_index%8)))
                ips114_write_16bit_data(WHITE);
            else
                ips114_write_16bit_data(BLACK);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS114 ��ʾ 8bit �Ҷ�ͼ�� ����ֵ����ֵ
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       *p              ͼ������ָ��
// @param       width           ͼ��ʵ�ʿ��
// @param       height          ͼ��ʵ�ʸ߶�
// @param       dis_width       ͼ����ʾ��� ������Χ [0, ips114_x_max]
// @param       dis_height      ͼ����ʾ�߶� ������Χ [0, ips114_y_max]
// @param       threshold       ��ֵ����ʾ��ֵ 0-��������ֵ��
// @return      void
// Sample usage:                ips114_show_gray_image(0,0,camera_buffer_addr,MT9V03X_W,MT9V03X_H,MT9V03X_W,MT9V03X_H,0);
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_gray_image (uint16_t x, uint16_t y, uint8_t *p, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height, uint8_t threshold)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);

    uint32_t i = 0, j = 0;
    uint16_t color = 0,temp = 0;
    uint32_t width_index = 0, height_index = 0;

    ips114_set_region(x, y, x+dis_width-1, y+dis_height-1);                     // ������ʾ����

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
                ips114_write_16bit_data(color);
            }
            else if(temp < threshold)
                ips114_write_16bit_data(BLACK);
            else
                ips114_write_16bit_data(WHITE);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS114 ��ʾ RGB565 ��ɫͼ��
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       *p              ͼ������ָ��
// @param       width           ͼ��ʵ�ʿ��
// @param       height          ͼ��ʵ�ʸ߶�
// @param       dis_width       ͼ����ʾ��� ������Χ [0, ips114_x_max]
// @param       dis_height      ͼ����ʾ�߶� ������Χ [0, ips114_y_max]
// @param       threshold       ��ֵ����ʾ��ֵ 0-��������ֵ��
// @return      void
// Sample usage:                ips114_show_gray_image(0,0,camera_buffer_addr,SCCB8660_W,SCCB8660_H,SCCB8660_W,SCCB8660_H);
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_rgb565_image (uint16_t x, uint16_t y, uint16_t *p, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);

    uint32_t i = 0, j = 0;
    uint16_t color = 0;
    uint32_t width_index = 0, height_index = 0;

    ips114_set_region(x, y, x+dis_width-1, y+dis_height-1);                     // ������ʾ����

    for(j=0;j<dis_height;j++)
    {
        height_index = j*height/dis_height;
        for(i=0;i<dis_width;i++)
        {
            width_index = i*width/dis_width;
            color = *(p+height_index*width+width_index);                        // ��ȡ���ص�
            color = ((color&0xff)<<8) | (color>>8);
            ips114_write_16bit_data(color);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS114 ��ʾ����2
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       *p              ��������ָ��
// @param       width           ����ʵ�ʿ��
// @param       value_max       ����ʵ�����ֵ
// @param       dis_width       ������ʾ��� ������Χ [0, ips114_x_max]
// @param       dis_value_max   ������ʾ���ֵ ������Χ [0, ips114_y_max]
// @return      void
// Sample usage:                ips114_show_gray_image(0,0,camera_buffer_addr,MT9V03X_W,MT9V03X_H,240,135,128);
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_wave(uint16_t x, uint16_t y, uint8_t *p, uint16_t width, uint16_t value_max, uint16_t dis_width, uint16_t dis_value_max)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);

    uint32_t i = 0, j = 0;
    uint32_t width_index = 0, value_max_index = 0;

    ips114_set_region(x, y, x+dis_width-1, y+dis_value_max-1);                  // ������ʾ����
    for(i=0;i<dis_value_max;i++)
    {
        for(j=0;j<dis_width;j++)
        {
            ips114_write_16bit_data(ips114_bgcolor); 
        }
    }

    for(i=0;i<dis_width;i++)
    {
        width_index = i*width/dis_width;
        value_max_index = *(p+width_index)*(dis_value_max-1)/value_max;
        ips114_draw_point(i+x, (dis_value_max-1)-value_max_index+y, ips114_pencolor);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ������ʾ
// @param       x               ����x�������� ������Χ [0, ips114_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips114_y_max-1]
// @param       size            ȡģ��ʱ�����õĺ��������С Ҳ����һ������ռ�õĵ��󳤿�Ϊ���ٸ��� ȡģ��ʱ����Ҫ������һ����
// @param       *p              ��Ҫ��ʾ�ĺ�������
// @param       number          ��Ҫ��ʾ����λ
// @param       color           ��ʾ��ɫ
// @return      void
// Sample usage:                ips114_show_chinese(0,0,16,chinese_test[0],4,RED);//��ʾfont�ļ������ ʾ��
// @Note                        ʹ��PCtoLCD2002���ȡģ           ���롢����ʽ��˳��   16*16
//-------------------------------------------------------------------------------------------------------------------
void ips114_show_chinese (uint16_t x, uint16_t y, uint8_t size, const uint8_t *p, uint8_t number, uint16_t color)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);

    int i, j, k; 
    uint8_t temp, temp1, temp2;
    const uint8_t *p_data;
    
    temp2 = size/8;
    
    ips114_set_region(x,y,number*size-1+x,y+size-1);
    
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
                    if(temp)    ips114_write_16bit_data(color);
                    else        ips114_write_16bit_data(ips114_bgcolor);
                }
                p_data++;
            }
            p_data = p_data - temp2 + temp2*size;
        }   
    }
}
//==============================================USER==================================================//
/***************************************************************
 * @team      : nefu_fengying
 * @editor    : zhaoyanguo
 * @name      : void ips114_display_myimage(uint8 *p, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height)
 * @param     : NULL
 * @return    : NULL
 * @function  : NULL
 * @invocation: ips114_display_myimage(&ImageUaed,80,160,80,160);
***************************************************************/
void ips114_display_myimage(uint16_t *p,uint16_t posi_x,uint16_t posi_y, \
        uint16_t width, uint16_t height, \
        uint16_t dis_width, uint16_t dis_height)
{
    uint32_t i,j;

    ips114_set_region(posi_x,posi_y,posi_x + dis_width-1,posi_y + dis_height-1);//������ʾ����

    for(j=0;j<dis_height;j++)
    {
        for(i=0;i<dis_width;i++)
        {
            ips114_write_16bit_data(*(p+(j*height/dis_height)*width+i*width/dis_width));
        }
    }
}


//���ƴ�ֱ��X���ֱ�� size�ǰ����м������ȥ�ľ���
void ips114_DrawXLine(uint16_t x,uint16_t color,uint16_t size)
{
    int16_t _y = 0;
    for(_y = 0;_y < 135;_y ++)
    {
        if(size == 1)
            ips114_draw_point(x,_y,color);
        else if(size == 3)
        {
            ips114_draw_point(x-1,_y,color);
            ips114_draw_point(x,_y,color);
            ips114_draw_point(x+1,_y,color);
        }
        else if(size == 5)
        {
            ips114_draw_point(x-2,_y,color);
            ips114_draw_point(x-1,_y,color);
            ips114_draw_point(x,_y,color);
            ips114_draw_point(x+1,_y,color);
            ips114_draw_point(x+2,_y,color);
        }
    }
}

//����һ����ֱ��Y���ֱ��
void ips114_DrawYLine(uint16_t y,uint16_t color,uint16_t size)
{
    uint16_t _x = 0;
    for(_x = 0;_x < 240;_x ++)
    {
        if(size == 1)
            ips114_draw_point(_x,y,color);
        else if(size == 3)
        {
            ips114_draw_point(_x,y - 1,color);
            ips114_draw_point(_x,y,color);
            ips114_draw_point(_x,y + 1,color);
        }
        else if(size == 5)
        {
            ips114_draw_point(_x,y - 2,color);
            ips114_draw_point(_x,y - 1,color);
            ips114_draw_point(_x,y,color);
            ips114_draw_point(_x,y + 1,color);
            ips114_draw_point(_x,y + 2,color);
        }
    }
}
/**
*@Name          :ips114_show_gray_image_vec
*@Description   :ips114_show_gray_image_vec ��ʾ͸�ӱ任ָ����ָ��ͼ��
*@Param         :
*@Return        :
*@Sample        :ips114_show_gray_image_vec(0,0,PerImg_ip,TRFED_COL,TRFED_ROW,TRFED_COL,TRFED_ROW,0);
**/
void ips114_show_gray_image_vec (uint16_t x, uint16_t y, uint8_t *p[][TRFED_COL], uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height, uint8_t threshold)
{
    zf_assert(x < ips114_x_max);
    zf_assert(y < ips114_y_max);

    uint32_t i = 0, j = 0;
    uint16_t color = 0,temp = 0;
    uint32_t width_index = 0, height_index = 0;

    ips114_set_region(x, y, x+dis_width-1, y+dis_height-1);                     // ������ʾ����

    for(j=0;j<dis_height;j++)
    {
        height_index = j*height/dis_height;
        for(i=0;i<dis_width;i++)
        {
            width_index = i*width/dis_width;
            temp = *p[height_index][width_index];                         // ��ȡ���ص�
            if(threshold == 0)
            {
                color=(0x001f&((temp)>>3))<<11;
                color=color|(((0x003f)&((temp)>>2))<<5);
                color=color|(0x001f&((temp)>>3));
                ips114_write_16bit_data(color);
            }
            else if(temp < threshold)
                ips114_write_16bit_data(BLACK);
            else
                ips114_write_16bit_data(WHITE);
        }
    }
}
