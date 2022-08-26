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


#include "ch32v30x_usart.h"
#include "ch32v30x_rcc.h"
#include "ch32v30x_gpio.h"

#include "zf_common_debug.h"
#include "zf_common_interrupt.h"

#include "zf_driver_uart.h"




uint8_t zf_debug_init_flag = 0;

//-------------------------------------------------------------------------------------------------------------------
// @brief       debug初始化函数
// @param       void
// @return      void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void debug_init (void)
{
//    GPIO_DeInit(GPIOA);
//    GPIO_DeInit(GPIOB);
//    GPIO_DeInit(GPIOC);
//    GPIO_DeInit(GPIOD);
//    GPIO_DeInit(GPIOE);

    uart_init(
        DEBUG_UART_INDEX,                                                       // 在 zf_common_debug.h 中查看对应值
        DEBUG_UART_BAUDRATE,                                                    // 在 zf_common_debug.h 中查看对应值
        DEBUG_UART_TX_PIN,                                                      // 在 zf_common_debug.h 中查看对应值
        DEBUG_UART_RX_PIN);                                                     // 在 zf_common_debug.h 中查看对应值

    if(DEBUG_UART_USE_INTERRUPT)                                                // 如果启用串口中断
    {
        uart_rx_interrupt(DEBUG_UART_INDEX, 1);                                 // 使能对应串口接收中断
    }
    zf_debug_init_flag = 1;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       debug 断言处理函数
// @param       bool        判断是否触发断言
// @param       *file       文件名
// @param       line        目标行数
// @return      void
// Sample usage:            这个函数不是直接调用的 使用 zf_commmon_debug.h 中的 zf_assert(x) 接口
//-------------------------------------------------------------------------------------------------------------------
void debug_assert_handler (uint8_t pass, char *file, uint32_t line)
{
    if(pass)
        return;
    if(zf_debug_init_flag)
    {
        printf("Assert error in %s line %ld.\r\n", file, line);
    }
    interrupt_disable_all();
    while(1)
    {
        // 如果代码跳转到这里停住了
        // 一般你的函数参数传递出错了
        // 或者你自己调用的 zf_assert(x) 接口处报错了

        // 如果调用了 debug_init 初始化了 log 输出
        // 就在对应串口输出去查看是哪个文件的哪一行报错

        // 如果没有初始化 debug
        // 那就看看这个 file 的字符串值和 line 的行数
        // 那代表报错的文件路径名称和对应报错行数

        // 再去调试看看是为什么参数出错
        __NOP();
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       debug 调试信息处理函数
// @param       bool        判断是否触发断言
// @param       *str        要输出的调试信息
// @param       *file       文件名
// @param       line        目标行数
// @return      void
// Sample usage:            这个函数不是直接调用的 使用 zf_commmon_debug.h 中的 zf_log(x,str) 接口
//-------------------------------------------------------------------------------------------------------------------
void debug_log_handler (uint8_t pass, char *str, char *file, uint32_t line)
{
    if(pass)
        return;
    if(zf_debug_init_flag)
    {
        printf("Log message from %s line %ld :\"%s\".\r\n", file, line, str);
    }
}

#if DEBUG_UART_USE_INTERRUPT                                                    // 条件编译 只有在启用串口中断才编译

uint8_t  debug_ring_buffer[DEBUG_RING_BUFFER_LEN];                              // 创建环形缓冲区
uint32_t debug_ring_index = 0;                                                  // 环形缓冲区下标索引

//-------------------------------------------------------------------------------------------------------------------
// @brief       debug 串口中断处理函数 isr.c 中对应串口中断服务函数调用
// @param       void
// @return      void
// Sample usage:            debug_interrupr_handler();
//-------------------------------------------------------------------------------------------------------------------
void debug_interrupr_handler (void)
{
    uart_query_byte(DEBUG_UART_INDEX, &debug_ring_buffer[debug_ring_index++]);  // 读取数据到环形缓冲区
    if(debug_ring_index == DEBUG_RING_BUFFER_LEN)   debug_ring_index = 0;       // 环形缓冲区收尾衔接
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       读取 debug 环形缓冲区数据
// @param       *data       读出数据存放的数组指针
// @return      uint32_t    读出数据长度
// Sample usage:            uint8_t data[64]; len = debug_read_ring_buffer(data);
//-------------------------------------------------------------------------------------------------------------------
uint32_t debug_read_ring_buffer (uint8_t *data)
{
    if(debug_ring_index == 0)   return 0;
    uint32_t temp = debug_ring_index;                                           // 获取数据长度
    memcpy(data, debug_ring_buffer, temp);                                      // 拷贝数据
    debug_ring_index = 0;                                                       // 清空下标
    return temp;
}

#endif


//-------------------------------------------------------------------------------------------------------------------
//  @brief      printf重定向
//  @param      void
//  @return     void
//  @since      v1.0
//  Sample usage:              重定向printf到DEBUG串口上
//-------------------------------------------------------------------------------------------------------------------
#if (1 == PRINTF_ENABLE)
int _write(int fd, char *buf, int size)
{
    int i;
    for(i=0; i<size; i++)
    {
        while (USART_GetFlagStatus((USART_TypeDef*)uart_index[DEBUG_UART_INDEX], USART_FLAG_TC) == RESET);
        USART_SendData((USART_TypeDef*)uart_index[DEBUG_UART_INDEX], *buf++);

    }
    return size;
}
#endif

