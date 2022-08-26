/**
 //@FileName    :control.c
 //@CreatedDate :2021年12月22日
 //@Author      :LiHao
 //@Description :
 **/

#include "control.h"
#include <math.h>

CarInfoTypedef CarInfo;
//定时器中断 1ms
static int16 Timer_IntCnt = 0;
int16 SpeedOut_L = 0;
int16 SpeedOut_R = 0;
int16 DirectionOut = 0;
int16 ExSpeed = 270; //320 NormalSpeed
int16 ExSpeed_SUM = 270; //250
int16 ExSpeed_SUM_tmp=270;
int16 circle_speed = 270 ; //240
int16 fork_speed=240;
uint16_t Ackerman_kl = 180;    //左右差速系数
uint16_t Ackerman_kr = 180;
int16 ExSpeed_L = 0;
int16 ExSpeed_R = 0;
int16 L_SingleControlOut = 0, R_SingleControlOut = 0;
int8 InControlPeriod_Flag = 'F';

/**
 *@Name          :Timer6_IT
 *@Description   :Timer6_IT基本定时器6中断服务函数，用于电机控制及其他中断服务
 *@Param         :None
 *@Return        :NULL
 *@Sample        :Timer6_IT();
 **/
int cam_die = 0;
bool protect_cam = 0;
void Timer6_IT(void) {
    Timer_IntCnt++;
    CarInfo.UpTime += 0.001f;
  //  Beep_IntCnt();
    KeyScan();
//    //陀螺仪数值获取
////  Data_Filter();
////  if(TIMx_Cnt % 5 == 0 )
////  {
////      //角度积分
////      ICM_I_Int();
////      //更新角度
////      IMUupdate(ICM_Treated.gyro.x*0.01745, ICM_Treated.gyro.y*0.01745, ICM_Treated.gyro.z*0.01745, 。
////              ICM_Treated.accdata.x , ICM_Treated.accdata.y, ICM_Treated.accdata.z);
////  }
//    //-------------------------------方向控制------------------------------------//
//
    DirectionControl();
//
//    //-------------------------------速度控制------------------------------------//

    if (Timer_IntCnt % 10 == 0) {
        ExSpeed_L = ExSpeed_SUM;
        ExSpeed_R = ExSpeed_SUM;
        ExSpeedControl(); //顺序不能错
        DiffSpeedControl();
        SpeedControl();
    }


    //清零计数
    if (Timer_IntCnt >= 1000) {
        gpio_toggle_level(LED0_PIN);
        Timer_IntCnt = 0;
        CarProtect_IntCnt = 0;
    }
}
//初始化一些设置
uint8 CarSystem_Init(void) {
    //设置标志
    connect.error_cnt=0;
    connect.last_room=0;
    connect.room_cnt=0;
    CarInfo.Protect_Flag = 'F';
    CarInfo.ControlMode = CAMMODE;
    CarInfo.ClosedLoopMode = AllLoop;
    CarInfo.CAR_PROTECT_MODE = 'F'; //开启电磁信号削减保护
    CarInfo.StopTime = 0.000f;
    CarInfo.UpTime = 0.000f;
    CarInfo.ReadFlashParams = 'F';
    CarInfo.Camera_FPS = 150;
    //ret
    return 1;
}

//中线计算
int32 point_center;
int32 point_center0;
int32 point_center1;
int32 point_center2;
uint8 CenterCal(void) {
    //计算当前行的偏移量
    float excursion[60];
    float sumcenter = 0;

#define CenterCalMinRow 12
#define CenterCalMaxRow 45
    for (Ysite = CenterCalMinRow; Ysite < CenterCalMaxRow; Ysite++) {
        excursion[Ysite] = (float) (Img.Center_Repair[Ysite] - (COL / 2))
                / (float) (RoadWide0 / 2) * 100;
    }
    for (Ysite = CenterCalMinRow; Ysite < CenterCalMaxRow; Ysite++) {
        sumcenter += excursion[Ysite];
    }
    point_center0 = (int32) (sumcenter / (CenterCalMaxRow - CenterCalMinRow)
            * 2.0f);
    point_center1 = point_center0;
    point_center2 = point_center1;
    point_center = point_center0 * 0.7f + point_center1 * 0.2f
            + point_center2 * 0.1f;
//  }
    return 1;
}

