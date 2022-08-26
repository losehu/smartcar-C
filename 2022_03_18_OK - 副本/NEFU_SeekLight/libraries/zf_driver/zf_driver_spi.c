/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            zf_driver_spi
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ790875685)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/


#include "zf_driver_gpio.h"
#include "zf_driver_spi.h"


const uint32_t spi_index[3] = {SPI1_BASE, SPI2_BASE, SPI3_BASE};
//-------------------------------------------------------------------------------------------------------------------
//  @brief      SPI���ų�ʼ��
//  @param      sck_pin     SCK����(��ѡ��Χ��zf_spi.h��SPI_PIN_enumö��ֵȷ��)
//  @param      mosi_pin    MOSI����(��ѡ��Χ��zf_spi.h��SPI_PIN_enumö��ֵȷ��)
//  @param      miso_pin    MISO����(��ѡ��Χ��zf_spi.h��SPI_PIN_enumö��ֵȷ��)
//  @param      cs_pin      Ƭѡ����(��ѡ��Χ��common.h��PIN_enumö��ֵȷ��)
//  @return     void
//  Sample usage:           �ڲ�ʹ��  �û��������
//-------------------------------------------------------------------------------------------------------------------
void spi_gpio_init(spi_pin_enum sck_pin, spi_pin_enum mosi_pin, spi_pin_enum miso_pin, gpio_pin_enum cs_pin)
{
    if(SPI1_SCK_A5        == sck_pin)    gpio_init(A5,  GPO, 1, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(SPI1_SCK_B3   == sck_pin)    gpio_init(B3,  GPO, 1, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(SPI2_SCK_B13  == sck_pin)    gpio_init(B13, GPO, 1, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(SPI3_SCK_B3   == sck_pin)    gpio_init(B3,  GPO, 1, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(SPI3_SCK_C10  == sck_pin)    gpio_init(C10, GPO, 1, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else zf_assert(0);         //�޴˲����������

    if(SPI1_MOSI_A7       == mosi_pin)   gpio_init(A7,  GPO, 1, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(SPI1_MOSI_B5  == mosi_pin)   gpio_init(B5,  GPO, 1, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(SPI2_MOSI_B15 == mosi_pin)   gpio_init(B15, GPO, 1, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(SPI3_MOSI_B5  == mosi_pin)   gpio_init(B5,  GPO, 1, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else if(SPI3_MOSI_C12 == mosi_pin)   gpio_init(C12, GPO, 1, SPEED_50MHZ|GPO_AF_PUSH_PULL);
    else zf_assert(0);         //�޴˲����������

    if(SPI1_MISO_A6       == miso_pin)   gpio_init(A6,  GPI, 1, SPEED_50MHZ|GPI_FLOATING_IN);
    else if(SPI1_MISO_B4  == miso_pin)   gpio_init(B4,  GPI, 1, SPEED_50MHZ|GPI_FLOATING_IN);
    else if(SPI2_MISO_B14 == miso_pin)   gpio_init(B14, GPI, 1, SPEED_50MHZ|GPI_FLOATING_IN);
    else if(SPI3_MISO_C11 == miso_pin)   gpio_init(C11, GPI, 1, SPEED_50MHZ|GPI_FLOATING_IN);
    else if(SPI3_MISO_B4  == miso_pin)   gpio_init(B4,  GPI, 1, SPEED_50MHZ|GPI_FLOATING_IN);
    else if(SPI_MISO_NULL == miso_pin)   miso_pin = SPI_MISO_NULL;
    else  zf_assert(0);         //�޴˲����������

    //��ʼ��CS����
    gpio_init(cs_pin, GPO, 0, SPEED_50MHZ|GPO_PUSH_PULL);

}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI �ӿڳ�ʼ��
// @param       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// @param       mode            SPI ģʽ ���� zf_driver_spi.h �� spi_mode_enum ö���嶨��
// @param       baud            ���� SPI �Ĳ����� ������ϵͳʱ�ӵ�һ�� �������ʻᱻ��������������
// @param       sck_pin         ѡ�� SCK ���� ���� zf_driver_spi.h �� spi_pin_enum ö���嶨��
// @param       mosi_pin        ѡ�� MOSI ���� ���� zf_driver_spi.h �� spi_pin_enum ö���嶨��
// @param       miso_pin        ѡ�� MISO ���� ���� zf_driver_spi.h �� spi_pin_enum ö���嶨��
// @param       cs_pin          ѡ�� CS ���� ���� zf_driver_gpio.h �� gpio_pin_enum ö���嶨��
// @return      void
// Sample usage:                spi_init(SPI_1, 0, 1*1000*1000, SPI1_SCK_A5, SPI1_MOSI_A7, SPI1_MISO_A6, A4);  //Ӳ��SPI��ʼ��  ģʽ0 ������Ϊ1Mhz
//-------------------------------------------------------------------------------------------------------------------
void spi_init(spi_index_enum spi_n, spi_mode_enum mode, uint32_t baud, spi_pin_enum sck_pin, spi_pin_enum mosi_pin, spi_pin_enum miso_pin, gpio_pin_enum cs_pin)
{
    uint16_t psc = 0;
    SPI_InitTypeDef  SPI_InitStructure;


    // SCK MISO MOSI���ű�����鴫��
//    if(((mosi_pin - 2) != sck_pin))
//    {
//        assert(0);
//        return;
//    }

    SPI_I2S_DeInit((SPI_TypeDef *)(spi_index[spi_n]));

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);                                           // ��������ʹ��

    if(sck_pin == SPI1_SCK_B3 && mosi_pin == SPI1_MOSI_B5 && miso_pin == SPI1_MISO_B4)
        GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);
    else if(sck_pin == SPI3_SCK_C10 && mosi_pin == SPI3_MOSI_C12 && miso_pin == SPI3_MISO_C11)
        GPIO_PinRemapConfig(GPIO_Remap_SPI3, ENABLE);


         if(SPI_1 == spi_n)   RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);                // SPI1ʱ��ʹ��
    else if(SPI_2 == spi_n)   RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);                // SPI2ʱ��ʹ��
    else if(SPI_3 == spi_n)   RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);                // SPI3ʱ��ʹ��
    spi_gpio_init(sck_pin, mosi_pin, miso_pin, cs_pin);


    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;      // SPI����Ϊ˫��˫��ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                           // ����SPI����ģʽ:����Ϊ��SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                       // ����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ

    switch(mode)
    {
        case SPI_MODE0:
        {
            SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                      // ����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
            SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                    // ����ͬ��ʱ�ӵĵ�һ�������أ��������½������ݱ�����
        }break;
        case SPI_MODE1:
        {
            SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                      // ����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
            SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                    // ����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
        }break;
        case SPI_MODE2:
        {
            SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                     // ����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
            SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                    // ����ͬ��ʱ�ӵĵ�һ�������أ��������½������ݱ�����
        }break;
        case SPI_MODE3:
        {
            SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                     // ����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
            SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                    // ����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
        }break;
    }

    psc = system_clock / baud;                                              // �����Ƶֵ
         if(psc >= 256) psc = SPI_BaudRatePrescaler_256;                    // ������ѷ�Ƶֵ
    else if(psc >= 128) psc = SPI_BaudRatePrescaler_128;
    else if(psc >= 64)  psc = SPI_BaudRatePrescaler_64;
    else if(psc >= 32)  psc = SPI_BaudRatePrescaler_32;
    else if(psc >= 16)  psc = SPI_BaudRatePrescaler_16;
    else if(psc >= 8)   psc = SPI_BaudRatePrescaler_8;
    else if(psc >= 4)   psc = SPI_BaudRatePrescaler_4;
    else if(psc >= 2)   psc = SPI_BaudRatePrescaler_2;
    else zf_assert(0);
    SPI_InitStructure.SPI_BaudRatePrescaler = psc;                          // ���岨����Ԥ��Ƶ��ֵ
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                               // ����ģʽ��ʹ���������NSS����
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                      // ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI_InitStructure.SPI_CRCPolynomial = 7;                                // CRCֵ����Ķ���ʽ
    SPI_Init((SPI_TypeDef *)(spi_index[spi_n]), &SPI_InitStructure);        // ����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���


    SPI_Cmd(((SPI_TypeDef *)(spi_index[spi_n])), ENABLE);                   // ʹ��SPI����
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI �ӿ�д 8bit ����
// @param       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// @param       data            ����
// @return      void
// Sample usage:                spi_write_8bit(SPI_1,0x11);
//-------------------------------------------------------------------------------------------------------------------
void spi_write_8bit (spi_index_enum spi_n, uint8_t dat)
{
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = dat;                                  // ��������
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);    // ����Ϊ��
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI �ӿ�д 8bit ����
// @param       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// @param       *data           ���ݴ�Ż�����
// @param       len             ����������
// @return      void
// Sample usage:                spi_write_8bit_array(SPI_1,data,64);
//-------------------------------------------------------------------------------------------------------------------
void spi_write_8bit_array (spi_index_enum spi_n, uint8_t *dat, uint32_t len)
{
    while(len--)
    {
        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = *(dat++);
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI �ӿ�д 16bit ����
// @param       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// @param       data            ����
// @return      void
// Sample usage:                spi_write_16bit(SPI_1,0x1101);
//-------------------------------------------------------------------------------------------------------------------
void spi_write_16bit (spi_index_enum spi_n, uint16_t dat)
{
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)((dat & 0xFF00)>>8);
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);

    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)(dat & 0x00FF);
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI �ӿ�д 16bit ����
// @param       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// @param       *data           ���ݴ�Ż�����
// @param       len             ����������
// @return      void
// Sample usage:                spi_write_16bit_array(SPI_1,data,64);
//-------------------------------------------------------------------------------------------------------------------
void spi_write_16bit_array (spi_index_enum spi_n, uint16_t *dat, uint32_t len)
{
    while(len--)
    {
        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)((*dat & 0xFF00)>>8);
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)(*dat++ & 0x00FF);
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI �ӿ��򴫸����ļĴ���д 8bit ����
// @param       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// @param       register_name   �Ĵ�����ַ
// @param       data            ����
// @return      void
// Sample usage:                spi_write_8bit_register(SPI_1,0x11,0x01);
//-------------------------------------------------------------------------------------------------------------------
void spi_write_8bit_register (spi_index_enum spi_n, uint8_t register_name, uint8_t dat)
{
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = register_name;
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY));
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;

    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = dat;
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY));
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI �ӿ��򴫸����ļĴ���д 8bit ����
// @param       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// @param       register_name   �Ĵ�����ַ
// @param       *data           ���ݴ�Ż�����
// @param       len             ����������
// @return      void
// Sample usage:                spi_write_8bit_registers(SPI_1,0x11,data,32);
//-------------------------------------------------------------------------------------------------------------------
void spi_write_8bit_registers (spi_index_enum spi_n, uint8_t register_name, uint8_t *dat, uint32_t len)
{
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = register_name;
       while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);

    while(len--)
    {
        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = *dat++;
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI �ӿ��򴫸����ļĴ���д 16bit ����
// @param       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// @param       register_name   �Ĵ�����ַ
// @param       data            ����
// @return      void
// Sample usage:                spi_write_16bit_register(SPI_1,0x1011,0x0101);
//-------------------------------------------------------------------------------------------------------------------
void spi_write_16bit_register (spi_index_enum spi_n, uint16_t register_name, uint16_t dat)
{

    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)((register_name & 0xFF00)>>8);
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)(register_name & 0x00FF);
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);

    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)((dat & 0xFF00)>>8);
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)(dat & 0x00FF);
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);

}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI �ӿ��򴫸����ļĴ���д 16bit ����
// @param       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// @param       register_name   �Ĵ�����ַ
// @param       *data           ���ݴ�Ż�����
// @param       len             ����������
// @return      void
// Sample usage:                spi_write_16bit_registers(SPI_1,0x1011,data,32);
//-------------------------------------------------------------------------------------------------------------------
void spi_write_16bit_registers (spi_index_enum spi_n, uint16_t register_name, uint16_t *dat, uint32_t len)
{
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)((register_name & 0xFF00)>>8);
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)(register_name & 0x00FF);
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);

    while(len--)
    {
        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)((*dat & 0xFF00)>>8);
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)(*dat++ & 0x00FF);
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
    }

}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI �ӿڶ� 8bit ����
// @param       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// @param       register_name   �Ĵ�����ַ
// @return      uint8_t         ����
// Sample usage:                spi_read_8bit(SPI_1);
//-------------------------------------------------------------------------------------------------------------------
uint8_t spi_read_8bit (spi_index_enum spi_n)
{
    uint8_t dat = 0;

    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = 0;
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);

    //while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_RXNE) == RESET);
    dat = ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;

    return dat;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI �ӿڶ� 8bit ����
