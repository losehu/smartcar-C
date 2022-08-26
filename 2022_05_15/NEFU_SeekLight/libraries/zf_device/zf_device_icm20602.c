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
bool icm_init_flag=0;

  int16_t  gyro_x_tmp ;
  int16_t  gyro_y_tmp;
  int16_t  gyro_z_tmp ;
  int16_t  acc_x_tmp ;
  int16_t  acc_y_tmp;
  int16_t  acc_z_tmp ;

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

        my_delay(10);
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

    my_delay(10);  //�ϵ���ʱ

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
    my_delay(2);


    do                                                                          // �ȴ���λ�ɹ�
    {
        val = icm20602_read_register(ICM20602_PWR_MGMT_1);

        my_delay(10);
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
	
    my_delay(100);
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
    ICM_State.accdata.x = icm_acc_x;
      ICM_State.accdata.y = icm_acc_y;
      ICM_State.accdata.z = icm_acc_z;
      if(icm_init_flag==0)
      {
          acc_x_tmp = icm_acc_x;
          acc_y_tmp = icm_acc_y;
          acc_z_tmp = icm_acc_z;
      }
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
    ICM_State.gyro.x = icm_gyro_x;
    ICM_State.gyro.y = icm_gyro_y;
    ICM_State.gyro.z = icm_gyro_z;
    if(icm_init_flag==0)
       {
        gyro_x_tmp = icm_gyro_x;
        gyro_y_tmp = icm_gyro_y;
        gyro_z_tmp = icm_gyro_z;
        icm_init_flag=1;
       }
}
/**************************************��̬����****************/
int32_t ACC_X_BUF[ACC_FILTER_NUM], ACC_Y_BUF[ACC_FILTER_NUM], ACC_Z_BUF[ACC_FILTER_NUM];  // �˲���������
int32_t GYRO_X_BUF[GYRO_FILTER_NUM], GYRO_Y_BUF[GYRO_FILTER_NUM], GYRO_Z_BUF[GYRO_FILTER_NUM];
#define Kp 25.0f //  1.2            // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.02f//0.001     // integral gain governs rate of convergence of gyroscope biases
#define halfT 0.003f  //0.003                   // half the sample period�������ڵ�һ��
float q0 = 1, q1 = 0, q2 = 0, q3 = 0;   // quaternion elements representing the estimated orientation
float exInt = 0, eyInt = 0, ezInt = 0;  // scaled integral error
float Attitude_Angel_P = 0.0000f;
float Attitude_Angel_R = 0.0000f;
float Attitude_Angel_Y = 0.0000f;


 ICM20602_IntegrationTypedef ICMIntegrate;


//
 ICM20602_Treated ICM_Treated;   //�����������������
 ICM20602 ICM_Offset,ICM_State;
 uint8_t ICM20602_Offset_Finished;
