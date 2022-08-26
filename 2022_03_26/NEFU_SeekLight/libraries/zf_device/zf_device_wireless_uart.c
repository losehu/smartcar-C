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
* @note             version:
*                   V2.0 ��д���ߴ���������
********************************************************************************************************************/

#include "zf_device_wireless_uart.h"
#include "zf_common_fifo.h"
#include "zf_device_type.h"

static  fifo_struct         wireless_uart_fifo;
static  uint8_t             wireless_uart_buffer[WIRELESS_UART_BUFFER_SIZE];                                // ��Ӧ��һ�� ���ߴ��� ���ݴ������




const   uart_index_enum     wireless_uart_list[WRIELESS_UART_NUMBER]        = {WRIELESS_UART_INDEX};
const   uint32_t            wireless_uart_baud_list[WRIELESS_UART_NUMBER]   = {WRIELESS_UART_BUAD_RATE};
const   uart_pin_enum       wireless_uart_tx_list[WRIELESS_UART_NUMBER]     = {WRIELESS_UART_TX_PIN};
const   uart_pin_enum       wireless_uart_rx_list[WRIELESS_UART_NUMBER]     = {WRIELESS_UART_RX_PIN};
const   gpio_pin_enum       wireless_uart_rts_list[WRIELESS_UART_NUMBER]    = {WRIELESS_UART_RTS_PIN};

static          uint8_t     wireless_uart_data[WRIELESS_UART_NUMBER];

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ת�����жϻص�����
// @param       void
// @return      void
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
void wireless_uart_callback (void)
{
    uart_query_byte(WRIELESS_UART_INDEX, &wireless_uart_data[0]);
    fifo_write_buffer(&wireless_uart_fifo, &wireless_uart_data[0], 1);
#if WIRELESS_UART_AUTO_BAUD_RATE // �����Զ�������
    if(wireless_auto_baud_flag == 1 && fifo_used(&wireless_uart_fifo) == 3)
    {
        wireless_auto_baud_flag = 3;
        fifo_read_buffer(&wireless_uart_fifo, (uint8_t *)wireless_auto_baud_data, (uint32_t *)&wireless_auto_baud_flag, FIFO_READ_AND_CLEAN);
    }
#endif
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ת����ģ���ʼ��
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
        gpio_init(wireless_uart_rts_list[loop_temp], GPI, GPIO_HIGH, GPI_PULL_UP);                                            // ��ʼ����������
    }
#if(0 == WIRELESS_UART_AUTO_BAUD_RATE)                                                               // �ر��Զ�������
    //������ʹ�õĲ�����Ϊ115200��Ϊ����ת����ģ���Ĭ�ϲ����ʣ�������������������������ģ�鲢�޸Ĵ��ڵĲ�����
    for(loop_temp = 0; loop_temp < WRIELESS_UART_NUMBER; loop_temp++)
    {
        uart_init (wireless_uart_list[loop_temp], wireless_uart_baud_list[loop_temp], wireless_uart_rx_list[loop_temp], wireless_uart_tx_list[loop_temp]);          // ��ʼ������
        uart_rx_interrupt(wireless_uart_list[loop_temp], 1);
    }
