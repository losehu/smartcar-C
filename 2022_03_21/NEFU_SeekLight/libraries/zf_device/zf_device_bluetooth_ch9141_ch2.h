/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             zf_device_bluetooth_ch9141
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ790875685)
* @version          查看doc内version文件 版本说明
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

#define BLUETOOTH_CH9141_INDEX_CH2              UART_7                                      // 蓝牙模块 1 对应使用的串口号
#define BLUETOOTH_CH9141_BUAD_RATE_CH2          115200                                      // 蓝牙模块 1 对应使用的串口波特率
#define BLUETOOTH_CH9141_TX_PIN_CH2             UART7_RX_E13                                 // 蓝牙模块 1 对应模块的 TX 要接到单片机的 RX
#define BLUETOOTH_CH9141_RX_PIN_CH2             UART7_TX_E12                                 // 蓝牙模块 1 对应模块的 RX 要接到单片机的 TX
#define BLUETOOTH_CH9141_RTS_PIN_CH2            E8                                         // 蓝牙模块 1 对应模块的 RTS 引脚
#define BLUETOOTH_CH9141_MODE_CH2               BLUETOOTH_CH9141_MASTER                               // 蓝牙模块 1 对应模块的模式
#define BLUETOOTH_CH9141_TAGET_INFO_CH2         "F0:78:19:26:3B:38,000000"                  // 蓝牙模块 1 主机模式下 准备连接的目标从机对应的 MAC 地址与密码
#define BLUETOOTH_CH9141_SELF_NAME_CH2          "ch9141_1"                                  // 蓝牙模块 1 从机模式下 自己的名称名称不超过 15 个字符
#define BLUETOOTH_CH9141_SELF_PASSWORD_CH2      "000000"                                    // 蓝牙模块 1 从机模式下 自己的密码信息 六个数字

#define BLUETOOTH_CH9141_BUFFER_SIZE_CH2        64
#define BLUETOOTH_CH9141_TIMEOUT_COUNT_CH2      500
#define BLUETOOTH_CH9141_DEFAULT_TX_POWER_CH2   BLUETOOTH_CH9141_TX_POWER_4DB



void        bluetooth_ch9141_uart_callback_ch2  (void);
uint8_t     bluetooth_ch9141_init_ch2           (void);
uint32_t    bluetooth_ch9141_send_buff_ch2      (uint8_t *buff, uint32_t len);
uint32_t    bluetooth_ch9141_read_buff_ch2      (uint8_t *buff, uint32_t len);

#endif

