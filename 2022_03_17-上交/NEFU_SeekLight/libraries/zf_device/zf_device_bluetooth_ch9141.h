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
#ifndef _zf_device_bluetooth_ch9141_h_
#define _zf_device_bluetooth_ch9141_h_


#include "zf_common_fifo.h"
#include "zf_driver_gpio.h"
#include "zf_driver_uart.h"
#include "zf_driver_delay.h"

#define BLUETOOTH_CH9141_INDEX              UART_6                                     // ����ģ�� 1 ��Ӧʹ�õĴ��ں�
#define BLUETOOTH_CH9141_BUAD_RATE          115200                                      // ����ģ�� 1 ��Ӧʹ�õĴ��ڲ�����
#define BLUETOOTH_CH9141_TX_PIN             UART6_RX_C1                                 // ����ģ�� 1 ��Ӧģ��� TX Ҫ�ӵ���Ƭ���� RX
#define BLUETOOTH_CH9141_RX_PIN             UART6_TX_C0                                 // ����ģ�� 1 ��Ӧģ��� RX Ҫ�ӵ���Ƭ���� TX
#define BLUETOOTH_CH9141_RTS_PIN            E0                                        // ����ģ�� 1 ��Ӧģ��� RTS ����
#define BLUETOOTH_CH9141_MODE               BLUETOOTH_CH9141_MASTER                               // ����ģ�� 1 ��Ӧģ���ģʽ
#define BLUETOOTH_CH9141_TAGET_INFO         "7E:7D:19:26:3B:38,000000"                  // ����ģ�� 1 ����ģʽ�� ׼�����ӵ�Ŀ��ӻ���Ӧ�� MAC ��ַ������
#define BLUETOOTH_CH9141_SELF_NAME          "ch9141_1"                                  // ����ģ�� 1 �ӻ�ģʽ�� �Լ����������Ʋ����� 15 ���ַ�
#define BLUETOOTH_CH9141_SELF_PASSWORD      "000000"                                    // ����ģ�� 1 �ӻ�ģʽ�� �Լ���������Ϣ ��������

#define BLUETOOTH_CH9141_BUFFER_SIZE        64
#define BLUETOOTH_CH9141_TIMEOUT_COUNT      500
#define BLUETOOTH_CH9141_DEFAULT_TX_POWER   BLUETOOTH_CH9141_TX_POWER_4DB

typedef enum
{
    BLUETOOTH_CH9141_TX_POWER_0DB = 0,   //0DB
    BLUETOOTH_CH9141_TX_POWER_1DB,       //1DB
    BLUETOOTH_CH9141_TX_POWER_2DB,       //2DB
    BLUETOOTH_CH9141_TX_POWER_3DB,       //3DB
    BLUETOOTH_CH9141_TX_POWER_4DB,       //4DB
    BLUETOOTH_CH9141_TX_POWER_MINUS_8DB, //-8DB
    BLUETOOTH_CH9141_TX_POWER_MINUS_14DB,//-14DB
    BLUETOOTH_CH9141_TX_POWER_MINUS_20DB,//-20DB
}bluetooth_ch9141_tx_power_enum;

typedef enum
{
    BLUETOOTH_CH9141_MASTER_NO_INIT = 0,        //����δ��ʼ��
    BLUETOOTH_CH9141_MASTER_SCANNING,           //����ɨ����
    BLUETOOTH_CH9141_MASTER_CONNECTING,         //����������
    BLUETOOTH_CH9141_MASTER_CONNECTED,          //����������
    BLUETOOTH_CH9141_MASTER_DISCONNECTED,       //�������ӶϿ�

    BLUETOOTH_CH9141_SLAVE_NO_INIT,         //�ӻ�δ��ʼ��
    BLUETOOTH_CH9141_SLAVE_OK_INIT,         //�ӻ���ɳ�ʼ��
    BLUETOOTH_CH9141_SLAVE_BROADCAST,       //�ӻ��㲥��
    BLUETOOTH_CH9141_SLAVE_BROADCAST_READY, //�ӻ�׼���㲥
    BLUETOOTH_CH9141_SLAVE_CONNECT_TIMEOUT, //�ӻ����ӳ�ʱ
    BLUETOOTH_CH9141_SLAVE_CONNECTED,       //�ӻ�������
    BLUETOOTH_CH9141_SLAVE_ERROR,           //�ӻ����ִ���
}bluetooth_ch9141_state_enum;

typedef enum
{
    BLUETOOTH_CH9141_BROADCAST,   //�㲥ģʽ
    BLUETOOTH_CH9141_MASTER,        //����ģʽ
    BLUETOOTH_CH9141_SLAVE,         //�ӻ�ģʽ
}bluetooth_ch9141_mode_enum;

void        bluetooth_ch9141_uart_callback_ch1      (void);
uint8_t     bluetooth_ch9141_init_ch1               (void);
uint32_t    bluetooth_ch9141_send_buff_ch1          (uint8_t *buff, uint32_t len);
uint32_t    bluetooth_ch9141_read_buff_ch1          (uint8_t *buff, uint32_t len);

#endif

