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
*                   SCL                 �鿴 zf_device_mpu6050.h �� MPU6050_SOFT_IIC_SCL_PIN �궨��
*                   SDA                 �鿴 zf_device_mpu6050.h �� MPU6050_SOFT_IIC_SDA_PIN �궨��
*                   Ӳ�� IIC ͨ�����Ŷ�Ӧ��ϵ
*                   SCL                 �鿴 zf_device_mpu6050.h �� MPU6050_IIC_SCL_PIN �궨��
*                   SDA                 �鿴 zf_device_mpu6050.h �� MPU6050_IIC_SDA_PIN �궨��
*                   ------------------------------------
********************************************************************************************************************/

#ifndef _zf_device_mpu6050_h_
#define _zf_device_mpu6050_h_

#include "zf_common_clock.h"
#include "zf_common_debug.h"

#include "zf_driver_delay.h"

#include "zf_driver_soft_iic.h"

                                                    // ������ ��ɫ�����Ĳ�����ȷ�� ��ɫ�ҵľ���û���õ�
//====================================================��� IIC ����====================================================
#define MPU6050_SOFT_IIC_DELAY      100                                         // ��� IIC ��ʱ����ʱ���� ��ֵԽС IIC ͨ������Խ��
#define MPU6050_SCL_PIN             B3                                          // ��� IIC SCL ���� ���� MPU6050 �� SCL ����
#define MPU6050_SDA_PIN             B5                                          // ��� IIC SDA ���� ���� MPU6050 �� SDA ����
//====================================================��� IIC ����====================================================


#define MPU6050_TIMEOUT_COUNT      0x00FF                                       // MPU6050 ��ʱ����
//================================================���� MPU6050 �ڲ���ַ================================================
#define MPU6050_DEV_ADDR            0xD0>>1                                     // IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ

#define SMPLRT_DIV                  0x19                                        // �����ǲ����ʣ�����ֵ��0x07(125Hz)
#define MPU6050_CONFIG              0x1A                                        // ��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define GYRO_CONFIG                 0x1B                                        // �������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define ACCEL_CONFIG                0x1C                                        // ���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
#define INT_PIN_CFG                 0x37                                        // ����6050����I2CΪֱͨģʽ�Ĵ���
#define ACCEL_XOUT_H                0x3B
#define ACCEL_XOUT_L                0x3C
#define ACCEL_YOUT_H                0x3D
#define ACCEL_YOUT_L                0x3E
#define ACCEL_ZOUT_H                0x3F
#define ACCEL_ZOUT_L                0x40
#define GYRO_XOUT_H                 0x43
#define GYRO_XOUT_L                 0x44
#define GYRO_YOUT_H                 0x45
#define GYRO_YOUT_L                 0x46
#define GYRO_ZOUT_H                 0x47
#define GYRO_ZOUT_L                 0x48
#define USER_CONTROL                0x6A                                        // �ر�6050�Ը���I2C�豸�Ŀ���
#define PWR_MGMT_1                  0x6B                                        // ��Դ��������ֵ��0x00(��������)
#define WHO_AM_I                    0x75                                        // IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)
//================================================���� MPU6050 �ڲ���ַ================================================

extern int16_t mpu_gyro_x,mpu_gyro_y,mpu_gyro_z;
extern int16_t mpu_acc_x,mpu_acc_y,mpu_acc_z;

uint8_t mpu6050_init                (void);
void    mpu6050_get_acc             (void);
void    mpu6050_get_gyro            (void);

#endif
