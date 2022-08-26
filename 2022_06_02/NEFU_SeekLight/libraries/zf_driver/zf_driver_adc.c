/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            zf_driver_adc
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 * @note            version:
 *                  V1.1 2021.11.30 关闭ADC BUFF使能
 *                  V1.2 2022.03.29 给ADC_InitStructure结构体赋初值
 ********************************************************************************************************************/

#include "zf_driver_adc.h"
#include "zf_driver_gpio.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC引脚初始化
//  @param      adc_ch      adc_ch通道(可选择范围由zf_adc.h内ADC_CH_enum枚举值确定)
//  @return     void
//  Sample usage:           内部使用  用户无需关心
//-------------------------------------------------------------------------------------------------------------------
void adc_gpio_init(ADC_CH_enum adc_ch)
{
    if(ADC_IN0_A0 == adc_ch)            gpio_init(A0, GPI, 0, GPI_ANAOG_IN);
    else if(ADC_IN1_A1 == adc_ch)       gpio_init(A1, GPI, 0, GPI_ANAOG_IN);
    else if(ADC_IN2_A2 == adc_ch)       gpio_init(A2, GPI, 0, GPI_ANAOG_IN);
    else if(ADC_IN3_A3 == adc_ch)       gpio_init(A3, GPI, 0, GPI_ANAOG_IN);
    else if(ADC_IN4_A4 == adc_ch)       gpio_init(A4, GPI, 0, GPI_ANAOG_IN);
    else if(ADC_IN5_A5 == adc_ch)       gpio_init(A5, GPI, 0, GPI_ANAOG_IN);
    else if(ADC_IN6_A6 == adc_ch)       gpio_init(A6, GPI, 0, GPI_ANAOG_IN);
    else if(ADC_IN7_A7 == adc_ch)       gpio_init(A7, GPI, 0, GPI_ANAOG_IN);
    else if(ADC_IN8_B0 == adc_ch)       gpio_init(B0, GPI, 0, GPI_ANAOG_IN);
    else if(ADC_IN9_B1 == adc_ch)       gpio_init(B1, GPI, 0, GPI_ANAOG_IN);
    else if(ADC_IN10_C0 == adc_ch)      gpio_init(C0, GPI, 0, GPI_ANAOG_IN);
    else if(ADC_IN11_C1 == adc_ch)      gpio_init(C1, GPI, 0, GPI_ANAOG_IN);
    else if(ADC_IN12_C2 == adc_ch)      gpio_init(C2, GPI, 0, GPI_ANAOG_IN);
    else if(ADC_IN13_C3 == adc_ch)      gpio_init(C3, GPI, 0, GPI_ANAOG_IN);
    else if(ADC_IN14_C4 == adc_ch)      gpio_init(C4, GPI, 0, GPI_ANAOG_IN);
    else if(ADC_IN15_C5 == adc_ch)      gpio_init(C5, GPI, 0, GPI_ANAOG_IN);
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC转换一次
//  @param      ch              选择ADC通道
//  @param      resolution      分辨率(8位 10位 12位)
//  @return     void
//  Sample usage:               adc_convert(ADC_IN0_A0, ADC_8BIT);  //采集A0端口返回8位分辨率的AD值
//-------------------------------------------------------------------------------------------------------------------
uint16_t adc_convert(ADC_CH_enum adc_ch, ADC_RES_enum resolution)
{
    ADC_RegularChannelConfig(ADC1, (uint8_t)adc_ch, 1, ADC_SampleTime_41Cycles5 );
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
    return ((ADC1->RDATAR)>>resolution);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC转换N次，平均值滤波
//  @param      ch              选择ADC通道
//  @param      resolution      分辨率(8位 10位 12位)
//  @param      count           转换次数
//  @return     void
//  Sample usage:               adc_mean_filter(ADC_IN0_A0, ADC_8BIT,5);  //采集A0端口返回8位分辨率的AD值，采集五次取平均值
//-------------------------------------------------------------------------------------------------------------------
uint16_t adc_mean_filter(ADC_CH_enum adc_ch, ADC_RES_enum resolution, uint8_t count)
{
    uint8_t i;
    uint32_t sum;

    zf_assert(count);//断言次数不能为0

    sum = 0;
    for(i=0; i<count; i++)
    {
        sum += adc_convert(adc_ch, resolution);
    }

    sum = sum/count;
    return sum;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC初始化
//  @param      adc_ch      adc_ch通道(可选择范围由zf_adc.h内ADC_CH_enum枚举值确定)
//  @return     void
//  Sample usage:           adc_init(ADC_IN0_A0);//初始化A0引脚为ADC功能
//-------------------------------------------------------------------------------------------------------------------
void adc_init(ADC_CH_enum adc_ch)
{


    ADC_InitTypeDef ADC_InitStructure={0};

    adc_gpio_init(adc_ch);                                              //GPIO初始化

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE );              //使能ADC1通道时钟
    RCC_ADCCLKConfig(RCC_PCLK2_Div2);

//    if(SystemCoreClock > 84000000)  RCC_ADCCLKConfig(RCC_PCLK2_Div8);   //设置ADC分频因子，ADC最大速率不能超过14M
//    else RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  //ADC工作模式:ADC1工作在独立模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                       //模数转换工作在单通道模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                 //模数转换工作在单次转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //转换由软件而不是外部触发启动
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              //ADC数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;                             //顺序进行规则转换的ADC通道的数目
    ADC_Init(ADC1, &ADC_InitStructure);                                 //根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器

    ADC_Cmd(ADC1, ENABLE);                                              //使能指定的ADC1
    ADC_BufferCmd(ADC1, DISABLE);                                       //disable buffer

    ADC_ResetCalibration(ADC1);                                         //使能复位校准
    while(ADC_GetResetCalibrationStatus(ADC1));                         //等待复位校准结束
    ADC_StartCalibration(ADC1);                                         //开启AD校准
    while(ADC_GetCalibrationStatus(ADC1));                              //等待校准结束
    //ADC_BufferCmd(ADC1, ENABLE);                                        //enable buffer

}