//uint32_t RunCnt = 0;
/**
 *@Name         :AllImageDeal
 *@Description  :AllImageDeal
 *@Param            :None
 *@Return       :NULL
 *@Sample       :AllImageDeal();
 **/

void AllImageDeal(void) {

    system_cnt(1);
    GetSimBinImage();
   search_line();
    judge_unit();
  GetBorder();
   solve_line();
   cal_middle_line();
//   ips114_show_int32(0, 4, left_left_cnt,4);
//   ips114_show_int32(50, 4, left_right_cnt,4);
//   ips114_show_int32(0, 5, right_left_cnt,4);
//   ips114_show_int32(50, 5, right_right_cnt,4);
    // ips114_show_int32(50, 5,     connect.room,4);

   ips114_show_int32(50, 4,        system_cnt(0),5);
 // img_send();
}

/**
 *@Name         :ControlPeriodCheck
 *@Description  :ControlPeriodCheck控制周期检查:在一个舵机周期内完成方向控制
 *@Param            :None
 *@Return       :NULL
 *@Sample       :ControlPeriodCheck();
 **/
void ControlPeriodCheck(void) {
    //144MHz下50Hz的PWM分频系数为43,分频后为3,348,837Hz,即3349次/ms
    //16位计数器计至65,536(20ms左右)时溢出产生中断
    uint16_t MinCnt = 7000;    //CNT寄存器小于此值时置标志位 需要计算&测试
    if (TIM_GetCounter(((TIM_TypeDef *) tim_index[SERVO_PWM >> 8])) <= MinCnt) { //timer_get();
        InControlPeriod_Flag = 'T';
    }
}

/**
 *@Name         :DirectionControl
 *@Description  :DirectionControl
 *@Param            :None
 *@Return       :None
 *@Sample       :DirectionControl();
 **/
void DirectionControl(void) {
    if ((CarInfo.ClosedLoopMode == DirectLoop
            || CarInfo.ClosedLoopMode == AllLoop
            || CarInfo.ClosedLoopMode == SpeedLoop)
            && CarInfo.Protect_Flag == 'F') {
        if (CarInfo.ControlMode == CAMMODE) {    //摄像头控制
            DirectionOut = ServoPosition_PID(&ServoPIDParam[0][0],
                    &ServoControlCam, 0, point_center);
            //      DirectionOut = Position_PID(&TurnPIDParam[0][0],&TurnLoopCam,0,point_center );
            //      DirectionOut = Fuzzy_PID(&AngleLoop_Fuzzy, point_center,0);
        } else if (CarInfo.ControlMode == ELCMODE) {    //电磁控制
            //      DirectionOut = Position_PID(&TurnPIDParam[0][0],&TurnLoopCam,0,ELC_Center_14);
        } else {
            DirectionOut = 0;
        }
    } else {
        DirectionOut = 0;
    }
    ServoAdd = DirectionOut;
    if(abs(ServoAdd)<40)ServoAdd=0;
    ServoWrite();
}

/**
 *@Name         :ExSpeedControl
 *@Description  :ExSpeedControl这里用于加减速控制
 *@Param            :None
 *@Return       :NULL
 *@Sample       :ExSpeedControl();
 **/
void ExSpeedControl(void) {
//    if (1) {
//        ExSpeed = ExSpeed;
//    }
}

/**
 *@Name         :DiffSpeedControl
 *@Description  :DiffSpeedControl差速控制
 *@Param            :None
 *@Return       :NULL
 *@Sample       :DiffSpeedControl();
 **/
