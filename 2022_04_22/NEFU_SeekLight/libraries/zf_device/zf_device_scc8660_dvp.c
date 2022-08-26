/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_device_scc8660
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-12-03
* @note             version:
*                   V1.1 2021.12.23 ����ͷ�ɼ���ɱ�־λ����volatile����
*
*                   ���߶��壺
*                   ------------------------------------
*                   ģ��ܽ�            ��Ƭ���ܽ�
*                   TXD                 �鿴 zf_device_scc8660.h �� SCC8660_COF_UART_TX_DVP        �궨��
*                   RXD                 �鿴 zf_device_scc8660.h �� SCC8660_COF_UART_RX_DVP        �궨��
*                   D0                  �鿴 zf_device_scc8660.h �� SCC8660_D0_PIN_DVP             �궨��
*                   D1                  �鿴 zf_device_scc8660.h �� SCC8660_D1_PIN_DVP             �궨��
*                   D2                  �鿴 zf_device_scc8660.h �� SCC8660_D2_PIN_DVP             �궨��
*                   D3                  �鿴 zf_device_scc8660.h �� SCC8660_D3_PIN_DVP             �궨��
*                   D4                  �鿴 zf_device_scc8660.h �� SCC8660_D4_PIN_DVP             �궨��
*                   D5                  �鿴 zf_device_scc8660.h �� SCC8660_D5_PIN_DVP             �궨��
*                   D6                  �鿴 zf_device_scc8660.h �� SCC8660_D6_PIN_DVP             �궨��
*                   D7                  �鿴 zf_device_scc8660.h �� SCC8660_D7_PIN_DVP             �궨��
*                   PCLK                �鿴 zf_device_scc8660.h �� SCC8660_PCLK_PIN_DVP           �궨��
*                   VSYNC               �鿴 zf_device_scc8660.h �� SCC8660_VSY_PIN_DVP            �궨��
*                   HSYNC               �鿴 zf_device_scc8660.h �� SCC8660_HERF_PIN_DVP           �궨��
*                   ------------------------------------
********************************************************************************************************************/

#include "zf_device_type.h"
#include "zf_device_camera.h"
#include "zf_device_scc8660_dvp.h"

__aligned(4) uint16_t scc8660_image_dvp[SCC8660_DVP_PIC_H][SCC8660_DVP_PIC_W];


//�û�����ͼ������ֱ�ӷ������ָ������Ϳ���
//���ʷ�ʽ�ǳ��򵥣�����ֱ��ʹ���±�ķ�ʽ����
//������ʵ�10�� 50�еĵ㣬scc8660_csi_image[10][50]�Ϳ�����


uint8_t   scc8660_uart_receive_dvp[3];
uint8_t   scc8660_uart_receive_num_dvp = 0;
volatile uint8_t  scc8660_uart_receive_flag_dvp;

