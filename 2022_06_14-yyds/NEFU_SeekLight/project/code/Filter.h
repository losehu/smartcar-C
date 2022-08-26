/**
 //@FileName	:Filter.h
 //@CreatedDate	:2021年12月22日
 //@Author		:LiHao
 //@Description	:
**/

#ifndef FILTER_H_
#define FILTER_H_

#include "zf_common_headfile.h"

//kalman滤波
//1. 结构体类型定义
typedef struct
{
    float LastP;//上次估算协方差 初始化值为0.02
    float Now_P;//当前估算协方差 初始化值为0
    float out;//卡尔曼滤波器输出 初始化值为0
    float Kg;//卡尔曼增益 初始化值为0
    float Q;//过程噪声协方差 初始化值为0.001
    float R;//观测噪声协方差 初始化值为0.543
}KFP_Typedef;//Kalman Filter parameter
/****************************高斯均值滤波*********************************/

typedef struct {
    uint8 FilterDepth;//滤波深度
    uint8 K;//K倍标准差
    float InSum;//和
    float OutSum;
    float Average;//均值
    float NxVariance;//N倍方差
    float StdDev;//标准差
    uint8 Counter;
    uint8 ReNum;//剩余的数量
    float Out;
    float Dat[256];
}GFP_Typedef;

extern GFP_Typedef GFP_VehiclesDis;

float GaussianFilter(GFP_Typedef *gfp, float input );

#define TxEn_PIN    P3_2
#define Rx_UART     DEBUG_UART_INDEX
#define RxRx_PIN    DEBUG_UART_RX_PIN
#define RxTx_Pin    DEBUG_UART_TX_PIN
#define Rx_UART_TIM TIM_2
//#define Rx_UART_BUF S3BUF

extern uint8  UltrasoundUseUart_Flag;//使用串口超声波标志位
extern uint16 VehiclesDis;//车距
extern uint16 tempdis ;
//fun
void Ultrasound_Init(uint8 Mode);
uint16 UltrasoundGetDistance(void);


//statement
extern KFP_Typedef KFP_Speed;
extern KFP_Typedef KFP_ELC14;
extern KFP_Typedef KFP_ELC23;

//function
float KalmanFilter(KFP_Typedef *kfp,float input);


#endif /* FILTER_H_ */
