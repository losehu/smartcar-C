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
 * @date            2021-11-25
 * @note            version:
 *                  V1.1 2021.6.23 ��ȡ��д�뺯������ر��жϵĲ����������ȡ��ʱ��д���ϣ�����ʣ��ռ�������
 *                  V1.2 2021.7.06 �޸���ȡ��д�뺯����βָ�뵽β������  ���Ͷ������
********************************************************************************************************************/

#include "zf_common_fifo.h"
#include "zf_common_interrupt.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief       FIFO ��ʼ�� ���ض�Ӧ������
// @param       *fifo           FIFO ����ָ��
// @param       *buffer_addr    Ҫ���صĻ�����
// @param       size            ��������С
// @return      fifo_state_enum ����״̬
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
fifo_state_enum fifo_init (fifo_struct *fifo, uint8_t *buffer_addr, uint32_t size)
{
    if(buffer_addr == NULL)
        return FIFO_BUFFER_NULL;
    fifo->buffer    = buffer_addr;
    fifo->head      = 0;
    fifo->end       = 0;
    fifo->size      = size;
    fifo->max       = size;
    return FIFO_SUCCESS;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       FIFO ͷָ��λ��
// @param       *fifo           FIFO ����ָ��
// @param       offset          ƫ����
// @return      void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void fifo_head_offset (fifo_struct *fifo, uint32_t offset)
{
    fifo->head += offset;
    
    while(fifo->max <= fifo->head)                                              // �����Χ���������������С ֱ��С����󻺳�����С
    {
        fifo->head -= fifo->max;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       FIFO βָ��λ��
// @param       *fifo           FIFO ����ָ��
// @param       offset          ƫ����
// @return      void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void fifo_end_offset (fifo_struct *fifo, uint32_t offset)
{
    fifo->end += offset;
    
    while(fifo->max <= fifo->end)                                               // �����Χ���������������С ֱ��С����󻺳�����С
    {
        fifo->end -= fifo->max;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       FIFO ���û�����
// @param       *fifo           FIFO ����ָ��
// @return      void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void fifo_clean (fifo_struct *fifo)
{
    fifo->head      = 0;
    fifo->end       = 0;
    fifo->size      = fifo->max;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       FIFO ���û�����
// @param       *fifo           FIFO ����ָ��
// @return      void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint32_t fifo_used (fifo_struct *fifo)
{
    return (fifo->max - fifo->size);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       FIFO ���û�����
// @param       *fifo           FIFO ����ָ��
// @param       *dat            ������Դ������ָ��
// @param       length          ��Ҫд������ݳ���
// @return      fifo_state_enum ����״̬
// Sample usage:                if(fifo_write(&fifo,data,32)!=FIFO_SUCCESS) while(1);
//-------------------------------------------------------------------------------------------------------------------
fifo_state_enum fifo_write_buffer (fifo_struct *fifo, uint8_t *dat, uint32_t length)
{
    uint32_t temp_length;
    
    if(length < fifo->size)                                                     // ʣ��ռ��㹻װ�±�������
    {
        interrupt_disable_all();
        temp_length = fifo->max - fifo->head;                                   // ����ͷָ����뻺����β���ж��ٿռ�

        if(length > temp_length)                                                // ���뻺����β���Ȳ���д������ ���λ������ֶβ���
        {
            memcpy(&fifo->buffer[fifo->head], dat, temp_length);                // ������һ������
            fifo_head_offset(fifo, temp_length);                                // ͷָ��ƫ��
            dat += temp_length;                                                 // ��ȡ����ƫ��
            memcpy(&fifo->buffer[fifo->head], dat, length - temp_length);       // ������һ������
            fifo_head_offset(fifo, length - temp_length);                       // ͷָ��ƫ��
        }
        else
        {
            memcpy(&fifo->buffer[fifo->head], dat, length);                     // һ������д��
            fifo_head_offset(fifo, length);                                     // ͷָ��ƫ��
        }

        fifo->size -= length;                                                   // ������ʣ�೤�ȼ�С
        interrupt_enable_all(0);
    }
    else
    {
        return FIFO_SPACE_NO_ENOUGH;
    }
    
    return FIFO_SUCCESS;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       FIFO ���û�����
// @param       *fifo           FIFO ����ָ��
// @param       *dat            Ŀ�껺����ָ��
// @param       *length         ��ȡ�����ݳ��� ���û����ô����������ᱻ�޸�
// @param       flag            �Ƿ��� FIFO ״̬ ��ѡ���Ƿ���ն�ȡ������
// @return      fifo_state_enum ����״̬
// Sample usage:                if(fifo_read(&fifo,data,32,FIFO_READ_ONLY)!=FIFO_SUCCESS) while(1);
//-------------------------------------------------------------------------------------------------------------------
fifo_state_enum fifo_read_buffer (fifo_struct *fifo, uint8_t *dat, uint32_t *length, fifo_operation_enum flag)
{
    uint8_t data_check = 0;
    uint32_t temp_length;

    if(*length > fifo_used(fifo))
    {
        *length = (fifo->max - fifo->size);                                     // ������ȡ�ĳ���
        data_check = 1;                                                         // ��־���ݲ���
    }

    temp_length = fifo->max - fifo->end;                                        // ����βָ����뻺����β���ж��ٿռ�
    if(*length <= temp_length)                                                  // �㹻һ���Զ�ȡ���
    {
        if(NULL != dat)    memcpy(dat, &fifo->buffer[fifo->end], *length);      // һ���Զ�ȡ���
    }
    else
    {
        if(NULL != dat)
        {
            memcpy(dat, &fifo->buffer[fifo->end], temp_length);                 // ������һ������
            memcpy(&dat[temp_length], &fifo->buffer[0], *length - temp_length); // �����ڶ�������
        }
    }
    
    if(flag == FIFO_READ_AND_CLEAN)                                             // ���ѡ���ȡ������ FIFO ״̬
    {
        interrupt_disable_all();
        fifo_end_offset(fifo, *length);                                         // �ƶ� FIFO ͷָ��
        fifo->size += *length;                                                  
        interrupt_enable_all(0);
    }

    return (data_check?FIFO_DATA_NO_ENOUGH:FIFO_SUCCESS);
}
