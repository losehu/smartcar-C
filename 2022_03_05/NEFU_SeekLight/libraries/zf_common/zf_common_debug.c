/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_common_debug
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
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
// @brief       debug��ʼ������
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
        DEBUG_UART_INDEX,                                                       // �� zf_common_debug.h �в鿴��Ӧֵ
        DEBUG_UART_BAUDRATE,                                                    // �� zf_common_debug.h �в鿴��Ӧֵ
        DEBUG_UART_TX_PIN,                                                      // �� zf_common_debug.h �в鿴��Ӧֵ
        DEBUG_UART_RX_PIN);                                                     // �� zf_common_debug.h �в鿴��Ӧֵ

    if(DEBUG_UART_USE_INTERRUPT)                                                // ������ô����ж�
    {
        uart_rx_interrupt(DEBUG_UART_INDEX, 1);                                 // ʹ�ܶ�Ӧ���ڽ����ж�
    }
    zf_debug_init_flag = 1;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       debug ���Դ�����
// @param       bool        �ж��Ƿ񴥷�����
// @param       *file       �ļ���
// @param       line        Ŀ������
// @return      void
// Sample usage:            �����������ֱ�ӵ��õ� ʹ�� zf_commmon_debug.h �е� zf_assert(x) �ӿ�
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
        // ���������ת������ͣס��
        // һ����ĺ����������ݳ�����
        // �������Լ����õ� zf_assert(x) �ӿڴ�������

        // ��������� debug_init ��ʼ���� log ���
        // ���ڶ�Ӧ�������ȥ�鿴���ĸ��ļ�����һ�б���

        // ���û�г�ʼ�� debug
        // �ǾͿ������ file ���ַ���ֵ�� line ������
        // �Ǵ�������ļ�·�����ƺͶ�Ӧ��������

        // ��ȥ���Կ�����Ϊʲô��������
        __NOP();
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       debug ������Ϣ������
// @param       bool        �ж��Ƿ񴥷�����
// @param       *str        Ҫ����ĵ�����Ϣ
// @param       *file       �ļ���
// @param       line        Ŀ������
// @return      void
// Sample usage:            �����������ֱ�ӵ��õ� ʹ�� zf_commmon_debug.h �е� zf_log(x,str) �ӿ�
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

#if DEBUG_UART_USE_INTERRUPT                                                    // �������� ֻ�������ô����жϲű���

uint8_t  debug_ring_buffer[DEBUG_RING_BUFFER_LEN];                              // �������λ�����
uint32_t debug_ring_index = 0;                                                  // ���λ������±�����

//-------------------------------------------------------------------------------------------------------------------
// @brief       debug �����жϴ����� isr.c �ж�Ӧ�����жϷ���������
// @param       void
// @return      void
// Sample usage:            debug_interrupr_handler();
//-------------------------------------------------------------------------------------------------------------------
void debug_interrupr_handler (void)
{
    uart_query_byte(DEBUG_UART_INDEX, &debug_ring_buffer[debug_ring_index++]);  // ��ȡ���ݵ����λ�����
    if(debug_ring_index == DEBUG_RING_BUFFER_LEN)   debug_ring_index = 0;       // ���λ�������β�ν�
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��ȡ debug ���λ���������
// @param       *data       �������ݴ�ŵ�����ָ��
// @return      uint32_t    �������ݳ���
// Sample usage:            uint8_t data[64]; len = debug_read_ring_buffer(data);
//-------------------------------------------------------------------------------------------------------------------
uint32_t debug_read_ring_buffer (uint8_t *data)
{
    if(debug_ring_index == 0)   return 0;
    uint32_t temp = debug_ring_index;                                           // ��ȡ���ݳ���
    memcpy(data, debug_ring_buffer, temp);                                      // ��������
    debug_ring_index = 0;                                                       // ����±�
    return temp;
}

#endif


//-------------------------------------------------------------------------------------------------------------------
//  @brief      printf�ض���
//  @param      void
//  @return     void
//  @since      v1.0
//  Sample usage:              �ض���printf��DEBUG������
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

