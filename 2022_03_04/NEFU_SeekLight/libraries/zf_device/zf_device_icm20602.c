/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_device_icm20602
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
*                   //------------------Ӳ�� SPI ����------------------//
*                   SCL/SPC             �鿴 zf_device_icm20602.h �� ICM20602_SPC_PIN �궨��
*                   SDA/DSI             �鿴 zf_device_icm20602.h �� ICM20602_SDI_PIN �궨��
*                   SA0/SDO             �鿴 zf_device_icm20602.h �� ICM20602_SDO_PIN �궨��
*                   CS                  �鿴 zf_device_icm20602.h �� IPS114_CS_PIN �궨��
*                   //------------------Ӳ�� SPI ����------------------//
*                   //------------------��� IIC ����------------------//
*                   SCL/SPC             �鿴 zf_device_icm20602.h �� ICM20602_SCL_PIN �궨��
*                   SDA/DSI             �鿴 zf_device_icm20602.h �� ICM20602_SDA_PIN �궨��
*                   //------------------��� IIC ����------------------//
*                   ��Դ����
*                   VCC                 3.3V��Դ
*                   GND                 ��Դ��
*                   ------------------------------------
********************************************************************************************************************/

#include "zf_device_icm20602.h"

int16_t icm_gyro_x,icm_gyro_y,icm_gyro_z;
int16_t icm_acc_x,icm_acc_y,icm_acc_z;

#if ICM20602_USE_SOFT_IIC
static soft_iic_info_struct icm20602_iic_struct;

#define icm20602_write_register(reg,data)       soft_iic_write_8bit_register(&icm20602_iic_struct,reg,data)
#define icm20602_read_register(reg)             soft_iic_read_8bit_register(&icm20602_iic_struct,reg)
#define icm20602_read_registers(reg,data,len)   soft_iic_read_8bit_registers(&icm20602_iic_struct,reg,data,len)
#else
static void icm20602_write_register(uint8_t reg, uint8_t dat)
{
    ICM20602_CS(0);
    spi_write_8bit_register(ICM20602_SPI, reg, dat);
    ICM20602_CS(1);
}

static uint8_t icm20602_read_register(uint8_t reg)
{
    uint8_t dat;
    ICM20602_CS(0);
    dat = spi_read_8bit_register(ICM20602_SPI, reg|ICM20602_SPI_R);
    ICM20602_CS(1);
    return dat;
}

static void icm20602_read_registers(uint8_t reg, uint8_t *dat, uint8_t len)
{
    ICM20602_CS(0);
    spi_read_8bit_registers(ICM20602_SPI, reg|ICM20602_SPI_R, dat, len);
    ICM20602_CS(1);
}
#endif


