/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ����������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            zf_driver_delay
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ790875685)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/
#include "zf_driver_delay.h"


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��શ�ʱ����ʱ
//  @param      time                us
//  @return     void
//  Sample usage:                   �ڲ�ʹ�ã��û��������
//-------------------------------------------------------------------------------------------------------------------
void system_delay(uint64_t time)
{
    SysTick->CTLR = 0;
    SysTick->CNT  = 0;
    SysTick->CTLR = 1;          //����ϵͳ������ systick��HCLK/8 ʱ���� us

    //while((*(volatile uint32*)0xE000F004) <= time);
    while(SysTick->CNT <= time);
}
int system_cnt(uint8_t flag)
{
    if(flag)
    {
        SysTick->CTLR = 0;
            SysTick->CNT  = 0;
            SysTick->CTLR = 1;          //����ϵͳ������ systick��HCLK/8 ʱ���� us

    }
    else {

        return (SysTick->CNT)/18;

    }
    return 0;
}
