/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            zf_driver_uart
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ790875685)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/
#ifndef _zf_uart_h
#define _zf_uart_h

#include "ch32v30x.h"
#include "ch32v30x_misc.h"
#include "ch32v30x_gpio.h"
#include "ch32v30x_rcc.h"
#include "ch32v30x_usart.h"

#include "zf_common_interrupt.h"
#include "zf_common_debug.h"


//  ��ö�ٶ��岻�����û��޸�
//  ��ʼ����ʱ�򣬱���ʹ��ͬһ���������磬UART1_TX_A9��UART1_RX_A10��һ������
//
typedef enum //  ��ö�ٶ��岻�����û��޸�
{
    //----------UART1-----------//
    UART1_TX_A9  = 0x00,    UART1_RX_A10,   //  Ĭ������
    UART1_TX_B6,            UART1_RX_B7,    //  ��ӳ��
    //UART1_TX_B15,
    //----------UART2-----------//
    UART2_TX_A2  = 0x10,    UART2_RX_A3,    //  Ĭ������
    UART2_TX_D5,            UART2_RX_D6,    //  ��ӳ��

    //----------UART3-----------//
    UART3_TX_B10 = 0x20,    UART3_RX_B11,   //  Ĭ������
    UART3_TX_C10,           UART3_RX_C11,   //  ������ӳ��
    UART3_TX_D8,            UART3_RX_D9,    //  ��ȫ��ӳ��

    //----------UART4-----------//
    UART4_TX_C10 = 0x40,    UART4_RX_C11,   //  Ĭ������
    UART4_TX_B0,            UART4_RX_B1,

    //----------UART5-----------//
    UART5_TX_C12 = 0x50,    UART5_RX_D2,    //  Ĭ������
    UART5_TX_E8,            UART5_RX_E9,
    //----------UART6-----------//
    UART6_TX_C0  = 0x60,    UART6_RX_C1,    //  Ĭ������
    UART6_TX_E10,           UART6_RX_E11,
    //----------UART7-----------//
    UART7_TX_C2  = 0x70,    UART7_RX_C3,    //  Ĭ������
    UART7_TX_A6,            UART7_RX_A7,
    UART7_TX_E12,           UART7_RX_E13,
    //----------UART8-----------//
    UART8_TX_C4  = 0x80,    UART8_RX_C5,     //  Ĭ������
    UART8_TX_E14,           UART8_RX_E15,
    //8������ ֻ��123��Ӳ��RTS CTS���� 45678û��
}uart_pin_enum;


typedef enum//  ��ö�ٶ��岻�����û��޸�
{
    UART_1 = 0,
    UART_2,
    UART_3,
    UART_4,
    UART_5,
    UART_6,
    UART_7,
    UART_8
}uart_index_enum;



extern const uint32_t uart_index[];




void    uart_write_byte         (uart_index_enum uartn, uint8_t dat);
void    uart_write_buffer       (uart_index_enum uartn, uint8_t *buff, uint32_t len);
void    uart_write_string       (uart_index_enum uartn, const uint8_t *str);
void    uart_read_byte          (uart_index_enum uartn, uint8_t *dat);
uint8_t uart_query_byte         (uart_index_enum uartn, uint8_t *dat);
void    uart_tx_interrupt       (uart_index_enum uartn, uint8_t status);
void    uart_rx_interrupt       (uart_index_enum uartn, uint8_t status);
void    uart_init               (uart_index_enum uartn, uint32_t baud, uart_pin_enum tx_pin, uart_pin_enum rx_pin);



#endif
