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

#ifndef _zf_board_h
#define _zf_board_h

#include "stdio.h"
#include "stdint.h"
#include "string.h"

#define PRINTF_ENABLE               1                   //printfʹ��

#define DEBUG_UART_INDEX            UART_3              //DEBUG����
#define DEBUG_UART_BAUDRATE         115200              //DEBUG���ڲ�����
#define DEBUG_UART_TX_PIN           UART3_TX_B10        //DEBUG����TX����
#define DEBUG_UART_RX_PIN           UART3_RX_B11        //DEBUG����RX����

#define zf_assert(x)                debug_assert_handler(x,__FILE__,__LINE__)   // ���� һ�����ڲ����ж� zf_assert(0) �Ͷ��Ա���
#define zf_log(x,str)               debug_log_handler(x,str,__FILE__,__LINE__)  // ������Ϣ��� ������һЩ������߾���֮������

#define DEBUG_UART_USE_INTERRUPT    1                                           // �Ƿ����� debug ���ڽ����ж�

#if DEBUG_UART_USE_INTERRUPT                                                    // ������� debug ���ڽ����ж�
#define DEBUG_RING_BUFFER_LEN       64                                          // ���廷�λ�������С Ĭ�� 64byte

extern uint8_t  debug_ring_buffer[DEBUG_RING_BUFFER_LEN];                       // ���λ�����
extern uint32_t debug_ring_index;                                               // �������±�

void        debug_interrupr_handler (void);
uint32_t    debug_read_ring_buffer  (uint8_t *data);
#endif

void        debug_init              (void);
void        debug_assert_handler    (uint8_t pass, char *file, uint32_t line);
void        debug_log_handler       (uint8_t pass, char *str, char *file, uint32_t line);
//int         _write                  (int fd, char *buf, int size);

#endif
