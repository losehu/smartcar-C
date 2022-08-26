/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             zf_device_ips200_parallel8
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
*                   BL                  查看 zf_device_ips200_parallel8.h 中 IPS200_BL_PIN 宏定义
*                   CS                  查看 zf_device_ips200_parallel8.h 中 IPS200_CS_PIN 宏定义
*                   RST                 查看 zf_device_ips200_parallel8.h 中 IPS200_RST_PIN 宏定义
*                   RS                  查看 zf_device_ips200_parallel8.h 中 IPS200_RS_PIN 宏定义
*                   WR                  查看 zf_device_ips200_parallel8.h 中 IPS200_WR_PIN 宏定义
*                   RD                  查看 zf_device_ips200_parallel8.h 中 IPS200_RD_PIN 宏定义
*                   D0-D7               查看 zf_device_ips200_parallel8.h 中 ICM20602_CS_PIN 宏定义
*                   电源引脚
*                   VCC                 3.3V电源
*                   GND                 电源地
*                   ------------------------------------
* @note            version:
*                  V1.1 2021.12.02 修改函数名称 (ips200_showfloat -> ips200_show_float)
*
********************************************************************************************************************/
#include "zf_device_ips200_parallel8.h"

uint16_t ips200_pencolor = IPS200_DEFAULT_PENCOLOR;
uint16_t ips200_bgcolor = IPS200_DEFAULT_BGCOLOR;

