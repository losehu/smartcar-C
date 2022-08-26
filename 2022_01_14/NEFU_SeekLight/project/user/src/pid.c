/**
 //@FileName	:pid.c
 //@CreatedDate	:2021年12月19日
 //@Author		:LiHao
 //@Description	:
**/

#include "pid.h"
#include <math.h>



//PID参数变化

#define     SPEEDLOOP_Kp        0
#define     SPEEDLOOP_Ki        1
#define     SPEEDLOOP_Kd        2
#define     SPEEDLOOP_MaxO      3
#define     SPEEDLOOP_MinO      4

/*******************速度环********************/
float SpeedPIDParam[][5] =
{
    /*   Kp       Ki       Kd     MaxO    MinO */
    {  0.120f , 0.150f , 1.600f , 0500.0f , -0500.0f  },//0.120f , 0.150f , 1.600f , 0400.0f , -0400.0f //3.000f , 5.000f , 15.000f
    {  2.000f , 0.600f , 0.100f , 0400.0f , -0400.0f  },
};

/**************速度环(单电机控制)*************/
//左电机
float LeftMotorSpeedPIDParam[][5] =
{
    /*   Kp       Ki       Kd     MaxO    MinO */
    {  3.000f , 0.300f , 0.000f , 1000.0f , -1000.0f }
};


//右电机
float RightMotorSpeedPIDParam[][5] =
{
    /*   Kp       Ki       Kd     MaxO    MinO */
    {  2.000f , 0.600f , 2.000f , 1000.0f , -1000.0f  }
};

/**************舵机控制************************/

#define     SERVOCONTROL_Kp1    0
#define     SERVOCONTROL_Kp2    1
#define     SERVOCONTROL_Ki     2
#define     SERVOCONTROL_Kd     3
#define     SERVOCONTROL_MaxO   4
#define     SERVOCONTROL_MinO   5

float ServoPIDParam[][6] =
{
    /*    Kp1 0.65      Kp2         Ki         Kd            MaxO            MinO */
    {  2.7000f , 0.000f , 0.00000f , 000.0000f ,   SERVO_ADDMAX , -SERVO_ADDMAX  },
//  {  4.00000f , 5.50000f , 0.00000f , 000.00000f ,   SERVO_ADDMAX , -SERVO_ADDMAX  },
    {  3.50000f , 1.75000f , 0.00000f , 080.00000f ,   600.0f , -600.0f },
    {  12.0000f , 0.00000f , 0.00000f , 000.00000f ,   600.0f , -600.0f },

};

/**************转向环(左加右减式)*************/

#define     TURNLOOP_Kp1        0
#define     TURNLOOP_Kp2        1
#define     TURNLOOP_Kp3        2
#define     TURNLOOP_Ki         3
#define     TURNLOOP_Kd         4
#define     TUNRLOOP_SumMax     5
#define     TURNLOOP_SumMin     6
#define     TURNLOOP_MaxO       7
#define     TURNLOOP_MinO       8

float TurnPIDParam[][9] =
{
    /*    Kp1       Kp2       kp3         Ki         Kd        SumMax       SumMin         MaxO      MinO */
    {  2.50000f , 4.50000f , 0.04000f , 0.00000f , 090.00000f ,   0100.0f ,   -0100.0f  , 1000.0f , -1000.0f },
//                 Kp2 5.00                         Kd 40
    {  3.50000f , 1.75000f , 0.07000f , 0.00000f , 080.00000f ,   0100.0f ,   -0100.0f  , 1000.0f , -1000.0f },
    {  12.0000f , 0.00000f , 0.00000f , 0.00000f , 000.00000f ,   0100.0f ,   -0100.0f  , 1000.0f , -1000.0f },
};

/*******************模糊PID*****************/

#define     FUZZY_Kp1       0
#define     FUZZY_Kp2       1
#define     FUZZY_Ki1       2
#define     FUZZY_Ki2       3
#define     FUZZY_Kd1       4
#define     FUZZY_Kd2       5
#define     FUZZY_SumMax    6
#define     FUZZY_SumMin    7
#define     FUZZY_MaxO      8
#define     FUZZY_MinO      9

