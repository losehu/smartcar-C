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
* @note             version:
*                   V1.1 2021.12.01 �޸� ��������ʾ���� BUG
********************************************************************************************************************/

#include "zf_device_oled.h"

#if IPS114_USE_SOFT_SPI
static soft_spi_info_struct             oled_spi;
#define oled_spi_write_8bit(data)       soft_spi_write_8bit(&oled_spi, data)
#else
#define oled_spi_write_8bit(data)       spi_write_8bit(OLED_SPI, data)
#endif

oled_dir_enum oled_display_dir = OLED_DEFAULT_DISPLAY_DIR;
oled_font_size_enum oled_display_font = OLED_DEFAULT_DISPLAY_FONT;

//-------------------------------------------------------------------------------------------------------------------
// @brief       д8λ����
// @param       data            ����
// @return      void
// @since       v1.0
// Sample usage:
// @note        �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void oled_write_data(uint8_t data)
{
    OLED_CS(0);                                                             // ʹ���źŶˣ�����ʱ����ʹ��
    OLED_DC(1);
    oled_spi_write_8bit(data);
    OLED_CS(1);                                                             // ʹ���źŶˣ�����ʱ����ʹ��
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       д����
// @param       cmd             ����
// @return      void
// @since       v1.0
// Sample usage:
// @note        �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void oled_write_command (uint8_t command)
{
    OLED_CS(0);                                                             // ʹ���źŶˣ�����ʱ����ʹ��
    OLED_DC(0);
    oled_spi_write_8bit(command);
    OLED_CS(1);                                                             // ʹ���źŶˣ�����ʱ����ʹ��
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       OLED��ʾ��������
// @param       x               x����������0-127
// @param       y               y����������0-7
// @return      void
// @since       v1.0
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
static void oled_set_coordinate (uint8_t x, uint8_t y)
{
    oled_write_command(0xb0+y);
    oled_write_command(((x&0xf0)>>4)|0x10);
    oled_write_command((x&0x0f)|0x00);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       OLED��ʼ������
// @param       NULL
// @return      void
// @since       v1.0
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void oled_init(void)
{
#if IPS114_USE_SOFT_SPI
    soft_spi_init(&oled_spi, 0, OLED_SOFT_SPI_DELAY, OLED_D0_PIN, OLED_D1_PIN, SOFT_SPI_PIN_NULL, SOFT_SPI_PIN_NULL);
#else
    zf_assert(OLED_SPI != (OLED_D0_PIN&0xF000>>12));
    zf_assert(OLED_SPI != (OLED_D1_PIN&0xF000>>12));
    spi_init(OLED_SPI, SPI_MODE0, OLED_SPI_SPEED, OLED_D0_PIN, OLED_D1_PIN, SPI_MISO_NULL, SPI_CS_NULL);
#endif
    gpio_init(OLED_RES_PIN, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    gpio_init(OLED_DC_PIN , GPO, GPIO_HIGH, GPO_PUSH_PULL);
    gpio_init(OLED_CS_PIN , GPO, GPIO_HIGH, GPO_PUSH_PULL);

    oled_set_dir(oled_display_dir);

    OLED_CS(0);
    OLED_RES(0);
    system_delay_ms(50);
    OLED_RES(1);

    oled_write_command(0xae);                                                       // --turn off oled panel
    oled_write_command(0x00);                                                       // ---set low column address
    oled_write_command(0x10);                                                       // ---set high column address
    oled_write_command(0x40);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         // --set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    oled_write_command(0x81);                                                       // --set contrast control register
    oled_write_command(OLED_BRIGHTNESS);                                                 //  Set SEG Output Current Brightness

    if (oled_display_dir == OLED_CROSSWISE)
    {
        oled_write_command(0xa1);                                                       // --Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
        oled_write_command(0xc8);                                                       // Set COM/Row Scan Direction   0xc0���·��� 0xc8����
    }
    else
    {
        oled_write_command(0xa0);                                                       // --Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
        oled_write_command(0xc0);                                                       // Set COM/Row Scan Direction   0xc0���·��� 0xc8����
    }

    oled_write_command(0xa6);                                                       // --set normal display
    oled_write_command(0xa8);                                                       // --set multiplex ratio(1 to 64)
    oled_write_command(0x3f);                                                       // --1/64 duty
    oled_write_command(0xd3);                                                       // -set display offset  Shift Mapping RAM Counter (0x00~0x3F)
    oled_write_command(0x00);                                                       // -not offset
    oled_write_command(0xd5);                                                       // --set display clock divide ratio/oscillator frequency
    oled_write_command(0x80);                                                       // --set divide ratio, Set Clock as 100 Frames/Sec
    oled_write_command(0xd9);                                                       // --set pre-charge period
    oled_write_command(0xf1);                                                       // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    oled_write_command(0xda);                                                       // --set com pins hardware configuration
    oled_write_command(0x12);
    oled_write_command(0xdb);                                                       // --set vcomh
    oled_write_command(0x40);                                                       // Set VCOM Deselect Level
    oled_write_command(0x20);                                                       // -Set Page Addressing Mode (0x00/0x01/0x02)
    oled_write_command(0x02);                                                       //
    oled_write_command(0x8d);                                                       // --set Charge Pump enable/disable
    oled_write_command(0x14);                                                       // --set(0x10) disable
    oled_write_command(0xa4);                                                       //  Disable Entire Display On (0xa4/0xa5)
    oled_write_command(0xa6);                                                       //  Disable Inverse Display On (0xa6/a7)
    oled_write_command(0xaf);                                                       // --turn on oled panel

    oled_clear(0x00);                                                        // ��ʼ����
    oled_set_coordinate(0,0);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       OLED��������
// @param       color           �����ɫѡ��(0x00 or 0xff)
// @return      void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void oled_clear (uint8_t color)
{
    uint8_t y,x;

    for(y=0;y<8;y++)
    {
        oled_write_command(0xb0+y);
        oled_write_command(0x01);
        oled_write_command(0x10);
        for(x=0;x<OLED_X_MAX;x++)
            oled_write_data(color);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ������ʾ���� �������ֻ���ڳ�ʼ����Ļ֮ǰ���ò���Ч
// @param       dir             ��ʾ����  ���� zf_device_oled.h �� oled_dir_enum ö���嶨��
// @return      void
// Sample usage:                oled_set_dir(IPS114_CROSSWISE);
//-------------------------------------------------------------------------------------------------------------------
void oled_set_dir (oled_dir_enum dir)
{
    oled_display_dir = dir;
}

void oled_set_font (oled_font_size_enum font)
{
    oled_display_font = font;
}

void oled_draw_point (uint16_t x,uint16_t y,uint8_t color)
{
    oled_set_coordinate(x,y);
    oled_write_command(0xb0+y);
    oled_write_command(((x&0xf0)>>4)|0x10);
    oled_write_command((x&0x0f)|0x00);
    oled_write_data(color);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       OLED��ʾ�ַ���
// @param       x               x���������� 0-127
// @param       y               y���������� 0-7
// @param       ch[]            �ַ���
// @return      void
// @since       v1.0
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void oled_show_string (uint8_t x,uint8_t y,char ch[])
{
    uint8_t c=0,i=0,j=0;
    while (ch[j]!='\0')
    {
        if(oled_display_font == OLED_6x8_FONT)
        {
        c =ch[j]-32;
        if(x>126)
        {
            x=0;
            y++;
        }
        oled_set_coordinate(x,y);
        for(i=0;i<6;i++)
            oled_write_data(oled_6x8[c][i]);
        x+=6;
        j++;
        }
        else
        {
            c =ch[j]-32;
            if(x>120)
            {
                x=0;
                y++;
            }
            oled_set_coordinate(x,y);
            for(i=0;i<8;i++)
                oled_write_data(oled_8x16[c*16+i]);

            oled_set_coordinate(x,y+1);
            for(i=0;i<8;i++)
                oled_write_data(oled_8x16[c*16+i+8]);
            x+=8;
            j++;
        }
    }
}

void oled_show_int8 (uint16_t x,uint16_t y,int8_t dat)
{
    char data_buffer[5] = {0,0,0,0,0};
    int32_to_str(data_buffer, dat);
    oled_show_string(x,y,data_buffer);
}

void oled_show_uint8 (uint16_t x,uint16_t y,uint8_t dat)
{
    char data_buffer[4] = {0,0,0,0};
    uint32_to_str(data_buffer, dat);
    oled_show_string(x,y,data_buffer);
}
void oled_show_int16 (uint16_t x,uint16_t y,int16_t dat)
{
    char data_buffer[7] = {0,0,0,0,0,0,0};
    int32_to_str(data_buffer, dat);
    oled_show_string(x,y,data_buffer);
}

void oled_show_uint16 (uint16_t x,uint16_t y,uint16_t dat)
{
    char data_buffer[6] = {0,0,0,0,0,0};
    uint32_to_str(data_buffer, dat);
    oled_show_string(x,y,data_buffer);
}

void oled_show_int32 (uint16_t x,uint16_t y,int32_t dat,uint8_t num)
{
    uint8_t offset = 0;
    char data_buffer[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int32_to_str(data_buffer, dat);
    offset = strlen(data_buffer) - num;
    oled_show_string(x,y,&data_buffer[offset]);
}

void oled_show_float (uint16_t x,uint16_t y,double dat,uint8_t num,uint8_t pointnum)
{
    uint8_t offset = 0;
    int16_t num_part = 0;
    uint16_t point_part = 0;

    if(dat < 0.000)
    {
        offset++;
        for(;dat < -1.00; )
        {
            num_part --;
            dat += 1;
        }
    }
    else
    {
        for(;dat > 1.00; )
        {
            num_part ++;
            dat -= 1;
        }
    }
    offset += num;

    for(;pointnum>0;pointnum--)
    {
        dat *= 10;
    }
    point_part = dat;

    oled_show_int16(x,y,num_part);
    oled_show_string(x+(offset)*(6+oled_display_font*2),y,".");
    oled_show_uint16(x+(offset+1)*(6+oled_display_font*2),y,point_part);
}

void oled_show_binary_image (uint16_t x, uint16_t y, uint8_t *p, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height)
{
    uint32_t i = 0, j = 0, z = 0;
    uint8_t dat;
    uint32_t width_index = 0, height_index = 0;

    dis_height = dis_height - dis_height%8;
    dis_width = dis_width - dis_width%8;
    for(j=0;j<dis_height;j+=8)
    {
        oled_set_coordinate(0,j/8);
        height_index = j*height/dis_height;
        for(i=0;i<dis_width;i+=8)
        {
            width_index = i*width/dis_width/8;
            for(z = 0; z < 8; z++)
            {
                dat = 0;
                if(*(p + height_index*width/8 + width_index + width/8*0) & (0x80>>z)) dat |= 0x01;
                if(*(p + height_index*width/8 + width_index + width/8*1) & (0x80>>z)) dat |= 0x02;
                if(*(p + height_index*width/8 + width_index + width/8*2) & (0x80>>z)) dat |= 0x04;
                if(*(p + height_index*width/8 + width_index + width/8*3) & (0x80>>z)) dat |= 0x08;
                if(*(p + height_index*width/8 + width_index + width/8*4) & (0x80>>z)) dat |= 0x10;
                if(*(p + height_index*width/8 + width_index + width/8*5) & (0x80>>z)) dat |= 0x20;
                if(*(p + height_index*width/8 + width_index + width/8*6) & (0x80>>z)) dat |= 0x40;
                if(*(p + height_index*width/8 + width_index + width/8*7) & (0x80>>z)) dat |= 0x80;
                oled_write_data(dat);
            }
        }
    }
}

void oled_show_gray_image (uint16_t x, uint16_t y, uint8_t *p, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height, uint8_t threshold)
{
    int16_t i,j;
    uint8_t dat;
    uint32_t width_index = 0, height_index = 0;

    dis_height = dis_height - dis_height%8;

    for(j=0;j<dis_height;j+=8)
    {
        oled_set_coordinate(0,j/8);
        height_index = j*height/dis_height;
        for(i=0;i<dis_width;i++)
        {
            width_index = i*width/dis_width;
            dat = 0;
            if(*(p + height_index*width + width_index + width*0) > threshold) dat |= 0x01;
            if(*(p + height_index*width + width_index + width*1) > threshold) dat |= 0x02;
            if(*(p + height_index*width + width_index + width*2) > threshold) dat |= 0x04;
            if(*(p + height_index*width + width_index + width*3) > threshold) dat |= 0x08;
            if(*(p + height_index*width + width_index + width*4) > threshold) dat |= 0x10;
            if(*(p + height_index*width + width_index + width*5) > threshold) dat |= 0x20;
            if(*(p + height_index*width + width_index + width*6) > threshold) dat |= 0x40;
            if(*(p + height_index*width + width_index + width*7) > threshold) dat |= 0x80;
            oled_write_data(dat);
        }
    }
}

void oled_show_wave (uint16_t x, uint16_t y, uint8_t *p, uint16_t width, uint16_t value_max, uint16_t dis_width, uint16_t dis_value_max)
{
    uint32_t i = 0;
    uint32_t width_index = 0, value_max_index = 0;
    uint8_t dis_h = 0;

    oled_clear(0x00);
    for(i=0;i<dis_width;i++)
    {
        width_index = i*width/dis_width;
        value_max_index = *(p+width_index)*(dis_value_max-1)/value_max;

        dis_h = (dis_value_max-1)-value_max_index;
        oled_set_coordinate(i,dis_h/8);
        dis_h = (0x01 << dis_h%8);
        oled_write_data(dis_h);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ������ʾ
// @param       x               ������ 0-127
// @param       y               ������ 0-7
// @param       size            ȡģ��ʱ�����õĺ��������С��Ҳ����һ������ռ�õĵ��󳤿�Ϊ���ٸ��㣬ȡģ��ʱ����Ҫ������һ���ġ�
// @param       *p              ��Ҫ��ʾ�ĺ�������
// @param       number          ��Ҫ��ʾ����λ
// @return      void
// Sample usage:
// @Note        ʹ��PCtoLCD2002���ȡģ       ���롢����ʽ��˳��       16*16
//-------------------------------------------------------------------------------------------------------------------
void oled_show_chinese (uint16_t x, uint16_t y, uint8_t size, const uint8_t *p, uint8_t number)
{
    int16_t i,j,k;

    for(i=0; i<number; i++)
    {
        for(j=0; j<(size/8); j++)
        {
            oled_set_coordinate(x+i*size,y+j);
            for(k=0; k<16; k++)
            {
                oled_write_data(*p);
                p++;
            }
        }
    }
}

