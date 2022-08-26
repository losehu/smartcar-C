/**
 //@FileName	:Filter.c
 //@CreatedDate	:2021��12��22��
 //@Author		:LiHao
 //@Description	:
**/

#include "Filter.h"




/****************************************** �������˲� ***************************************************/

KFP_Typedef KFP_Speed ={0.02,0,0,0,0.001,0.010};
KFP_Typedef KFP_ELC14 ={0.02,0,0,0,0.001,0.234};
KFP_Typedef KFP_ELC23 ={0.02,0,0,0,0.001,0.234};

//�������˲���
float KalmanFilter(KFP_Typedef *kfp,float input)
{
     //Ԥ��Э����̣�kʱ��ϵͳ����Э���� = k-1ʱ�̵�ϵͳЭ���� + ��������Э����
     kfp->Now_P = kfp->LastP + kfp->Q;
     //���������淽�̣����������� = kʱ��ϵͳ����Э���� / ��kʱ��ϵͳ����Э���� + �۲�����Э���
     kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
     //��������ֵ���̣�kʱ��״̬����������ֵ = ״̬������Ԥ��ֵ + ���������� * ������ֵ - ״̬������Ԥ��ֵ��
     kfp->out = kfp->out + kfp->Kg * (input -kfp->out);//��Ϊ��һ�ε�Ԥ��ֵ������һ�ε����ֵ
     //����Э�����: ���ε�ϵͳЭ����� kfp->LastP ����һ������׼����
     kfp->LastP = (1-kfp->Kg) * kfp->Now_P;
     return kfp->out;
}
