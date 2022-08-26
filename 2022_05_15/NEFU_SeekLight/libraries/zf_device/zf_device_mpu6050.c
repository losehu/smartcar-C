/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_device_mpu6050
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
*                   ��� IIC ͨ�����Ŷ�Ӧ��ϵ
*                   SCL                 �鿴 zf_device_mpu6050.h �� MPU6050_SOFT_IIC_SCL �궨��
*                   SDA                 �鿴 zf_device_mpu6050.h �� MPU6050_SOFT_IIC_SDA �궨��
*                   Ӳ�� IIC ͨ�����Ŷ�Ӧ��ϵ
*                   SCL                 �鿴 zf_device_mpu6050.h �� MPU6050_IIC_SCL �궨��
*                   SDA                 �鿴 zf_device_mpu6050.h �� MPU6050_IIC_SDA �궨��
*                   ------------------------------------
********************************************************************************************************************/

#include "zf_device_mpu6050.h"

int16_t mpu_gyro_x,mpu_gyro_y,mpu_gyro_z;
int16_t mpu_acc_x,mpu_acc_y,mpu_acc_z;


static soft_iic_info_struct mpu6050_iic_struct;
#define mpu6050_write_register(reg,data)       soft_iic_write_8bit_register(&mpu6050_iic_struct,reg,data)
#define mpu6050_read_register(reg)             soft_iic_read_8bit_register(&mpu6050_iic_struct,reg)
#define mpu6050_read_registers(reg,data,len)   soft_iic_read_8bit_registers(&mpu6050_iic_struct,reg,data,len)

//-------------------------------------------------------------------------------------------------------------------
// @brief       MPU6050 �Լ� �ڲ�����
// @param       void
// @return      uint8_t         1-�Լ�ʧ�� 0-�Լ�ɹ�
//-------------------------------------------------------------------------------------------------------------------
static uint8_t mpu6050_self1_check(void)
{
    uint8_t dat = 0;
    volatile int16_t timeout_count = MPU6050_TIMEOUT_COUNT;

    mpu6050_write_register(PWR_MGMT_1, 0x00);                                   // �������״̬
    mpu6050_write_register(SMPLRT_DIV, 0x07);                                   // 125HZ������
    while(0x07 != dat && timeout_count)
    {
        timeout_count--;
        dat = mpu6050_read_register(SMPLRT_DIV);
        system_delay_ms(10);
    }

    if(timeout_count < 0)
        return 1;
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��ʼ�� MPU6050
// @param       void
// @return      uint8_t         1-��ʼ��ʧ�� 0-��ʼ���ɹ�
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint8_t mpu6050_init (void)
{

    soft_iic_init(&mpu6050_iic_struct, MPU6050_DEV_ADDR, MPU6050_SOFT_IIC_DELAY, MPU6050_SCL_PIN, MPU6050_SDA_PIN);

    system_delay_ms(100);                                                       // �ϵ���ʱ

    if(mpu6050_self1_check())
    {
        zf_log(0, "MPU6050 self check error.");
        return 1;
    }
    mpu6050_write_register(PWR_MGMT_1, 0x00);                                   // �������״̬
    mpu6050_write_register(SMPLRT_DIV, 0x07);                                   // 125HZ������
    mpu6050_write_register(MPU6050_CONFIG, 0x04);
    mpu6050_write_register(GYRO_CONFIG, 0x18);                                  // 2000
    mpu6050_write_register(ACCEL_CONFIG, 0x10);                                 // 8g
    mpu6050_write_register(USER_CONTROL, 0x00);
    mpu6050_write_register(INT_PIN_CFG, 0x02);
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��ȡMPU6050���ٶȼ�����
// @param       NULL
// @return      void
// Sample usage:                ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//-------------------------------------------------------------------------------------------------------------------
void mpu6050_get_acc (void)
{
    uint8_t dat[6];

    mpu6050_read_registers(ACCEL_XOUT_H, dat, 6);  
    mpu_acc_x = (int16_t)(((uint16_t)dat[0]<<8 | dat[1]));
    mpu_acc_y = (int16_t)(((uint16_t)dat[2]<<8 | dat[3]));
    mpu_acc_z = (int16_t)(((uint16_t)dat[4]<<8 | dat[5]));
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��ȡMPU6050����������
// @param       NULL
// @return      void
// Sample usage:                ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//-------------------------------------------------------------------------------------------------------------------
void mpu6050_get_gyro (void)
{
    uint8_t dat[6];

    mpu6050_read_registers(GYRO_XOUT_H, dat, 6);  
    mpu_gyro_x = (int16_t)(((uint16_t)dat[0]<<8 | dat[1]));
    mpu_gyro_y = (int16_t)(((uint16_t)dat[2]<<8 | dat[3]));
    mpu_gyro_z = (int16_t)(((uint16_t)dat[4]<<8 | dat[5]));
}