// @param       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// @param       *data           ���ݴ�Ż�����
// @param       len             ���ͻ���������
// @return      void
// Sample usage:                spi_read_8bit_array(SPI_1,data,64);
//-------------------------------------------------------------------------------------------------------------------
void spi_read_8bit_array (spi_index_enum spi_n, uint8_t *dat, uint32_t len)
{
    while(len--)
    {
        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = 0;
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
        //while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_RXNE) == RESET);
        *dat++ = ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI �ӿڶ� 16bit ����
// @param       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// @param       register_name   �Ĵ�����ַ
// @return      uint16_t        ����
// Sample usage:                spi_read_16bit(SPI_1);
//-------------------------------------------------------------------------------------------------------------------
uint16_t spi_read_16bit (spi_index_enum spi_n)
{
    uint16_t dat = 0;

    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = 0;
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
    dat = ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;

    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = 0;
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
    dat = ((dat << 8)| ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR);

    return dat;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI �ӿڶ� 16bit ����
// @param       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// @param       *data           ���ݴ�Ż�����
// @param       len             ���ͻ���������
// @return      void
// Sample usage:                spi_read_16bit_array(SPI_1,data,64);
//-------------------------------------------------------------------------------------------------------------------
void spi_read_16bit_array (spi_index_enum spi_n, uint16_t *dat, uint32_t len)
{
    while(len--)
    {
        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = 0;
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
        //while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_RXNE) == RESET);
        *dat = ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;

        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = 0;
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
        //while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_RXNE) == RESET);
        *dat = ((*dat << 8)| ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR);
        dat++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI �ӿڴӴ������ļĴ����� 8bit ����
// @param       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// @param       register_name   �Ĵ�����ַ
// @return      uint8_t         ����
// Sample usage:                spi_read_8bit_register(SPI_1,0x11);
 //-------------------------------------------------------------------------------------------------------------------
uint8_t spi_read_8bit_register (spi_index_enum spi_n, uint8_t register_name)
{
    uint8_t dat;

    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = register_name;
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY));
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;


    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = 0;
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY));
    dat = ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;

    return dat;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI �ӿڴӴ������ļĴ����� 8bit ����
// @param       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// @param       register_name   �Ĵ�����ַ
// @param       *data           ���ݴ�Ż�����
// @param       len             ���ͻ���������
// @return      void
// Sample usage:                spi_read_8bit_registers(SPI_1,0x11,data,32);
//-------------------------------------------------------------------------------------------------------------------
void spi_read_8bit_registers (spi_index_enum spi_n, uint8_t register_name, uint8_t *dat, uint32_t len)
{
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = register_name;
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY));
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;

    while(len--)
    {
        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = 0;
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY));
        *dat++ = ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI �ӿڴӴ������ļĴ����� 16bit ����
