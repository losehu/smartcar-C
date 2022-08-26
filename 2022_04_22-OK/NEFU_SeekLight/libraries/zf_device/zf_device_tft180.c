/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             zf_device_tft180
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ790875685)
* @version          查看doc内version文件 版本说明
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
* @note             接线定义：
*                   ------------------------------------
*                   模块管脚                                        单片机管脚
*                   SCL                 查看 zf_device_tft180.h 中 TFT180_SCL_PIN 宏定义
*                   SDA                 查看 zf_device_tft180.h 中 TFT180_SDA_PIN 宏定义
*                   RES                 查看 zf_device_tft180.h 中 TFT180_RES_PIN 宏定义
*                   DC                  查看 zf_device_tft180.h 中 TFT180_DC_PIN 宏定义
*                   CS                  查看 zf_device_tft180.h 中 TFT180_CS_PIN 宏定义
*                   BL                  查看 zf_device_tft180.h 中 TFT180_BL_PIN 宏定义
*                   电源引脚
*                   VCC                 3.3V电源
*                   GND                 电源地
*                   最大分辨率160*128
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
// @brief       写命令 内部调用
// @note        内部调用 用户无需关心
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
// @brief       设置显示区域 内部调用
// @param       x1              起始x轴坐标
// @param       y1              起始y轴坐标
// @param       x2              结束x轴坐标
// @param       y2              结束y轴坐标
// @return      void
// @note        内部调用 用户无需关心
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
// @brief       1.14寸 IPS液晶初始化
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
        case 0: tft180_write_8bit_data(1<<7 | 1<<6 | 0<<5);  break;                          // 竖屏模式
        case 1: tft180_write_8bit_data(0<<7 | 0<<6 | 0<<5);  break;                          // 竖屏模式  旋转180
        case 2: tft180_write_8bit_data(1<<7 | 0<<6 | 1<<5);  break;                          // 横屏模式
        case 3: tft180_write_8bit_data(0<<7 | 1<<6 | 1<<5);  break;                          // 横屏模式  旋转180
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
// @brief       液晶清屏函数
// @param       color           颜色格式 RGB565 或者可以使用 zf_common_font.h 内常用颜色宏定义
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
// @brief       设置显示方向 这个函数只有在初始化屏幕之前调用才生效
// @param       dir             显示方向  参照 zf_device_ips114.h 内 tft180_dir_enum 枚举体定义
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
// @brief       设置显示颜色
// @param       pen             颜色格式 RGB565 或者可以使用 zf_common_font.h 内常用颜色宏定义
// @param       bgcolor         颜色格式 RGB565 或者可以使用 zf_common_font.h 内常用颜色宏定义
// @return      void
// Sample usage:                tft180_set_color(WHITE,BLACK);
//-------------------------------------------------------------------------------------------------------------------
void tft180_set_color (uint16_t pen, uint16_t bgcolor)
{
    tft180_pencolor = pen;
    tft180_bgcolor = bgcolor;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       液晶画点
// @param       x               坐标x方向的起点 参数范围 [0, tft180_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, tft180_y_max-1]
// @param       dat             需要显示的颜色
// @return      void
// Sample usage:                tft180_draw_point(0,0,RED);                     // 坐标 0,0 画一个红色的点
//-------------------------------------------------------------------------------------------------------------------
void tft180_draw_point (uint16_t x,uint16_t y,uint16_t color)
{
    zf_assert(x < tft180_x_max);
    zf_assert(y < tft180_y_max);

    tft180_set_region(x,y,x,y);
    tft180_write_16bit_data(color);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       液晶显示字符
// @param       x               坐标x方向的起点 参数范围 [0, tft180_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, tft180_y_max-1]
// @param       dat             需要显示的字符
// @return      void
// Sample usage:                tft180_show_char(0,0,'x');                      // 坐标 0,0 写一个字符 x
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
        temp = tft_ascii[dat-32][i];                                            //减 32 因为是取模是从空格开始取得 空格在 ascii 中序号是 32
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
// @brief       液晶显示字符串
// @param       x               坐标x方向的起点 参数范围 [0, tft180_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, tft180_y_max-1]
// @param       dat             需要显示的字符串
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
//  @brief      液晶显示8位有符号
// @param       x               坐标x方向的起点 参数范围 [0, tft180_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, tft180_y_max-1]
//  @param      dat             需要显示的变量，数据类型int8_t
//  @return     void
//  Sample usage:               tft180_show_int8(0,0,x);                        // x为 int8 类型
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
// @brief       液晶显示8位无符号
// @param       x               坐标x方向的起点 参数范围 [0, tft180_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, tft180_y_max-1]
// @param       dat             需要显示的变量 数据类型uint8_t
// @return      void
// Sample usage:                tft180_show_uint8(0,0,x);                       // x 为 uint8_t 类型
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
// @brief       液晶显示16位有符号
// @param       x               坐标x方向的起点 参数范围 [0, tft180_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, tft180_y_max-1]
// @param       dat             需要显示的变量 数据类型int16_t
// @return      void
// Sample usage:                tft180_show_int16(0,0,x);                       // x 为 int16_t 类型
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
// @brief       液晶显示16位无符号
// @param       x               坐标x方向的起点 参数范围 [0, tft180_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, tft180_y_max-1]
// @param       dat             需要显示的变量，数据类型uint16_t
// @return      void
// Sample usage:                tft180_show_uint16(0,0,x);                      // x 为 uint16_t 类型
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
// @brief       液晶显示32位有符号(去除整数部分无效的0)
// @param       x               坐标x方向的起点 参数范围 [0, tft180_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, tft180_y_max-1]
// @param       dat             需要显示的变量，数据类型uint32_t
// @param       num             需要显示的位数 最高10位  不包含正负号
// @return      void
// Sample usage:                tft180_showuint32(0,0,x,3);                     // x 可以为 int32_t uint16_t int16_t uint8_t int8_t 类型
// note:                        负数会显示一个 ‘-’号   正数显示一个空格
//-------------------------------------------------------------------------------------------------------------------
void tft180_show_int32(uint16_t x,uint16_t y,int32_t dat,uint8_t num)
{
    zf_assert(x < tft180_x_max);
    zf_assert(y < tft180_y_max);

    int8_t    buff[34];
    uint8_t   length;
    
    if(10<num)      num = 10;
    
    num++;
    if(0>dat)   length = zf_sprintf( &buff[0],(const int8_t *)"%d",dat);        // 负数
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

    tft180_show_string(x, y, (const char *)buff);                               // 显示数字
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       液晶显示浮点数(去除整数部分无效的0)
// @param       x               坐标x方向的起点 参数范围 [0, tft180_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, tft180_y_max-1]
// @param       dat             需要显示的变量，数据类型float或double
// @param       num             整数位显示长度   最高10位
// @param       pointnum        小数位显示长度   最高6位
// @return      void
// Sample usage:                tft180_showfloat(0,0,x,2,3);                    // 显示浮点数   整数显示2位   小数显示三位
// @note                        特别注意当发现小数部分显示的值与你写入的值不一样的时候，
//                              可能是由于浮点数精度丢失问题导致的，这并不是显示函数的问题，
//                              有关问题的详情，请自行百度学习   浮点数精度丢失问题。
//                              负数会显示一个 ‘-’号   正数显示一个空格
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
        
    if(0>dat)   length = zf_sprintf( &buff[0],(const int8_t *)"%f",dat);        // 负数
    else
    {
        length = zf_sprintf( &buff[1],(const int8_t *)"%f",dat);
        length++;
    }
    point = length - 7;                                                         // 计算小数点位置
    start = point - num - 1;                                                    // 计算起始位
    end = point + pointnum + 1;                                                 // 计算结束位
    while(0>start)                                                              // 整数位不够  末尾应该填充空格
    {
        buff[end] = ' ';
        end++;
        start++;
    }
    
    if(0>dat)   buff[start] = '-';
    else        buff[start] = ' ';
    
    buff[end] = '\0';

    tft180_show_string(x, y, (const char *)buff);                               // 显示数字
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       TFT180 显示二值图像 数据每八个点组成一个字节数据
// @param       x               坐标x方向的起点 参数范围 [0, tft180_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, tft180_y_max-1]
// @param       *p              图像数组指针
// @param       width           图像实际宽度
// @param       height          图像实际高度
// @param       dis_width       图像显示宽度 参数范围 [0, tft180_x_max]
// @param       dis_height      图像显示高度 参数范围 [0, tft180_y_max]
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

    tft180_set_region(x, y, x+dis_width-1, y+dis_height-1);                     // 设置显示区域

    for(j=0;j<dis_height;j++)
    {
        height_index = j*height/dis_height;
        for(i=0;i<dis_width;i++)
        {
            width_index = i*width/dis_width;
            temp = *(p+height_index*width/8+width_index/8);                     // 读取像素点
            if(0x80 & (temp << (width_index%8)))
                tft180_write_16bit_data(WHITE);
            else
                tft180_write_16bit_data(BLACK);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       TFT180 显示 8bit 灰度图像 带二值化阈值
// @param       x               坐标x方向的起点 参数范围 [0, tft180_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, tft180_y_max-1]
// @param       *p              图像数组指针
// @param       width           图像实际宽度
// @param       height          图像实际高度
// @param       dis_width       图像显示宽度 参数范围 [0, tft180_x_max]
// @param       dis_height      图像显示高度 参数范围 [0, tft180_y_max]
// @param       threshold       二值化显示阈值 0-不开启二值化
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

    tft180_set_region(x, y, x+dis_width-1, y+dis_height-1);                     // 设置显示区域

    for(j=0;j<dis_height;j++)
    {
        height_index = j*height/dis_height;
        for(i=0;i<dis_width;i++)
        {
            width_index = i*width/dis_width;
            temp = *(p+height_index*width+width_index);                         // 读取像素点
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
// @brief       TFT180 显示 RGB565 彩色图像
// @param       x               坐标x方向的起点 参数范围 [0, tft180_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, tft180_y_max-1]
// @param       *p              图像数组指针
// @param       width           图像实际宽度
// @param       height          图像实际高度
// @param       dis_width       图像显示宽度 参数范围 [0, tft180_x_max]
// @param       dis_height      图像显示高度 参数范围 [0, tft180_y_max]
// @param       threshold       二值化显示阈值 0-不开启二值化
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

    tft180_set_region(x, y, x+dis_width-1, y+dis_height-1);                     // 设置显示区域

    for(j=0;j<dis_height;j++)
    {
        height_index = j*height/dis_height;
        for(i=0;i<dis_width;i++)
        {
            width_index = i*width/dis_width;
            color = *(p+height_index*width+width_index);                        // 读取像素点
            color = ((color&0xff)<<8) | (color>>8);
            tft180_write_16bit_data(color);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       TFT180 显示波形2
// @param       x               坐标x方向的起点 参数范围 [0, tft180_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, tft180_y_max-1]
// @param       *p              波形数组指针
// @param       width           波形实际宽度
// @param       value_max       波形实际最大值
// @param       dis_width       波形显示宽度 参数范围 [0, tft180_x_max]
// @param       dis_value_max   波形显示最大值 参数范围 [0, tft180_y_max]
// @return      void
// Sample usage:                tft180_show_gray_image(0,0,camera_buffer_addr,MT9V03X_W,MT9V03X_H,240,135,128);
//-------------------------------------------------------------------------------------------------------------------
void tft180_show_wave(uint16_t x, uint16_t y, uint8_t *p, uint16_t width, uint16_t value_max, uint16_t dis_width, uint16_t dis_value_max)
{
    zf_assert(x < tft180_x_max);
    zf_assert(y < tft180_y_max);

    uint32_t i = 0, j = 0;
    uint32_t width_index = 0, value_max_index = 0;

    tft180_set_region(x, y, x+dis_width-1, y+dis_value_max-1);                  // 设置显示区域
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
// @brief       汉字显示
// @param       x               坐标x方向的起点 参数范围 [0, tft180_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, tft180_y_max-1]
// @param       size            取模的时候设置的汉字字体大小 也就是一个汉字占用的点阵长宽为多少个点 取模的时候需要长宽是一样的
// @param       *p              需要显示的汉字数组
// @param       number          需要显示多少位
// @param       color           显示颜色
// @return      void
// Sample usage:                tft180_show_chinese(0,0,16,chinese_test[0],4,RED);//显示font文件里面的 示例
// @Note                        使用PCtoLCD2002软件取模           阴码、逐行式、顺向   16*16
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


