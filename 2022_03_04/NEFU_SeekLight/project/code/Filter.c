/**
 //@FileName	:Filter.c
 //@CreatedDate	:2021年12月22日
 //@Author		:LiHao
 //@Description	:
**/

#include "Filter.h"




/****************************************** 卡尔曼滤波 ***************************************************/

KFP_Typedef KFP_Speed ={0.02,0,0,0,0.001,0.010};
KFP_Typedef KFP_ELC14 ={0.02,0,0,0,0.001,0.234};
KFP_Typedef KFP_ELC23 ={0.02,0,0,0,0.001,0.234};

//卡尔曼滤波器
float KalmanFilter(KFP_Typedef *kfp,float input)
{
     //预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差
     kfp->Now_P = kfp->LastP + kfp->Q;
     //卡尔曼增益方程：卡尔曼增益 = k时刻系统估算协方差 / （k时刻系统估算协方差 + 观测噪声协方差）
     kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
     //更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值 - 状态变量的预测值）
     kfp->out = kfp->out + kfp->Kg * (input -kfp->out);//因为这一次的预测值就是上一次的输出值
     //更新协方差方程: 本次的系统协方差付给 kfp->LastP 威下一次运算准备。
     kfp->LastP = (1-kfp->Kg) * kfp->Now_P;
     return kfp->out;
}