#elif(1 == WIRELESS_UART_AUTO_BAUD_RATE)        // �����Զ�������
    uint8_t rts_init_status;
    uint16_t time_count = 0;

    for(loop_temp = 0; loop_temp < WRIELESS_UART_NUMBER; loop_temp++)
    {
        wireless_auto_baud_flag = 0;
        wireless_auto_baud_data[0] = 0;
        wireless_auto_baud_data[1] = 1;
        wireless_auto_baud_data[2] = 3;

        rts_init_status = gpio_get(wireless_uart_rts_list[loop_temp]);
        gpio_init(wireless_uart_rts_list[loop_temp], GPO, rts_init_status, GPO_PUSH_PULL);                                    // ��ʼ����������

        uart_init (wireless_uart_list[loop_temp], wireless_uart_baud_list[loop_temp], wireless_uart_rx_list[loop_temp], wireless_uart_tx_list[loop_temp]);          // ��ʼ������
        uart_rx_interrupt(wireless_uart_list[loop_temp], 1);

        system_delay_ms(5);                                                                        // ģ���ϵ�֮����Ҫ��ʱ�ȴ�
        gpio_set(wireless_uart_rts_list[loop_temp], !rts_init_status);                                                        // RTS�������ߣ������Զ�������ģʽ
        system_delay_ms(100);                                                                      // RTS����֮�������ʱ20ms
        gpio_toggle(wireless_uart_rts_list[loop_temp]);                                                                       // RTS����ȡ��

        wireless_auto_baud_flag = 1;

        uart_putchar(wireless_uart_list[loop_temp], wireless_auto_baud_data[0]);                  // �����ض����� ����ģ���Զ��жϲ�����
        uart_putchar(wireless_uart_list[loop_temp], wireless_auto_baud_data[1]);                  // �����ض����� ����ģ���Զ��жϲ�����
        uart_putchar(wireless_uart_list[loop_temp], wireless_auto_baud_data[2]);                  // �����ض����� ����ģ���Զ��жϲ�����
        system_delay_ms(20);

        time_count = 0;
        if(3 != wireless_auto_baud_flag)                                                            // �����Զ��������Ƿ����
        {
            while(time_count++)
                system_delay_ms(1);
        }
        if(time_count >= WIRELESS_UART_TIMEOUT_COUNT)
            return 1;                                                                        // ���������뵽������� ˵���Զ�������ʧ����

        time_count = 0;
        if( 0xa5 != wireless_auto_baud_data[0] &&                                                   // �����Զ��������Ƿ���ȷ
            0xff != wireless_auto_baud_data[1] &&                                                   // �����Զ��������Ƿ���ȷ
            0xff != wireless_auto_baud_data[2] )                                                    // �����Զ��������Ƿ���ȷ
        {
            while(time_count++)
                system_delay_ms(1);
        }
        if(time_count >= WIRELESS_UART_TIMEOUT_COUNT)
            return 1;                                                                        // ���������뵽������� ˵���Զ�������ʧ����
        wireless_auto_baud_flag = 0;

        gpio_init(wireless_uart_rts_list[loop_temp], GPI, 0, GPI_PULL_UP);                                                    // ��ʼ����������
        system_delay_ms(10);                                                                       // ��ʱ�ȴ� ģ��׼������
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
// @brief       ����ת����ģ�� ���ڷ�������
// @param       buff            ���ݵ�ַ
// @param       len             ���ݳ���
// @return      uint32_t        ��ʾ��Ϣ
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
//uint32_t wireless_uart_send_buff (uint8_t *buff, uint32_t len)
//{
//    while(len > 30)
//    {
////        time_count = 0;
////        while(gpio_get_level(WRIELESS_UART_RTS_PIN) && time_count++ < WIRELESS_UART_TIMEOUT_COUNT)                  // ���RTSΪ�͵�ƽ���������������
////        if(time_count >= WIRELESS_UART_TIMEOUT_COUNT)
////            return len;                                                                         // ģ��æ,�������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
//        uart_write_buffer(WRIELESS_UART_INDEX, buff, 30);
//
//        buff += 30;                                                                             // ��ַƫ��
//        len -= 30;                                                                              // ����
//    }
//
////    time_count = 0;
////    while(gpio_get_level(WRIELESS_UART_RTS_PIN) && time_count++ < WIRELESS_UART_TIMEOUT_COUNT)                      // ���RTSΪ�͵�ƽ���������������
////        system_delay_ms(1);
////    if(time_count >= WIRELESS_UART_TIMEOUT_COUNT)
////        return len;// ģ��æ,�������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
//
//uart_write_buffer(WRIELESS_UART_INDEX, buff, len);                                          // ������������
//
//    return 0;
//}
uint32_t wireless_uart_send_buff (uint8_t *buff, uint32_t len)
{
    uint16_t time_count = 0;
    while(len > 30)
    {
        time_count = 0;
        while(gpio_get_level(WRIELESS_UART_RTS_PIN) && time_count++ < WIRELESS_UART_TIMEOUT_COUNT)                  // ���RTSΪ�͵�ƽ���������������
            system_delay_ms(1);
        if(time_count >= WIRELESS_UART_TIMEOUT_COUNT)
            return len;                                                                         // ģ��æ,�������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
        uart_write_buffer(WRIELESS_UART_INDEX, buff, 30);

        buff += 30;                                                                             // ��ַƫ��
        len -= 30;                                                                              // ����
    }

    time_count = 0;
    while(gpio_get_level(WRIELESS_UART_RTS_PIN) && time_count++ < WIRELESS_UART_TIMEOUT_COUNT)                      // ���RTSΪ�͵�ƽ���������������
        system_delay_ms(1);
    if(time_count >= WIRELESS_UART_TIMEOUT_COUNT)
        return len;                                                                             // ģ��æ,�������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
    uart_write_buffer(WRIELESS_UART_INDEX, buff, len);                                          // ������������

    return 0;
}
//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ת����ģ�� ���ڽ�������
// @param       buff            ���ݵ�ַ
// @param       len             ���ݳ���
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





