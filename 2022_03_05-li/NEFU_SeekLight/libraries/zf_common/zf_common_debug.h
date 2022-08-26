/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             zf_common_debug
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ790875685)
* @version          查看doc内version文件 版本说明
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
********************************************************************************************************************/

#ifndef _zf_board_h
#define _zf_board_h

#include "stdio.h"
#include "stdint.h"
#include "string.h"

#define PRINTF_ENABLE               1                   //printf使能

#define DEBUG_UART_INDEX            UART_8              //DEBUG串口
#define DEBUG_UART_BAUDRATE         115200              //DEBUG串口波特率
#define DEBUG_UART_TX_PIN           UART8_TX_E14        //DEBUG串口TX引脚
#define DEBUG_UART_RX_PIN           UART8_RX_E15        //DEBUG串口RX引脚



#define zf_assert(x)                debug_assert_handler(x,__FILE__,__LINE__)   // 断言 一般用于参数判断 zf_assert(0) 就断言报错
#define zf_log(x,str)               debug_log_handler(x,str,__FILE__,__LINE__)  // 调试信息输出 用来做一些报错或者警告之类的输出

#define DEBUG_UART_USE_INTERRUPT    1                                           // 是否启用 debug 串口接收中断

#if DEBUG_UART_USE_INTERRUPT                                                    // 如果启用 debug 串口接收中断
#define DEBUG_RING_BUFFER_LEN       64                                          // 定义环形缓冲区大小 默认 64byte

extern uint8_t  debug_ring_buffer[DEBUG_RING_BUFFER_LEN];                       // 环形缓冲区
extern uint32_t debug_ring_index;                                               // 缓冲区下标

void        debug_interrupr_handler (void);
uint32_t    debug_read_ring_buffer  (uint8_t *data);
#endif

void        debug_init              (void);
void        debug_assert_handler    (uint8_t pass, char *file, uint32_t line);
void        debug_log_handler       (uint8_t pass, char *str, char *file, uint32_t line);
//int         _write                  (int fd, char *buf, int size);

#endif