//��Ҫ���õ�����ͷ������
uint16_t SCC8660_CFG_DVP[SCC8660_CONFIG_FINISH][2]=
{
    {SCC8660_AUTO_EXP,          0},                     //�Զ��ع�     Ĭ�ϣ�0     	��ѡ����Ϊ��0 1��      0���ֶ��ع�  1���Զ��ع�
    {SCC8660_BRIGHT,            800},                   //��������     �ֶ��ع�Ĭ�ϣ�800	�ֶ��ع�ʱ��������Χ0-65535   �Զ��ع��Ƽ�ֵ��100 �Զ��ع�ʱ�������÷�Χ0-255
														//��Ҫע��SCC8660_BRIGHT�Ĳ����̶�ʱ����ͬ��SCC8660_PCLK_DIV������Ӱ��ͼ������ȡ�
														//����SCC8660_BRIGHT�Ĳ���Ϊ800��SCC8660_PCLK_DIV�Ĳ���Ϊ0ʱ �� SCC8660_PCLK_DIV�Ĳ���Ϊ2ʱ 
														//����Ϊ2��ʱ��ͼ������Ҫ��Ϊ0��ʱ��������ʹ��˫���ʱ��о���������ͷ���Ȳ�һ��ʱ��Ҫע��������⡣
														
    {SCC8660_FPS,               60},                    //ͼ��֡��     Ĭ�ϣ�50    	��ѡ����Ϊ��60 50 30 25�� ʵ��֡�ʻ���Ҫ��SCC8660_PCLK_DIV����������
    {SCC8660_SET_COL,           SCC8660_DVP_PIC_W},     //ͼ������     Ĭ�ϣ�160   	����.h�ĺ궨�崦�޸�
    {SCC8660_SET_ROW,           SCC8660_DVP_PIC_H},     //ͼ������     Ĭ�ϣ�120   	����.h�ĺ궨�崦�޸�
    {SCC8660_PCLK_DIV,          1},                     //PCLK��Ƶϵ�� Ĭ�ϣ�0     	��ѡ����Ϊ��0:1/1 1:2/3 2:1/2 3:1/3 4:1/4 5:1/8��
                                                        //��Ƶϵ��Խ��PCLKƵ��Խ�ͣ�����PCLK���Լ���DVP�ӿڵĸ��ţ�������PCLKƵ�����Ӱ��֡�ʡ��������������뱣��Ĭ�ϡ�
                                                        //��������FPSΪ50֡������pclk��Ƶϵ��ѡ���Ϊ5��������ͷ�����֡��Ϊ50*��1/8��=6.25֡
													    //�����������������£�SCC8660_PCLK_DIV����Խ��ͼ���Խ��
    
    {SCC8660_PCLK_MODE,         1},                     //PCLKģʽ     Ĭ�ϣ�0		��ѡ����Ϊ��0 1��         0������������źţ�1����������źš�(ͨ��������Ϊ0�����ʹ��CH32V306��DVP�ӿڻ�STM32��DCMI�ӿڲɼ���Ҫ����Ϊ1)
    {SCC8660_COLOR_MODE,        0},                     //ͼ��ɫ��ģʽ Ĭ�ϣ�0		��ѡ����Ϊ��0 1��         0��������ɫģʽ    1������ģʽ��ɫ�ʱ��Ͷ���ߣ�
    {SCC8660_DATA_FORMAT,       0},                     //������ݸ�ʽ Ĭ�ϣ�0		��ѡ����Ϊ��0 1 2 3��     0��RGB565 1��RGB565(�ֽڽ���) 2��YUV422(YUYV) 3��YUV422(UYVY)
    {SCC8660_MANUAL_WB,         0},                     //�ֶ���ƽ��   Ĭ�ϣ�0		��ѡ����Ϊ��0 0x65-0xa0�� 0���ر��ֶ���ƽ�⣬�����Զ���ƽ��    �������ֶ���ƽ�� �ֶ���ƽ��ʱ ������Χ0x65-0xa0
    
    {SCC8660_INIT,              0}                      //����ͷ��ʼ��ʼ��
};

//������ͷ�ڲ���ȡ������������
uint16_t SCC8660_GET_CFG_DVP[SCC8660_CONFIG_FINISH-1][2]=
{
    {SCC8660_AUTO_EXP,          0},
    {SCC8660_BRIGHT,            0}, //��������          
    {SCC8660_FPS,               0}, //ͼ��֡��           
    {SCC8660_SET_COL,           0}, //ͼ������           
    {SCC8660_SET_ROW,           0}, //ͼ������          
    {SCC8660_PCLK_DIV,          0}, //PCLK��Ƶϵ��      
    {SCC8660_PCLK_MODE,         0}, //PCLKģʽ      
    {SCC8660_COLOR_MODE,        0}, //ͼ��ɫ��ģʽ
    {SCC8660_DATA_FORMAT,       0}, //������ݸ�ʽ 	
    {SCC8660_MANUAL_WB,         0}, //��ƽ������
};



//-------------------------------------------------------------------------------------------------------------------
//  @brief      SCC8660(��ͫ����ͷ)�����жϺ���
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       �ú�����SDK�ײ�fsl_lpuart�ļ��еĴ���3�жϺ����ڵ���
//-------------------------------------------------------------------------------------------------------------------
void scc8660_uart_callback_dvp(void)
{
    if(uart_query_byte(SCC8660_COF_UART_DVP, &scc8660_uart_receive_dvp[scc8660_uart_receive_num_dvp]))
        scc8660_uart_receive_num_dvp++;

    if(1==scc8660_uart_receive_num_dvp && 0XA5!=scc8660_uart_receive_dvp[0])  scc8660_uart_receive_num_dvp = 0;
    if(3 == scc8660_uart_receive_num_dvp)
    {
        scc8660_uart_receive_num_dvp = 0;
        scc8660_uart_receive_flag_dvp = 1;
    }
}

