/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            zf_driver_delay
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/
#include "zf_driver_delay.h"


//-------------------------------------------------------------------------------------------------------------------
//  @brief      嘀嗒定时器延时
//  @param      time                us
//  @return     void
//  Sample usage:                   内部使用，用户无需关心
//-------------------------------------------------------------------------------------------------------------------
void system_delay(uint64_t time)
{
    SysTick->CTLR = 0;
    SysTick->CNT  = 0;
    SysTick->CTLR = 1;          //启动系统计数器 systick（HCLK/8 时基） us

    //while((*(volatile uint32*)0xE000F004) <= time);
    while(SysTick->CNT <= time);

}

