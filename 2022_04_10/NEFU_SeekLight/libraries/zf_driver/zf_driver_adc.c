/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            zf_driver_adc
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ790875685)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        MounRiver Studio V1.51
 * @Target core     CH32V307VCT6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2021-11-25
 * @note            version:
 *                  V1.1 2021.11.30 �ر�ADC BUFFʹ��
 *                  V1.2 2022.03.29 ��ADC_InitStructure�ṹ�帳��ֵ
 ********************************************************************************************************************/

#include "zf_driver_adc.h"
#include "zf_driver_gpio.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC���ų�ʼ��
//  @param      adc_ch      adc_chͨ��(��ѡ��Χ��zf_adc.h��ADC_CH_enumö��ֵȷ��)
//  @return     void
//  Sample usage:           �ڲ�ʹ��  �û��������
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
//  @brief      ADCת��һ��
//  @param      ch              ѡ��ADCͨ��
//  @param      resolution      �ֱ���(8λ 10λ 12λ)
//  @return     void
//  Sample usage:               adc_convert(ADC_IN0_A0, ADC_8BIT);  //�ɼ�A0�˿ڷ���8λ�ֱ��ʵ�ADֵ
//-------------------------------------------------------------------------------------------------------------------
uint16_t adc_convert(ADC_CH_enum adc_ch, ADC_RES_enum resolution)
{
    ADC_RegularChannelConfig(ADC1, (uint8_t)adc_ch, 1, ADC_SampleTime_41Cycles5 );
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
    return ((ADC1->RDATAR)>>resolution);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADCת��N�Σ�ƽ��ֵ�˲�
//  @param      ch              ѡ��ADCͨ��
//  @param      resolution      �ֱ���(8λ 10λ 12λ)
//  @param      count           ת������
//  @return     void
//  Sample usage:               adc_mean_filter(ADC_IN0_A0, ADC_8BIT,5);  //�ɼ�A0�˿ڷ���8λ�ֱ��ʵ�ADֵ���ɼ����ȡƽ��ֵ
//-------------------------------------------------------------------------------------------------------------------
uint16_t adc_mean_filter(ADC_CH_enum adc_ch, ADC_RES_enum resolution, uint8_t count)
{
    uint8_t i;
    uint32_t sum;

    zf_assert(count);//���Դ�������Ϊ0

    sum = 0;
    for(i=0; i<count; i++)
    {
        sum += adc_convert(adc_ch, resolution);
    }

    sum = sum/count;
    return sum;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC��ʼ��
//  @param      adc_ch      adc_chͨ��(��ѡ��Χ��zf_adc.h��ADC_CH_enumö��ֵȷ��)
//  @return     void
//  Sample usage:           adc_init(ADC_IN0_A0);//��ʼ��A0����ΪADC����
//-------------------------------------------------------------------------------------------------------------------
void adc_init(ADC_CH_enum adc_ch)
{


    ADC_InitTypeDef ADC_InitStructure={0};

    adc_gpio_init(adc_ch);                                              //GPIO��ʼ��

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE );              //ʹ��ADC1ͨ��ʱ��
    RCC_ADCCLKConfig(RCC_PCLK2_Div2);

//    if(SystemCoreClock > 84000000)  RCC_ADCCLKConfig(RCC_PCLK2_Div8);   //����ADC��Ƶ���ӣ�ADC������ʲ��ܳ���14M
//    else RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  //ADC����ģʽ:ADC1�����ڶ���ģʽ
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                       //ģ��ת�������ڵ�ͨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                 //ģ��ת�������ڵ���ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //ת��������������ⲿ��������
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              //ADC�����Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 1;                             //˳����й���ת����ADCͨ������Ŀ
    ADC_Init(ADC1, &ADC_InitStructure);                                 //����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���

    ADC_Cmd(ADC1, ENABLE);                                              //ʹ��ָ����ADC1
    ADC_BufferCmd(ADC1, DISABLE);                                       //disable buffer

    ADC_ResetCalibration(ADC1);                                         //ʹ�ܸ�λУ׼
    while(ADC_GetResetCalibrationStatus(ADC1));                         //�ȴ���λУ׼����
    ADC_StartCalibration(ADC1);                                         //����ADУ׼
    while(ADC_GetCalibrationStatus(ADC1));                              //�ȴ�У׼����
    //ADC_BufferCmd(ADC1, ENABLE);                                        //enable buffer

}