float ABS(float a)
{
    if(a<0 )return -a;
   return a;
}
void Data_Filter(void)  // �����˲�
{
    int i;
    int64_t temp1 = 0, temp2 = 0, temp3 = 0, temp4 = 0, temp5 = 0, temp6 = 0;

        icm20602_get_acc ();
        icm20602_get_gyro   ();

        ICM_State.accdata.x-=acc_x_tmp;
   ICM_State.accdata.z-=acc_z_tmp;
     ICM_State.gyro.x-=gyro_x_tmp;
      ICM_State.gyro.y-=gyro_y_tmp;
 ICM_State.gyro.z-=gyro_z_tmp;
    ACC_X_BUF[0] = ICM_State.accdata.x; // ���»�����������
    ACC_Y_BUF[0] = ICM_State.accdata.y;
    ACC_Z_BUF[0] = ICM_State.accdata.z;
    GYRO_X_BUF[0] = ICM_State.gyro.x;
    GYRO_Y_BUF[0] = ICM_State.gyro.y;
    GYRO_Z_BUF[0] = ICM_State.gyro.z;

    for (i = 0; i < ACC_FILTER_NUM; i++)
    {
        temp1 += ACC_X_BUF[i];
        temp2 += ACC_Y_BUF[i];
        temp3 += ACC_Z_BUF[i];

    }
    for (i = 0; i < GYRO_FILTER_NUM; i++)
    {
        temp4 += GYRO_X_BUF[i];
        temp5 += GYRO_Y_BUF[i];
        temp6 += GYRO_Z_BUF[i];
    }

    ICM_Treated.accdata.x = temp1 / ACC_FILTER_NUM / AcceRatio;
    ICM_Treated.accdata.y = temp2 / ACC_FILTER_NUM / AcceRatio;
    ICM_Treated.accdata.z = temp3 / ACC_FILTER_NUM / AcceRatio;
    ICM_Treated.gyro.x = temp4 / GYRO_FILTER_NUM / GyroRatio;
    ICM_Treated.gyro.y = temp5 / GYRO_FILTER_NUM / GyroRatio;
    ICM_Treated.gyro.z = temp6 / GYRO_FILTER_NUM / GyroRatio;

    for (i = 0; i < ACC_FILTER_NUM - 1; i++)
    {
        ACC_X_BUF[ACC_FILTER_NUM - 1 - i] = ACC_X_BUF[ACC_FILTER_NUM - 2 - i];
        ACC_Y_BUF[ACC_FILTER_NUM - 1 - i] = ACC_Y_BUF[ACC_FILTER_NUM - 2 - i];
        ACC_Z_BUF[ACC_FILTER_NUM - 1 - i] = ACC_Z_BUF[ACC_FILTER_NUM - 2 - i];

    }
    for (i = 0; i < GYRO_FILTER_NUM - 1; i++)
    {
        GYRO_X_BUF[GYRO_FILTER_NUM - 1 - i] = GYRO_X_BUF[GYRO_FILTER_NUM - 2 - i];
        GYRO_Y_BUF[GYRO_FILTER_NUM - 1 - i] = GYRO_Y_BUF[GYRO_FILTER_NUM - 2 - i];
        GYRO_Z_BUF[GYRO_FILTER_NUM - 1 - i] = GYRO_Z_BUF[GYRO_FILTER_NUM - 2 - i];
    }
}


void ICM_I_Int(void)
{
    if (ICMIntegrate.Yaw_I_Enable == 'T')
    {
        ICMIntegrate.TurnRight_I += (ICM_Treated.gyro.z < 0 ? ABS(ICM_Treated.gyro.z) * 0.005  : 0);
        ICMIntegrate.TurnLeft_I += (ICM_Treated.gyro.z > 0 ? ABS(ICM_Treated.gyro.z) * 0.005  : 0);
    }
    if (ICMIntegrate.Pitch_I_Enable == 'T')
    {
        ICMIntegrate.Down_I += (ICM_Treated.gyro.y < 0 ? ABS(ICM_Treated.gyro.y) * 0.005  : 0);
        ICMIntegrate.Up_I += (ICM_Treated.gyro.y > 0 ? ABS(ICM_Treated.gyro.y) * 0.005  : 0);
    }
    if (ICMIntegrate.Row_I_Enable == 'T')
    {
        ICMIntegrate.Clockwise_I += (ICM_Treated.gyro.x < 0 ? ABS(ICM_Treated.gyro.x) * 0.005  : 0);
        ICMIntegrate.Anticlockwise_I += (ICM_Treated.gyro.x > 0 ? ABS(ICM_Treated.gyro.x) * 0.005  : 0);
    }
}

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
    float norm;
    float vx, vy, vz;
    float ex, ey, ez;

    // �Ȱ���Щ�õõ���ֵ���
    float q0q0 = q0 * q0;
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q1q1 = q1 * q1;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;

    if (ax * ay * az == 0)
    {
        return;
    }

    norm = my_sqrt(ax * ax + ay * ay + az * az); // acc���ݹ�һ��
    ax = ax / norm;
    ay = ay / norm;
    az = az / norm;

    // estimated direction of gravity and flux (v and w)    �����������������/��Ǩ
    vx = 2 * (q1q3 - q0q2);                                 // ��Ԫ����xyz�ı�ʾ
    vy = 2 * (q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3 ;

    // error is sum of cross product between reference direction of fields and direction measured by sensors
    ex = (ay * vz - az * vy) ;  // �������������õ���־������
    ey = (az * vx - ax * vz) ;
    ez = (ax * vy - ay * vx) ;

    exInt = exInt + ex * Ki;    // �������л���
    eyInt = eyInt + ey * Ki;
    ezInt = ezInt + ez * Ki;

    // adjusted gyroscope measurements
    gx = gx + Kp * ex + exInt;  // �����PI�󲹳��������ǣ����������Ư��
    gy = gy + Kp * ey + eyInt;
    gz = gz + Kp * ez + ezInt;  // �����gz����û�й۲��߽��н��������Ư�ƣ����ֳ����ľ��ǻ����������Լ�

    // integrate quaternion rate and normalise  // ��Ԫ�ص�΢�ַ���
    q0 = q0 + (-q1 * gx - q2 * gy - q3 * gz) * halfT;
    q1 = q1 + (q0 * gx + q2 * gz - q3 * gy) * halfT;
    q2 = q2 + (q0 * gy - q1 * gz + q3 * gx) * halfT;
    q3 = q3 + (q0 * gz + q1 * gy - q2 * gx) * halfT;

    // normalise quaternion
    norm = my_sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 = q0 / norm;
    q1 = q1 / norm;
    q2 = q2 / norm;
    q3 = q3 / norm;

    Attitude_Angel_P = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3; // pitch
    Attitude_Angel_R =atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3; // roll
    Attitude_Angel_Y =atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3 * q3 + 1) * 57.3; // yaw
    //  Attitude_Angle.Z = 0;
}