float FuzzyPIDParam[][10] =
{
    /*    Kp1       Kp2         Ki1       Ki2         Kd1       Kd2     SumMax   SumMin     MaxO      MinO*/
    {  2.50000f , 4.50000f , 0.00000f , 0.00000f , 0.00000f , 0.00000 , 000.0f , 000.0f , 1000.0f , -1000.0f },

};


//初始化PID的参数
void PIDParams_Init(void)
{
    //--------------速度环---------------------//
        SpeedPID.maxout = 600;
        SpeedPID.minout = -600;
//      SpeedPID.kp = 0.06f;
//      SpeedPID.ki = 0.1f;
//      SpeedPID.kd = 0.800f;
        SpeedPID.nowError = 0;
        SpeedPID.lastError = 0;
        SpeedPID.beforeError = 0;
    //--------------控制停车用-----------------//
        //左
        SpeedPIDLeft.maxout = 1000;
        SpeedPIDLeft.minout = -1000;
//      SpeedPIDLeft.kp = 2.0f;
//      SpeedPIDLeft.ki = 0.3f;
        SpeedPIDLeft.nowError = 0;
        SpeedPIDLeft.lastError = 0;
        SpeedPIDLeft.beforeError = 0;
        //右
        SpeedPIDRight.maxout = 1000;
        SpeedPIDRight.minout = -1000;
//      SpeedPIDRight.kp = 3.0f;
//      SpeedPIDRight.ki = 0.6f;
        SpeedPIDRight.nowError = 0;
        SpeedPIDRight.lastError = 0;
        SpeedPIDRight.beforeError = 0;
    //--------------舵机控制环  摄像头----------------//
        ServoControlCam.maxout = 0;
        ServoControlCam.minout = 0;
//      ServoControlCam.kp1 = 0;
//      ServoControlCam.kp2 = 0;
//      ServoControlCam.ki = 0;
//      ServoControlCam.kd = 0;
        ServoControlCam.Up = 0;
        ServoControlCam.Ui = 0;
        ServoControlCam.Ud = 0;
        ServoControlCam.nowError = 0;
        ServoControlCam.lastError = 0;
        ServoControlCam.beforeError = 0;
    //--------------转向环  摄像头控制-----------------//
        TurnLoopCam.maxout = 1000;
        TurnLoopCam.minout = -1000;
//      TurnLoopCam.kp1 = 3.50000f;//3.1
//      TurnLoopCam.kp2 = 4.40000f;//3.2
        TurnLoopCam.kp2_K = 1.000f;
//      TurnLoopCam.kp3 = 0.03500f;//归一到 -6 ~ 6 0.00035
//      TurnLoopCam.ki =  0.00000f;
//      TurnLoopCam.kd = 40.00000f;
        TurnLoopCam.Ud = 0.000f;
        TurnLoopCam.Ud_a = 0.200f;
        TurnLoopCam.Ud_last = 0.000f;
        TurnLoopCam.sumError = 0;
        TurnLoopCam.sumErrormax = 100;
        TurnLoopCam.sumErrormin = -100;
        TurnLoopCam.lastError = 0;
        TurnLoopCam.beforeError = 0;
    //---------------模糊PID------------//
        FuzzyControlCam.outmax = 1000;
        FuzzyControlCam.outmin = -1000;
//      FuzzyControlCam.kp1 = 4.3;//3.3
//      FuzzyControlCam.ki1 = 0.0;//0.0
//      FuzzyControlCam.kd1 = 0.15;//0.15
//      FuzzyControlCam.kp2 = 8.0;//8.0
//      FuzzyControlCam.ki2 = 0.0;//0.0
//      FuzzyControlCam.kd2 = 0.0;//0.0
        FuzzyControlCam.nowError = 0;
        FuzzyControlCam.lastError = 0;
        FuzzyControlCam.beforeError = 0;
        FuzzyControlCam.nowErrmax = 200;
        FuzzyControlCam.nowErrmin = -200;
        FuzzyControlCam.ECmax = 10;
        FuzzyControlCam.ECmin = -10;
        FuzzyControlCam.sumError = 0;
}


