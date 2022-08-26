/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            zf_driver_dvp
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ790875685)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 ********************************************************************************************************************/
#include "zf_driver_delay.h"
#include "zf_driver_gpio.h"
#include "zf_driver_dvp.h"

//vuint32_t frame_cnt = 0;
//vuint32_t addr_cnt = 0;
//vuint32_t href_cnt = 0;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      dvp�ӿ����ų�ʼ��
//  @param      dvp_d0��dvp_d0       ����ͷD0-D7����
//  @param      dvp_pclk            ����ͷPCLK����
//  @param      dvp_vsync           ����ͷvysnc����
//  @param      dvp_herf            ����ͷherf����
//  @return     void
//  Sample usage:                   �ڲ�ʹ�ã��û��������
//-------------------------------------------------------------------------------------------------------------------
void dvp_gpio_init(
        uint8_t dvp_d0,uint8_t dvp_d1,uint8_t dvp_d2,uint8_t dvp_d3,
        uint8_t dvp_d4,uint8_t dvp_d5,uint8_t dvp_d6,uint8_t dvp_d7,
        uint8_t dvp_pclk,uint8_t dvp_vsync,uint8_t dvp_hsync)
{
    //DVP_D0 �� DVP_ D7
    gpio_init(dvp_d0, GPI, 0, GPI_PULL_UP);
    gpio_init(dvp_d1, GPI, 0, GPI_PULL_UP);
    gpio_init(dvp_d2, GPI, 0, GPI_PULL_UP);
    gpio_init(dvp_d3, GPI, 0, GPI_PULL_UP);
    gpio_init(dvp_d4, GPI, 0, GPI_PULL_UP);
    gpio_init(dvp_d5, GPI, 0, GPI_PULL_UP);
    gpio_init(dvp_d6, GPI, 0, GPI_PULL_UP);
    gpio_init(dvp_d7, GPI, 0, GPI_PULL_UP);
    //DVP_VSYNC
    gpio_init(dvp_vsync, GPI, 0, GPI_PULL_UP);
    //DVP_PCLK
    gpio_init(dvp_pclk,  GPI, 0, GPI_PULL_UP);
    //DVP_HSYNC
    gpio_init(dvp_hsync, GPI, 0, GPI_PULL_UP);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      dvp�ӿڳ�ʼ��
//  @param      *image0                ����buff0�ĵ�ַ
//  @param      *image1                ����buff1�ĵ�ַ
//  @param      col_len                �г���
//  @param      row_len                �г���
//  @return     void
//  Sample usage:                      �ڲ�ʹ�ã��û��������
//-------------------------------------------------------------------------------------------------------------------
void dvp_camera_init(uint32_t *image0, uint32_t *image1, uint16_t col_len, uint16_t row_len)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DVP, ENABLE);

    DVP->CR0 &= ~RB_DVP_MSK_DAT_MOD;

    /* VSYNC��HSYNC:High level active */
    DVP->CR0 |= RB_DVP_D8_MOD | RB_DVP_JPEG | RB_DVP_V_POLAR ;
    DVP->CR1 &= ~((RB_DVP_ALL_CLR)| RB_DVP_RCV_CLR);
    DVP->ROW_NUM = row_len;                         // rows����
    DVP->COL_NUM = col_len;                         // cols����

    DVP->DMA_BUF0 = (uint32_t)image0;               //DMA addr0
    DVP->DMA_BUF1 = (uint32_t)image1;               //DMA addr1


    /* Set frame capture rate */
    DVP->CR1 &= ~RB_DVP_FCRC;
    DVP->CR1 |= DVP_RATE_100P;  //100%
  //  DVP->CR1 |= RB_DVP_CM;



    //Interupt Enable
//    DVP->IER |= RB_DVP_IE_STR_FRM;
//    DVP->IER |= RB_DVP_IE_ROW_DONE;
    DVP->IER |= RB_DVP_IE_FRM_DONE;
//    DVP->IER |= RB_DVP_IE_FIFO_OV;
//    DVP->IER |= RB_DVP_IE_STP_FRM;


    DVP->CR1 |= RB_DVP_DMA_EN;  //enable DMA
    DVP->CR0 |= RB_DVP_ENABLE;  //enable DVP

    interrupt_set_priority(DVP_IRQn, 0x03);
    interrupt_enable(DVP_IRQn);

}

