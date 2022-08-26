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
* @note             version:
*                   V2.0 重写无线串口驱动。
********************************************************************************************************************/

#include "zf_device_wireless_uart.h"
#include "zf_common_fifo.h"
#include "zf_device_type.h"

static  fifo_struct         wireless_uart_fifo;
static  uint8_t             wireless_uart_buffer[WIRELESS_UART_BUFFER_SIZE];                                // 对应第一个 无线串口 数据存放数组




const   uart_index_enum     wireless_uart_list[WRIELESS_UART_NUMBER]        = {WRIELESS_UART_INDEX};
const   uint32_t            wireless_uart_baud_list[WRIELESS_UART_NUMBER]   = {WRIELESS_UART_BUAD_RATE};
const   uart_pin_enum       wireless_uart_tx_list[WRIELESS_UART_NUMBER]     = {WRIELESS_UART_TX_PIN};
const   uart_pin_enum       wireless_uart_rx_list[WRIELESS_UART_NUMBER]     = {WRIELESS_UART_RX_PIN};
const   gpio_pin_enum       wireless_uart_rts_list[WRIELESS_UART_NUMBER]    = {WRIELESS_UART_RTS_PIN};

static          uint8_t     wireless_uart_data[WRIELESS_UART_NUMBER];

//-------------------------------------------------------------------------------------------------------------------
// @brief       无线转串口中断回调函数
// @param       void
// @return      void
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
void wireless_uart_callback (void)
{
    uart_query_byte(WRIELESS_UART_INDEX, &wireless_uart_data[0]);
    fifo_write_buffer(&wireless_uart_fifo, &wireless_uart_data[0], 1);
#if WIRELESS_UART_AUTO_BAUD_RATE // 开启自动波特率
    if(wireless_auto_baud_flag == 1 && fifo_used(&wireless_uart_fifo) == 3)
    {
        wireless_auto_baud_flag = 3;
        fifo_read_buffer(&wireless_uart_fifo, (uint8_t *)wireless_auto_baud_data, (uint32_t *)&wireless_auto_baud_flag, FIFO_READ_AND_CLEAN);
    }
#endif
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       无线转串口模块初始化
// @param       void
// @return      void
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
uint8_t wireless_uart_init (void)
{

    fifo_struct         *fifo_list[WRIELESS_UART_NUMBER]    = {&wireless_uart_fifo};
    uint8_t             *buffer_list[WRIELESS_UART_NUMBER]  = {wireless_uart_buffer};

    set_wireless_type(WIRELESS_UART);
    uint8_t loop_temp;
    for(loop_temp = 0; loop_temp < WRIELESS_UART_NUMBER; loop_temp++)
    {
        fifo_init(fifo_list[loop_temp], buffer_list[loop_temp], WIRELESS_UART_BUFFER_SIZE);
        gpio_init(wireless_uart_rts_list[loop_temp], GPI, GPIO_HIGH, GPI_PULL_UP);                                            // 初始化流控引脚
    }
#if(0 == WIRELESS_UART_AUTO_BAUD_RATE)                                                               // 关闭自动波特率
    //本函数使用的波特率为115200，为无线转串口模块的默认波特率，如需其他波特率请自行配置模块并修改串口的波特率
    for(loop_temp = 0; loop_temp < WRIELESS_UART_NUMBER; loop_temp++)
    {
        uart_init (wireless_uart_list[loop_temp], wireless_uart_baud_list[loop_temp], wireless_uart_rx_list[loop_temp], wireless_uart_tx_list[loop_temp]);          // 初始化串口
        uart_rx_interrupt(wireless_uart_list[loop_temp], 1);
    }
#elif(1 == WIRELESS_UART_AUTO_BAUD_RATE)        // 开启自动波特率
    uint8_t rts_init_status;
    uint16_t time_count = 0;

    for(loop_temp = 0; loop_temp < WRIELESS_UART_NUMBER; loop_temp++)
    {
        wireless_auto_baud_flag = 0;
        wireless_auto_baud_data[0] = 0;
        wireless_auto_baud_data[1] = 1;
        wireless_auto_baud_data[2] = 3;

        rts_init_status = gpio_get(wireless_uart_rts_list[loop_temp]);
        gpio_init(wireless_uart_rts_list[loop_temp], GPO, rts_init_status, GPO_PUSH_PULL);                                    // 初始化流控引脚

        uart_init (wireless_uart_list[loop_temp], wireless_uart_baud_list[loop_temp], wireless_uart_rx_list[loop_temp], wireless_uart_tx_list[loop_temp]);          // 初始化串口
        uart_rx_interrupt(wireless_uart_list[loop_temp], 1);

        system_delay_ms(5);                                                                        // 模块上电之后需要延时等待
        gpio_set(wireless_uart_rts_list[loop_temp], !rts_init_status);                                                        // RTS引脚拉高，进入自动波特率模式
        system_delay_ms(100);                                                                      // RTS拉高之后必须延时20ms
        gpio_toggle(wireless_uart_rts_list[loop_temp]);                                                                       // RTS引脚取反

        wireless_auto_baud_flag = 1;

        uart_putchar(wireless_uart_list[loop_temp], wireless_auto_baud_data[0]);                  // 发送特定数据 用于模块自动判断波特率
        uart_putchar(wireless_uart_list[loop_temp], wireless_auto_baud_data[1]);                  // 发送特定数据 用于模块自动判断波特率
        uart_putchar(wireless_uart_list[loop_temp], wireless_auto_baud_data[2]);                  // 发送特定数据 用于模块自动判断波特率
        system_delay_ms(20);

        time_count = 0;
        if(3 != wireless_auto_baud_flag)                                                            // 检验自动波特率是否完成
        {
            while(time_count++)
                system_delay_ms(1);
        }
        if(time_count >= WIRELESS_UART_TIMEOUT_COUNT)
            return 1;                                                                        // 如果程序进入到此语句内 说明自动波特率失败了

        time_count = 0;
        if( 0xa5 != wireless_auto_baud_data[0] &&                                                   // 检验自动波特率是否正确
            0xff != wireless_auto_baud_data[1] &&                                                   // 检验自动波特率是否正确
            0xff != wireless_auto_baud_data[2] )                                                    // 检验自动波特率是否正确
        {
            while(time_count++)
                system_delay_ms(1);
        }
        if(time_count >= WIRELESS_UART_TIMEOUT_COUNT)
            return 1;                                                                        // 如果程序进入到此语句内 说明自动波特率失败了
        wireless_auto_baud_flag = 0;

        gpio_init(wireless_uart_rts_list[loop_temp], GPI, 0, GPI_PULL_UP);                                                    // 初始化流控引脚
        system_delay_ms(10);                                                                       // 延时等待 模块准备就绪
        uart_rx_irq(wireless_uart_list[loop_temp], 0);
    }
    for(loop_temp = 0; loop_temp < WRIELESS_UART_NUMBER; loop_temp++)
    {
        uart_rx_irq(wireless_uart_list[loop_temp], 1);
    }
#endif
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       无线转串口模块 串口发送数据
// @param       buff            数据地址
// @param       len             数据长度
// @return      uint32_t        提示信息
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
//uint32_t wireless_uart_send_buff (uint8_t *buff, uint32_t len)
//{
//    while(len > 30)
//    {
////        time_count = 0;
////        while(gpio_get_level(WRIELESS_UART_RTS_PIN) && time_count++ < WIRELESS_UART_TIMEOUT_COUNT)                  // 如果RTS为低电平，则继续发送数据
////        if(time_count >= WIRELESS_UART_TIMEOUT_COUNT)
////            return len;                                                                         // 模块忙,如果允许当前程序使用while等待 则可以使用后面注释的while等待语句替换本if语句
//        uart_write_buffer(WRIELESS_UART_INDEX, buff, 30);
//
//        buff += 30;                                                                             // 地址偏移
//        len -= 30;                                                                              // 数量
//    }
//
////    time_count = 0;
////    while(gpio_get_level(WRIELESS_UART_RTS_PIN) && time_count++ < WIRELESS_UART_TIMEOUT_COUNT)                      // 如果RTS为低电平，则继续发送数据
////        system_delay_ms(1);
////    if(time_count >= WIRELESS_UART_TIMEOUT_COUNT)
////        return len;// 模块忙,如果允许当前程序使用while等待 则可以使用后面注释的while等待语句替换本if语句
//
//uart_write_buffer(WRIELESS_UART_INDEX, buff, len);                                          // 发送最后的数据
//
//    return 0;
//}
uint32_t wireless_uart_send_buff (uint8_t *buff, uint32_t len)
{
    uint16_t time_count = 0;
    while(len > 30)
    {
        time_count = 0;
        while(gpio_get_level(WRIELESS_UART_RTS_PIN) && time_count++ < WIRELESS_UART_TIMEOUT_COUNT)                  // 如果RTS为低电平，则继续发送数据
            system_delay_ms(1);
        if(time_count >= WIRELESS_UART_TIMEOUT_COUNT)
            return len;                                                                         // 模块忙,如果允许当前程序使用while等待 则可以使用后面注释的while等待语句替换本if语句
        uart_write_buffer(WRIELESS_UART_INDEX, buff, 30);

        buff += 30;                                                                             // 地址偏移
        len -= 30;                                                                              // 数量
    }

    time_count = 0;
    while(gpio_get_level(WRIELESS_UART_RTS_PIN) && time_count++ < WIRELESS_UART_TIMEOUT_COUNT)                      // 如果RTS为低电平，则继续发送数据
        system_delay_ms(1);
    if(time_count >= WIRELESS_UART_TIMEOUT_COUNT)
        return len;                                                                             // 模块忙,如果允许当前程序使用while等待 则可以使用后面注释的while等待语句替换本if语句
    uart_write_buffer(WRIELESS_UART_INDEX, buff, len);                                          // 发送最后的数据

    return 0;
}
//-------------------------------------------------------------------------------------------------------------------
// @brief       无线转串口模块 串口接收数据
// @param       buff            数据地址
// @param       len             数据长度
// @return      void
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
uint32_t wireless_uart_read_buff (uint8_t *buff, uint32_t len)
{
    uint32_t data_len = len;
    fifo_read_buffer(&wireless_uart_fifo, buff, &data_len, FIFO_READ_AND_CLEAN);
    return data_len;
}





