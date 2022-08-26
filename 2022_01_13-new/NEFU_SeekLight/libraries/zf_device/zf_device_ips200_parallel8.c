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
* @note            version:
*                  V1.1 2021.12.02 �޸ĺ������� (ips200_showfloat -> ips200_show_float)
*
********************************************************************************************************************/
#include "zf_device_ips200_parallel8.h"

uint16_t ips200_pencolor = IPS200_DEFAULT_PENCOLOR;
uint16_t ips200_bgcolor = IPS200_DEFAULT_BGCOLOR;

ips200_dir_enum ips200_display_dir = IPS200_DEFAULT_DISPLAY_DIR;
uint16_t ips200_x_max = 240;
uint16_t ips200_y_max = 320;

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS200 д���� �ڲ�����
// @param       dat             ����
// @return      void
// @note        �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void ips200_write_data (uint8_t dat)
{
    IPS200_DATAPORT->OUTDR = (dat<< DATA_START_NUM) | (IPS200_DATAPORT->OUTDR & ~((uint32_t)(0xFF << DATA_START_NUM)));
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS200 д���� �ڲ�����
// @param       command         ����
// @return      void
// @note        �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void ips200_write_command (uint8_t command)
{
    IPS200_RS(0);
    IPS200_RD(1);
    IPS200_DATAPORT->OUTDR = (command<< DATA_START_NUM) | (IPS200_DATAPORT->OUTDR & ~((uint32_t)(0xFF << DATA_START_NUM)));
    IPS200_CS(0);
    IPS200_WR(0);
    IPS200_WR(1);
    IPS200_CS(1);

}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS200 ��Һ����д 8bit ���� �ڲ�����
// @param       dat             ����
// @return      void
// @note        �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void ips200_write_8bit_data (uint8_t dat)
{
    IPS200_RS(1);
    IPS200_RD(1);
    ips200_write_data(dat);
    IPS200_CS(0);
    IPS200_WR(0);
    IPS200_WR(1);
    IPS200_CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS200 ��Һ����д 16bit ���� �ڲ�����
// @param       dat             ����
// @return      void
// @note        �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void ips200_write_16bit_data (uint16_t dat)
{
    IPS200_RS(1);
    IPS200_RD(1);
    ips200_write_data((uint8_t)(dat>>8));
    IPS200_CS(0);
    IPS200_WR(0);
    IPS200_WR(1);
    ips200_write_data((uint8_t)(dat));
    IPS200_CS(0);
    IPS200_WR(0);
    IPS200_WR(1);
    IPS200_CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS200 д�Ĵ��� �ڲ�����
// @param       com             ����
// @param       dat             ����
// @return      void
// @note        �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
//static void ips200_write_register (uint8_t com, uint8_t dat)
//{
//    ips200_write_command(com);
//    ips200_write_8bit_data(dat);
//}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ������ʾ���� �ڲ�����
// @param       x1              ��ʼx������
// @param       y1              ��ʼy������
// @param       x2              ����x������
// @param       y2              ����y������
// @return      void
// @note        �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void ips200_set_region(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
    ips200_write_command(0x2a);
    ips200_write_16bit_data(x1);
    ips200_write_16bit_data(x2);

    ips200_write_command(0x2b);
    ips200_write_16bit_data(y1);
    ips200_write_16bit_data(y2);

    ips200_write_command(0x2c);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ������ʾ���� �������ֻ���ڳ�ʼ����Ļ֮ǰ���ò���Ч
// @param       dir             ��ʾ����  ���� zf_device_ips200.h �� ips200_dir_enum ö���嶨��
// @return      void
// Sample usage:                ips200_set_dir(IPS114_CROSSWISE);
//-------------------------------------------------------------------------------------------------------------------
void ips200_set_dir (ips200_dir_enum dir)
{
    ips200_display_dir = dir;
    if(dir < 2)
    {
        ips200_x_max = 240;
        ips200_y_max = 320;
    }
    else
    {
        ips200_x_max = 320;
        ips200_y_max = 240;
    }
}
//-------------------------------------------------------------------------------------------------------------------
// @brief       ������ʾ��ɫ
// @param       pen             ��ɫ��ʽ RGB565 ���߿���ʹ�� zf_common_font.h �ڳ�����ɫ�궨��
// @param       bgcolor         ��ɫ��ʽ RGB565 ���߿���ʹ�� zf_common_font.h �ڳ�����ɫ�궨��
// @return      void
// Sample usage:                ips200_set_color(WHITE,BLACK);
//-------------------------------------------------------------------------------------------------------------------
void ips200_set_color (uint16_t pen, uint16_t bgcolor)
{
    ips200_pencolor = pen;
    ips200_bgcolor = bgcolor;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ������
//  @param      x               ����x��������
//  @param      y               ����y��������
//  @param      dat             ��Ҫ��ʾ����ɫ
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_drawpoint(0,0,RED);  //����0,0��һ����ɫ�ĵ�
//-------------------------------------------------------------------------------------------------------------------
void ips200_draw_point(uint16_t x,uint16_t y,uint16_t color)
{
    ips200_set_region(x,y,x,y);
    ips200_write_16bit_data(color);
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      2.0�� IPSҺ��GPIO��ʼ��
//  @return     void
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void ips200_gpio_init(void)
{
    uint8_t i = 0;
    uint8_t pin_value = (((uint32_t)IPS200_DATAPORT - GPIOA_BASE)/0x400) * 0x20 + DATA_START_NUM;  // ��ȡ�˿ڵ�ö����ֵ

    gpio_init(IPS200_RD_PIN,  GPO, 1, GPO_PUSH_PULL);
    gpio_init(IPS200_WR_PIN,  GPO, 1, GPO_PUSH_PULL);
    gpio_init(IPS200_RS_PIN,  GPO, 1, GPO_PUSH_PULL);
    gpio_init(IPS200_RST_PIN, GPO, 1, GPO_PUSH_PULL);
    gpio_init(IPS200_CS_PIN,  GPO, 1, GPO_PUSH_PULL);
    gpio_init(IPS200_BL_PIN,  GPO, 1, GPO_PUSH_PULL);

//#define GPIOA_BASE            (APB2PERIPH_BASE + 0x0800)
//#define GPIOB_BASE            (APB2PERIPH_BASE + 0x0C00)
//#define GPIOC_BASE            (APB2PERIPH_BASE + 0x1000)
//#define GPIOD_BASE            (APB2PERIPH_BASE + 0x1400)
//#define GPIOE_BASE            (APB2PERIPH_BASE + 0x1800)
//#define GPIOF_BASE            (APB2PERIPH_BASE + 0x1C00)
//#define GPIOG_BASE            (APB2PERIPH_BASE + 0x2000)

    for(i = 0;i < 8;i++)
    {
        gpio_init((gpio_pin_enum)(pin_value + i), GPO, 0, GPO_PUSH_PULL);
    }

}


//-------------------------------------------------------------------------------------------------------------------
// @brief       2�� IPSҺ����ʼ��
// @param       void
// @return      void
// Sample usage:                ips200_init();
//-------------------------------------------------------------------------------------------------------------------
void ips200_init (void)
{
    ips200_gpio_init();

    IPS200_BL(1);
    IPS200_RST(0);
    system_delay_ms(5);
    IPS200_RST(1);
    system_delay_ms(120);

    ips200_write_command(0x11);
    system_delay_ms(120);

    ips200_write_command(0x36);
    if      (ips200_display_dir==0) ips200_write_8bit_data(0x00);
    else if (ips200_display_dir==1) ips200_write_8bit_data(0xC0);
    else if (ips200_display_dir==2) ips200_write_8bit_data(0x70);
    else                            ips200_write_8bit_data(0xA0);

    ips200_write_command(0x3A);
    ips200_write_8bit_data(0x05);

    ips200_write_command(0xB2);
    ips200_write_8bit_data(0x0C);
    ips200_write_8bit_data(0x0C);
    ips200_write_8bit_data(0x00);
    ips200_write_8bit_data(0x33);
    ips200_write_8bit_data(0x33);

    ips200_write_command(0xB7);
    ips200_write_8bit_data(0x35);

    ips200_write_command(0xBB);
    ips200_write_8bit_data(0x29); //32 Vcom=1.35V

    ips200_write_command(0xC2);
    ips200_write_8bit_data(0x01);

    ips200_write_command(0xC3);
    ips200_write_8bit_data(0x19); //GVDD=4.8V

    ips200_write_command(0xC4);
    ips200_write_8bit_data(0x20); //VDV, 0x20:0v

    ips200_write_command(0xC5);
    ips200_write_8bit_data(0x1A);//VCOM Offset Set

    ips200_write_command(0xC6);
    ips200_write_8bit_data(0x01F); //0x0F:60Hz

    ips200_write_command(0xD0);
    ips200_write_8bit_data(0xA4);
    ips200_write_8bit_data(0xA1);

    ips200_write_command(0xE0);
    ips200_write_8bit_data(0xD0);
    ips200_write_8bit_data(0x08);
    ips200_write_8bit_data(0x0E);
    ips200_write_8bit_data(0x09);
    ips200_write_8bit_data(0x09);
    ips200_write_8bit_data(0x05);
    ips200_write_8bit_data(0x31);
    ips200_write_8bit_data(0x33);
    ips200_write_8bit_data(0x48);
    ips200_write_8bit_data(0x17);
    ips200_write_8bit_data(0x14);
    ips200_write_8bit_data(0x15);
    ips200_write_8bit_data(0x31);
    ips200_write_8bit_data(0x34);

    ips200_write_command(0xE1);
    ips200_write_8bit_data(0xD0);
    ips200_write_8bit_data(0x08);
    ips200_write_8bit_data(0x0E);
    ips200_write_8bit_data(0x09);
    ips200_write_8bit_data(0x09);
    ips200_write_8bit_data(0x15);
    ips200_write_8bit_data(0x31);
    ips200_write_8bit_data(0x33);
    ips200_write_8bit_data(0x48);
    ips200_write_8bit_data(0x17);
    ips200_write_8bit_data(0x14);
    ips200_write_8bit_data(0x15);
    ips200_write_8bit_data(0x31);
    ips200_write_8bit_data(0x34);

    ips200_write_command(0x21);

    ips200_write_command(0x29);

    ips200_clear(ips200_bgcolor);   //��ʼ��Ϊ����
} 


//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����������
// @param       color           ��ɫ��ʽ RGB565 ���߿���ʹ�� zf_common_font.h �ڳ�����ɫ�궨��
// @return      void
// Sample usage:                ips200_clear(YELLOW);
//-------------------------------------------------------------------------------------------------------------------
void ips200_clear(uint16_t color)
{
    uint16_t i,j;
    ips200_set_region(0,0,ips200_x_max-1,ips200_y_max-1);
    for(i=0;i<ips200_x_max;i++)
    {
        for (j=0;j<ips200_y_max;j++)
        {
            ips200_write_16bit_data(color);
        }
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ�ַ�
//  @param      x               ����x�������� ������Χ 0 - (ips200_x_max-1)
//  @param      y               ����y�������� ������Χ 0 - (ips200_y_max/16-1)
//  @param      dat             ��Ҫ��ʾ���ַ�
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_showchar(0,0,'x');//����0,0дһ���ַ�x
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_char (uint16_t x,uint16_t y,const char dat)
{
    uint8_t i,j;
    uint8_t temp;

    for(i=0; i<16; i++)
    {
        ips200_set_region(x,y+i,x+7,y+i);
        temp = tft_ascii[(uint16_t)dat-32][i];//��32��Ϊ��ȡģ�Ǵӿո�ʼȡ�� �ո���ascii�������32
        for(j=0; j<8; j++)
        {
            if(temp&0x01)   ips200_write_16bit_data(ips200_pencolor);
            else            ips200_write_16bit_data(ips200_bgcolor);
            temp>>=1;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ�ַ���
// @param       x               ����x�������� ������Χ [0, ips200_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips200_y_max-1]
// @param       dat             ��Ҫ��ʾ���ַ���
// @return      void
// Sample usage:                ips200_show_string(0,0,"seekfree");
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_string (uint16_t x,uint16_t y,const char dat[])
{
    zf_assert(x < ips200_x_max);
    zf_assert(y < ips200_y_max);

    uint16_t j = 0;
    while(dat[j] != '\0')
    {
        ips200_show_char(x+8*j,y,dat[j]);
        j++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ8λ�з���
// @param       x               ����x�������� ������Χ [0, ips200_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips200_y_max-1]
//  @param      dat             ��Ҫ��ʾ�ı�������������int8_t
//  @return     void
//  Sample usage:               ips200_show_int8(0,0,x);                        // xΪ int8_t ����
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_int8(uint16_t x,uint16_t y,int8_t dat)
{
    zf_assert(x < ips200_x_max);
    zf_assert(y < ips200_y_max);

    uint8_t a[3];
    uint8_t i;
    if(dat<0)
    {
        ips200_show_char(x,y,'-');
        dat = -dat;
    }
    else
        ips200_show_char(x,y,' ');
    
    a[0] = dat/100;
    a[1] = dat/10%10;
    a[2] = dat%10;
    i = 0;
    while(i<3)
    {
        ips200_show_char(x+(8*(i+1)),y,'0' + a[i]);
        i++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ8λ�޷���
// @param       x               ����x�������� ������Χ [0, ips200_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips200_y_max-1]
// @param       dat             ��Ҫ��ʾ�ı��� ��������uint8_t
// @return      void
// Sample usage:                ips200_show_uint8(0,0,x);                       // x Ϊ uint8_t ����
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_uint8(uint16_t x,uint16_t y,uint8_t dat)
{
    zf_assert(x < ips200_x_max);
    zf_assert(y < ips200_y_max);

    uint8_t a[3];
    uint8_t i;

    a[0] = dat/100;
    a[1] = dat/10%10;
    a[2] = dat%10;
    i = 0;
    while(i<3)
    {
        ips200_show_char(x+(8*i),y,'0' + a[i]);
        i++;
    }

}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ16λ�з���
// @param       x               ����x�������� ������Χ [0, ips200_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips200_y_max-1]
// @param       dat             ��Ҫ��ʾ�ı��� ��������int16_t
// @return      void
// Sample usage:                ips200_show_int16(0,0,x);                       // x Ϊ int16_t ����
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_int16(uint16_t x,uint16_t y,int16_t dat)
{
    zf_assert(x < ips200_x_max);
    zf_assert(y < ips200_y_max);

    uint8_t a[5];
    uint8_t i;

    if(dat<0)
    {
        ips200_show_char(x,y,'-');
        dat = -dat;
    }
    else    ips200_show_char(x,y,' ');

    a[0] = dat/10000;
    a[1] = dat/1000%10;
    a[2] = dat/100%10;
    a[3] = dat/10%10;
    a[4] = dat%10;

    i = 0;
    while(i<5)
    {
        ips200_show_char(x+(8*(i+1)),y,'0' + a[i]);
        i++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ16λ�޷���
// @param       x               ����x�������� ������Χ [0, ips200_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips200_y_max-1]
// @param       dat             ��Ҫ��ʾ�ı�������������uint16_t
// @return      void
// Sample usage:                ips200_show_uint16(0,0,x);                      // x Ϊ uint16_t ����
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_uint16(uint16_t x,uint16_t y,uint16_t dat)
{
    zf_assert(x < ips200_x_max);
    zf_assert(y < ips200_y_max);

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
        ips200_show_char(x+(8*i),y,'0' + a[i]);
        i++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ32λ�з���(ȥ������������Ч��0)
// @param       x               ����x�������� ������Χ [0, ips200_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips200_y_max-1]
// @param       dat             ��Ҫ��ʾ�ı�������������uint32_t
// @param       num             ��Ҫ��ʾ��λ�� ���10λ  ������������
// @return      void
// Sample usage:                ips200_showuint32(0,0,x,3);                     // x ����Ϊ int32_t uint16_t int16_t uint8_t int8_t ����
// note:                        ��������ʾһ�� ��-����   ������ʾһ���ո�
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_int32(uint16_t x,uint16_t y,int32_t dat,uint8_t num)
{
    zf_assert(x < ips200_x_max);
    zf_assert(y < ips200_y_max);

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

    ips200_show_string(x, y, (const char *)buff);                               // ��ʾ����
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       Һ����ʾ������(ȥ������������Ч��0)
// @param       x               ����x�������� ������Χ [0, ips200_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips200_y_max-1]
// @param       dat             ��Ҫ��ʾ�ı�������������float��double
// @param       num             ����λ��ʾ����   ���10λ
// @param       pointnum        С��λ��ʾ����   ���6λ
// @return      void
// Sample usage:                ips200_showfloat(0,0,x,2,3);                    // ��ʾ������   ������ʾ2λ   С����ʾ��λ
// @note                        �ر�ע�⵱����С��������ʾ��ֵ����д���ֵ��һ����ʱ��
//                              ���������ڸ��������ȶ�ʧ���⵼�µģ��Ⲣ������ʾ���������⣬
//                              �й���������飬�����аٶ�ѧϰ   ���������ȶ�ʧ���⡣
//                              ��������ʾһ�� ��-����   ������ʾһ���ո�
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_float(uint16_t x,uint16_t y,double dat,uint8_t num,uint8_t pointnum)
{
    zf_assert(x < ips200_x_max);
    zf_assert(y < ips200_y_max);

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

    ips200_show_string(x, y, (const char *)buff);                               // ��ʾ����
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS114 ��ʾ��ֵͼ�� ����ÿ�˸������һ���ֽ�����
// @param       x               ����x�������� ������Χ [0, ips200_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips200_y_max-1]
// @param       *p              ͼ������ָ��
// @param       width           ͼ��ʵ�ʿ��
// @param       height          ͼ��ʵ�ʸ߶�
// @param       dis_width       ͼ����ʾ��� ������Χ [0, ips200_x_max]
// @param       dis_height      ͼ����ʾ�߶� ������Χ [0, ips200_y_max]
// @return      void
// Sample usage:                ips200_show_binary_image(0,0,ov7725_image_binary[0],OV7725_W,OV7725_H,OV7725_W,OV7725_H);
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_binary_image (uint16_t x, uint16_t y, uint8_t *p, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height)
{
    zf_assert(x < ips200_x_max);
    zf_assert(y < ips200_y_max);

    uint32_t i = 0, j = 0;
    uint8_t temp = 0;
    uint32_t width_index = 0, height_index = 0;

    ips200_set_region(x, y, x+dis_width-1, y+dis_height-1);                     // ������ʾ����

    for(j=0;j<dis_height;j++)
    {
        height_index = j*height/dis_height;
        for(i=0;i<dis_width;i++)
        {
            width_index = i*width/dis_width;
            temp = *(p+height_index*width/8+width_index/8);                     // ��ȡ���ص�
            if(0x80 & (temp << (width_index%8)))
                ips200_write_16bit_data(WHITE);
            else
                ips200_write_16bit_data(BLACK);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS114 ��ʾ 8bit �Ҷ�ͼ�� ����ֵ����ֵ
// @param       x               ����x�������� ������Χ [0, ips200_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips200_y_max-1]
// @param       *p              ͼ������ָ��
// @param       width           ͼ��ʵ�ʿ��
// @param       height          ͼ��ʵ�ʸ߶�
// @param       dis_width       ͼ����ʾ��� ������Χ [0, ips200_x_max]
// @param       dis_height      ͼ����ʾ�߶� ������Χ [0, ips200_y_max]
// @param       threshold       ��ֵ����ʾ��ֵ 0-��������ֵ��
// @return      void
// Sample usage:                ips200_show_gray_image(0,0,camera_buffer_addr,MT9V03X_W,MT9V03X_H,MT9V03X_W,MT9V03X_H,0);
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_gray_image (uint16_t x, uint16_t y, uint8_t *p, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height, uint8_t threshold)
{
    zf_assert(x < ips200_x_max);
    zf_assert(y < ips200_y_max);

    uint32_t i = 0, j = 0;
    uint16_t color = 0,temp = 0;
    uint32_t width_index = 0, height_index = 0;

    ips200_set_region(x, y, x+dis_width-1, y+dis_height-1);                     // ������ʾ����

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
                ips200_write_16bit_data(color);
            }
            else if(temp < threshold)
                ips200_write_16bit_data(BLACK);
            else
                ips200_write_16bit_data(WHITE);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS114 ��ʾ RGB565 ��ɫͼ��
// @param       x               ����x�������� ������Χ [0, ips200_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips200_y_max-1]
// @param       *p              ͼ������ָ��
// @param       width           ͼ��ʵ�ʿ��
// @param       height          ͼ��ʵ�ʸ߶�
// @param       dis_width       ͼ����ʾ��� ������Χ [0, ips200_x_max]
// @param       dis_height      ͼ����ʾ�߶� ������Χ [0, ips200_y_max]
// @param       threshold       ��ֵ����ʾ��ֵ 0-��������ֵ��
// @return      void
// Sample usage:                ips200_show_gray_image(0,0,camera_buffer_addr,SCCB8660_W,SCCB8660_H,SCCB8660_W,SCCB8660_H);
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_rgb565_image (uint16_t x, uint16_t y, uint16_t *p, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height)
{
    zf_assert(x < ips200_x_max);
    zf_assert(y < ips200_y_max);

    uint32_t i = 0, j = 0;
    uint16_t color = 0;
    uint32_t width_index = 0, height_index = 0;

    ips200_set_region(x, y, x+dis_width-1, y+dis_height-1);                     // ������ʾ����

    for(j=0;j<dis_height;j++)
    {
        height_index = j*height/dis_height;
        for(i=0;i<dis_width;i++)
        {
            width_index = i*width/dis_width;
            color = *(p+height_index*width+width_index);                        // ��ȡ���ص�
            color = ((color&0xff)<<8) | (color>>8);
            ips200_write_16bit_data(color);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS114 ��ʾ����2
// @param       x               ����x�������� ������Χ [0, ips200_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips200_y_max-1]
// @param       *p              ��������ָ��
// @param       width           ����ʵ�ʿ��
// @param       value_max       ����ʵ�����ֵ
// @param       dis_width       ������ʾ��� ������Χ [0, ips200_x_max]
// @param       dis_value_max   ������ʾ���ֵ ������Χ [0, ips200_y_max]
// @return      void
// Sample usage:                ips200_show_gray_image(0,0,camera_buffer_addr,MT9V03X_W,MT9V03X_H,240,135,128);
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_wave(uint16_t x, uint16_t y, uint8_t *p, uint16_t width, uint16_t value_max, uint16_t dis_width, uint16_t dis_value_max)
{
    zf_assert(x < ips200_x_max);
    zf_assert(y < ips200_y_max);

    uint32_t i = 0, j = 0;
    uint32_t width_index = 0, value_max_index = 0;

    ips200_set_region(x, y, x+dis_width-1, y+dis_value_max-1);                  // ������ʾ����
    for(i=0;i<dis_value_max;i++)
    {
        for(j=0;j<dis_width;j++)
        {
            ips200_write_16bit_data(ips200_bgcolor);
        }
    }

    for(i=0;i<dis_width;i++)
    {
        width_index = i*width/dis_width;
        value_max_index = *(p+width_index)*(dis_value_max-1)/value_max;
        ips200_draw_point(i+x, (dis_value_max-1)-value_max_index+y, ips200_pencolor);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ������ʾ
// @param       x               ����x�������� ������Χ [0, ips200_x_max-1]
// @param       y               ����y�������� ������Χ [0, ips200_y_max-1]
// @param       size            ȡģ��ʱ�����õĺ��������С Ҳ����һ������ռ�õĵ��󳤿�Ϊ���ٸ��� ȡģ��ʱ����Ҫ������һ����
// @param       *p              ��Ҫ��ʾ�ĺ�������
// @param       number          ��Ҫ��ʾ����λ
// @param       color           ��ʾ��ɫ
// @return      void
// Sample usage:                ips200_show_chinese(0,0,16,chinese_test[0],4,RED);//��ʾfont�ļ������ ʾ��
// @Note                        ʹ��PCtoLCD2002���ȡģ           ���롢����ʽ��˳��   16*16
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_chinese (uint16_t x, uint16_t y, uint8_t size, const uint8_t *p, uint8_t number, uint16_t color)
{
    zf_assert(x < ips200_x_max);
    zf_assert(y < ips200_y_max);

    int i, j, k; 
    uint8_t temp, temp1, temp2;
    const uint8_t *p_data;
    
    temp2 = size/8;
    
    ips200_set_region(x,y,number*size-1+x,y+size-1);
    
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
                    if(temp)    ips200_write_16bit_data(color);
                    else        ips200_write_16bit_data(ips200_bgcolor);
                }
                p_data++;
            }
            p_data = p_data - temp2 + temp2*size;
        }   
    }
}
