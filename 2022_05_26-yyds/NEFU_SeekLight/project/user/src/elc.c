/**
 //@FileName    :elc.c
 //@CreatedDate :2021年12月22日
 //@Author      :LiHao
 //@Description :
**/

#include "elc.h"

//statement
#define PROTECR_INDUCTOR_VAL  100

//原始值
int32   LNow[ELC_NUM] = {0,0,0,0},
        LMax[ELC_NUM] = {0,0,0,0},
        LMin[ELC_NUM] = {0,0,0,0};
uint16  ELC[ELC_GROUP][ELC_TIME][ELC_NUM];
float   LNor[ELC_NUM];
uint16  LNORMAX[ELC_NUM] = {4095,4095,4095,4095};
uint8   MaxNorIndex = 0;
uint8   MinNorIndex = 0;
uint16    ELC_Sum_14 = 0;
uint16    ELC_Sum_23 = 0;
uint16    ELC_Sum_1234 = 0;
uint16  ElcSum_14 = 0;
uint16  ElcSum_23 = 0;
uint16  ElcSum_1234 = 0;
int16   ElcCenter_14 = 0;
int16   ElcCenter_23 = 0;
uint8   ElcProtect_Flag = 'F';
uint16  KFP_ElcSum_14 = 0;
uint16  KFP_ElcSum_23 = 0;
float  ElcCenter_14_100;

//读取电感值
void ElcRead(void)
{
    uint16 L_sum[ELC_NUM] = {0,0,0,0};
    uint8 i = 0,j = 0,k = 0;
    uint16  L_max = 0,L_max_index = 0,len = 0;
    //四路ADC
    for(i = 0;i < ELC_GROUP;i ++)
    {
        for(j = 0;j < ELC_TIME;j ++)
        {//在头文件L1-L4中调整顺序
            ELC[i][j][3] = adc_convert(ADC_IN4,ADC_12BIT);
            ELC[i][j][2] = adc_convert(ADC_IN3,ADC_12BIT);
            ELC[i][j][1] = adc_convert(ADC_IN2,ADC_12BIT);
            ELC[i][j][0] = adc_convert(ADC_IN1,ADC_12BIT);
        }
    }
    //选择排序
    //四路ADC
    for(i = 0;i < ELC_GROUP;i ++)//所有的组都需要排序
    {
        for(k = 0;k < ELC_NUM;k ++)//每个电感号组内排序
        {
            len = ELC_TIME;
            while(len > 1)//排序到只剩下一个元素后不再进行排序
            {
                L_max = 0;
                L_max_index = 0;
                for(j = 0;j < len;j ++)
                {
                    if(ELC[i][j][k] > L_max)
                    {
                        L_max = ELC[i][j][k];//记录最大值
                        L_max_index = j;//记录最大值下标
                    }
                }
                ELC[i][L_max_index][k] = ELC[i][len - 1][k];//交换数据
                ELC[i][len - 1][k] = L_max;//最大值后移
                len --;
            }
            L_sum[k] += ELC[i][ELC_NEED][k];
            if(LMax[k] < ELC[i][ELC_TIME - 1][k])
                LMax[k] = ELC[i][ELC_TIME - 1][k];
            if(LMin[k] > ELC[i][0][k])
                LMin[k] = ELC[i][0][k];
        }
    }
    //中值
    for(i = 0;i < ELC_NUM;i ++)
    {
        LNow[i] = L_sum[i] / ELC_GROUP;
    }
}


//归一化
void ElcNormalized(void)
{
    uint8 i = 0;
    uint8 max_nor = 0;
    uint8 min_nor = 100;
    //第一次归一化
    for(i = 0;i < ELC_NUM;i ++)
    {
        LNor[i] = LNow[i];
        LNor[i] = LNor[i]*100/LNORMAX[i];
    }
    //求取最大归一化电感下标 用于停车保护
    for(i = 0;i < ELC_NUM;i ++)
    {
        if(max_nor < LNor[i])
        {
            max_nor = LNor[i];
            MaxNorIndex = i;
        }
    }
    //求取最小归一化电感下标 用与元素保护
    for(i = 0;i < ELC_NUM;i ++)
    {
        if(min_nor > LNor[i])
        {
            min_nor = LNor[i];
            MinNorIndex = i;
        }
    }
    if(max_ab(LNow[0],LNow[3]) <= PROTECR_INDUCTOR_VAL)
        ElcProtect_Flag = 'T';
    else
        ElcProtect_Flag = 'F';

    //求取135电感之和 并且做一个队列
    ELC_Sum_14 = LNow[0] + LNow[3];
        ELC_Sum_23 = LNow[1] + LNow[2];
        ELC_Sum_1234= ELC_Sum_14 + ELC_Sum_23;
    ElcSum_14 = LNor[0] + LNor[3];
    ElcSum_23 = LNor[1] + LNor[2];
    ElcSum_1234 =  ElcSum_14 + ElcSum_23;
}


