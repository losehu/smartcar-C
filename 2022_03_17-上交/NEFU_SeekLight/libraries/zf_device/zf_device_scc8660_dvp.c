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

#include "zf_device_type.h"
#include "zf_device_camera.h"
#include "zf_device_scc8660_dvp.h"

__aligned(4) uint16_t scc8660_image_dvp[SCC8660_DVP_PIC_H][SCC8660_DVP_PIC_W];


//用户访问图像数据直接访问这个指针变量就可以
//访问方式非常简单，可以直接使用下标的方式访问
//例如访问第10行 50列的点，scc8660_csi_image[10][50]就可以了


uint8_t   scc8660_uart_receive_dvp[3];
uint8_t   scc8660_uart_receive_num_dvp = 0;
volatile uint8_t  scc8660_uart_receive_flag_dvp;

//需要配置到摄像头的数据
uint16_t SCC8660_CFG_DVP[SCC8660_CONFIG_FINISH][2]=
{
    {SCC8660_AUTO_EXP,          0},                     //自动曝光     默认：0     	可选参数为：0 1。      0：手动曝光  1：自动曝光
    {SCC8660_BRIGHT,            800},                   //亮度设置     手动曝光默认：800	手动曝光时：参数范围0-65535   自动曝光推荐值：100 自动曝光时参数设置范围0-255
														//需要注意SCC8660_BRIGHT的参数固定时，不同的SCC8660_PCLK_DIV参数会影响图像的亮度。
														//假设SCC8660_BRIGHT的参数为800，SCC8660_PCLK_DIV的参数为0时 和 SCC8660_PCLK_DIV的参数为2时 
														//参数为2的时候图像明显要比为0的时候亮，在使用双摄的时候感觉两个摄像头亮度不一致时需要注意这个问题。
														
    {SCC8660_FPS,               60},                    //图像帧率     默认：50    	可选参数为：60 50 30 25。 实际帧率还需要看SCC8660_PCLK_DIV参数的设置
    {SCC8660_SET_COL,           SCC8660_DVP_PIC_W},     //图像列数     默认：160   	请在.h的宏定义处修改
    {SCC8660_SET_ROW,           SCC8660_DVP_PIC_H},     //图像行数     默认：120   	请在.h的宏定义处修改
    {SCC8660_PCLK_DIV,          1},                     //PCLK分频系数 默认：0     	可选参数为：0:1/1 1:2/3 2:1/2 3:1/3 4:1/4 5:1/8。
                                                        //分频系数越大，PCLK频率越低，降低PCLK可以减轻DVP接口的干扰，但降低PCLK频率则会影响帧率。若无特殊需求请保持默认。
                                                        //例如设置FPS为50帧，但是pclk分频系数选择的为5，则摄像头输出的帧率为50*（1/8）=6.25帧
													    //其他参数不变的情况下，SCC8660_PCLK_DIV参数越大图像会越亮
    
    {SCC8660_PCLK_MODE,         1},                     //PCLK模式     默认：0		可选参数为：0 1。         0：不输出消隐信号，1：输出消隐信号。(通常都设置为0，如果使用CH32V306的DVP接口或STM32的DCMI接口采集需要设置为1)
    {SCC8660_COLOR_MODE,        0},                     //图像色彩模式 默认：0		可选参数为：0 1。         0：正常彩色模式    1：鲜艳模式（色彩饱和度提高）
    {SCC8660_DATA_FORMAT,       0},                     //输出数据格式 默认：0		可选参数为：0 1 2 3。     0：RGB565 1：RGB565(字节交换) 2：YUV422(YUYV) 3：YUV422(UYVY)
    {SCC8660_MANUAL_WB,         0},                     //手动白平衡   默认：0		可选参数为：0 0x65-0xa0。 0：关闭手动白平衡，启用自动白平衡    其他：手动白平衡 手动白平衡时 参数范围0x65-0xa0
    
    {SCC8660_INIT,              0}                      //摄像头开始初始化
};

