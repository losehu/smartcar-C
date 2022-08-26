/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             zf_device_wireless_uart
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ790875685)
* @version          查看doc内version文件 版本说明
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



#define WRIELESS_UART_NUMBER       1                                           // 定义接入的无线串口的数量 默认最大就三个
#define WRIELESS_UART_INDEX        UART_2                                      // 无线串口 1 对应使用的串口号
#define WRIELESS_UART_BUAD_RATE    115200                                      // 无线串口 1 对应使用的串口波特率
#define WRIELESS_UART_TX_PIN       UART2_RX_D6                                 // 无线串口 1 对应模块的 TX 要接到单片机的 RX
#define WRIELESS_UART_RX_PIN       UART2_TX_D5                                 // 无线串口 1 对应模块的 RX 要接到单片机的 TX
#define WRIELESS_UART_RTS_PIN      E10                                         // 无线串口 1 对应模块的 RTS 引脚



// ------------------------------------ 自动波特率 ------------------------------------
// 注意事项1：无线转串口模块版本是V2.0以下的是无法开启自动波特率的。
// 注意事项2：开启自动波特率务必连接RTS引脚 否则会开启失败。
// 注意事项3：模块自动波特率失败的话 可以尝试断电重启

// 开启自动波特率务必阅读上面两条 注意事项
// 开启自动波特率务必阅读上面两条 注意事项
// 开启自动波特率务必阅读上面两条 注意事项

// 0：关闭自动波特率
// 1：开启自动波特率 自动波特率的作用是修改 WIRELESS_UART_BAUD 之后不需要对模块进行配置 模块会自动设置为对应的波特率

#define WIRELESS_UART_AUTO_BAUD_RATE    0
// ------------------------------------ 自动波特率 ------------------------------------

#define WIRELESS_UART_BUFFER_SIZE       64
#define WIRELESS_UART_TIMEOUT_COUNT     0x64

uint8_t     wireless_uart_init              (void);

void        wireless_uart_callback(void);
uint32_t    wireless_uart_send_buff     (uint8_t *buff, uint32_t len);
uint32_t    wireless_uart_read_buff     (uint8_t *buff, uint32_t len);



#endif