volatile uint8_t scc8660_finish_flag_dvp;  //ͼ��ɼ���ɵı�־λ


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ͷ���ô��ڳ�ʼ��
//  @param      NULL
//  @return     void                    
//  @since      v1.0
//  Sample usage:       �ڲ����ã������û����á�
//-------------------------------------------------------------------------------------------------------------------
void scc8660_dvp_cof_uart_init(void)
{
    //��ʼ������ͷ���ô���
    uart_init (SCC8660_COF_UART_DVP, 9600, SCC8660_COF_UART_RX_DVP, SCC8660_COF_UART_TX_DVP);    //��ʼ������ ��������ͷ
    uart_rx_interrupt(SCC8660_COF_UART_DVP, ENABLE);
    //�����ж�
    interrupt_enable_all(0);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��������ͷ�ڲ�������Ϣ
//  @param      uartn       ѡ��ʹ�õĴ���
//  @param      buff        ����������Ϣ�ĵ�ַ
//  @return     void
//  @since      v1.0
//  Sample usage:           ���øú���ǰ���ȳ�ʼ������
//-------------------------------------------------------------------------------------------------------------------
static uint8_t scc8660_set_all_config(uart_index_enum uartn, uint16_t buff[SCC8660_CONFIG_FINISH-1][2])
{
    uint16_t temp, i;
    uint8_t  send_buffer[4];
    volatile int16_t timeout = SCC8660_INIT_TIMEOUT;

    scc8660_uart_receive_flag_dvp = 0;
    
    //���ò���  ������ο���������ֲ�
    //��ʼ��������ͷ�����³�ʼ��
    for(i=0; i<SCC8660_CONFIG_FINISH; i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = buff[i][0];
        temp           = buff[i][1];
        send_buffer[2] = temp>>8;
        send_buffer[3] = (uint8_t)temp;
        
        uart_write_buffer(uartn,send_buffer,4);
        system_delay_ms(2);
    }
    
    //�ȴ�����ͷ��ʼ���ɹ�
    while(!scc8660_uart_receive_flag_dvp && timeout-- > 0)
    {
        system_delay_ms(1);
    }


    scc8660_uart_receive_flag_dvp = 0;
    timeout = SCC8660_INIT_TIMEOUT;

    while(((0xff != scc8660_uart_receive_dvp[1]) || (0xff != scc8660_uart_receive_dvp[2])) && timeout-- > 0)   // �ж������Ƿ��ȡ����Ӧ����
    {
        system_delay_ms(1);
    }
    //���ϲ��ֶ�����ͷ���õ�����ȫ�����ᱣ��������ͷ��51��Ƭ����eeprom��
    //����set_exposure_time�����������õ��ع����ݲ��洢��eeprom��

    if(timeout <= 0)                                                     // ��ʱ
        return 1;

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ����ͷ�ڲ�ȫ��������Ϣ
//  @param      uartn       ѡ��ʹ�õĴ���
//  @param      buff        ����������Ϣ�ĵ�ַ
//  @return     void
//  @since      v1.0
//  Sample usage:           ���øú���ǰ���ȳ�ʼ������ͷ���ô���
//-------------------------------------------------------------------------------------------------------------------
static uint8_t scc8660_get_all_config(uart_index_enum uartn, uint16_t buff[SCC8660_CONFIG_FINISH-1][2])
{
    uint16_t temp, i;
    uint8_t  send_buffer[4];
    volatile int16_t timeout = SCC8660_INIT_TIMEOUT;
    
    for(i=0; i<SCC8660_CONFIG_FINISH-1; i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = SCC8660_GET_STATUS;
        temp           = buff[i][0];
        send_buffer[2] = temp>>8;
        send_buffer[3] = (uint8_t)temp;
        
        uart_write_buffer(uartn,send_buffer,4);
        
        //�ȴ����ܻش�����
        while(!scc8660_uart_receive_flag_dvp && timeout-- > 0)
        {
            system_delay_ms(1);
        }

        if(timeout <= 0)                                                     // ��ʱ
            return 1;

        scc8660_uart_receive_flag_dvp = 0;
        buff[i][1] = scc8660_uart_receive_dvp[1]<<8 | scc8660_uart_receive_dvp[2];
    }
    return 0;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ����ͷ�̼��汾
//  @param      uartn               ѡ��ʹ�õĴ���
//  @return     (uint16_t)camera_id   ��������ͷID
//  @since      v1.0
//  Sample usage:           ���øú���ǰ���ȳ�ʼ������ͷ���ô���
//-------------------------------------------------------------------------------------------------------------------
uint16_t scc8660_get_id(uart_index_enum uartn)
{
    uint16_t temp;
    uint8_t  send_buffer[4];
    volatile int16_t timeout = SCC8660_INIT_TIMEOUT;

    send_buffer[0] = 0xA5;
    send_buffer[1] = SCC8660_GET_WHO_AM_I;
    temp           = 0;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8_t)temp;
    
    uart_write_buffer(uartn, send_buffer, 4);
        
    //�ȴ����ܻش�����
    while(!scc8660_uart_receive_flag_dvp && timeout-- > 0)
    {
        system_delay_ms(1);
    }
    scc8660_uart_receive_flag_dvp = 0;
    
    if(timeout <= 0)                                                     // ��ʱ
          return 0;

    return ((uint16_t)(scc8660_uart_receive_dvp[1]<<8) | scc8660_uart_receive_dvp[2]);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ����ͷĳһ������������Ϣ
//  @param      uartn               ѡ��ʹ�õĴ���
//  @return     (uint16_t)config      ���ز���ֵ
//  @since      v1.0
//  Sample usage:           ���øú���ǰ���ȳ�ʼ������ͷ���ô���
//-------------------------------------------------------------------------------------------------------------------
uint16_t scc8660_get_config(uart_index_enum uartn, uint8_t config)
{
    uint8_t send_buffer[4];
    volatile int16_t timeout = SCC8660_INIT_TIMEOUT;

    send_buffer[0] = 0xA5;
    send_buffer[1] = SCC8660_GET_WHO_AM_I;
    send_buffer[2] = 0x00;
    send_buffer[3] = config;
    
    uart_write_buffer(uartn, send_buffer, 4);

    //�ȴ����ܻش�����
    while(!scc8660_uart_receive_flag_dvp && timeout-- > 0)
    {
        system_delay_ms(1);
    }
    scc8660_uart_receive_flag_dvp = 0;
    
    if(timeout <= 0)                                                     // ��ʱ
          return 0;

    return ((uint16_t)(scc8660_uart_receive_dvp[1]<<8) | scc8660_uart_receive_dvp[2]);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ��ɫ����ͷ�̼��汾
//  @param      uartn       ѡ��ʹ�õĴ���
//  @return     uint16_t      �汾��
//  @since      v1.0
//  Sample usage:           ���øú���ǰ���ȳ�ʼ������ͷ���ô���
//-------------------------------------------------------------------------------------------------------------------
uint16_t scc8660_get_version(uart_index_enum uartn)
{
    uint16_t temp;
    uint8_t  send_buffer[4];
    volatile int16_t timeout = SCC8660_INIT_TIMEOUT;

    send_buffer[0] = 0xA5;
    send_buffer[1] = SCC8660_GET_STATUS;
    temp           = SCC8660_GET_VERSION;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8_t)temp;
    
    uart_write_buffer(uartn, send_buffer, 4);
        
    //�ȴ����ܻش�����
    while(!scc8660_uart_receive_flag_dvp && timeout-- > 0)
    {
        system_delay_ms(1);
    }
    scc8660_uart_receive_flag_dvp = 0;
    
    if(timeout <= 0)                                                     // ��ʱ
          return 0;

    return ((uint16_t)(scc8660_uart_receive_dvp[1]<<8) | scc8660_uart_receive_dvp[2]);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��������ͼ������
//  @param      uartn       ѡ��ʹ�õĴ���
//  @param      data        ��Ҫ���õ�����ֵ
//  @return     uint16_t      �������õ�����ֵ
//  @since      v1.0
//  Sample usage:           ���øú���ǰ���ȳ�ʼ������ͷ���ô���
//  @note                   ͨ���ú������õĲ��������ᱻ51��Ƭ������
//-------------------------------------------------------------------------------------------------------------------
uint16_t  scc8660_set_bright(uart_index_enum uartn, uint16_t data)
{
    uint8_t  send_buffer[4];
    volatile int16_t timeout = SCC8660_INIT_TIMEOUT;

    send_buffer[0] = 0xA5;
    send_buffer[1] = SCC8660_SET_BRIGHT;
    send_buffer[2] = data>>8;
    send_buffer[3] = (uint8_t)data;
    
    uart_write_buffer(uartn,send_buffer,4);
    
    //�ȴ����ܻش�����
    while(!scc8660_uart_receive_flag_dvp && timeout-- > 0)
    {
        system_delay_ms(1);
    }
    scc8660_uart_receive_flag_dvp = 0;
    
    if(timeout <= 0)                                                     // ��ʱ
          return 0;

    return ((uint16_t)(scc8660_uart_receive_dvp[1]<<8) | scc8660_uart_receive_dvp[2]);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �������ð�ƽ��
//  @param      uartn       ѡ��ʹ�õĴ���
//  @param      data        ��Ҫ���õ�����ֵ
//  @return     uint16_t      �������õ�����ֵ
//  @since      v1.0
//  Sample usage:           ���øú���ǰ���ȳ�ʼ������ͷ���ô���
//  @note                   ͨ���ú������õĲ��������ᱻ51��Ƭ������
//-------------------------------------------------------------------------------------------------------------------
uint16_t  scc8660_set_maunal_wb(uart_index_enum uartn, uint16_t data)
{
    uint8_t  send_buffer[4];
    volatile int16_t timeout = SCC8660_INIT_TIMEOUT;

    send_buffer[0] = 0xA5;
    send_buffer[1] = SCC8660_SET_MANUAL_WB;
    send_buffer[2] = data>>8;
    send_buffer[3] = (uint8_t)data;
    
    uart_write_buffer(uartn,send_buffer,4);
    
    //�ȴ����ܻش�����
    while(!scc8660_uart_receive_flag_dvp && timeout-- > 0)
    {
        system_delay_ms(1);
    }
    scc8660_uart_receive_flag_dvp = 0;
    
    if(timeout <= 0)                                                     // ��ʱ
          return 0;

    return ((uint16_t)(scc8660_uart_receive_dvp[1]<<8) | scc8660_uart_receive_dvp[2]);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ������������ͷ�Ĵ���
//  @param      uartn       ѡ��ʹ�õĴ���
//  @param      reg         �Ĵ�����ַ
//  @param      data        ��Ҫд��Ĵ�������ֵ
//  @return     uint16_t      �ش�д��Ĵ�����ֵ
//  @since      v1.0
//  Sample usage:           ���øú���ǰ���ȳ�ʼ������
//-------------------------------------------------------------------------------------------------------------------
uint16_t scc8660_set_reg_addr(uart_index_enum uartn, uint8_t reg, uint16_t data)
{
    uint16_t temp;
    uint8_t  send_buffer[4];
    volatile int16_t timeout = SCC8660_INIT_TIMEOUT;

    send_buffer[0] = 0xA5;
    send_buffer[1] = SCC8660_SET_REG_ADDR;
    send_buffer[2] = 0x00;
    send_buffer[3] = (uint8_t)reg;
    
    uart_write_buffer(uartn,send_buffer,4);
    
    system_delay_ms(2);
    
    send_buffer[0] = 0xA5;
    send_buffer[1] = SCC8660_SET_REG_DATA;
    temp           = data;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8_t)temp;
    
    uart_write_buffer(uartn,send_buffer,4);
    
    //�ȴ����ܻش�����
    while(!scc8660_uart_receive_flag_dvp && timeout-- > 0)
    {
        system_delay_ms(1);
    }
    scc8660_uart_receive_flag_dvp = 0;
    
    if(timeout <= 0)                                                     // ��ʱ
          return 0;

    return ((uint16_t)(scc8660_uart_receive_dvp[1]<<8) | scc8660_uart_receive_dvp[2]);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      SCC8660����ͷ���ж�
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:               ��isr.c�����ȴ�����Ӧ���жϺ�����Ȼ����øú���(֮�����������жϱ�־λ)
//-------------------------------------------------------------------------------------------------------------------
void scc8660_vsync_dvp(void)
{
    //�ָ�DMA��ַ
    DVP->DMA_BUF0 = (uint32_t)camera_buffer_addr;

    //�ɼ���ɱ�־λ��1
    scc8660_finish_flag_dvp = 1;

    //����DVP
    DVP->CR0 |= RB_DVP_ENABLE;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      SCC8660����ͷ���ж�
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:               �ݲ���Ҫ
//-------------------------------------------------------------------------------------------------------------------
void scc8660_dvp_hsync(void)
{

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      SCC8660(��ͫ����ͷ)��ʼ�� ʹ��DVP�ӿ�
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:		
//-------------------------------------------------------------------------------------------------------------------
uint8_t scc8660_init_dvp(void)
{

    //����ͷ��������Ϊ��ͫ
    set_camera_type(CAMERA_COLOR);
    //����ͷ��ʼ��ʼ��֮ǰ��ؽ����ź�����
    gpio_init(SCC8660_VSY_PIN_DVP, GPO, 1, GPIO_PIN_CONFIG);
    //����ͷ���ô��ڳ�ʼ��
    scc8660_dvp_cof_uart_init();

    //�ȴ�����ͷ�ϵ��ʼ���ɹ� ��ʽ�����֣���ʱ����ͨ����ȡ���õķ�ʽ ��ѡһ
    //system_delay_ms(500);//��ʱ��ʽ
    scc8660_uart_receive_flag_dvp = 0;
    if(scc8660_get_all_config(SCC8660_COF_UART_DVP, SCC8660_GET_CFG_DVP))//��ȡ���õķ�ʽ
    {
        zf_assert(0);
        return 1;
    }
    //������ͷ����������Ϣ
    scc8660_uart_receive_flag_dvp = 0;
    if(scc8660_set_all_config(SCC8660_COF_UART_DVP, SCC8660_CFG_DVP))
    {
        zf_assert(0);
        return 1;
    }
    //��ȡ���ñ��ڲ鿴�����Ƿ���ȷ
    scc8660_uart_receive_flag_dvp = 0;
    if(scc8660_get_all_config(SCC8660_COF_UART_DVP, SCC8660_GET_CFG_DVP))
    {
        zf_assert(0);
        return 1;
    }
    //Ϊ����ʹ�ã���ȡ������Ϣ��δ�����ݽ���У�飬�����Ҫȷ�������Ƿ�ɹ��������н������ݱȶԡ�
    interrupt_disable_all();

    //DVP���ų�ʼ��
    dvp_gpio_init(
            SCC8660_D0_PIN_DVP, SCC8660_D1_PIN_DVP, SCC8660_D2_PIN_DVP, SCC8660_D3_PIN_DVP,
            SCC8660_D4_PIN_DVP, SCC8660_D5_PIN_DVP, SCC8660_D6_PIN_DVP, SCC8660_D7_PIN_DVP,
            SCC8660_PCLK_PIN_DVP, SCC8660_VSY_PIN_DVP, SCC8660_HERF_PIN_DVP);

    //����ͼ���ַ��ֵ����ر���
    camera_buffer_addr = (uint8_t *)scc8660_image_dvp[0];

    //DVP �ɼ���ʼ��
    dvp_camera_init((uint32_t *)camera_buffer_addr, NULL, SCC8660_DVP_W*SCC8660_DVP_H, 1);
    return 0;

}




//-------------------------------------------------------------------------------------------------------------------
//  @brief      SCC8660(��ͫ����ͷ)��ȡ���ص�RGB����
//  @param      *dat    ͼ������ĵ�ַ
//  @param      x       ��Ҫ��ȡ������������
//  @param      y       ��Ҫ��ȡ������������
//  @param      z       ͼ����
//  @param      *r      ����r������ַ ����ֵ��Χ0-31
//  @param      *g      ����g������ַ ����ֵ��Χ0-63
//  @param      *b      ����b������ַ ����ֵ��Χ0-31
//  @return     void
//  @since      v1.0
//  Sample usage:		color_camera_take_point(scc8660_dvp_image[0],0, 0, SCC8660_DVP_PIC_W, &r_value,&g_value,&b_value);//��ȡ��0�� ��0�����ص�RGB����  ����ͷ�����ݸ�ʽ��������Ϊ0
//	@note				�˺�����ҪĿ����Ϊ���ô���ܹ��������˽�RGB��������δ洢��
//-------------------------------------------------------------------------------------------------------------------
void inline color_camera_take_point(uint16_t *dat, uint16_t x, uint16_t y, uint16_t z, uint8_t *r, uint8_t *g, uint8_t *b)
{
    uint16_t pixel;

    //��ȡָ���������������
    pixel = dat[x+y*z];

    //��ΪR5G3�Ǵ洢�ڵͰ�λ G3B5�Ǵ洢�ڸ߰�λ
    //��Ϊ�����Ƚ�λ�ý��н��������ڻ�ȡÿ������������
    pixel = ((pixel&0xff)<<8) |(pixel>>8);

    *r = pixel>>11;
    *g = (pixel>>5)&0x3f;
    *b = pixel&0x1f;
}



