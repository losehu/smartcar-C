/**
 //@FileName	:Filter.h
 //@CreatedDate	:2021��12��22��
 //@Author		:LiHao
 //@Description	:
**/

#ifndef FILTER_H_
#define FILTER_H_

#include "zf_common_headfile.h"

//kalman�˲�
//1. �ṹ�����Ͷ���
typedef struct
{
    float LastP;//�ϴι���Э���� ��ʼ��ֵΪ0.02
    float Now_P;//��ǰ����Э���� ��ʼ��ֵΪ0
    float out;//�������˲������ ��ʼ��ֵΪ0
    float Kg;//���������� ��ʼ��ֵΪ0
    float Q;//��������Э���� ��ʼ��ֵΪ0.001
    float R;//�۲�����Э���� ��ʼ��ֵΪ0.543
}KFP_Typedef;//Kalman Filter parameter
/****************************��˹��ֵ�˲�*********************************/

typedef struct {
    uint8 FilterDepth;//�˲����
    uint8 K;//K����׼��
    float InSum;//��
    float OutSum;
    float Average;//��ֵ
    float NxVariance;//N������
    float StdDev;//��׼��
    uint8 Counter;
    uint8 ReNum;//ʣ�������
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

extern uint8  UltrasoundUseUart_Flag;//ʹ�ô��ڳ�������־λ
extern uint16 VehiclesDis;//����
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
