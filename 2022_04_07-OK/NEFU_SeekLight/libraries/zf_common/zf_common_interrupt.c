/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             zf_common_interrupt
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ790875685)
* @version          查看doc内version文件 版本说明
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
********************************************************************************************************************/


#include "zf_common_interrupt.h"



//-------------------------------------------------------------------------------------------------------------------
// @brief       指定中断使能
// @param       irqn            指定中断号 可查看 isr.c 对应中断服务函数的标注
// @return      void
// Sample usage:                interrupt_enable(UART1_IRQn);
//-------------------------------------------------------------------------------------------------------------------
void interrupt_enable (IRQn_Type irqn)
{
    NVIC_EnableIRQ(irqn);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       指定中断屏蔽
// @param       irqn            指定中断号 可查看 isr.c 对应中断服务函数的标注
// @return      void
// Sample usage:                interrupt_disable(UART1_IRQn);
//-------------------------------------------------------------------------------------------------------------------
void interrupt_disable (IRQn_Type irqn)
{
    NVIC_DisableIRQ(irqn);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       指定中断设置优先级
// @param       irqn            指定中断号 可查看 isr.c 对应中断服务函数的标注
// @param       priority        中断优先级 0-7 越低越高
// @return      void
// Sample usage:                interrupt_enable(UART1_IRQn, 0);
//-------------------------------------------------------------------------------------------------------------------
void interrupt_set_priority (IRQn_Type irqn, uint8_t priority)
{
    NVIC_SetPriority(irqn, priority);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       中断组初始化 clock_init 内部调用
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
// @brief       全局中断使能
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
// @brief       全局中断屏蔽
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
    //返回当前mstatus寄存器的值
    return temp;
}
