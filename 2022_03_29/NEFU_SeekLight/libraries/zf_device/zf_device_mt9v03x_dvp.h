/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_device_mt9v03x
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
* @note             version:
*                   V1.1 2021.12.23 ����ͷ�ɼ���ɱ�־λ����volatile����
*
*                   ���߶��壺
*                   ------------------------------------
*                   ģ��ܽ�            ��Ƭ���ܽ�
*                   TXD                 �鿴 zf_device_mt9v03x.h �� MT9V03X_COF_UART_TX_DVP        �궨��
*                   RXD                 �鿴 zf_device_mt9v03x.h �� MT9V03X_COF_UART_RX_DVP        �궨��
*                   D0                  �鿴 zf_device_mt9v03x.h �� MT9V03X_D0_PIN_DVP             �궨��
*                   D1                  �鿴 zf_device_mt9v03x.h �� MT9V03X_D1_PIN_DVP             �궨��
*                   D2                  �鿴 zf_device_mt9v03x.h �� MT9V03X_D2_PIN_DVP             �궨��
*                   D3                  �鿴 zf_device_mt9v03x.h �� MT9V03X_D3_PIN_DVP             �궨��
*                   D4                  �鿴 zf_device_mt9v03x.h �� MT9V03X_D4_PIN_DVP             �궨��
*                   D5                  �鿴 zf_device_mt9v03x.h �� MT9V03X_D5_PIN_DVP             �궨��
*                   D6                  �鿴 zf_device_mt9v03x.h �� MT9V03X_D6_PIN_DVP             �궨��
*                   D7                  �鿴 zf_device_mt9v03x.h �� MT9V03X_D7_PIN_DVP             �궨��
*                   PCLK                �鿴 zf_device_mt9v03x.h �� MT9V03X_PCLK_PIN_DVP           �궨��
*                   VSYNC               �鿴 zf_device_mt9v03x.h �� MT9V03X_VSY_PIN_DVP            �궨��
*                   HSYNC               �鿴 zf_device_mt9v03x.h �� MT9V03X_HERF_PIN_DVP           �궨��
*                   ------------------------------------
********************************************************************************************************************/

#ifndef _zf_device_mt9v03x_h_
#define _zf_device_mt9v03x_h_

#include "zf_common_debug.h"
#include "zf_common_interrupt.h"

#include "zf_driver_delay.h"
#include "zf_driver_dvp.h"
#include "zf_driver_gpio.h"
#include "zf_driver_uart.h"

// ��������ͷ����
#define MT9V03X_DVP_W               188                                             // ͼ����  ��Χ1-752
#define MT9V03X_DVP_H               120                                             // ͼ��߶� ��Χ1-480
#define MT9V03X_IMAGE_SIZE_DVP      (MT9V03X_DVP_W*MT9V03X_DVP_H)

//--------------------------------------------------------------------------------------------------
// ��������
//--------------------------------------------------------------------------------------------------
#define MT9V03X_COF_UART_DVP        UART_5                                          // ��������ͷ��ʹ�õ��Ĵ���
#define MT9V03X_COF_BAUR_DVP        9600                                            // ��������ô��ڲ����� ��ֹ�޸�
#define MT9V03X_COF_UART_RX_DVP     UART5_TX_C12                                    // ����� UART-RX ���� Ҫ���ڵ�Ƭ�� TX ��
#define MT9V03X_COF_UART_TX_DVP     UART5_RX_D2                                     // ����� UART-TX ���� Ҫ���ڵ�Ƭ�� RX ��

//--------------------------------------------------------------------------------------------------
// ����ͷ�������ţ�DVPר�����ţ���ֹ�û��޸�����
//--------------------------------------------------------------------------------------------------
#define MT9V03X_D0_PIN_DVP          A9
#define MT9V03X_D1_PIN_DVP          A10
#define MT9V03X_D2_PIN_DVP          C8
#define MT9V03X_D3_PIN_DVP          C9
#define MT9V03X_D4_PIN_DVP          C11
#define MT9V03X_D5_PIN_DVP          B6
#define MT9V03X_D6_PIN_DVP          B8
#define MT9V03X_D7_PIN_DVP          B9

#define MT9V03X_PCLK_PIN_DVP        A6
#define MT9V03X_VSY_PIN_DVP         A5
#define MT9V03X_HERF_PIN_DVP        A4

// ��ʱ����
#define MT9V03X_INIT_TIMEOUT                  0x0080

// ����ͷ����ö��
typedef enum
{
    INIT = 0,                                                                   // ����ͷ��ʼ������
    AUTO_EXP,                                                                   // �Զ��ع�����
    EXP_TIME,                                                                   // �ع�ʱ������
    FPS,                                                                        // ����ͷ֡������
    SET_COL,                                                                    // ͼ��������
    SET_ROW,                                                                    // ͼ��������
    LR_OFFSET,                                                                  // ͼ������ƫ������
    UD_OFFSET,                                                                  // ͼ������ƫ������
    GAIN,                                                                       // ͼ��ƫ������
    PCLK_MODE,                                                                  // ����ʱ��ģʽ����(�������MT9V034 V1.5�Լ����ϰ汾֧�ָ�����)
    CONFIG_FINISH,                                                              // ������λ����Ҫ����ռλ����

    COLOR_GET_WHO_AM_I = 0xEF,
    SET_EXP_TIME = 0XF0,                                                        // ���������ع�ʱ������
    GET_STATUS,                                                                 // ��ȡ����ͷ��������
    GET_VERSION,                                                                // �̼��汾������

    SET_ADDR = 0XFE,                                                            // �Ĵ�����ַ����
    SET_DATA                                                                    // �Ĵ�����������
}m9v03x_cmd_enum;

extern volatile uint8_t    mt9v03x_finish_flag_dvp;//һ��ͼ��ɼ���ɱ�־λ
extern uint8_t    mt9v03x_image_dvp[MT9V03X_DVP_H][MT9V03X_DVP_W];

void     mt9v03x_uart_callback_dvp      (void);
void     mt9v03x_handler_dvp            (void);
uint16_t mt9v03x_get_version_dvp        (void);
uint8_t  mt9v03x_set_exposure_time_dvp  (uint16_t light);
uint8_t  mt9v03x_set_reg_dvp            (uint8_t addr, uint16_t data);
uint8_t  mt9v03x_init_dvp               (void);

#endif