void DiffSpeedControl(void) {
    if (stop_flag == 1)
        ExSpeed = 0;
    //  else   ExSpeed=ExSpeed_SUM-(ROW-1-Base.Topline)/(ROW-1)*10 ;
    else
        ExSpeed = ExSpeed_SUM;

    //  Ackerman_kl=(1.0*Wheel_L.SpeedNow/   ExSpeed)*200;
    //     Ackerman_kr=(1.0*Wheel_R.SpeedNow/   ExSpeed)*200;

    if (CarInfo.ClosedLoopMode == AllLoop && CarInfo.Protect_Flag == 'F') {
        if (ServoAdd <= -150)    //右转
                {
            ExSpeed_L = (int) (ExSpeed
                    * (1
                            - ((float) Ackerman_kl / 1000)
                                    * tan((float) ServoAdd * 3.14 / 4600)
                                    / 0.396));;
            ExSpeed_R = (int) (ExSpeed
                    * (1
                            + ((float) Ackerman_kr / 1000)
                                    * tan((float) ServoAdd * 3.14 / 4600)
                                    / 0.396));
    } else if (ServoAdd > 150)    //左转
                {
            ExSpeed_L = (int) (ExSpeed
                    * (1
                            - ((float) Ackerman_kl / 1000)
                                    * tan((float) ServoAdd * 3.14 / 4600)
                                    / 0.396));
            ExSpeed_R = (int) (ExSpeed
                    * (1
                            + ((float) Ackerman_kr / 1000)
                                    * tan((float) ServoAdd * 3.14 / 4600)
                                    / 0.396));;
        } else {
            ExSpeed_L = ExSpeed;
            ExSpeed_R = ExSpeed;    //内轮减速外轮加速确保中轴线速度不变
        }
    }
}
/**
 *@Name         :SpeedControl
 *@Description  :SpeedControl速度PID控制
 *@Param            :None
 *@Return       :NULL
 *@Sample       :SpeedControl();
 **/
void SpeedControl(void) {

//    EncoderRead();
//
//    if (CarInfo.Protect_Flag == 'F') {
//        if (CarInfo.ClosedLoopMode == AllLoop
//                || CarInfo.ClosedLoopMode == SpeedLoop) {
//            SpeedOut_L = SimpleIncremental_PID(&SpeedPIDParam[0][0],
//                    &SpeedPIDLeft, (float) (ExSpeed_L), Wheel_L.SpeedNow);
//            SpeedOut_R = SimpleIncremental_PID(&SpeedPIDParam[0][0],
//                    &SpeedPIDRight, (float) (ExSpeed_R), Wheel_R.SpeedNow);
//        } else if (CarInfo.ClosedLoopMode == LR_SingleControl) {
//            SpeedOut_L = SimpleIncremental_PID(&SpeedPIDParam[0][0],
//                    &SpeedPIDLeft, (float) (ExSpeed_L), Wheel_L.SpeedNow);
//            SpeedOut_R = SimpleIncremental_PID(&SpeedPIDParam[0][0],
//                    &SpeedPIDRight, (float) (ExSpeed_R), Wheel_R.SpeedNow);
//        } else {
//            SpeedOut_L = 0;
//            SpeedOut_R = 0;
//        }
//
//    } else {    //速度控零保护
//
//        SpeedOut_L = SimpleIncremental_PID(&LeftMotorSpeedPIDParam[0][0],
//                &SpeedPIDLeft, 0, Wheel_L.SpeedNow);
//        SpeedOut_R = SimpleIncremental_PID(&RightMotorSpeedPIDParam[0][0],
//                &SpeedPIDRight, 0, Wheel_R.SpeedNow);
//    }
//    pwm_r = SpeedOut_L;
//    pwm_l = SpeedOut_R;
//
//    MotorWrite();

    EncoderRead();
    if (CarInfo.ClosedLoopMode != NoLoop) {
        if (CarInfo.Protect_Flag == 'F') {
            if (CarInfo.ClosedLoopMode == AllLoop
                    || CarInfo.ClosedLoopMode == SpeedLoop) {
                SpeedOut_L = SimpleIncremental_PID(&SpeedPIDParam[0][0],
                        &SpeedPIDLeft, (float) (ExSpeed_L), Wheel_L.SpeedNow);
                SpeedOut_R = SimpleIncremental_PID(&SpeedPIDParam[0][0],
                        &SpeedPIDRight, (float) (ExSpeed_R), Wheel_R.SpeedNow);
            } else if (CarInfo.ClosedLoopMode == LR_SingleControl) {
                SpeedOut_L = SimpleIncremental_PID(&SpeedPIDParam[0][0],
                        &SpeedPIDLeft, (float) (ExSpeed_L), Wheel_L.SpeedNow);
                SpeedOut_R = SimpleIncremental_PID(&SpeedPIDParam[0][0],
                        &SpeedPIDRight, (float) (ExSpeed_R), Wheel_R.SpeedNow);
            } else {
                SpeedOut_L = 0;
                SpeedOut_R = 0;
            }
        } else if (CarInfo.Protect_Flag == 'T') {    //速度控零保护
            SpeedOut_L = SimpleIncremental_PID(&LeftMotorSpeedPIDParam[0][0],
                    &SpeedPIDLeft, 0, Wheel_L.SpeedNow);
            SpeedOut_R = SimpleIncremental_PID(&RightMotorSpeedPIDParam[0][0],
                    &SpeedPIDRight, 0, Wheel_R.SpeedNow);
        }
        pwm_l = SpeedOut_L;
        pwm_r = SpeedOut_R;
        MotorWrite();
    } else
        return;

}
/**
 *@Name         :Timer7_IT
 *@Description  :Timer7_IT基本定时器7中断服务函数，用于图像处理及舵机控制
 *@Param            :None
 *@Return       :NULL
 *@Sample       :Timer7_IT();
 **/