//-------------------------------------------------------------------------------------------------------------------
// @brief       ICM20602 �Լ� �ڲ�����
// @param       void
// @return      uint8_t         1-�Լ�ʧ�� 0-�Լ�ɹ�
//-------------------------------------------------------------------------------------------------------------------
static uint8_t icm20602_self_check(void)
{
    uint8_t dat = 0;
    volatile int16_t timeout_count = ICM20602_TIMEOUT_COUNT;

    while(0x12 != dat && timeout_count)                                         // �ж� ID �Ƿ���ȷ
    {
        timeout_count--;
        dat = icm20602_read_register(ICM20602_WHO_AM_I);

        system_delay_ms(10);
    }
    if(timeout_count < 0)
        return 1;
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��ʼ�� ICM20602
// @param       void
// @return      uint8_t         1-��ʼ��ʧ�� 0-��ʼ���ɹ�
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint8_t icm20602_init (void)
{
    uint8_t val = 0x0;
    volatile int16_t timeout_count = ICM20602_TIMEOUT_COUNT;

    system_delay_ms(10);  //�ϵ���ʱ

#if ICM20602_USE_SOFT_IIC
    soft_iic_init(&icm20602_iic_struct, ICM20602_DEV_ADDR, ICM20602_SOFT_IIC_DELAY, ICM20602_SCL_PIN, ICM20602_SDA_PIN);
#else
    spi_init(ICM20602_SPI, SPI_MODE0, ICM20602_SPI_SPEED, ICM20602_SPC_PIN, ICM20602_SDI_PIN, ICM20602_SDO_PIN, ICM20602_CS_PIN);
#endif


    if(icm20602_self_check())
    {
        zf_log(0, "ICM20602 self check error.");
        return 1;
    }

    icm20602_write_register(ICM20602_PWR_MGMT_1, 0x80);                          //��λ�豸
    system_delay_ms(2);


    do                                                                          // �ȴ���λ�ɹ�
    {
        val = icm20602_read_register(ICM20602_PWR_MGMT_1);

        system_delay_ms(10);
    }while(0x41 != val && timeout_count--);
    if(timeout_count < 0)
    {
        zf_log(0, "ICM20602 restart error.");
        return 1;
    }

    icm20602_write_register(ICM20602_PWR_MGMT_1,     0x01);                     // ʱ������
    icm20602_write_register(ICM20602_PWR_MGMT_2,     0x00);                     // ���������Ǻͼ��ٶȼ�
    icm20602_write_register(ICM20602_CONFIG,         0x01);                     // 176HZ 1KHZ
    icm20602_write_register(ICM20602_SMPLRT_DIV,     0x07);                     // �������� SAMPLE_RATE = INTERNAL_SAMPLE_RATE / (1 + SMPLRT_DIV)
    icm20602_write_register(ICM20602_GYRO_CONFIG,    0x18);                     // ��2000 dps
    icm20602_write_register(ICM20602_ACCEL_CONFIG,   0x10);                     // ��8g
    icm20602_write_register(ICM20602_ACCEL_CONFIG_2, 0x03);                     // Average 4 samples   44.8HZ   //0x23 Average 16 samples
	//ICM20602_GYRO_CONFIG�Ĵ���
    //����Ϊ:0x00 ����������Ϊ:��250 dps     ��ȡ�������������ݳ���131           ����ת��Ϊ������λ�����ݣ���λΪ����/s
    //����Ϊ:0x08 ����������Ϊ:��500 dps     ��ȡ�������������ݳ���65.5          ����ת��Ϊ������λ�����ݣ���λΪ����/s
    //����Ϊ:0x10 ����������Ϊ:��1000dps     ��ȡ�������������ݳ���32.8          ����ת��Ϊ������λ�����ݣ���λΪ����/s
    //����Ϊ:0x18 ����������Ϊ:��2000dps     ��ȡ�������������ݳ���16.4          ����ת��Ϊ������λ�����ݣ���λΪ����/s

    //ICM20602_ACCEL_CONFIG�Ĵ���
    //����Ϊ:0x00 ���ٶȼ�����Ϊ:��2g          ��ȡ���ļ��ٶȼ����� ����16384      ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
    //����Ϊ:0x08 ���ٶȼ�����Ϊ:��4g          ��ȡ���ļ��ٶȼ����� ����8192       ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
    //����Ϊ:0x10 ���ٶȼ�����Ϊ:��8g          ��ȡ���ļ��ٶȼ����� ����4096       ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
    //����Ϊ:0x18 ���ٶȼ�����Ϊ:��16g         ��ȡ���ļ��ٶȼ����� ����2048       ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
	
    system_delay_ms(100);
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��ȡ ICM20602 ���ٶȼ�����
// @param       void
// @return      void
// Sample usage:                ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//-------------------------------------------------------------------------------------------------------------------
void icm20602_get_acc (void)
{
    uint8_t reg = ICM20602_ACCEL_XOUT_H;
    uint8_t dat[6];

    icm20602_read_registers(reg, dat, 6);
    icm_acc_x = (int16_t)(((uint16_t)dat[0]<<8 | dat[1]));
    icm_acc_y = (int16_t)(((uint16_t)dat[2]<<8 | dat[3]));
    icm_acc_z = (int16_t)(((uint16_t)dat[4]<<8 | dat[5]));
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��ȡICM20602����������
// @param       void
// @return      void
// Sample usage:                ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//-------------------------------------------------------------------------------------------------------------------
void icm20602_get_gyro (void)
{
    uint8_t reg = ICM20602_GYRO_XOUT_H;
    uint8_t dat[6];

    icm20602_read_registers(reg, dat, 6);
    icm_gyro_x = (int16_t)(((uint16_t)dat[0]<<8 | dat[1]));
    icm_gyro_y = (int16_t)(((uint16_t)dat[2]<<8 | dat[3]));
    icm_gyro_z = (int16_t)(((uint16_t)dat[4]<<8 | dat[5]));
}
