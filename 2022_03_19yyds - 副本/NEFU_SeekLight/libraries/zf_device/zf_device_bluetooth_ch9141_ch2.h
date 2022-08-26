/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_device_bluetooth_ch9141
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
* @note
********************************************************************************************************************/
#ifndef _zf_device_bluetooth_ch9141_ch2_h_
#define _zf_device_bluetooth_ch9141_ch2_h_

#include "zf_device_bluetooth_ch9141.h"
#include "zf_common_fifo.h"
#include "zf_driver_gpio.h"
#include "zf_driver_uart.h"
#include "zf_driver_delay.h"

#define BLUETOOTH_CH9141_INDEX_CH2              UART_7                                      // ����ģ�� 1 ��Ӧʹ�õĴ��ں�
#define BLUETOOTH_CH9141_BUAD_RATE_CH2          115200                                      // ����ģ�� 1 ��Ӧʹ�õĴ��ڲ�����
#define BLUETOOTH_CH9141_TX_PIN_CH2             UART7_RX_E13                                 // ����ģ�� 1 ��Ӧģ��� TX Ҫ�ӵ���Ƭ���� RX
#define BLUETOOTH_CH9141_RX_PIN_CH2             UART7_TX_E12                                 // ����ģ�� 1 ��Ӧģ��� RX Ҫ�ӵ���Ƭ���� TX
#define BLUETOOTH_CH9141_RTS_PIN_CH2            E8                                         // ����ģ�� 1 ��Ӧģ��� RTS ����
#define BLUETOOTH_CH9141_MODE_CH2               BLUETOOTH_CH9141_MASTER                               // ����ģ�� 1 ��Ӧģ���ģʽ
#define BLUETOOTH_CH9141_TAGET_INFO_CH2         "F0:78:19:26:3B:38,000000"                  // ����ģ�� 1 ����ģʽ�� ׼�����ӵ�Ŀ��ӻ���Ӧ�� MAC ��ַ������
#define BLUETOOTH_CH9141_SELF_NAME_CH2          "ch9141_1"                                  // ����ģ�� 1 �ӻ�ģʽ�� �Լ����������Ʋ����� 15 ���ַ�
#define BLUETOOTH_CH9141_SELF_PASSWORD_CH2      "000000"                                    // ����ģ�� 1 �ӻ�ģʽ�� �Լ���������Ϣ ��������

#define BLUETOOTH_CH9141_BUFFER_SIZE_CH2        64
#define BLUETOOTH_CH9141_TIMEOUT_COUNT_CH2      500
#define BLUETOOTH_CH9141_DEFAULT_TX_POWER_CH2   BLUETOOTH_CH9141_TX_POWER_4DB



void        bluetooth_ch9141_uart_callback_ch2  (void);
uint8_t     bluetooth_ch9141_init_ch2           (void);
uint32_t    bluetooth_ch9141_send_buff_ch2      (uint8_t *buff, uint32_t len);
uint32_t    bluetooth_ch9141_read_buff_ch2      (uint8_t *buff, uint32_t len);

#endif

