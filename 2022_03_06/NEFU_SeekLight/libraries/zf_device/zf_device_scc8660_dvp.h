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



#ifndef _zf_device_scc8660_h_
#define _zf_device_scc8660_h_

#include "zf_common_debug.h"
#include "zf_common_interrupt.h"

#include "zf_driver_delay.h"
#include "zf_driver_dvp.h"
#include "zf_driver_gpio.h"
#include "zf_driver_uart.h"



//Ĭ��ͼ��ֱ���Ϊ160*120��


//------------ ����ϸ����ע�ͺ��ٸ��ķֱ��� ------------
//------------ ����ϸ����ע�ͺ��ٸ��ķֱ��� ------------
//------------ ����ϸ����ע�ͺ��ٸ��ķֱ��� ------------
#define SCC8660_DVP_PIC_W				160			// ʵ��ͼ��ֱ��ʿ��	��ѡ����Ϊ��160 180
#define SCC8660_DVP_PIC_H				120			// ʵ��ͼ��ֱ��ʸ߶�	��ѡ����Ϊ��120 160

#define SCC8660_DVP_W		SCC8660_DVP_PIC_W*2		// �˲���Ϊͼ�����ݴ洢��� �����޸�
#define SCC8660_DVP_H		SCC8660_DVP_PIC_H       // �˲���Ϊͼ�����ݴ洢�߶� �����޸�

//--------------------------------------------------------------------------------------------------
//��������
//--------------------------------------------------------------------------------------------------
#define SCC8660_COF_UART_DVP        UART_5          // ��������ͷ��ʹ�õ��Ĵ���
#define SCC8660_COF_BAUR_DVP        9600            // ��������ͷ�õĴ��ڲ����� ��ֹ�޸�
#define SCC8660_COF_UART_RX_DVP     UART5_TX_C12    // ����ͷ��  UART-RX ���� Ҫ���ڵ�Ƭ�� TX ��
#define SCC8660_COF_UART_TX_DVP     UART5_RX_D2     // ����ͷ��  UART-TX ���� Ҫ���ڵ�Ƭ�� RX ��


//--------------------------------------------------------------------------------------------------
//����ͷ�������ţ�DVPר�����ţ���ֹ�û��޸�����
//--------------------------------------------------------------------------------------------------
#define SCC8660_D0_PIN_DVP         	A9
#define SCC8660_D1_PIN_DVP         	A1
#define SCC8660_D2_PIN_DVP         	C8
#define SCC8660_D3_PIN_DVP         	C9
#define SCC8660_D4_PIN_DVP         	C1
#define SCC8660_D5_PIN_DVP         	B6
#define SCC8660_D6_PIN_DVP         	B8
#define SCC8660_D7_PIN_DVP         	B9

#define SCC8660_PCLK_PIN_DVP        A6
#define SCC8660_VSY_PIN_DVP         A5
#define SCC8660_HERF_PIN_DVP        A4


// ��ʱ����
#define SCC8660_INIT_TIMEOUT                  0x0080

extern uint16_t  scc8660_image_dvp[SCC8660_DVP_PIC_H][SCC8660_DVP_PIC_W];

extern volatile uint8_t  scc8660_finish_flag_dvp;       // һ��ͼ��ɼ���ɱ�־λ

extern uint8_t   scc8660_uart_receive_dvp[3];
extern uint8_t   scc8660_uart_receive_num_dvp;
extern volatile uint8_t  scc8660_uart_receive_flag_dvp;


typedef enum
{
    SCC8660_INIT 			= 0x00, // ����ͷ��ʼ������
    SCC8660_AUTO_EXP,               // �Զ��ع�����
    SCC8660_BRIGHT,                 // ��������
    SCC8660_FPS,                    // ����ͷ֡������
    SCC8660_SET_COL,                // ͼ��������
    SCC8660_SET_ROW,                // ͼ��������
    SCC8660_PCLK_DIV,               // ����ʱ�ӷ�Ƶ����
    SCC8660_PCLK_MODE,              // ����ʱ��ģʽ����
    SCC8660_COLOR_MODE,             // ɫ��ģʽ����
    SCC8660_DATA_FORMAT,	        // ���ݸ�ʽ����
	SCC8660_MANUAL_WB,	            // �ֶ���ƽ������
    SCC8660_CONFIG_FINISH,          // ������λ����Ҫ����ռλ����
        
    SCC8660_GET_WHO_AM_I  = 0xEF,   // ����˭��������ж�����ͷ�ͺ�
    SCC8660_SET_BRIGHT    = 0xF0,   // ������������
    SCC8660_GET_STATUS 	  = 0XF1,   // ��ȡ����ͷ��������
    SCC8660_GET_VERSION	  = 0xF2,   // �̼��汾��
	SCC8660_SET_MANUAL_WB = 0xF3,   // ���������ֶ���ƽ��
        
    SCC8660_SET_REG_ADDR  = 0xFE,
    SCC8660_SET_REG_DATA  = 0xFF,
}SCC8660_CMD;



uint8_t   scc8660_init_dvp(void);
void      scc8660_vsync_dvp(void);
void      scc8660_hsync_dvp(void);
void      scc8660_uart_callback_dvp(void);

uint16_t  scc8660_get_id(uart_index_enum uartn);
uint16_t  scc8660_get_config(uart_index_enum uartn, uint8_t config);
uint16_t  scc8660_get_version(uart_index_enum uartn);
uint16_t  scc8660_set_bright(uart_index_enum uartn, uint16_t data);
uint16_t  scc8660_set_maunal_wb(uart_index_enum uartn, uint16_t data);
uint16_t  scc8660_set_reg_addr(uart_index_enum uartn, uint8_t reg, uint16_t data);
void      dvp_seekfree_sendimg_scc8660(uart_index_enum uartn, uint8_t *image, uint16_t width, uint16_t height);
void 	  seekfree_sendimg_scc8660_usb_cdc(uint8_t *image, uint16_t width, uint16_t height);
//void inline color_camera_take_point(uint16_t *dat, uint16_t x, uint16_t y, uint16_t z, uint8_t *r, uint8_t *g, uint8_t *b);

#endif