void Timer7_IT(void) {
//    ControlPeriodCheck();
//    if (InControlPeriod_Flag == 'T') {//暂不使用
//       AllImageDeal();
//        DirectionControl();
//        InControlPeriod_Flag = 'F';
//    }
}

//冲出赛道电磁削减保护
int16 CarProtect_IntCnt = 0;
void CarProtect(void) {
    if ((CarInfo.UpTime >= 2.000f)
            && ((CarInfo.ClosedLoopMode == AllLoop)
                    || (CarInfo.ClosedLoopMode == SpeedLoop))) {
        //---------------电磁保护--------------//
        if (ElcProtect_Flag == 'T' && CarInfo.CAR_PROTECT_MODE == 'T') {
            CarProtect_IntCnt++;
        }
        if (CarProtect_IntCnt >= 5) {
            CarInfo.Protect_Flag = 'T';
        }
    }
    if (CarInfo.StopTime != 0.000f && CarInfo.UpTime >= CarInfo.StopTime) {
        CarInfo.Protect_Flag = 'T';
        Base.element_check = 'F';
    }
}

//显示界面调整参数
const char CarInfoItem0[] = "Protect_flag En:";
const char CarInfoItem1[] = "Control Loop:";
const char CarInfoItem2[] = "Params Read:";

const char ClosedLoopModePrams[4][10] = { "NOLoop    ", "SpeedLoop ",
        "DirectLoop", "AllLoop   " };

#define CarInfo_MENU_NUM 2
#define ControlMood_MENU_NUM 4

uint8 CarInfoShow(void) {
    while (KeyLast != onepress) {
//      static int8 NowChooseControlMood = 1;
        static uint8 NowCarInfoItem = 0;
        MenuChoose(&NowCarInfoItem, CarInfo_MENU_NUM);
        //显示条目
        ips114_pencolor = RED;
        ips114_show_string(16, 0, CarInfoItem0);
        ips114_show_string(16, 1, CarInfoItem1);
        ips114_show_string(16, 2, CarInfoItem2);
        //显示参数
        ips114_pencolor = BLUE;
        if (CarInfo.CAR_PROTECT_MODE == 'T')
            ips114_show_string(128, 0, (const char*) ("T"));
        else
            ips114_show_string(128, 0, (const char*) ("F"));

        if (CarInfo.ClosedLoopMode == AllLoop)
            ips114_show_string(128, 1, (const char*) ("AllLoop"));
        else if (CarInfo.ClosedLoopMode == SpeedLoop)
            ips114_show_string(128, 1, (const char*) ("SpeedLoop"));
        else if (CarInfo.ClosedLoopMode == DirectLoop)
            ips114_show_string(128, 1, (const char*) ("DirectLoop"));
        else if (CarInfo.ClosedLoopMode == NoLoop)
            ips114_show_string(128, 1, (const char*) ("NoLoop"));

        if (CarInfo.ReadFlashParams == 'T')
            ips114_show_string(128, 2, (const char*) ("T"));
        else
            ips114_show_string(128, 2, (const char*) ("F"));
        //调参确认
        if (KeyNext == onepress) {
            KeyNext = nopress;
            switch (NowCarInfoItem) {
            case (0): {
                StateAdjust(&CarInfo.CAR_PROTECT_MODE, 128, 0);
                break;
            }
            case (1): {
                EnumAdjust((uint8*) (&CarInfo.ClosedLoopMode),
                        (char (*)[]) (&ClosedLoopModePrams), 4, 128, 1);
                break;
            }
            case (2): {
                StateAdjust(&CarInfo.ReadFlashParams, 128, 2);
                break;
            }
            }
        }
        my_delay(10);
    }
    KeyLast = nopress;
    //
    return 1;
}

