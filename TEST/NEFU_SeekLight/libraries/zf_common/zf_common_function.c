/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
* @file             zf_common_function
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ790875685)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/

#include "zf_common_function.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��ȡ�����������Լ�� ��������֮���������
// @param       num1            ����1
// @param       num2            ����2
// @return      uint32_t        ���Լ��
// Sample usage:                return get_greatest_common_divisor(144, 36);    // ��ȡ 144 �� 36 �����Լ��
//-------------------------------------------------------------------------------------------------------------------
uint32_t get_greatest_common_divisor (uint32_t num1, uint32_t num2)
{
    while(num1 != num2)
    {
        if(num1 > num2)
            num1 = num1 -num2;
        if(num1 < num2)
            num2 = num2 - num1;
    }
    return num1;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ֵ���� ���ݷ�Χ�� [-32767,32767]
// @param       dat             ��Ҫ�����ֵ����
// @return      int             ���ؾ���ֵ
// Sample usage:                dat = myabs(dat);                               // ��dat�������
//-------------------------------------------------------------------------------------------------------------------
//int  myabs (int dat)                                                          // ����鿴 zf_common_function.h

//-------------------------------------------------------------------------------------------------------------------
// @brief       �޷� ���ݷ�Χ�� [-32768,32767]
// @param       x               ���޷�������
// @param       y               �޷���Χ(���ݻᱻ������-y��+y֮��)
// @return      int             �޷�֮�������
// Sample usage:                int dat = limit(500,300);                       // ���ݱ�������-300��+300֮��  ��˷��صĽ����300
//-------------------------------------------------------------------------------------------------------------------
//int limit(int x, int32 y)                                                     // ����鿴 zf_common_function.h

//-------------------------------------------------------------------------------------------------------------------
// @brief       ˫���޷� ���ݷ�Χ�� [-32768,32767]
// @param       x               ���޷�������
// @param       a               �޷���Χ��߽�
// @param       b               �޷���Χ�ұ߽�
// @return      int             �޷�֮�������
// Sample usage:                int dat = limit_ab(500,-300,400);               //���ݱ�������-300��+400֮��  ��˷��صĽ����400
//-------------------------------------------------------------------------------------------------------------------
//int limit_ab(int x, int a, int b)                                             // ����鿴 zf_common_function.h

//-------------------------------------------------------------------------------------------------------------------
// @brief       �����ʱ
// @param       t               ��ʱʱ��
// @return      void
// Sample usage:                my_delay(100);                                  // ��ʼ������ʱ��Ϊ 144MHz
//-------------------------------------------------------------------------------------------------------------------
void my_delay (volatile unsigned long t)
{
    //system_delay_ms(t);
    volatile unsigned long time = t*18000;
    while(time--);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       �ַ���ת�������� ���ݷ�Χ�� [-32768,32767]
// @param       str             �����ַ��� �ɴ�����
// @return      int             ת���������
// Sample usage:                int dat = str_to_int("-100");                   // ������ dat = -100
//-------------------------------------------------------------------------------------------------------------------
int32_t str_to_int32 (char *str)
{
    uint8_t sign = 0;
    int32_t temp=0;

    if(*str == '-')
    {
        sign = 1;
        str ++;
    }
    else if(*str == '+')
    {
        str ++;
    }

    while((*str >= '0')&&(*str <= '9'))
    {
        temp = temp*10+((uint8_t)(*str)-0x30);
        str ++;
    }

    if(sign)
        return -temp;
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       �ַ���ת�������� ���ݷ�Χ�� [0,65535]
// @param       str             �����ַ��� �޷���
// @return      int             ת���������
// Sample usage:                int dat = str_to_int("100");                   // ������ dat = 100
//-------------------------------------------------------------------------------------------------------------------
uint32_t str_to_uint32 (char *str)
{
    uint32_t temp=0;

    while((*str >= '0')&&(*str <= '9'))
    {
        temp = temp*10+((uint8_t)(*str)-0x30);
        str ++;
    }

    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��������ת�ַ��� ���ݷ�Χ�� [-32768,32767]
// @param       *str            �ַ���ָ��
// @param       number          ���������
// @return      NULL
// Sample usage:                int_to_str(data_buffer, -300);                  // ������ data_buffer = "-300"
//-------------------------------------------------------------------------------------------------------------------
void int32_to_str (char *str, int32_t number)
{
    uint8_t data_temp[16];
    uint8_t bit = 0;

    if(number < 0)
    {
        *str++ = '-';
    }
    else if(number == 0)
    {
        *str = '0';
        return;
    }

    // int [-32768,32767]
    while(number != 0)
    {
        data_temp[bit++] = myabs(number%10);
        number /= 10;
    }
    while(bit != 0)
    {
        *str++ = (data_temp[bit-1]+0x30);
        bit--;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��������ת�ַ��� ���ݷ�Χ�� [0,65535]
// @param       *str            �ַ���ָ��
// @param       number          ���������
// @return      NULL
// Sample usage:                int_to_str(data_buffer, 300);                  // ������ data_buffer = "300"
//-------------------------------------------------------------------------------------------------------------------
void uint32_to_str (char *str, uint32_t number)
{
    uint8_t data_temp[16];
    uint8_t bit = 0;

    if(number == 0)
    {
        *str = '0';
        return;
    }

    while(number != 0)
    {
        data_temp[bit++] = number%10;
        number /= 10;
    }
    while(bit != 0)
    {
        *str++ = (myabs(data_temp[bit-1])+0x30);
        bit--;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       �ַ���ת������
// @param       str             �����ַ��� �ɴ�����
// @return      float           ת���������
// Sample usage:                float dat = str_to_int("-100.2");               // ������ dat = -100.2
//-------------------------------------------------------------------------------------------------------------------
float str_to_float (char *str)
{
    uint8_t sign = 0;
    float temp = 0.0;
    float temp_point = 0.0;
    uint8_t point_bit = 0;

    if(*str == '-')
    {
        sign = 1;
        str ++;
    }
    else if(*str == '+')
    {
        str ++;
    }

    while((*str >= '0')&&(*str <= '9'))
    {
        temp = temp*10+((uint8_t)(*str)-0x30);
        str ++;
    }
    if(*str == '.')
    {
        str++;
        while((*str >= '0')&&(*str <= '9'))
        {
            temp_point = temp_point*10+((uint8_t)(*str)-0x30);
            point_bit++;
            str ++;
        }
        while(point_bit--)
        {
            temp_point /= 10;
        }
    }
    temp += temp_point;

    if(sign)
        return -temp;
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��������ת�ַ���
// @param       *str            �ַ���ָ��
// @param       number          ���������
// @param       point_bit       С���㾫��
// @return      NULL
// Sample usage:                float_to_str(data_buffer, 3.1415, 2);           // ������ data_buffer = "3.14"
//-------------------------------------------------------------------------------------------------------------------
void float_to_str (char *str, float number, uint8_t point_bit)
{
    int data_int = 0;
    int data_float = 0.0;
    int data_temp[5];
    int data_temp_point[5];
    uint8_t bit = 0;

    data_int = (int)number;
    if(number < 0)
    {
        *str++ = '-';
    }
    else if(number == 0.0)
    {
        *str = '0';
        return;
    }

    number = number - data_int;
    while(point_bit--)
    {
        number = number*10;
    }
    data_float = (int)number;

    while(data_int != 0)
    {
        data_temp[bit++] = data_int%10;
        data_int /= 10;
    }
    while(bit != 0)
    {
        *str++ = (myabs(data_temp[bit-1])+0x30);
        bit--;
    }
    *str++ = '.';
    if(data_float == 0)
        *str = '0';
    else
    {
        while(data_float != 0)
        {
            data_temp_point[bit++] = data_float%10;
            data_float /= 10;
        }
        while(bit != 0)
        {
            *str++ = (myabs(data_temp_point[bit-1])+0x30);
            bit--;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ת��Ϊ ASCII ֵ �ڲ�����
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
static uint8_t number_conversion_ascii(uint32_t dat, int8_t *p, uint8_t neg_type, uint8_t radix)
{
    int32_t   neg_dat;
    uint32_t  pos_dat;
    uint8_t   temp_data = 0;
    uint8_t   valid_num = 0;

    if(neg_type)
    {
        neg_dat = (int32_t)dat;
        if(neg_dat<0)   neg_dat = -neg_dat;
        while(1)
        {
            *p = neg_dat%radix + '0';
            neg_dat = neg_dat/radix;
            valid_num++;

            if(!neg_dat) break;
            p++;
        }
    }
    else
    {
        pos_dat = dat;
        while(1)
        {
            temp_data = pos_dat%radix;
            if(10 <= temp_data) temp_data += 'A'-10;
            else                temp_data += '0';

            *p = temp_data;

            pos_dat = pos_dat/radix;
            valid_num++;

            if(!pos_dat) break;
            p++;
        }
    }
    return valid_num;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       printf ��ʾת�� �ڲ�����
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
static void printf_reverse_order(int8_t *d_buff, uint32_t len)
{
    uint32_t i;
    int8_t  temp_data;
    for(i=0;i<len/2;i++)
    {
        temp_data = d_buff[len-1-i];
        d_buff[len-1-i] = d_buff[i];
        d_buff[i] = temp_data; 
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       sprintf ����ʵ��
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint32_t zf_sprintf(int8_t *buff, const int8_t *format, ...)
{
    uint32_t buff_len=0;
    va_list arg;
    va_start(arg, format);

    while (*format)
    {
        int8_t ret = *format;
        if (ret == '%')
        {
            switch (*++format)
            {
                case 'a':// ʮ������p��������������� ��δʵ��
                    {
                    }
                    break;

                case 'c':// һ���ַ�
                    {
                        int8_t ch = (int8_t)va_arg(arg, uint32_t);
                        *buff = ch;
                        buff++;
                        buff_len++;
                    }
                    break;

                case 'd':
                case 'i':// �з���ʮ��������
                    {
                        int8_t vstr[33];
                        int32_t ival = (int32_t)va_arg(arg, int32_t);
                        uint8_t vlen = number_conversion_ascii((uint32_t)ival, vstr, 1, 10);

                        if(ival<0)  
                        {
                            vstr[vlen] = '-';
                            vlen++;
                        }
                        printf_reverse_order(vstr,vlen);
                        memcpy(buff,vstr,vlen);
                        buff += vlen;
                        buff_len += vlen;
                    }
                    break;

                case 'f':// �����������С�������λ  ����ָ���������
                case 'F':// �����������С�������λ  ����ָ���������
                {
                    int8_t vstr[33];
                    double ival = (double)va_arg(arg, double);
                    uint8_t vlen = number_conversion_ascii((uint32_t)(int32_t)ival, vstr, 1, 10);

                    if(ival<0)  
                    {
                        vstr[vlen] = '-';
                        vlen++;
                    }
                    printf_reverse_order(vstr,vlen);
                    memcpy(buff,vstr,vlen);
                    buff += vlen;
                    buff_len += vlen;

                    ival = ((double)ival - (int32_t)ival)*1000000;
                    if(ival)
                    {
                        vlen = number_conversion_ascii((uint32_t)(int32_t)ival, vstr, 1, 10);
                    }
                    else
                    {
                        vstr[0] = vstr[1] = vstr[2] = vstr[3] = vstr[4] = vstr[5] = '0';
                        vlen = 6;
                    }

                    while(6>vlen)
                    {
                        vstr[vlen] = '0';
                        vlen++;
                    }

                    vstr[vlen] = '.';
                    vlen++;

                    printf_reverse_order(vstr,vlen);
                    memcpy(buff,vstr,vlen);
                    buff_len += vlen;
                }
                break;

                case 'u':// �޷���ʮ��������
                    {
                        int8_t vstr[33];
                        uint32_t ival = (uint32_t)va_arg(arg, uint32_t);
                        uint8_t vlen = number_conversion_ascii(ival, vstr, 0, 10);

                        printf_reverse_order(vstr,vlen);
                        memcpy(buff,vstr,vlen);
                        buff += vlen;
                        buff_len += vlen;
                    }
                    break;

                case 'o':// �޷��Ű˽�������
                    {
                        int8_t vstr[33];
                        uint32_t ival = (uint32_t)va_arg(arg, uint32_t);
                        uint8_t vlen = number_conversion_ascii(ival, vstr, 0, 8);

                        printf_reverse_order(vstr,vlen);
                        memcpy(buff,vstr,vlen);
                        buff += vlen;
                        buff_len += vlen;

                    }
                    break;

                case 'x':// �޷���ʮ����������
                case 'X':// �޷���ʮ����������
                    {
                        int8_t vstr[33];
                        uint32_t ival = (uint32_t)va_arg(arg, uint32_t);
                        uint8_t vlen = number_conversion_ascii(ival, vstr, 0, 16);

                        printf_reverse_order(vstr,vlen);
                        memcpy(buff,vstr,vlen);
                        buff += vlen;
                        buff_len += vlen;
                    }
                    break;

                case 's':// �ַ���
                    {
                        int8_t *pc = va_arg(arg, int8_t *);
                        while (*pc)
                        {
                            *buff = *pc;
                            buff++;
                            buff_len++;
                            pc++;
                        }
                    }
                    break;

                case 'p':// ��16������ʽ���ָ��
                    {
                        int8_t vstr[33];
                        uint32_t ival = (uint32_t)va_arg(arg, uint32_t);
                        uint8_t vlen = number_conversion_ascii(ival, vstr, 0, 16);
                        vlen = vlen*1;
                        printf_reverse_order(vstr,8);
                        memcpy(buff,vstr,8);
                        buff += 8;
                        buff_len += 8;
                    }
                    break;

                case '%':// ����ַ�%
                    {
                        *buff = '%';
                        buff++;
                        buff_len++;
                    }
                    break;

                default:
                    break;
            }
        }
        else
        {
            *buff = (int8_t)(*format);
            buff++;
            buff_len++;
        }
        format++;
    }
    va_end(arg);

    return buff_len;
}