//从摄像头内部获取到的配置数据
uint16_t SCC8660_GET_CFG_DVP[SCC8660_CONFIG_FINISH-1][2]=
{
    {SCC8660_AUTO_EXP,          0},
    {SCC8660_BRIGHT,            0}, //亮度设置          
    {SCC8660_FPS,               0}, //图像帧率           
    {SCC8660_SET_COL,           0}, //图像列数           
    {SCC8660_SET_ROW,           0}, //图像行数          
    {SCC8660_PCLK_DIV,          0}, //PCLK分频系数      
    {SCC8660_PCLK_MODE,         0}, //PCLK模式      
    {SCC8660_COLOR_MODE,        0}, //图像色彩模式
    {SCC8660_DATA_FORMAT,       0}, //输出数据格式 	
    {SCC8660_MANUAL_WB,         0}, //白平衡设置
};



//-------------------------------------------------------------------------------------------------------------------
//  @brief      SCC8660(凌瞳摄像头)串口中断函数
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       该函数在SDK底层fsl_lpuart文件中的串口3中断函数内调用
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

volatile uint8_t scc8660_finish_flag_dvp;  //图像采集完成的标志位


//-------------------------------------------------------------------------------------------------------------------
//  @brief      摄像头配置串口初始化
//  @param      NULL
//  @return     void                    
//  @since      v1.0
//  Sample usage:       内部调用，无需用户调用。
//-------------------------------------------------------------------------------------------------------------------
void scc8660_dvp_cof_uart_init(void)
{
    //初始化摄像头配置串口
    uart_init (SCC8660_COF_UART_DVP, 9600, SCC8660_COF_UART_RX_DVP, SCC8660_COF_UART_TX_DVP);    //初始换串口 配置摄像头
    uart_rx_interrupt(SCC8660_COF_UART_DVP, ENABLE);
    //开总中断
    interrupt_enable_all(0);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      配置摄像头内部配置信息
//  @param      uartn       选择使用的串口
//  @param      buff        发送配置信息的地址
//  @return     void
//  @since      v1.0
//  Sample usage:           调用该函数前请先初始化串口
//-------------------------------------------------------------------------------------------------------------------
static uint8_t scc8660_set_all_config(uart_index_enum uartn, uint16_t buff[SCC8660_CONFIG_FINISH-1][2])
{
    uint16_t temp, i;
    uint8_t  send_buffer[4];
    volatile int16_t timeout = SCC8660_INIT_TIMEOUT;

    scc8660_uart_receive_flag_dvp = 0;
    
    //设置参数  具体请参看问题锦集手册
    //开始配置摄像头并重新初始化
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
    
    //等待摄像头初始化成功
    while(!scc8660_uart_receive_flag_dvp && timeout-- > 0)
    {
        system_delay_ms(1);
    }


    scc8660_uart_receive_flag_dvp = 0;
    timeout = SCC8660_INIT_TIMEOUT;

    while(((0xff != scc8660_uart_receive_dvp[1]) || (0xff != scc8660_uart_receive_dvp[2])) && timeout-- > 0)   // 判断数据是否截取到对应内容
    {
        system_delay_ms(1);
    }
    //以上部分对摄像头配置的数据全部都会保存在摄像头上51单片机的eeprom中
    //利用set_exposure_time函数单独配置的曝光数据不存储在eeprom中

    if(timeout <= 0)                                                     // 超时
        return 1;

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取摄像头内部全部配置信息
//  @param      uartn       选择使用的串口
//  @param      buff        接收配置信息的地址
//  @return     void
//  @since      v1.0
//  Sample usage:           调用该函数前请先初始化摄像头配置串口
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
        
        //等待接受回传数据
        while(!scc8660_uart_receive_flag_dvp && timeout-- > 0)
        {
            system_delay_ms(1);
        }

        if(timeout <= 0)                                                     // 超时
            return 1;

        scc8660_uart_receive_flag_dvp = 0;
        buff[i][1] = scc8660_uart_receive_dvp[1]<<8 | scc8660_uart_receive_dvp[2];
    }
    return 0;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取摄像头固件版本
//  @param      uartn               选择使用的串口
//  @return     (uint16_t)camera_id   返回摄像头ID
//  @since      v1.0
//  Sample usage:           调用该函数前请先初始化摄像头配置串口
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
        
    //等待接受回传数据
    while(!scc8660_uart_receive_flag_dvp && timeout-- > 0)
    {
        system_delay_ms(1);
    }
    scc8660_uart_receive_flag_dvp = 0;
    
    if(timeout <= 0)                                                     // 超时
          return 0;

    return ((uint16_t)(scc8660_uart_receive_dvp[1]<<8) | scc8660_uart_receive_dvp[2]);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取摄像头某一个参数配置信息
//  @param      uartn               选择使用的串口
//  @return     (uint16_t)config      返回参数值
//  @since      v1.0
//  Sample usage:           调用该函数前请先初始化摄像头配置串口
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

    //等待接受回传数据
    while(!scc8660_uart_receive_flag_dvp && timeout-- > 0)
    {
        system_delay_ms(1);
    }
    scc8660_uart_receive_flag_dvp = 0;
    
    if(timeout <= 0)                                                     // 超时
          return 0;

    return ((uint16_t)(scc8660_uart_receive_dvp[1]<<8) | scc8660_uart_receive_dvp[2]);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取彩色摄像头固件版本
//  @param      uartn       选择使用的串口
//  @return     uint16_t      版本号
//  @since      v1.0
//  Sample usage:           调用该函数前请先初始化摄像头配置串口
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
        
    //等待接受回传数据
    while(!scc8660_uart_receive_flag_dvp && timeout-- > 0)
    {
        system_delay_ms(1);
    }
    scc8660_uart_receive_flag_dvp = 0;
    
    if(timeout <= 0)                                                     // 超时
          return 0;

    return ((uint16_t)(scc8660_uart_receive_dvp[1]<<8) | scc8660_uart_receive_dvp[2]);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      单独设置图像亮度
//  @param      uartn       选择使用的串口
//  @param      data        需要设置的亮度值
//  @return     uint16_t      返回设置的亮度值
//  @since      v1.0
//  Sample usage:           调用该函数前请先初始化摄像头配置串口
//  @note                   通过该函数设置的参数，不会被51单片机保存
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
    
    //等待接受回传数据
    while(!scc8660_uart_receive_flag_dvp && timeout-- > 0)
    {
        system_delay_ms(1);
    }
    scc8660_uart_receive_flag_dvp = 0;
    
    if(timeout <= 0)                                                     // 超时
          return 0;

    return ((uint16_t)(scc8660_uart_receive_dvp[1]<<8) | scc8660_uart_receive_dvp[2]);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      单独设置白平衡
//  @param      uartn       选择使用的串口
//  @param      data        需要设置的亮度值
//  @return     uint16_t      返回设置的亮度值
//  @since      v1.0
//  Sample usage:           调用该函数前请先初始化摄像头配置串口
//  @note                   通过该函数设置的参数，不会被51单片机保存
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
    
    //等待接受回传数据
    while(!scc8660_uart_receive_flag_dvp && timeout-- > 0)
    {
        system_delay_ms(1);
    }
    scc8660_uart_receive_flag_dvp = 0;
    
    if(timeout <= 0)                                                     // 超时
          return 0;

    return ((uint16_t)(scc8660_uart_receive_dvp[1]<<8) | scc8660_uart_receive_dvp[2]);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      单独设置摄像头寄存器
//  @param      uartn       选择使用的串口
//  @param      reg         寄存器地址
//  @param      data        需要写入寄存器的数值
//  @return     uint16_t      回传写入寄存器的值
//  @since      v1.0
//  Sample usage:           调用该函数前请先初始化串口
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
    
    //等待接受回传数据
    while(!scc8660_uart_receive_flag_dvp && timeout-- > 0)
    {
        system_delay_ms(1);
    }
    scc8660_uart_receive_flag_dvp = 0;
    
    if(timeout <= 0)                                                     // 超时
          return 0;

    return ((uint16_t)(scc8660_uart_receive_dvp[1]<<8) | scc8660_uart_receive_dvp[2]);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      SCC8660摄像头场中断
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:               在isr.c里面先创建对应的中断函数，然后调用该函数(之后别忘记清除中断标志位)
//-------------------------------------------------------------------------------------------------------------------
void scc8660_vsync_dvp(void)
{
    //恢复DMA地址
    DVP->DMA_BUF0 = (uint32_t)camera_buffer_addr;

    //采集完成标志位置1
    scc8660_finish_flag_dvp = 1;

    //开启DVP
    DVP->CR0 |= RB_DVP_ENABLE;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      SCC8660摄像头行中断
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:               暂不需要
//-------------------------------------------------------------------------------------------------------------------
void scc8660_dvp_hsync(void)
{

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      SCC8660(凌瞳摄像头)初始化 使用DVP接口
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:		
//-------------------------------------------------------------------------------------------------------------------
uint8_t scc8660_init_dvp(void)
{

    //摄像头类型设置为凌瞳
    set_camera_type(CAMERA_COLOR);
    //摄像头开始初始化之前务必将场信号拉高
    gpio_init(SCC8660_VSY_PIN_DVP, GPO, 1, GPIO_PIN_CONFIG);
    //摄像头配置串口初始化
    scc8660_dvp_cof_uart_init();

    //等待摄像头上电初始化成功 方式有两种：延时或者通过获取配置的方式 二选一
    //system_delay_ms(500);//延时方式
    scc8660_uart_receive_flag_dvp = 0;
    if(scc8660_get_all_config(SCC8660_COF_UART_DVP, SCC8660_GET_CFG_DVP))//获取配置的方式
    {
        zf_assert(0);
        return 1;
    }
    //向摄像头发送配置信息
    scc8660_uart_receive_flag_dvp = 0;
    if(scc8660_set_all_config(SCC8660_COF_UART_DVP, SCC8660_CFG_DVP))
    {
        zf_assert(0);
        return 1;
    }
    //获取配置便于查看配置是否正确
    scc8660_uart_receive_flag_dvp = 0;
    if(scc8660_get_all_config(SCC8660_COF_UART_DVP, SCC8660_GET_CFG_DVP))
    {
        zf_assert(0);
        return 1;
    }
    //为方便使用，获取配置信息后并未对数据进行校验，如果需要确认配置是否成功，请自行进行数据比对。
    interrupt_disable_all();

    //DVP引脚初始化
    dvp_gpio_init(
            SCC8660_D0_PIN_DVP, SCC8660_D1_PIN_DVP, SCC8660_D2_PIN_DVP, SCC8660_D3_PIN_DVP,
            SCC8660_D4_PIN_DVP, SCC8660_D5_PIN_DVP, SCC8660_D6_PIN_DVP, SCC8660_D7_PIN_DVP,
            SCC8660_PCLK_PIN_DVP, SCC8660_VSY_PIN_DVP, SCC8660_HERF_PIN_DVP);

    //设置图像地址初值，务必保留
    camera_buffer_addr = (uint8_t *)scc8660_image_dvp[0];

    //DVP 采集初始化
    dvp_camera_init((uint32_t *)camera_buffer_addr, NULL, SCC8660_DVP_W*SCC8660_DVP_H, 1);
    return 0;

}




//-------------------------------------------------------------------------------------------------------------------
//  @brief      SCC8660(凌瞳摄像头)获取像素点RGB分量
//  @param      *dat    图像数组的地址
//  @param      x       需要获取的像素所在列
//  @param      y       需要获取的像素所在行
//  @param      z       图像宽度
//  @param      *r      接收r分量地址 返回值范围0-31
//  @param      *g      接收g分量地址 返回值范围0-63
//  @param      *b      接收b分量地址 返回值范围0-31
//  @return     void
//  @since      v1.0
//  Sample usage:		color_camera_take_point(scc8660_dvp_image[0],0, 0, SCC8660_DVP_PIC_W, &r_value,&g_value,&b_value);//获取第0列 第0行像素的RGB分量  摄像头的数据格式必须设置为0
//	@note				此函数主要目的是为了让大家能够清晰的了解RGB数据是如何存储的
//-------------------------------------------------------------------------------------------------------------------
void inline color_camera_take_point(uint16_t *dat, uint16_t x, uint16_t y, uint16_t z, uint8_t *r, uint8_t *g, uint8_t *b)
{
    uint16_t pixel;

    //获取指定坐标的像素数据
    pixel = dat[x+y*z];

    //因为R5G3是存储在低八位 G3B5是存储在高八位
    //因为我们先将位置进行交换，便于获取每个分量的数据
    pixel = ((pixel&0xff)<<8) |(pixel>>8);

    *r = pixel>>11;
    *g = (pixel>>5)&0x3f;
    *b = pixel&0x1f;
}



