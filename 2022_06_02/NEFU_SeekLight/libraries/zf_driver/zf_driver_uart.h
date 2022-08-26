/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            zf_driver_uart
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
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


//  此枚举定义不允许用户修改
//  初始化的时候，必须使用同一组引脚例如，UART1_TX_A9和UART1_RX_A10是一组引脚
//
typedef enum //  此枚举定义不允许用户修改
{
    //----------UART1-----------//
    UART1_TX_A9  = 0x00,    UART1_RX_A10,   //  默认引脚
    UART1_TX_B6,            UART1_RX_B7,    //  重映射
    //UART1_TX_B15,
    //----------UART2-----------//
    UART2_TX_A2  = 0x10,    UART2_RX_A3,    //  默认引脚
    UART2_TX_D5,            UART2_RX_D6,    //  重映射

    //----------UART3-----------//
    UART3_TX_B10 = 0x20,    UART3_RX_B11,   //  默认引脚
    UART3_TX_C10,           UART3_RX_C11,   //  部分重映射
    UART3_TX_D8,            UART3_RX_D9,    //  完全重映射

    //----------UART4-----------//
    UART4_TX_C10 = 0x40,    UART4_RX_C11,   //  默认引脚
    UART4_TX_B0,            UART4_RX_B1,

    //----------UART5-----------//
    UART5_TX_C12 = 0x50,    UART5_RX_D2,    //  默认引脚
    UART5_TX_E8,            UART5_RX_E9,
    //----------UART6-----------//
    UART6_TX_C0  = 0x60,    UART6_RX_C1,    //  默认引脚
    UART6_TX_E10,           UART6_RX_E11,
    //----------UART7-----------//
    UART7_TX_C2  = 0x70,    UART7_RX_C3,    //  默认引脚
    UART7_TX_A6,            UART7_RX_A7,
    UART7_TX_E12,           UART7_RX_E13,
    //----------UART8-----------//
    UART8_TX_C4  = 0x80,    UART8_RX_C5,     //  默认引脚
    UART8_TX_E14,           UART8_RX_E15,
    //8个串口 只有123有硬件RTS CTS引脚 45678没有
}uart_pin_enum;


typedef enum//  此枚举定义不允许用户修改
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