//------------------------普通增量式pid-------------------------//
SpeedPIDTypedef SpeedPID,SpeedPIDLeft,SpeedPIDRight;
int16 SimpleIncremental_PID(float *pid_params,SpeedPIDTypedef *pid,int16 _exval,int16 _nowval)
{
    //读取参数
    pid->kp = pid_params[SPEEDLOOP_Kp];
    pid->ki = pid_params[SPEEDLOOP_Ki];
    pid->kd = pid_params[SPEEDLOOP_Kd];
    pid->maxout = (int16)pid_params[SPEEDLOOP_MaxO];
    pid->minout = (int16)pid_params[SPEEDLOOP_MinO];

    //运算
    pid->exval = _exval;
    pid->nowval = _nowval;
    //当前误差
    pid->nowError =  pid->exval - pid->nowval;
    //增量
    pid->increase =
        pid->kp * (float)(pid->nowError - pid->lastError) +
        pid->ki * (float)pid->nowError +
        pid->kd * (float)(pid->nowError - 2 * pid->lastError + pid->beforeError);
    //更新误差
    pid->lastError = pid->nowError;
    pid->beforeError = pid->lastError;
    pid->out += (int16)pid->increase;
    //限制幅度
    pid->out = limit_ab(pid->out,(int32)pid->minout,(int32)pid->maxout);
    //输出
    return pid->out;
}


//----------------------祖传舵机pid控制 摄像头有效行---------------------------//
//非线性
ServoControlCamTypedef ServoControlCam;

int16 ServoPosition_PID(float *pid_params,ServoControlCamTypedef *pid,int16 _exval,int16 _nowval)
{
    //读取参数
    pid->kp1 = pid_params[SERVOCONTROL_Kp1];
    pid->kp2 = pid_params[SERVOCONTROL_Kp2];
    pid->ki = pid_params[SERVOCONTROL_Ki];
    pid->kd = pid_params[SERVOCONTROL_Kd];
    pid->maxout = pid_params[SERVOCONTROL_MaxO];
    pid->minout = pid_params[SERVOCONTROL_MinO];
    pid->exval = _exval;
    pid->nowval = _nowval;
    //当前误差
    pid->nowError =  pid->exval - pid->nowval;
    //计算Kp
    pid->Up = (short)(pid->kp1 * pid->nowError) + \
              (short)((pid->kp2 * (ROW - 1 - Base.Topline) * (ROW - 1 - Base.Topline) * pid->nowError) / 1000);
    //计算Ki
    pid->Ui = (short)(pid->ki * (pid->nowError + pid->lastError + pid->beforeError));
    //计算Kd
    pid->Ud = (short)(pid->kd * (pid->nowError - pid->lastError));
    //计算输出
    pid->out = (int16)(pid->Up + pid->Ui + pid->Ud );
    //更新误差
    pid->lastError = pid->nowError;
    pid->beforeError = pid->lastError;
    //限制幅度
    pid->out = limit_ab(pid->out,(int32)pid->minout,(int32)pid->maxout);
    //输出
    return pid->out;
}


//----------------------位置式pid控制 摄像头---------------------------//
//非线性
TurnLoopCamTypedef TurnLoopCam;

