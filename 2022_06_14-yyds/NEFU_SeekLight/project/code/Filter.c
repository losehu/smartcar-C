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

///////////Communicate.c


uint8 UltrasoundUseUart_Flag = 'F';//使用串口超声波标志位
uint16 VehiclesDis=0;//与后车车距
/**************************超声波设备***********************/
/**
*@Name          :Ultrasound_Init
*@Description   :超声波设备初始化
*@Param         :Mode:1发送初始化;2接受初始化;3发送和接受初始化
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
//      DEBUG_UART_INDEX,                                                       // 在 zf_common_debug.h 中查看对应值
//      DEBUG_UART_BAUDRATE,                                                    // 在 zf_common_debug.h 中查看对应值
//      DEBUG_UART_TX_PIN,                                                      // 在 zf_common_debug.h 中查看对应值
//      DEBUG_UART_RX_PIN);                                                     // 在 zf_common_debug.h 中查看对应值
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
*@Description   :超声波测距,有接受端时有效;中断中调用
*@Param         :
*@Return        :距离/mm
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
    if(dat[0] != 0xa5)  num = 0;//检查头帧是否正确，不正确就重新接收
    else                num++;

    if(num==3)                  //接收完成，开始处理数据
    {
        num = 0;
        //将数据的高八位与低八位进行组合，得到距离信息单位MM
        if(myabs(lastdis - dis <= 200) && (dat[1]<<8 | dat[2]) <= 5000)
            dis = dat[1]<<8 | dat[2];
        lastdis = dis;
        dat[0] = 0;
    }
//  tempdis = dis;
//    VehiclesDis = SlidingFilter(&SFP_VehiclesDis, dis);//滑动滤波
    VehiclesDis = (uint16)GaussianFilter(&GFP_VehiclesDis, dis);
    return dis;
}



///////////Communicate.h

/**************************超声波设备***********************/


///////////////filter.c

/***************************** 高斯均值滤波 ***********************************/
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




