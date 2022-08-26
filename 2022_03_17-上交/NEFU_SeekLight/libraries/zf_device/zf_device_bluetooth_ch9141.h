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
#ifndef _zf_device_bluetooth_ch9141_h_
#define _zf_device_bluetooth_ch9141_h_


#include "zf_common_fifo.h"
#include "zf_driver_gpio.h"
#include "zf_driver_uart.h"
#include "zf_driver_delay.h"

#define BLUETOOTH_CH9141_INDEX              UART_6                                     // 蓝牙模块 1 对应使用的串口号
#define BLUETOOTH_CH9141_BUAD_RATE          115200                                      // 蓝牙模块 1 对应使用的串口波特率
#define BLUETOOTH_CH9141_TX_PIN             UART6_RX_C1                                 // 蓝牙模块 1 对应模块的 TX 要接到单片机的 RX
#define BLUETOOTH_CH9141_RX_PIN             UART6_TX_C0                                 // 蓝牙模块 1 对应模块的 RX 要接到单片机的 TX
#define BLUETOOTH_CH9141_RTS_PIN            E0                                        // 蓝牙模块 1 对应模块的 RTS 引脚
#define BLUETOOTH_CH9141_MODE               BLUETOOTH_CH9141_MASTER                               // 蓝牙模块 1 对应模块的模式
#define BLUETOOTH_CH9141_TAGET_INFO         "7E:7D:19:26:3B:38,000000"                  // 蓝牙模块 1 主机模式下 准备连接的目标从机对应的 MAC 地址与密码
#define BLUETOOTH_CH9141_SELF_NAME          "ch9141_1"                                  // 蓝牙模块 1 从机模式下 自己的名称名称不超过 15 个字符
#define BLUETOOTH_CH9141_SELF_PASSWORD      "000000"                                    // 蓝牙模块 1 从机模式下 自己的密码信息 六个数字

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
    BLUETOOTH_CH9141_MASTER_NO_INIT = 0,        //主机未初始化
    BLUETOOTH_CH9141_MASTER_SCANNING,           //主机扫描中
    BLUETOOTH_CH9141_MASTER_CONNECTING,         //主机连接中
    BLUETOOTH_CH9141_MASTER_CONNECTED,          //主机已连接
    BLUETOOTH_CH9141_MASTER_DISCONNECTED,       //主机连接断开

    BLUETOOTH_CH9141_SLAVE_NO_INIT,         //从机未初始化
    BLUETOOTH_CH9141_SLAVE_OK_INIT,         //从机完成初始化
    BLUETOOTH_CH9141_SLAVE_BROADCAST,       //从机广播中
    BLUETOOTH_CH9141_SLAVE_BROADCAST_READY, //从机准备广播
    BLUETOOTH_CH9141_SLAVE_CONNECT_TIMEOUT, //从机连接超时
    BLUETOOTH_CH9141_SLAVE_CONNECTED,       //从机已连接
    BLUETOOTH_CH9141_SLAVE_ERROR,           //从机出现错误
}bluetooth_ch9141_state_enum;

typedef enum
{
    BLUETOOTH_CH9141_BROADCAST,   //广播模式
    BLUETOOTH_CH9141_MASTER,        //主机模式
    BLUETOOTH_CH9141_SLAVE,         //从机模式
}bluetooth_ch9141_mode_enum;

void        bluetooth_ch9141_uart_callback_ch1      (void);
uint8_t     bluetooth_ch9141_init_ch1               (void);
uint32_t    bluetooth_ch9141_send_buff_ch1          (uint8_t *buff, uint32_t len);
uint32_t    bluetooth_ch9141_read_buff_ch1          (uint8_t *buff, uint32_t len);

#endif