ips200_dir_enum ips200_display_dir = IPS200_DEFAULT_DISPLAY_DIR;
uint16_t ips200_x_max = 240;
uint16_t ips200_y_max = 320;

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS200 写数据 内部调用
// @param       dat             数据
// @return      void
// @note        内部调用 用户无需关心
//-------------------------------------------------------------------------------------------------------------------
static void ips200_write_data (uint8_t dat)
{
    IPS200_DATAPORT->OUTDR = (dat<< DATA_START_NUM) | (IPS200_DATAPORT->OUTDR & ~((uint32_t)(0xFF << DATA_START_NUM)));
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS200 写命令 内部调用
// @param       command         命令
// @return      void
// @note        内部调用 用户无需关心
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
// @brief       IPS200 向液晶屏写 8bit 数据 内部调用
// @param       dat             数据
// @return      void
// @note        内部调用 用户无需关心
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
// @brief       IPS200 向液晶屏写 16bit 数据 内部调用
// @param       dat             数据
// @return      void
// @note        内部调用 用户无需关心
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
// @brief       IPS200 写寄存器 内部调用
// @param       com             命令
// @param       dat             数据
// @return      void
// @note        内部调用 用户无需关心
//-------------------------------------------------------------------------------------------------------------------
//static void ips200_write_register (uint8_t com, uint8_t dat)
//{
//    ips200_write_command(com);
//    ips200_write_8bit_data(dat);
//}

//-------------------------------------------------------------------------------------------------------------------
// @brief       设置显示区域 内部调用
// @param       x1              起始x轴坐标
// @param       y1              起始y轴坐标
// @param       x2              结束x轴坐标
// @param       y2              结束y轴坐标
// @return      void
// @note        内部调用 用户无需关心
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
// @brief       设置显示方向 这个函数只有在初始化屏幕之前调用才生效
// @param       dir             显示方向  参照 zf_device_ips200.h 内 ips200_dir_enum 枚举体定义
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
// @brief       设置显示颜色
// @param       pen             颜色格式 RGB565 或者可以使用 zf_common_font.h 内常用颜色宏定义
// @param       bgcolor         颜色格式 RGB565 或者可以使用 zf_common_font.h 内常用颜色宏定义
// @return      void
// Sample usage:                ips200_set_color(WHITE,BLACK);
//-------------------------------------------------------------------------------------------------------------------
void ips200_set_color (uint16_t pen, uint16_t bgcolor)
{
    ips200_pencolor = pen;
    ips200_bgcolor = bgcolor;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶画点
//  @param      x               坐标x方向的起点
//  @param      y               坐标y方向的起点
//  @param      dat             需要显示的颜色
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_drawpoint(0,0,RED);  //坐标0,0画一个红色的点
//-------------------------------------------------------------------------------------------------------------------
void ips200_draw_point(uint16_t x,uint16_t y,uint16_t color)
{
    ips200_set_region(x,y,x,y);
    ips200_write_16bit_data(color);
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      2.0寸 IPS液晶GPIO初始化
//  @return     void
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void ips200_gpio_init(void)
{
    uint8_t i = 0;
    uint8_t pin_value = (((uint32_t)IPS200_DATAPORT - GPIOA_BASE)/0x400) * 0x20 + DATA_START_NUM;  // 获取端口的枚举体值

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
// @brief       2寸 IPS液晶初始化
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

    ips200_clear(ips200_bgcolor);   //初始化为白屏
} 


//-------------------------------------------------------------------------------------------------------------------
// @brief       液晶清屏函数
// @param       color           颜色格式 RGB565 或者可以使用 zf_common_font.h 内常用颜色宏定义
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
//  @brief      液晶显示字符
//  @param      x               坐标x方向的起点 参数范围 0 - (ips200_x_max-1)
//  @param      y               坐标y方向的起点 参数范围 0 - (ips200_y_max/16-1)
//  @param      dat             需要显示的字符
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_showchar(0,0,'x');//坐标0,0写一个字符x
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_char (uint16_t x,uint16_t y,const char dat)
{
    uint8_t i,j;
    uint8_t temp;

    for(i=0; i<16; i++)
    {
        ips200_set_region(x,y+i,x+7,y+i);
        temp = tft_ascii[(uint16_t)dat-32][i];//减32因为是取模是从空格开始取得 空格在ascii中序号是32
        for(j=0; j<8; j++)
        {
            if(temp&0x01)   ips200_write_16bit_data(ips200_pencolor);
            else            ips200_write_16bit_data(ips200_bgcolor);
            temp>>=1;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       液晶显示字符串
// @param       x               坐标x方向的起点 参数范围 [0, ips200_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, ips200_y_max-1]
// @param       dat             需要显示的字符串
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
//  @brief      液晶显示8位有符号
// @param       x               坐标x方向的起点 参数范围 [0, ips200_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, ips200_y_max-1]
//  @param      dat             需要显示的变量，数据类型int8_t
//  @return     void
//  Sample usage:               ips200_show_int8(0,0,x);                        // x为 int8_t 类型
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
// @brief       液晶显示8位无符号
// @param       x               坐标x方向的起点 参数范围 [0, ips200_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, ips200_y_max-1]
// @param       dat             需要显示的变量 数据类型uint8_t
// @return      void
// Sample usage:                ips200_show_uint8(0,0,x);                       // x 为 uint8_t 类型
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
// @brief       液晶显示16位有符号
// @param       x               坐标x方向的起点 参数范围 [0, ips200_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, ips200_y_max-1]
// @param       dat             需要显示的变量 数据类型int16_t
// @return      void
// Sample usage:                ips200_show_int16(0,0,x);                       // x 为 int16_t 类型
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
// @brief       液晶显示16位无符号
// @param       x               坐标x方向的起点 参数范围 [0, ips200_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, ips200_y_max-1]
// @param       dat             需要显示的变量，数据类型uint16_t
// @return      void
// Sample usage:                ips200_show_uint16(0,0,x);                      // x 为 uint16_t 类型
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
// @brief       液晶显示32位有符号(去除整数部分无效的0)
// @param       x               坐标x方向的起点 参数范围 [0, ips200_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, ips200_y_max-1]
// @param       dat             需要显示的变量，数据类型uint32_t
// @param       num             需要显示的位数 最高10位  不包含正负号
// @return      void
// Sample usage:                ips200_showuint32(0,0,x,3);                     // x 可以为 int32_t uint16_t int16_t uint8_t int8_t 类型
// note:                        负数会显示一个 ‘-’号   正数显示一个空格
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_int32(uint16_t x,uint16_t y,int32_t dat,uint8_t num)
{
    zf_assert(x < ips200_x_max);
    zf_assert(y < ips200_y_max);

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

    ips200_show_string(x, y, (const char *)buff);                               // 显示数字
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       液晶显示浮点数(去除整数部分无效的0)
// @param       x               坐标x方向的起点 参数范围 [0, ips200_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, ips200_y_max-1]
// @param       dat             需要显示的变量，数据类型float或double
// @param       num             整数位显示长度   最高10位
// @param       pointnum        小数位显示长度   最高6位
// @return      void
// Sample usage:                ips200_showfloat(0,0,x,2,3);                    // 显示浮点数   整数显示2位   小数显示三位
// @note                        特别注意当发现小数部分显示的值与你写入的值不一样的时候，
//                              可能是由于浮点数精度丢失问题导致的，这并不是显示函数的问题，
//                              有关问题的详情，请自行百度学习   浮点数精度丢失问题。
//                              负数会显示一个 ‘-’号   正数显示一个空格
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

    ips200_show_string(x, y, (const char *)buff);                               // 显示数字
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS114 显示二值图像 数据每八个点组成一个字节数据
// @param       x               坐标x方向的起点 参数范围 [0, ips200_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, ips200_y_max-1]
// @param       *p              图像数组指针
// @param       width           图像实际宽度
// @param       height          图像实际高度
// @param       dis_width       图像显示宽度 参数范围 [0, ips200_x_max]
// @param       dis_height      图像显示高度 参数范围 [0, ips200_y_max]
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

    ips200_set_region(x, y, x+dis_width-1, y+dis_height-1);                     // 设置显示区域

    for(j=0;j<dis_height;j++)
    {
        height_index = j*height/dis_height;
        for(i=0;i<dis_width;i++)
        {
            width_index = i*width/dis_width;
            temp = *(p+height_index*width/8+width_index/8);                     // 读取像素点
            if(0x80 & (temp << (width_index%8)))
                ips200_write_16bit_data(WHITE);
            else
                ips200_write_16bit_data(BLACK);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS114 显示 8bit 灰度图像 带二值化阈值
// @param       x               坐标x方向的起点 参数范围 [0, ips200_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, ips200_y_max-1]
// @param       *p              图像数组指针
// @param       width           图像实际宽度
// @param       height          图像实际高度
// @param       dis_width       图像显示宽度 参数范围 [0, ips200_x_max]
// @param       dis_height      图像显示高度 参数范围 [0, ips200_y_max]
// @param       threshold       二值化显示阈值 0-不开启二值化
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

    ips200_set_region(x, y, x+dis_width-1, y+dis_height-1);                     // 设置显示区域

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
// @brief       IPS114 显示 RGB565 彩色图像
// @param       x               坐标x方向的起点 参数范围 [0, ips200_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, ips200_y_max-1]
// @param       *p              图像数组指针
// @param       width           图像实际宽度
// @param       height          图像实际高度
// @param       dis_width       图像显示宽度 参数范围 [0, ips200_x_max]
// @param       dis_height      图像显示高度 参数范围 [0, ips200_y_max]
// @param       threshold       二值化显示阈值 0-不开启二值化
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

    ips200_set_region(x, y, x+dis_width-1, y+dis_height-1);                     // 设置显示区域

    for(j=0;j<dis_height;j++)
    {
        height_index = j*height/dis_height;
        for(i=0;i<dis_width;i++)
        {
            width_index = i*width/dis_width;
            color = *(p+height_index*width+width_index);                        // 读取像素点
            color = ((color&0xff)<<8) | (color>>8);
            ips200_write_16bit_data(color);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS114 显示波形2
// @param       x               坐标x方向的起点 参数范围 [0, ips200_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, ips200_y_max-1]
// @param       *p              波形数组指针
// @param       width           波形实际宽度
// @param       value_max       波形实际最大值
// @param       dis_width       波形显示宽度 参数范围 [0, ips200_x_max]
// @param       dis_value_max   波形显示最大值 参数范围 [0, ips200_y_max]
// @return      void
// Sample usage:                ips200_show_gray_image(0,0,camera_buffer_addr,MT9V03X_W,MT9V03X_H,240,135,128);
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_wave(uint16_t x, uint16_t y, uint8_t *p, uint16_t width, uint16_t value_max, uint16_t dis_width, uint16_t dis_value_max)
{
    zf_assert(x < ips200_x_max);
    zf_assert(y < ips200_y_max);

    uint32_t i = 0, j = 0;
    uint32_t width_index = 0, value_max_index = 0;

    ips200_set_region(x, y, x+dis_width-1, y+dis_value_max-1);                  // 设置显示区域
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
// @brief       汉字显示
// @param       x               坐标x方向的起点 参数范围 [0, ips200_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, ips200_y_max-1]
// @param       size            取模的时候设置的汉字字体大小 也就是一个汉字占用的点阵长宽为多少个点 取模的时候需要长宽是一样的
// @param       *p              需要显示的汉字数组
// @param       number          需要显示多少位
// @param       color           显示颜色
// @return      void
// Sample usage:                ips200_show_chinese(0,0,16,chinese_test[0],4,RED);//显示font文件里面的 示例
// @Note                        使用PCtoLCD2002软件取模           阴码、逐行式、顺向   16*16
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