// @param       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// @param       register_name   �Ĵ�����ַ
// @return      uint16_t        ����
// Sample usage:                spi_read_16bit_register(SPI_1,0x1011);
//-------------------------------------------------------------------------------------------------------------------
uint16_t spi_read_16bit_register (spi_index_enum spi_n, uint16_t register_name)
{

    uint16_t dat = 0;

    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)((register_name & 0xFF00)>>8);
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);

    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)(register_name & 0x00FF);
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);

    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = 0;
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
    //while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_RXNE) == RESET);
    dat = ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;

    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = 0;
     while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
     //while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_RXNE) == RESET);
     dat = ((dat << 8)|  ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR);

     return dat;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI �ӿڴӴ������ļĴ����� 16bit ����
// @param       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// @param       register_name   �Ĵ�����ַ
// @param       *data           ���ݴ�Ż�����
// @param       len             ���ͻ���������
// @return      void
// Sample usage:                spi_read_16bit_registers(SPI_1,0x1101,data,32);
//-------------------------------------------------------------------------------------------------------------------
void spi_read_16bit_registers (spi_index_enum spi_n, uint16_t register_name, uint16_t *dat, uint32_t len)
{
    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)((register_name & 0xFF00)>>8);
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);

    ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (uint8_t)(register_name & 0x00FF);
    while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);

    while(len--)
    {
        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = 0;
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
        //while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_RXNE) == RESET);
        *dat = ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;

        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = 0;
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
        //while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_RXNE) == RESET);
        *dat = ((*dat << 8)|  ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR);
        dat++;
    }

}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI 8bit ���ݴ��� ���������������ͬʱ���е�
// @param       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// @param       write_buffer    ���͵����ݻ�������ַ
// @param       read_buffer     ��������ʱ���յ������ݵĴ洢��ַ(����Ҫ������ NULL)
// @param       len             ����������
// @return      void
// Sample usage:                spi_transfer_8bit(SPI_1,buf,buf,1);
//-------------------------------------------------------------------------------------------------------------------
void spi_transfer_8bit (spi_index_enum spi_n, uint8_t *write_buffer, uint8_t *read_buffer, uint32_t len)
{
    while(len--)
    {
        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = *(write_buffer++);
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
        if(read_buffer != NULL)
        {
            //while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_RXNE) == RESET);
            *read_buffer++ = ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI 16bit ���ݴ��� ���������������ͬʱ���е�
// @param       spi_n           SPI ģ��� ���� zf_driver_spi.h �� spi_index_enum ö���嶨��
// @param       write_buffer    ���͵����ݻ�������ַ
// @param       read_buffer     ��������ʱ���յ������ݵĴ洢��ַ(����Ҫ������ NULL)
// @param       len             ����������
// @return      void
// Sample usage:                spi_transfer_16bit(SPI_1,buf,buf,1);
//-------------------------------------------------------------------------------------------------------------------
void spi_transfer_16bit (spi_index_enum spi_n, uint16_t *write_buffer, uint16_t *read_buffer, uint32_t len)
{
    while(len--)
    {
        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (*write_buffer&0xFF00)>>8;
        while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
        if(read_buffer != NULL)
        {
            //while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_RXNE) == RESET);
            *read_buffer++ = ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;
        }

        ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR = (*write_buffer&0x00FF);
         while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_BSY) != RESET);
         if(read_buffer != NULL)
         {
             //while((((SPI_TypeDef *)(spi_index[spi_n]))->STATR & SPI_I2S_FLAG_RXNE) == RESET);
             *read_buffer = ((SPI_TypeDef *)(spi_index[spi_n]))->DATAR;
         }
         write_buffer++;
         read_buffer++;
    }
}