int16 Position_PID(float *pid_params,TurnLoopCamTypedef *pid,int16 _exval,int16 _nowval)
{
    //读取参数
    pid->kp1 = pid_params[TURNLOOP_Kp1];
    pid->kp2 = pid_params[TURNLOOP_Kp2];
    pid->kp3 = pid_params[TURNLOOP_Kp3];
    pid->ki = pid_params[TURNLOOP_Ki];
    pid->kd = pid_params[TURNLOOP_Kd];
    pid->sumErrormax = (int16)pid_params[TUNRLOOP_SumMax];
    pid->sumErrormin = (int16)pid_params[TURNLOOP_SumMin];
    pid->maxout = (int16)pid_params[TURNLOOP_MaxO];
    pid->minout =(int16) pid_params[TURNLOOP_MinO];

    //运算
    pid->exval = _exval;
    pid->nowval = _nowval;
    //当前误差
    pid->nowError =  pid->exval - pid->nowval;
    //误差积分
    pid->sumError += pid->nowError;
    //误差积分限幅
    pid->sumError = limit_ab(pid->sumError,(int32)pid->sumErrormin,(int32)pid->sumErrormax);
    //计算kp2
    pid->kp2Real = pid->kp2_K * pid->kp2;
    //计算kp_sum
    pid->kp = pid->kp1 + pid->kp2*(1 - (2 / ( exp(pid->kp3 * pid->nowError) + \
        exp(- pid->kp3 * pid->nowError) ) ) );
    //计算Ud
    pid->Ud = pid->kd * (1.000f - pid->Ud_a) * (float)(pid->nowError - pid->lastError) +
        pid->Ud_a * pid->Ud_last;
    //计算输出
    pid->out = (int16)(
        pid->kp * (float)pid->nowError + \
        pid->ki * (float)pid->sumError + \
        pid->Ud);
    //更新误差
    pid->lastError = pid->nowError;
    pid->beforeError = pid->lastError;
    //限制幅度
    pid->out = limit_ab(pid->out,(int32)pid->minout,(int32)pid->maxout);
    //记录Ud
    pid->Ud_last = pid->out;
    //输出
    return pid->out;
}



//------------------------------------ 模糊PID ---------------------------------------------//

/*论域*/
#define     NB      -6
#define     NM      -4
#define     NS      -2
#define     ZO      0
#define     PS      2
#define     PM      4
#define     PB      6


/*Kp规则表*/
int8 ruleKp[7][7] = {

/*  \ Ec */      /* NB 负大   NM 负中  NS 负小   ZO 零  PS 正小  PM 正中  PB 正大 */
/* E \ */           /* -6        -4        -2      0        2        4         6*/
/* NB 负大 -6 */{    PB    ,    PB    ,  PM    ,  PB   ,   PM   ,   ZO   ,   ZO   },
/* NM 负中 -4 */{    PB    ,    PB    ,  PM    ,  PM   ,   PS   ,   ZO   ,   NS   },
/* NS 负小 -2 */{    PM    ,    PM    ,  PS    ,  PS   ,   ZO   ,   NS   ,   NS   },
/* ZO 零    0 */{   PM    ,    PM    ,  PS    ,  ZO   ,   NS   ,   NM   ,   NM    },
/* PS 正小  2 */{    PS    ,    PS    ,  ZO    ,  NS   ,   NS   ,   NM   ,   NM   },
/* PM 正中  4 */{    PS    ,    ZO    ,  NS    ,  NM   ,   NM   ,   NB   ,   NB   },
/* PB 正大  6 */{    ZO    ,    ZO    ,  NM    ,  NB   ,   NM   ,   NB   ,   NB   },

};

/*Ki规则表*/
int8 ruleKi[7][7] = {

/*  \ Ec */      /* NB 负大   NM 负中  NS 负小   ZO 零  PS 正小  PM 正中  PB 正大 */
/* E \ */           /* -6        -4        -2      0        2        4         6*/
/* NB 负大 -6 */{    NB    ,    NB    ,  NM    ,  NB   ,   NM   ,   ZO   ,   ZO   },
/* NM 负中 -4 */{    NB    ,    NB    ,  NM    ,  NM   ,   NS   ,   ZO   ,   ZO   },
/* NS 负小 -2 */{    NM    ,    NM    ,  NS    ,  NS   ,   ZO   ,   PS   ,   PS   },
/* ZO 零    0 */{   NM    ,    NM    ,  NS    ,  ZO   ,   PS   ,   PM   ,   PM   },
/* PS 正小  2 */{    NS    ,    NS    ,  ZO    ,  PS   ,   PS   ,   PM   ,   PM   },
/* PM 正中  4 */{    ZO    ,    ZO    ,  PS    ,  PM   ,   PM   ,   PB   ,   PB   },
/* PB 正大  6 */{    ZO    ,    ZO    ,  PM    ,  PB   ,   PM   ,   PB   ,   PB   },

};