float my_sqrt(float number)
{
    long i;
    float x, y;
    const float f = 1.5F;
    x = number * 0.5F;
    y = number;
    i = * (long *) &y;
    i = 0x5f3759df - (i >> 1);

    y = * (float *) &i;
    y = y * (f - (x * y * y));
    y = y * (f - (x * y * y));
    return number * y;
}
float  fast_atan2(float y, float x)
{
    float ax = ABS_FLOAT(x), ay = ABS_FLOAT(y);
    float a = MIN(ax, ay) / (MAX(ax, ay) + (float)DBL_EPSILON);
    float s = a * a;
    float r = ((-0.0464964749 * s + 0.15931422) * s - 0.327622764) * s * a + a;
    if (ay > ax) r = 1.57079637 - r;
    if (x < 0) return 3.14159274f - r;
    if (y < 0) return   -r;
    return r;
}

bool ICM20602_Offset(void)
{
    uint8 i, j, Count = 100;
    int64 temp[6] = {0};

    //���û��ֱ�־
    ICMIntegrate.Yaw_I_Enable = 'F';
    ICMIntegrate.TurnLeft_I = 0.000f;
    ICMIntegrate.TurnRight_I = 0.000f;
    ICMIntegrate.Pitch_I_Enable = 'F';
    ICMIntegrate.Up_I = 0.000f;
    ICMIntegrate.Down_I = 0.000f;
    ICMIntegrate.Row_I_Enable = 'F';
    ICMIntegrate.Clockwise_I = 0.000f;
    ICMIntegrate.Anticlockwise_I = 0.000f;


    for (i = 0; i < Count; i++)
    {
        icm20602_get_acc ();
         icm20602_get_gyro   ();
        my_delay(1);

        temp[0] += ICM_State.accdata.x;
        temp[1] += ICM_State.accdata.y;
        temp[2] += ICM_State.accdata.z;

        temp[3] += ICM_State.gyro.x;
        temp[4] += ICM_State.gyro.y;
        temp[5] += ICM_State.gyro.z;
    }
    ICM_Offset.accdata.x = temp[0] / Count;
    ICM_Offset.accdata.y = temp[1] / Count;
    ICM_Offset.accdata.z = temp[2] / Count;

    ICM_Offset.gyro.x = temp[3] / Count;
    ICM_Offset.gyro.y = temp[4] / Count;
    ICM_Offset.gyro.z = temp[5] / Count;
    ICM20602_Offset_Finished = 1;

    //����ǶȲ����˲� ���������ǵ�׼ȷ��
    for (i = 0; i < 100; i++)
    {
        for (j = 0; j < 3; j++)
        {
            icm20602_get_acc ();
             icm20602_get_gyro   ();
            Data_Filter();                  // ��ԭʼ���ݻ����˲�
        }
    }
    return true;
}