//中线误差计算 使用第一次归一化的数值
//使用差比和算法
#define ElcMAX_CENTER_ERROR 100
void ElcCenterCalculate(void)
{
    uint8 LNorm[4] = {0,0,0};
    int32 NearLError[2] = {0,0};
    int32 NearLSum[2] = {0,0};
    //复制原归一化值
    LNorm[0] = LNor[0] + 1;
    LNorm[1] = LNor[1] + 1;
    LNorm[2] = LNor[2] + 1;
    LNorm[3] = LNor[3] + 1;
    //14 23电感作差
    NearLError[0] = LNorm[3] - LNorm[0];//横电感值差
    NearLError[1] = LNorm[2] - LNorm[1];//斜电感值差
    //14 23电感做和
    NearLSum[0] = LNorm[0] + LNorm[3];//横电感值差
    NearLSum[1] = LNorm[1] + LNorm[2];//斜电感值差
    //差比和计算两种中线
    ElcCenter_14_100 = (LNorm[3] - LNorm[0]) * 10000 / NearLSum[0];
    ElcCenter_14 = NearLError[0] * 100 / NearLSum[0];//横电感
    ElcCenter_23 = NearLError[1] * 100 / NearLSum[1];//斜电感
    //中线限幅 200
    ElcCenter_14 = limit_ab(ElcCenter_14,-ElcMAX_CENTER_ERROR,ElcMAX_CENTER_ERROR);
    ElcCenter_23 = limit_ab(ElcCenter_23,-ElcMAX_CENTER_ERROR,ElcMAX_CENTER_ERROR);
}


//电磁标志设置
void ElcStatus_Init(void)
{
    //Circle
//    ElcCircle.FindFlag = 'F';
//    ElcCircle.NowValLessThanMaxCnt = 0;
//    ElcCircle.SUM_MAX = 0;
//    ElcCircle.CircleInTurnFlag = 'F';
}


//电感检测元素 环岛 十字
uint8 ElcJudge(void)
{
    //对电磁进行滤波
    KFP_ElcSum_14 = KalmanFilter(&KFP_ELC14,ElcSum_14);
    KFP_ElcSum_23 = KalmanFilter(&KFP_ELC23,ElcSum_23);
//  if(ElcSum_14 >= 80 && ElcSum_23 >= 70)
//      Beep_DiDi(1,500);
//  ElcCircle_Judge();
//    if(KFP_ElcSum_14 >= Circle.CircleELC14 &&KFP_ElcSum_23 >= Circle.CircleELC23)
        return 1;
//    else return 0;
}

/**
*@Name          :ElcGetDate
*@Description   :
*@Param         :
*@Return        :
*@Sample        :
**/
void ElcGetDate(void)
{
    ElcRead();
    ElcNormalized();
//    ElcCenterCalculate();
//    ElcJudge();
}


//电感的显示界面
uint8 ElcShow(void)
{
    while(KeyLast != onepress)
    {
        ElcRead();
        ElcNormalized();
        ElcCenterCalculate();
//        uint16 dat[4];
//        dat[0] = adc_convert(ELCR0,ADC_12BIT);
//        dat[1] = adc_convert(ELCR1,ADC_12BIT);
//        dat[0] = adc_convert(ADC_IN12_C2,ADC_12BIT);
//        dat[1] = adc_convert(ADC_IN13_C3,ADC_12BIT);
//        dat[2] = adc_convert(ELCL0,ADC_12BIT);
//        dat[3] = adc_convert(ELCL1,ADC_12BIT);
        ips114_pencolor = RED;
        ips114_show_string(0,0,(const char*)"    L0    L1    R0   R1 ");
        ips114_pencolor = BLUE;
        //原始值
        ips114_show_uint16(6  ,1,LNow[0]);
        ips114_show_uint16(52 ,1,LNow[1]);
        ips114_show_uint16(98 ,1,LNow[2]);
        ips114_show_uint16(144,1,LNow[3]);
        //归一化值
        ips114_pencolor = RED;
        ips114_show_string(0,2,(const char*)"    L0    L1   R0    R1  ");
        ips114_pencolor = BLUE;
        ips114_show_uint8(6  ,3,LNor[0]);
        ips114_show_uint8(52 ,3,LNor[1]);
        ips114_show_uint8(98 ,3,LNor[2]);
        ips114_show_uint8(144,3,LNor[3]);
        //14和 23和
        ips114_pencolor = RED;
        ips114_show_string(0,4,(const char*)"  LSUM_14  LSUM_23 LSUM_1234");
        ips114_pencolor = BLUE;
        ips114_show_int16(0 ,5,ElcSum_14);
        ips114_show_int16(80 ,5,ElcSum_23);
        ips114_show_int16(160 ,5,ElcSum_1234);
        //两种中线的值
        ips114_pencolor = RED;
        ips114_show_string(0,6,(const char*)"CENTER_14   CENTER_23");
        ips114_pencolor = BLUE;
        ips114_show_int16(16 ,7,ElcCenter_14);
        ips114_show_int16(88 ,7,ElcCenter_23);
        //return
        ips114_pencolor = RED;
    }
    KeyLast = nopress;
    return 1;
}
