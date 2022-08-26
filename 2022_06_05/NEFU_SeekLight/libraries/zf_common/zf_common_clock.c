/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_common_clock
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-11-25
********************************************************************************************************************/


#include "zf_common_function.h"
#include "zf_common_interrupt.h"
#include "zf_common_clock.h"

uint32_t system_clock;                                                          // ϵͳʱ����Ϣ

static void clock_reset     (void);
static void clock_set_freq  (uint32_t clock);

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ʱ�ӳ�ʼ��
// @param       clock           ʱ��Ƶ�� �Ƽ�ʹ�� zf_common_clock.h �� system_clock_enum �����ѡ��
// @return      void
// Sample usage:                clock_init(SYSTEM_CLOCK_144M);                   // ��ʼ������ʱ��Ϊ 144MHz
//-------------------------------------------------------------------------------------------------------------------
void clock_init (uint32_t clock)
{
    system_clock = clock;                                                       // ��¼����ʱ��Ƶ��
    clock_reset();
    clock_set_freq(clock);
    interrupt_init();
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ʱ�ӻָ���ʼ���� �ڲ�����
// @param       void
// @return      void
//-------------------------------------------------------------------------------------------------------------------
static void clock_reset (void)
{
    RCC->CTLR |= (uint32_t)0x00000001;      //ʹ��HSI����
    RCC->CFGR0 &= (uint32_t)0xF8FF0000;
    RCC->CTLR &= (uint32_t)0xFEF6FFFF;
    RCC->CTLR &= (uint32_t)0xFFFBFFFF;
    RCC->CFGR0 &= (uint32_t)0xFF80FFFF;
    RCC->INTR = 0x009F0000;                 // ���������жϲ��������λ
}



//-------------------------------------------------------------------------------------------------------------------
// @brief       ����ʱ������
// @param       clock           ʱ��Ƶ�� �Ƽ�ʹ�� zf_common_clock.h �� system_clock_enum �����ѡ��
// @return      void
//-------------------------------------------------------------------------------------------------------------------
static void clock_set_freq (uint32_t clock)
{
    __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

    RCC->CTLR |= ((uint32_t)RCC_HSEON);

    /* Wait till HSE is ready and if Time out is reached exit */
    do
    {
        HSEStatus = RCC->CTLR & RCC_HSERDY;
        StartUpCounter++;
    } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CTLR & RCC_HSERDY) != RESET)
    {
        HSEStatus = (uint32_t)0x01;
    }
    else
    {
        HSEStatus = (uint32_t)0x00;
    }

    if (HSEStatus == (uint32_t)0x01)
    {
        /* Enable Prefetch Buffer */
        FLASH->ACTLR |= FLASH_ACTLR_PRFTBE;

        /* Flash wait state */
        FLASH->ACTLR &= (uint32_t)((uint32_t)~FLASH_ACTLR_LATENCY);

        FLASH->ACTLR |= (((clock-1)/24000000)&0x07);

        /* HCLK = SYSCLK */
        RCC->CFGR0 |= (uint32_t)RCC_HPRE_DIV1;
        /* PCLK2 = HCLK */
        RCC->CFGR0 |= (uint32_t)RCC_PPRE2_DIV1;
        /* PCLK1 = HCLK */
        RCC->CFGR0 |= (uint32_t)RCC_PPRE1_DIV1;


        if(clock == SYSTEM_CLOCK_XTAL)
        {
            /* Select HSE as system clock source */
            RCC->CFGR0 &= (uint32_t)((uint32_t)~(RCC_SW));
            RCC->CFGR0 |= (uint32_t)RCC_SW_HSE;
            /* Wait till HSE is used as system clock source */
            while ((RCC->CFGR0 & (uint32_t)RCC_SWS) != (uint32_t)0x04)
            {
            }
        }
        else
        {
            /*  PLL configuration: PLLCLK = HSE * ? = ? MHz */
            RCC->CFGR0 &= (uint32_t)((uint32_t)~(RCC_PLLSRC | RCC_PLLXTPRE | RCC_PLLMULL));

            if(clock == SYSTEM_CLOCK_144M)
            RCC->CFGR0 |= (uint32_t)(RCC_PLLSRC_HSE | RCC_PLLXTPRE_HSE | RCC_PLLMULL18_EXTEN);
            else if(clock == SYSTEM_CLOCK_120M)
            RCC->CFGR0 |= (uint32_t)(RCC_PLLSRC_HSE | RCC_PLLXTPRE_HSE | RCC_PLLMULL15_EXTEN);
            else if(clock == SYSTEM_CLOCK_96M)
            RCC->CFGR0 |= (uint32_t)(RCC_PLLSRC_HSE | RCC_PLLXTPRE_HSE | RCC_PLLMULL12_EXTEN);
            else if(clock == SYSTEM_CLOCK_72M)
            RCC->CFGR0 |= (uint32_t)(RCC_PLLSRC_HSE | RCC_PLLXTPRE_HSE | RCC_PLLMULL9_EXTEN);
            else if(clock == SYSTEM_CLOCK_48M)
            RCC->CFGR0 |= (uint32_t)(RCC_PLLSRC_HSE | RCC_PLLXTPRE_HSE | RCC_PLLMULL6_EXTEN);
            else if(clock == SYSTEM_CLOCK_24M)
            RCC->CFGR0 |= (uint32_t)(RCC_PLLSRC_HSE | RCC_PLLXTPRE_HSE | RCC_PLLMULL3_EXTEN);
//       if(((*(uint32_t*)0x1FFFF70C) & (1<<14)) != (1<<14))
//           RCC->CFGR0 |= (uint32_t)(RCC_PLLSRC_HSE | RCC_PLLXTPRE_HSE | RCC_PLLMULL18);
//       else
//           RCC->CFGR0 |= (uint32_t)(RCC_PLLSRC_HSE | RCC_PLLXTPRE_HSE | RCC_PLLMULL18_EXTEN);

            /* Enable PLL */
            RCC->CTLR |= RCC_PLLON;
            /* Wait till PLL is ready */
            while((RCC->CTLR & RCC_PLLRDY) == 0)
            {
            }
            /* Select PLL as system clock source */
            RCC->CFGR0 &= (uint32_t)((uint32_t)~(RCC_SW));
            RCC->CFGR0 |= (uint32_t)RCC_SW_PLL;
            /* Wait till PLL is used as system clock source */
            while ((RCC->CFGR0 & (uint32_t)RCC_SWS) != (uint32_t)0x08)
            {
            }
        }

    }
    else
    {
        while(1);           // �ⲿ�����ȶ���ȱʧ ʱ������ʧ��
        /*
        * If HSE fails to start-up, the application will have wrong clock
        * configuration. User can add here some code to deal with this error
        */
    }
}
