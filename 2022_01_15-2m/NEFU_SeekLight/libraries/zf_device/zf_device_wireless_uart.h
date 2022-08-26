/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_device_wireless_uart
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
* @note
********************************************************************************************************************/
#ifndef _zf_device_wireless_uart_h_
#define _zf_device_wireless_uart_h_


#include "zf_driver_gpio.h"
#include "zf_driver_uart.h"
#include "zf_driver_delay.h"



#define WRIELESS_UART_NUMBER       1                                           // �����������ߴ��ڵ����� Ĭ����������
#define WRIELESS_UART_INDEX        UART_2                                      // ���ߴ��� 1 ��Ӧʹ�õĴ��ں�
#define WRIELESS_UART_BUAD_RATE    115200                                      // ���ߴ��� 1 ��Ӧʹ�õĴ��ڲ�����
#define WRIELESS_UART_TX_PIN       UART2_RX_D6                                 // ���ߴ��� 1 ��Ӧģ��� TX Ҫ�ӵ���Ƭ���� RX
#define WRIELESS_UART_RX_PIN       UART2_TX_D5                                 // ���ߴ��� 1 ��Ӧģ��� RX Ҫ�ӵ���Ƭ���� TX
#define WRIELESS_UART_RTS_PIN      E10                                         // ���ߴ��� 1 ��Ӧģ��� RTS ����



// ------------------------------------ �Զ������� ------------------------------------
// ע������1������ת����ģ��汾��V2.0���µ����޷������Զ������ʵġ�
// ע������2�������Զ��������������RTS���� ����Ὺ��ʧ�ܡ�
// ע������3��ģ���Զ�������ʧ�ܵĻ� ���Գ��Զϵ�����

// �����Զ�����������Ķ��������� ע������
// �����Զ�����������Ķ��������� ע������
// �����Զ�����������Ķ��������� ע������

// 0���ر��Զ�������
// 1�������Զ������� �Զ������ʵ��������޸� WIRELESS_UART_BAUD ֮����Ҫ��ģ��������� ģ����Զ�����Ϊ��Ӧ�Ĳ�����

#define WIRELESS_UART_AUTO_BAUD_RATE    0
// ------------------------------------ �Զ������� ------------------------------------

#define WIRELESS_UART_BUFFER_SIZE       64
#define WIRELESS_UART_TIMEOUT_COUNT     0x64

uint8_t     wireless_uart_init              (void);

void        wireless_uart_callback(void);
uint32_t    wireless_uart_send_buff     (uint8_t *buff, uint32_t len);
uint32_t    wireless_uart_read_buff     (uint8_t *buff, uint32_t len);



#endif
