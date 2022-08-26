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

///////////Communicate.c


uint8 UltrasoundUseUart_Flag = 'F';//ʹ�ô��ڳ�������־λ
uint16 VehiclesDis=0;//��󳵳���
/**************************�������豸***********************/
/**
*@Name          :Ultrasound_Init
*@Description   :�������豸��ʼ��
*@Param         :Mode:1���ͳ�ʼ��;2���ܳ�ʼ��;3���ͺͽ��ܳ�ʼ��
*@Return        :
*@Sample        :
**/
void Ultrasound_Init(uint8 Mode)
{
//    switch (Mode) {
//        case 1:
//            gpio_mode(TxEn_PIN, GPO_PP);
//            break;
//        case 2:
//            uart_init(Rx_UART, RxTx_Pin, RxRx_PIN, 115200, Rx_UART_TIM);
//uart_init(
//      DEBUG_UART_INDEX,                                                       // �� zf_common_debug.h �в鿴��Ӧֵ
//      DEBUG_UART_BAUDRATE,                                                    // �� zf_common_debug.h �в鿴��Ӧֵ
//      DEBUG_UART_TX_PIN,                                                      // �� zf_common_debug.h �в鿴��Ӧֵ
//      DEBUG_UART_RX_PIN);                                                     // �� zf_common_debug.h �в鿴��Ӧֵ
debug_init();
            UltrasoundUseUart_Flag = 'T';
//            break;
//        case 3:
//            gpio_mode(TxEn_PIN, GPO_PP);
//            uart_init(Rx_UART, RxTx_Pin, RxRx_PIN, 115200, Rx_UART_TIM);
//            UltrasoundUseUart_Flag = 'T';
//            break;
//        default:
//            break;
//    }
}

/**
*@Name          :UltrasoundGetDistance
*@Description   :���������,�н��ܶ�ʱ��Ч;�ж��е���
*@Param         :
*@Return        :����/mm
*@Sample        :
**/
uint16 UltrasoundGetDistance(void)
{
    static uint8 dat[3];
    static uint8 num = 0;
    static uint16 dis = 0;
    static uint16 lastdis = 0;

//    dat[num] = Rx_UART_BUF;
    uart_read_byte(DEBUG_UART_INDEX,&dat[num]);
    if(dat[0] != 0xa5)  num = 0;//���ͷ֡�Ƿ���ȷ������ȷ�����½���
    else                num++;

    if(num==3)                  //������ɣ���ʼ��������
    {
        num = 0;
        //�����ݵĸ߰�λ��Ͱ�λ������ϣ��õ�������Ϣ��λMM
        if(myabs(lastdis - dis <= 200) && (dat[1]<<8 | dat[2]) <= 5000)
            dis = dat[1]<<8 | dat[2];
        lastdis = dis;
        dat[0] = 0;
    }
//  tempdis = dis;
//    VehiclesDis = SlidingFilter(&SFP_VehiclesDis, dis);//�����˲�
    VehiclesDis = (uint16)GaussianFilter(&GFP_VehiclesDis, dis);
    return dis;
}



///////////Communicate.h

/**************************�������豸***********************/


///////////////filter.c

/***************************** ��˹��ֵ�˲� ***********************************/
GFP_Typedef GFP_VehiclesDis = {20, 1, 0, 0, 0, 0, 0, 0, 0, 0};

float GaussianFilter(GFP_Typedef *gfp, float input )
{
    uint8 i;
    if(gfp->Counter < gfp->FilterDepth) {
        gfp->Dat[gfp->Counter] = input;
        gfp->InSum += input;
        gfp->Counter ++;
    }
    if(gfp->Counter >= gfp->FilterDepth) {
        gfp->ReNum = gfp->FilterDepth;
        gfp->OutSum = gfp->InSum;
        gfp->Average = gfp->InSum/(float)gfp->FilterDepth;
        for ( i = 0; i < gfp->FilterDepth; i++)
            gfp->NxVariance += (gfp->Dat[i] - gfp->Average) * (gfp->Dat[i] - gfp->Average);
        gfp->StdDev = sqrt(gfp->NxVariance/(float)gfp->FilterDepth);
        for ( i = 0; i < gfp->FilterDepth; i++) {
            if ((!((gfp->Dat[i] > gfp->Average - (float)gfp->K * gfp->StdDev) &&
                  (gfp->Dat[i] < gfp->Average + (float)gfp->K * gfp->StdDev))) &&
                gfp->ReNum > 1) {
                gfp->ReNum--;
                gfp->OutSum -= gfp->Dat[i];
            }
        }
        gfp->Out = gfp->OutSum/(float)gfp->ReNum;
        gfp->InSum = 0;
        gfp->Counter = 0;
        gfp->NxVariance = 0;
    }
    return gfp->Out;
}


///////////////filter.h




