/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             zf_device_scc8660
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ790875685)
* @version          查看doc内version文件 版本说明
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2021-12-03
* @note             version:
*                   V1.1 2021.12.23 摄像头采集完成标志位增加volatile修饰
*
*                   接线定义：
*                   ------------------------------------
*                   模块管脚            单片机管脚
*                   TXD                 查看 zf_device_scc8660.h 中 SCC8660_COF_UART_TX_DVP        宏定义
*                   RXD                 查看 zf_device_scc8660.h 中 SCC8660_COF_UART_RX_DVP        宏定义
*                   D0                  查看 zf_device_scc8660.h 中 SCC8660_D0_PIN_DVP             宏定义
*                   D1                  查看 zf_device_scc8660.h 中 SCC8660_D1_PIN_DVP             宏定义
*                   D2                  查看 zf_device_scc8660.h 中 SCC8660_D2_PIN_DVP             宏定义
*                   D3                  查看 zf_device_scc8660.h 中 SCC8660_D3_PIN_DVP             宏定义
*                   D4                  查看 zf_device_scc8660.h 中 SCC8660_D4_PIN_DVP             宏定义
*                   D5                  查看 zf_device_scc8660.h 中 SCC8660_D5_PIN_DVP             宏定义
*                   D6                  查看 zf_device_scc8660.h 中 SCC8660_D6_PIN_DVP             宏定义
*                   D7                  查看 zf_device_scc8660.h 中 SCC8660_D7_PIN_DVP             宏定义
*                   PCLK                查看 zf_device_scc8660.h 中 SCC8660_PCLK_PIN_DVP           宏定义
*                   VSYNC               查看 zf_device_scc8660.h 中 SCC8660_VSY_PIN_DVP            宏定义
*                   HSYNC               查看 zf_device_scc8660.h 中 SCC8660_HERF_PIN_DVP           宏定义
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



//默认图像分辨率为160*120。


//------------ 请仔细以上注释后再更改分辨率 ------------
//------------ 请仔细以上注释后再更改分辨率 ------------
//------------ 请仔细以上注释后再更改分辨率 ------------
#define SCC8660_DVP_PIC_W				160			// 实际图像分辨率宽度	可选参数为：160 180
#define SCC8660_DVP_PIC_H				120			// 实际图像分辨率高度	可选参数为：120 160

#define SCC8660_DVP_W		SCC8660_DVP_PIC_W*2		// 此参数为图像数据存储宽度 请勿修改
#define SCC8660_DVP_H		SCC8660_DVP_PIC_H       // 此参数为图像数据存储高度 请勿修改

//--------------------------------------------------------------------------------------------------
//引脚配置
//--------------------------------------------------------------------------------------------------
#define SCC8660_COF_UART_DVP        UART_5          // 配置摄像头所使用到的串口
#define SCC8660_COF_BAUR_DVP        9600            // 配置摄像头用的串口波特率 禁止修改
#define SCC8660_COF_UART_RX_DVP     UART5_TX_C12    // 摄像头的  UART-RX 引脚 要接在单片机 TX 上
#define SCC8660_COF_UART_TX_DVP     UART5_RX_D2     // 摄像头的  UART-TX 引脚 要接在单片机 RX 上


//--------------------------------------------------------------------------------------------------
//摄像头数据引脚，DVP专用引脚，禁止用户修改引脚
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


// 超时设置
#define SCC8660_INIT_TIMEOUT                  0x0080

extern uint16_t  scc8660_image_dvp[SCC8660_DVP_PIC_H][SCC8660_DVP_PIC_W];

extern volatile uint8_t  scc8660_finish_flag_dvp;       // 一场图像采集完成标志位

extern uint8_t   scc8660_uart_receive_dvp[3];
extern uint8_t   scc8660_uart_receive_num_dvp;
extern volatile uint8_t  scc8660_uart_receive_flag_dvp;


typedef enum
{
    SCC8660_INIT 			= 0x00, // 摄像头初始化命令
    SCC8660_AUTO_EXP,               // 自动曝光命令
    SCC8660_BRIGHT,                 // 亮度命令
    SCC8660_FPS,                    // 摄像头帧率命令
    SCC8660_SET_COL,                // 图像列命令
    SCC8660_SET_ROW,                // 图像行命令
    SCC8660_PCLK_DIV,               // 像素时钟分频命令
    SCC8660_PCLK_MODE,              // 像素时钟模式命令
    SCC8660_COLOR_MODE,             // 色彩模式命令
    SCC8660_DATA_FORMAT,	        // 数据格式命令
	SCC8660_MANUAL_WB,	            // 手动白平衡命令
    SCC8660_CONFIG_FINISH,          // 非命令位，主要用来占位计数
        
    SCC8660_GET_WHO_AM_I  = 0xEF,   // 我是谁命令，用于判断摄像头型号
    SCC8660_SET_BRIGHT    = 0xF0,   // 单独设置亮度
    SCC8660_GET_STATUS 	  = 0XF1,   // 获取摄像头配置命令
    SCC8660_GET_VERSION	  = 0xF2,   // 固件版本号
	SCC8660_SET_MANUAL_WB = 0xF3,   // 单独设置手动白平衡
        
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