/*Kd规则表*/
int8 ruleKd[7][7] = {

/*  \ Ec */      /* NB 负大   NM 负中  NS 负小   ZO 零  PS 正小  PM 正中  PB 正大 */
/* E \ */           /* -6        -4        -2      0        2        4         6*/
/* NB 负大 -6 */{    PS    ,    NS    ,  NB    ,  NB   ,   NB   ,   NM   ,   PS   },
/* NM 负中 -4 */{    PS    ,    NS    ,  NB    ,  NM   ,   NM   ,   NS   ,   PS   },
/* NS 负小 -2 */{    ZO    ,    NS    ,  NM    ,  NM   ,   NS   ,   NS   ,   ZO   },
/* ZO 零    0 */{   ZO    ,    NS    ,  NS    ,  NS   ,   NS   ,   NS   ,   ZO   },
/* PS 正小  2 */{    ZO    ,    ZO    ,  ZO    ,  ZO   ,   ZO   ,   ZO   ,   ZO   },
/* PM 正中  4 */{    PB    ,    NS    ,  PS    ,  PS   ,   PS   ,   PS   ,   PB   },
/* PB 正大  6 */{    PB    ,    PM    ,  PM    ,  PM   ,   PS   ,   PS   ,   PB   },

};

//statement
FuzzyPID_Typedef  FuzzyControlCam;

/*隶属度计算函数*/
/**
* @parm qv:     量化后的参数
* @parm index： index[0]为规则表中E(-6~6);index[1]为规则表中EC(-6~6);example:rule[index[0]][rule[1]]
* @parm ms:     最终的隶属度
**/
void Calculate_Membership(float qv,int * index,float *ms)
{
  if((qv>=NB)&&(qv<NM))//-6 ~ -4
  {
    index[0]=0;
    index[1]=1;
    ms[0]=-0.5*qv-2.0;  //y=-0.5x-2.0
    ms[1]=0.5*qv+3.0;   //y=0.5x+3.0
  }
  else if((qv>=NM)&&(qv<NS))//-4 ~ -2
  {
    index[0]=1;
    index[1]=2;
    ms[0]=-0.5*qv-1.0;  //y=-0.5x-1.0
    ms[1]=0.5*qv+2.0;   //y=0.5x+2.0
  }
  else if((qv>=NS)&&(qv<ZO))//-2 ~ 0
  {
    index[0]=2;
    index[1]=3;
    ms[0]=-0.5*qv;      //y=-0.5x
    ms[1]=0.5*qv+1.0;   //y=0.5x+1.0
  }
  else if((qv>=ZO)&&(qv<PS))//0 ~ 2
  {
    index[0]=3;
    index[1]=4;
    ms[0]=-0.5*qv+1.0;  //y=-0.5x+1.0
    ms[1]=0.5*qv;       //y=0.5x
  }
  else if((qv>=PS)&&(qv<PM))//2 ~ 4
  {
    index[0]=4;
    index[1]=5;
    ms[0]=-0.5*qv+2.0;  //y=-0.5x+2.0
    ms[1]=0.5*qv-1.0;   //y=0.5x-1.0
  }
  else if((qv>=PM)&&(qv<=PB))//4 ~ 6
  {
    index[0]=5;
    index[1]=6;
    ms[0]=-0.5*qv+3.0;  //y=-0.5x+3.0
    ms[1]=0.5*qv-2.0;   //y=0.5x-2.0
  }
}

