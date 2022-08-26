/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            zf_common_fifo
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ790875685)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-11
 * @note            version:
 *                  V1.1 2021.6.23 ��ȡ��д�뺯������ر��жϵĲ����������ȡ��ʱ��д���ϣ�����ʣ��ռ�������
 *                  V1.2 2021.7.06 �޸���ȡ��д�뺯����βָ�뵽β������  ���Ͷ������
********************************************************************************************************************/

#ifndef _zf_common_fifo_h_
#define _zf_common_fifo_h_

#include "stdint.h"
#include "string.h"

typedef struct
{
    uint8_t     *buffer;                                                        // ����ָ��
    uint32_t    head;                                                           // ����ͷָ�� ����ָ��յĻ���
    uint32_t    end;                                                            // ����βָ�� ����ָ��ǿջ��棨����ȫ�ճ��⣩
    uint32_t    size;                                                           // ����ʣ���С
    uint32_t    max;                                                            // �����ܴ�С
}fifo_struct;

typedef enum
{
    FIFO_SUCCESS,

    FIFO_BUFFER_NULL,
    FIFO_SPACE_NO_ENOUGH,
    FIFO_DATA_NO_ENOUGH,
}fifo_state_enum;

typedef enum
{
    FIFO_READ_AND_CLEAN,
    FIFO_READ_ONLY,
}fifo_operation_enum;

fifo_state_enum fifo_init           (fifo_struct *fifo, uint8_t *buffer_addr, uint32_t size);
void            fifo_head_offset    (fifo_struct *fifo, uint32_t offset);
void            fifo_end_offset     (fifo_struct *fifo, uint32_t offset);
void            fifo_clean          (fifo_struct *fifo);
uint32_t        fifo_used           (fifo_struct *fifo);

fifo_state_enum fifo_read_buffer    (fifo_struct *fifo, uint8_t *dat, uint32_t *length, fifo_operation_enum flag);
fifo_state_enum fifo_write_buffer   (fifo_struct *fifo, uint8_t *dat, uint32_t length);

#endif
