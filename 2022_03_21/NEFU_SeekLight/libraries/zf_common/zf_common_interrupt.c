/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_common_interrupt
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
********************************************************************************************************************/


#include "zf_common_interrupt.h"



//-------------------------------------------------------------------------------------------------------------------
// @brief       ָ���ж�ʹ��
// @param       irqn            ָ���жϺ� �ɲ鿴 isr.c ��Ӧ�жϷ������ı�ע
// @return      void
// Sample usage:                interrupt_enable(UART1_IRQn);
//-------------------------------------------------------------------------------------------------------------------
void interrupt_enable (IRQn_Type irqn)
{
    NVIC_EnableIRQ(irqn);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ָ���ж�����
// @param       irqn            ָ���жϺ� �ɲ鿴 isr.c ��Ӧ�жϷ������ı�ע
// @return      void
// Sample usage:                interrupt_disable(UART1_IRQn);
//-------------------------------------------------------------------------------------------------------------------
void interrupt_disable (IRQn_Type irqn)
{
    NVIC_DisableIRQ(irqn);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ָ���ж��������ȼ�
// @param       irqn            ָ���жϺ� �ɲ鿴 isr.c ��Ӧ�жϷ������ı�ע
// @param       priority        �ж����ȼ� 0-7 Խ��Խ��
// @return      void
// Sample usage:                interrupt_enable(UART1_IRQn, 0);
//-------------------------------------------------------------------------------------------------------------------
void interrupt_set_priority (IRQn_Type irqn, uint8_t priority)
{
    NVIC_SetPriority(irqn, priority);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       �ж����ʼ�� clock_init �ڲ�����
// @param       void
// @return      void
// Sample usage:                interrupt_init();
//-------------------------------------------------------------------------------------------------------------------
void interrupt_init (void)
{
    //NVIC_SetPriorityGrouping(4);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ȫ���ж�ʹ��
// @param       void
// @return      void
// Sample usage:                interrupt_enable_all();
//-------------------------------------------------------------------------------------------------------------------
void interrupt_enable_all (uint8_t state)
{
    if(state == 0)
    {
        extern void en_interrupt();
        en_interrupt();
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ȫ���ж�����
// @param       void
// @return      void
// Sample usage:                interrupt_disable_all();
//-------------------------------------------------------------------------------------------------------------------
uint32_t interrupt_disable_all (void)
{
    uint32_t temp;
    extern void dis_interrupt();
    dis_interrupt();
    temp = __get_MSTATUS();
    //���ص�ǰmstatus�Ĵ�����ֵ
    return temp;
}