//模糊PID实现
float Fuzzy_PID(float *pid_params, FuzzyPID_Typedef *pid,int16 _exval,int16 _nowval)
{
    //读取数值
    pid->kp1 = pid_params[FUZZY_Kp1];
    pid->kp2 = pid_params[FUZZY_Kp2];
    pid->ki1 = pid_params[FUZZY_Ki1];
    pid->ki2 = pid_params[FUZZY_Ki2];
    pid->kd1 = pid_params[FUZZY_Kd1];
    pid->kd2 = pid_params[FUZZY_Kd2];
    pid->sumErrormax = pid_params[FUZZY_SumMax];
    pid->sumErrormin = pid_params[FUZZY_SumMin];
    pid->outmax = pid_params[FUZZY_MaxO];
    pid->outmin = pid_params[FUZZY_MinO];
    pid->exval = _exval;
    pid->nowval = _nowval;
    //误差以及误差增量限幅
    pid->nowError = limit_ab(pid->nowError,pid->nowErrmin,pid->nowErrmax);
    pid->ErrorInc = limit_ab(pid->ErrorInc,pid->ECmin,pid->ECmax);
    //更新当前误差
    pid->nowError =  pid->exval - pid->nowval;
    pid->ErrorInc = pid->nowError - pid->lastError;
    /********  模糊化  *******/
    //量化 论域{-6，-5，-4，-3，-2，-1，0，1，2，3，4，5，6}
    pid->E_Quantized  = 6.0 * pid->nowError / pid->nowErrmax;
    pid->EC_Quantized = 6.0 * pid->ErrorInc / pid->ECmax;
    //计算隶属度
    Calculate_Membership(pid->E_Quantized,pid->E_Index,pid->E_Membership);
    Calculate_Membership(pid->EC_Quantized,pid->EC_Index,pid->EC_Membership);
    /********  解模糊  *******/
    //计算kx2的系数
    pid->kp2k =
        pid->E_Membership[0] * pid->EC_Membership[0] * ruleKp[pid->E_Index[0]][pid->EC_Index[0]] +\
        pid->E_Membership[0] * pid->EC_Membership[1] * ruleKp[pid->E_Index[0]][pid->EC_Index[1]] +\
        pid->E_Membership[1] * pid->EC_Membership[0] * ruleKp[pid->E_Index[1]][pid->EC_Index[0]] +\
        pid->E_Membership[1] * pid->EC_Membership[1] * ruleKp[pid->E_Index[1]][pid->EC_Index[1]] ;
    pid->ki2k =
        pid->E_Membership[0] * pid->EC_Membership[0] * ruleKi[pid->E_Index[0]][pid->EC_Index[0]] +\
        pid->E_Membership[0] * pid->EC_Membership[1] * ruleKi[pid->E_Index[0]][pid->EC_Index[1]] +\
        pid->E_Membership[1] * pid->EC_Membership[0] * ruleKi[pid->E_Index[1]][pid->EC_Index[0]] +\
        pid->E_Membership[1] * pid->EC_Membership[1] * ruleKi[pid->E_Index[1]][pid->EC_Index[1]] ;
    pid->kd2k =
        pid->E_Membership[0] * pid->EC_Membership[0] * ruleKd[pid->E_Index[0]][pid->EC_Index[0]] +\
        pid->E_Membership[0] * pid->EC_Membership[1] * ruleKd[pid->E_Index[0]][pid->EC_Index[1]] +\
        pid->E_Membership[1] * pid->EC_Membership[0] * ruleKd[pid->E_Index[1]][pid->EC_Index[0]] +\
        pid->E_Membership[1] * pid->EC_Membership[1] * ruleKd[pid->E_Index[1]][pid->EC_Index[1]] ;
    //计算总的kx
    pid->kp = pid->kp1 + pid->kp2 * (myabs(pid->kp2k) / 6.0);//除以论域极值 计算的是kp增量百分比
    pid->kd = pid->kd1 + pid->kd2 * (myabs(pid->kd2k) / 6.0);//除以论域极值 计算的是kp增量百分比
    //计算输出
    pid->out =
        pid->kp * (float)pid->nowError + \
        pid->kd * (float)(pid->nowError - pid->lastError);
    //更新误差
    pid->lastError = pid->nowError;
    pid->beforeError = pid->lastError;
    //输出限幅
    pid->out = limit_ab(pid->out,(int32)pid->outmin,(int32)pid->outmax);
    //输出
    return pid->out;
}


