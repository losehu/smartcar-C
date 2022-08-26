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

//statement
extern KFP_Typedef KFP_Speed;
extern KFP_Typedef KFP_ELC14;
extern KFP_Typedef KFP_ELC23;

//function
float KalmanFilter(KFP_Typedef *kfp,float input);


#endif /* FILTER_H_ */
